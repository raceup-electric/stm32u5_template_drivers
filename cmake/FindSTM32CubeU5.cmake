# A local STM32CubeU5 checkout is required; no downloads during configuration.
set(STM32CubeU5_ROOT "${CMAKE_SOURCE_DIR}/third_party/STM32/STM32U5/STM32CubeU5"
    CACHE PATH "STM32CubeU5 root")
find_path(STM32CubeU5_HAL_INCLUDE_DIR stm32u5xx_hal.h
  HINTS "${STM32CubeU5_ROOT}/Drivers/STM32U5xx_HAL_Driver/Inc")
find_path(STM32CubeU5_HAL_SOURCE_DIR stm32u5xx_hal.c
  HINTS "${STM32CubeU5_ROOT}/Drivers/STM32U5xx_HAL_Driver/Src")
find_path(STM32CubeU5_CMSIS_INCLUDE_DIR core_cm33.h
  HINTS "${STM32CubeU5_ROOT}/Drivers/CMSIS/Include")
find_path(STM32CubeU5_CMSIS_DEVICE_INCLUDE_DIR stm32u545xx.h
  HINTS "${STM32CubeU5_ROOT}/Drivers/CMSIS/Device/ST/STM32U5xx/Include")
find_path(STM32_USBX_ROOT common/core/inc/ux_api.h
  HINTS "${STM32CubeU5_ROOT}/Middlewares/ST/usbx")
find_path(STM32_EEPROM_EMULATION_ROOT core/eeprom_emul_core.h
  HINTS "${CMAKE_SOURCE_DIR}/third_party/STM32/stm32-util-eeprom-emulation")
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(STM32CubeU5 REQUIRED_VARS
  STM32CubeU5_HAL_INCLUDE_DIR STM32CubeU5_HAL_SOURCE_DIR
  STM32CubeU5_CMSIS_INCLUDE_DIR STM32CubeU5_CMSIS_DEVICE_INCLUDE_DIR
  STM32_USBX_ROOT STM32_EEPROM_EMULATION_ROOT)
