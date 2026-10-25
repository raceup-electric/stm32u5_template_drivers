#include "stm32u5xx_hal.h"
extern PCD_HandleTypeDef hpcd_USB_DRD_FS;
void USB_IRQHandler(void) {
  HAL_PCD_IRQHandler(&hpcd_USB_DRD_FS);
}

/* Kept as an alias for projects whose startup vector uses the peripheral name. */
void USB_DRD_FS_IRQHandler(void) {
  USB_IRQHandler();
}
