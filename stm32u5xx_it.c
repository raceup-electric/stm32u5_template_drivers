/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32u5xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32u5xx_it.h"

/* External variables ---------------------------------------------------------*/
#ifdef HAL_PCD_MODULE_ENABLED
#if defined(STM32U545xx)
extern PCD_HandleTypeDef hpcd_USB_DRD_FS;
#elif defined(STM32U585xx)
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
#elif defined(STM32U595xx)
extern PCD_HandleTypeDef hpcd_USB_OTG_HS;
#endif
#endif

/*           Cortex Processor Interruption and Exception Handlers             */
void NMI_Handler(void)
{
  while (1)
  {
  }
}

void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

#ifdef NOT_USING_FREERTOS_CALLBACKIMPL
void SVC_Handler(void)
{
}
#endif

void DebugMon_Handler(void)
{
}

#ifdef NOT_USING_FREERTOS_CALLBACKIMPL
void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}
#endif

/* STM32U5xx Peripheral Interrupt Handlers                                    */
#ifdef HAL_PCD_MODULE_ENABLED
#if defined(STM32U545xx)
void USB_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_DRD_FS);
}
#elif defined(STM32U585xx)
void OTG_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}
#elif defined(STM32U595xx)
void OTG_HS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
}
#endif
#endif
