#pragma once

#include <stddef.h>
#include <stdint.h>

#include "stm32u5xx_hal.h"

#if defined(STM32U545xx)
#define RU_STM32_USB_INSTANCE USB_DRD_FS
#define RU_STM32_USB_HANDLE hpcd_USB_DRD_FS
#elif defined(STM32U585xx)
#define RU_STM32_USB_INSTANCE USB_OTG_FS
#define RU_STM32_USB_HANDLE hpcd_USB_OTG_FS
#elif defined(STM32U595xx)
#define RU_STM32_USB_INSTANCE USB_OTG_HS
#define RU_STM32_USB_HANDLE hpcd_USB_OTG_HS
#else
#error "Unsupported STM32U5 USB peripheral"
#endif

#ifdef __cplusplus
extern "C" {
#endif

int ru_stm32_usb_cdc_start(void);
int ru_stm32_usb_cdc_init(void);
int ru_stm32_usb_cdc_stop(void);
void ru_stm32_usb_cdc_tasks_run(void);
int ru_stm32_usb_cdc_is_configured(void);
int ru_stm32_usb_cdc_raw_try_write(const uint8_t* data, size_t len);
int ru_stm32_usb_cdc_write(const uint8_t* data, size_t len, uint64_t timeout_us);
int ru_stm32_usb_cdc_read(uint8_t* data, size_t len, uint64_t timeout_us);
int ru_stm32_usb_cdc_try_write(const uint8_t* data, size_t len);
uint32_t ru_stm32_usb_cdc_service_task_priority_get(void);
uint32_t ru_stm32_usb_cdc_service_task_period_get(void);

extern PCD_HandleTypeDef RU_STM32_USB_HANDLE;

#ifdef __cplusplus
}
#endif
