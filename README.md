# Driver STM32U545CET6Q

Backend `stm32u5xx`, specifico per `STM32U545xx`, derivato dall'istanza H5
(commit `b883468ac93ddef9792f2a7635c473e405549ba1`) e adattato alle HAL U5.
È una libreria di driver: startup, linker script, configurazione TrustZone,
clock e collegamenti della scheda appartengono al progetto firmware.

## Integrazione

Prima di `add_subdirectory(lib/drivers)`:

```cmake
set(DRIVERS_INSTANCE stm32u5xx)
set(STM32_DEVICE STM32U545xx)
set(DRIVER_IDS_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/config/driver_ids")
set(DRIVERS_MAPPING_FILE "${PROJECT_SOURCE_DIR}/config/u545/mapping.hpp")
set(STM32CubeU5_ROOT "/percorso/STM32CubeU5" CACHE PATH "")
set(STM32_EEPROM_EMULATION_ROOT "/percorso/stm32-util-eeprom-emulation" CACHE PATH "")
add_subdirectory(lib/drivers)
```

Il progetto deve fornire il target `freertos_kernel` con gli include del port e
`FreeRTOSConfig.h`. USB CDC richiede allocazione statica dei task e
`INCLUDE_xTaskGetSchedulerState=1`. Usare una toolchain ARM con Cortex-M33,
`fpv5-sp-d16`, ABI coerente con il firmware e C++20. Il port FreeRTOS e la
configurazione di sicurezza devono corrispondere alle option bytes della MCU.

CubeU5 deve contenere HAL, CMSIS device/core e USBX. In alternativa si possono
passare separatamente `STM32CubeU5_HAL_INCLUDE_DIR`,
`STM32CubeU5_HAL_SOURCE_DIR`, `STM32CubeU5_CMSIS_INCLUDE_DIR`,
`STM32CubeU5_CMSIS_DEVICE_INCLUDE_DIR` e `STM32_USBX_ROOT`.
`tl::expected` viene risolto dal CMake comune dei driver.

Se non esiste `device_hal`, il backend lo crea. Se viene fornito dal progetto,
deve contenere i moduli U5 elencati nel CMake del backend, con la stessa
configurazione HAL. Le definizioni `STM32U545xx` e `USE_HAL_DRIVER` vengono
propagate ai consumatori. Non vengono scaricate dipendenze durante CMake.

Implementare `ru::driver::stm32u5xx::configure_board() noexcept`, dichiarata in
`config/board_support.hpp`: `Common::start()` la chiama dopo `HAL_Init()`.
Configurare alimentazione, clock di sistema/periferica, GPIO alternativi e
callback MSP. Non esiste un clock HSE presunto né un fallback a 250 MHz.
Usare startup e `system_stm32u5xx.c` ufficiali per STM32U545xx; gestire SysTick
in modo che HAL e FreeRTOS continuino entrambi a ricevere i tick.

Il mapping usa `ru::driver::stm32u5xx::cfg` e macro `RU_STM32U5XX_*_MAP(X)`.
Definire le configurazioni `inline constexpr`, poi includere
`mapping_defaults.hpp`. I mapping predefiniti sono vuoti: non assegnano pin
arbitrari. Selezionare pin e alternate function disponibili nel package LQFP48.
Una sola configurazione deve possedere ciascuna periferica hardware; gli accessi
concorrenti alla stessa periferica devono essere serializzati dall'applicazione.

## Periferiche

- GPIO, PWM TIM/LPTIM e timer: clock dei bus U5; riconoscimento dei timer a 32 bit.
  I timestamp dei timer sono in microsecondi e ripartono da zero al wrap del
  contatore hardware. I timer condivisi tra PWM, ADC trigger e Timer richiedono
  assegnazioni compatibili da parte della scheda.
- ADC1/ADC4: polling e DMA circolare GPDMA; canali injected solo ADC1.
  Configurare risoluzione e `sample_max` coerentemente (default 4095).
  I DMA devono usare GPDMA1 canali 0–7 e la relativa IRQ/request corretta.
  Gli handler forniti instradano per canale. Per condividere gli handler con
  altre periferiche, definire `RU_STM32U5XX_EXTERNAL_DMA_IRQ_HANDLERS` e chiamare
  `ru::driver::adc_dma_channel_irq(channel)` dagli handler del firmware.
- CAN: FDCAN1; le API M_can/Bx_can/Flex_can/Multi_can condividono il controller.
  I timestamp RX sono convertiti in microsecondi e mantengono il wrap del
  contatore hardware a 16 bit. Non configurare un inesistente FDCAN2.
- SPI: trasferimenti full duplex a 8 bit, con handle persistente.
- RTC: calendario dal 2000 al 2099, timestamp Unix in microsecondi con
  risoluzione di un secondo. Clock RTC e accesso al backup domain sono della scheda.
- SDMMC: handle persistente, clock e pin configurati nelle callback MSP.
- IWDG: timeout nominale di 1 secondo basato su `LSI_VALUE`; non arrestabile
  dopo l'avvio. La tolleranza LSI influenza il timeout reale.
- Serial: USB CDC su `SerialId::USB`, lettura e scrittura mediante task USBX
  standalone. Un solo lettore e uno scrittore; chiamate da task, non da ISR.
  La scrittura conferma l'accodamento, non la ricezione da parte dell'host.
  Un timeout può lasciare una lettura/scrittura parziale. Lo stack USB resta
  attivo per la durata del firmware; `Serial::stop()` restituisce errore
  recuperabile quando è già avviato.
- USB Mass Storage non è implementato: restituisce `UNRECOVERABLE_ERROR`.

USB richiede `HAL_PCD_MspInit/DeInit` della scheda: clock USB a 48 MHz,
alimentazione VDDUSB, clock `__HAL_RCC_USB_FS_CLK_ENABLE()` e IRQ
`USB_DRD_FS_IRQn`. L'handler USB è fornito in `usb/usb_irq.c`:
non definirne un secondo. USBX usa il port C GNU cortex_m4, comune ai tipi
Cortex-M in modalità standalone, e le primitive CMSIS di `ux_standalone_support.c`.

## EEPROM emulata

Non è presente un indirizzo flash predefinito. Fornire `eeprom_mapping.h` nella
cartella del mapping con:

```c
#define RU_STM32U5XX_EEPROM_START_PAGE_ADDRESS /* inizio riservato */
#define RU_STM32U5XX_EEPROM_END_ADDRESS        /* fine esclusiva */
```

Usare indirizzi numerici realmente riservati nel linker script. Sono richiesti
allineamento a pagine da 8 KiB e spazio sufficiente per l'algoritmo (32 KiB con
40 variabili e i default correnti). Il backend verifica i limiti dei 512 KiB,
la dimensione della regione e ogni operazione; richiede dual bank abilitato e
gestisce bank swap. Scrive quadword da 16 byte ed elimina pagine, non settori H5.
La regione deve essere accessibile al dominio di sicurezza del firmware.
Le operazioni EEPROM devono essere serializzate e usate fuori dagli ISR.
Il backend `FlashRegion` non è implementato; usare `EmulatedEeprom`.

## Verifica

Compilazione ARM con `STM32U545xx`, HAL/CMSIS ufficiali, C++20, USBX e algoritmo
EEPROM; compilazione con mapping vuoti e popolati, incluso ADC DMA; controllo
del collegamento degli oggetti per simboli duplicati.

Riferimenti usati per la compilazione: GCC ARM 16.2.0, HAL U5
`6555ec181c456cf8f0cd04d2c8a4501003271a7e`, CMSIS device U5
`624374fa1e21ca195d6f2102ac0caaa50d0ea4c8`, FreeRTOS ed EEPROM alle
revisioni dei sottomoduli del progetto.
Questa verifica non sostituisce una prova sulla scheda: clock, pin, USB,
conversioni ADC/DMA, timing CAN e persistenza dopo interruzione di alimentazione
richiedono validazione hardware.

Riferimenti ST: [STM32U545CE](https://www.st.com/en/microcontrollers-microprocessors/stm32u545ce.html),
[datasheet](https://www.st.com/resource/en/datasheet/stm32u545ce.pdf),
[HAL U5](https://github.com/STMicroelectronics/stm32u5xx-hal-driver),
[CMSIS U5](https://github.com/STMicroelectronics/cmsis-device-u5).
