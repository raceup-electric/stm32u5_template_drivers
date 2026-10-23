set(_drivers_instance_dir "${CMAKE_CURRENT_LIST_DIR}")
include("${CMAKE_CURRENT_LIST_DIR}/../instance_layout.cmake")

if(DEFINED STM32_DEVICE AND NOT STM32_DEVICE STREQUAL "STM32U545xx")
  message(FATAL_ERROR "stm32u5xx: this backend targets STM32U545xx, not ${STM32_DEVICE}")
endif()
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(STM32CubeU5 REQUIRED MODULE)
if(NOT TARGET freertos_kernel)
  message(FATAL_ERROR "stm32u5xx: provide a freertos_kernel target with port/config include directories (USB CDC service)")
endif()

enable_language(C)
if(NOT TARGET device_hal)
  set(_u5_hal_sources)
  foreach(_module IN ITEMS hal hal_adc hal_adc_ex hal_cortex hal_dma hal_dma_ex
      hal_exti hal_fdcan hal_flash hal_flash_ex hal_gpio hal_iwdg hal_lptim
      hal_pcd hal_pcd_ex hal_pwr hal_pwr_ex hal_rcc hal_rcc_ex hal_rtc hal_rtc_ex
      hal_sd hal_spi hal_spi_ex hal_tim hal_tim_ex hal_uart hal_uart_ex hal_icache
      ll_sdmmc ll_usb)
    list(APPEND _u5_hal_sources "${STM32CubeU5_HAL_SOURCE_DIR}/stm32u5xx_${_module}.c")
  endforeach()
  add_library(device_hal STATIC ${_u5_hal_sources})
  target_include_directories(device_hal PUBLIC "${CMAKE_CURRENT_LIST_DIR}/config")
  target_include_directories(device_hal SYSTEM PUBLIC
    "${STM32CubeU5_HAL_INCLUDE_DIR}" "${STM32CubeU5_CMSIS_INCLUDE_DIR}"
    "${STM32CubeU5_CMSIS_DEVICE_INCLUDE_DIR}")
  target_compile_definitions(device_hal PUBLIC USE_HAL_DRIVER STM32U545xx)
endif()

file(GLOB _u5_usbx_core CONFIGURE_DEPENDS "${STM32_USBX_ROOT}/common/core/src/*.c")
file(GLOB _u5_usbx_cdc CONFIGURE_DEPENDS
  "${STM32_USBX_ROOT}/common/usbx_device_classes/src/ux_device_class_cdc_acm*.c")
file(GLOB _u5_usbx_dcd CONFIGURE_DEPENDS
  "${STM32_USBX_ROOT}/common/usbx_stm32_device_controllers/*.c")
list(APPEND DRIVERS_INSTANCE_SOURCES
  "${STM32_EEPROM_EMULATION_ROOT}/core/eeprom_emul_core.c"
  "${STM32_EEPROM_EMULATION_ROOT}/core/algo/eeprom_algo_flitf.c"
  "${CMAKE_CURRENT_LIST_DIR}/usb/usb_cdc_standalone.c"
  "${CMAKE_CURRENT_LIST_DIR}/usb/ux_standalone_support.c"
  "${CMAKE_CURRENT_LIST_DIR}/usb/usb_irq.c"
  ${_u5_usbx_core} ${_u5_usbx_cdc} ${_u5_usbx_dcd})
list(APPEND DRIVERS_INSTANCE_PUBLIC_INCLUDE_DIRS
  "${CMAKE_CURRENT_LIST_DIR}" "${CMAKE_CURRENT_LIST_DIR}/config")
list(APPEND DRIVERS_INSTANCE_PRIVATE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/usb")
list(APPEND DRIVERS_INSTANCE_SYSTEM_PUBLIC_INCLUDE_DIRS
  "${STM32CubeU5_HAL_INCLUDE_DIR}" "${STM32CubeU5_CMSIS_INCLUDE_DIR}"
  "${STM32CubeU5_CMSIS_DEVICE_INCLUDE_DIR}")
list(APPEND DRIVERS_INSTANCE_SYSTEM_PRIVATE_INCLUDE_DIRS
  "${STM32_EEPROM_EMULATION_ROOT}/core"
  "${STM32_EEPROM_EMULATION_ROOT}/core/algo"
  "${STM32_EEPROM_EMULATION_ROOT}/interface_flash"
  "${STM32_EEPROM_EMULATION_ROOT}/interface_crc"
  "${STM32_USBX_ROOT}/common/core/inc"
  "${STM32_USBX_ROOT}/common/usbx_device_classes/inc"
  "${STM32_USBX_ROOT}/common/usbx_stm32_device_controllers"
  "${STM32_USBX_ROOT}/ports/cortex_m4/gnu/inc")
list(APPEND DRIVERS_INSTANCE_COMPILE_DEFINITIONS
  USE_HAL_DRIVER STM32U545xx UX_INCLUDE_USER_DEFINE_FILE)
# Public because the opaque headers expose HAL types and need its device macro.
add_library(drivers_stm32u545_config INTERFACE)
target_compile_definitions(drivers_stm32u545_config INTERFACE USE_HAL_DRIVER STM32U545xx)
list(APPEND DRIVERS_INSTANCE_PUBLIC_LINK_LIBRARIES device_hal drivers_stm32u545_config)
list(APPEND DRIVERS_INSTANCE_PRIVATE_LINK_LIBRARIES freertos_kernel)
