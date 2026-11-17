#include "main.h"

void HAL_MspInit(void)
{

}

#ifdef HAL_PCD_MODULE_ENABLED
void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
{
  RCC_PeriphCLKInitTypeDef periph_clk_init = {0};

#if defined(STM32U545xx)
  if (hpcd->Instance != USB_DRD_FS)
  {
    return;
  }

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  periph_clk_init.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
  periph_clk_init.IclkClockSelection = RCC_CLK48CLKSOURCE_HSI48;
  if (HAL_RCCEx_PeriphCLKConfig(&periph_clk_init) != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableVddUSB();
  __HAL_RCC_USB_FS_CLK_ENABLE();
  HAL_NVIC_SetPriority(USB_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(USB_IRQn);
#elif defined(STM32U585xx)
  if (hpcd->Instance != USB_OTG_FS)
  {
    return;
  }

  periph_clk_init.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
  periph_clk_init.IclkClockSelection = RCC_CLK48CLKSOURCE_HSI48;
  if (HAL_RCCEx_PeriphCLKConfig(&periph_clk_init) != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableVddUSB();
  __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
  HAL_NVIC_SetPriority(OTG_FS_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
#elif defined(STM32U595xx)
  if (hpcd->Instance != USB_OTG_HS)
  {
    return;
  }

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  periph_clk_init.PeriphClockSelection = RCC_PERIPHCLK_USBPHY;
  periph_clk_init.UsbPhyClockSelection = RCC_USBPHYCLKSOURCE_HSE;
  if (HAL_RCCEx_PeriphCLKConfig(&periph_clk_init) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSCFG_SetOTGPHYReferenceClockSelection(SYSCFG_OTG_HS_PHY_CLK_SELECT_1);
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableVddUSB();
  HAL_PWREx_EnableUSBHSTranceiverSupply();
  __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
  __HAL_RCC_USBPHYC_CLK_ENABLE();
  HAL_SYSCFG_EnableOTGPHY(SYSCFG_OTG_HS_PHY_ENABLE);
  HAL_NVIC_SetPriority(OTG_HS_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
#else
#error "Unsupported STM32U5 device"
#endif
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)
{
#if defined(STM32U545xx)
  if (hpcd->Instance != USB_DRD_FS)
  {
    return;
  }

  __HAL_RCC_USB_FS_CLK_DISABLE();
  HAL_PWREx_DisableVddUSB();
  HAL_NVIC_DisableIRQ(USB_IRQn);
#elif defined(STM32U585xx)
  if (hpcd->Instance != USB_OTG_FS)
  {
    return;
  }

  __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
  HAL_PWREx_DisableVddUSB();
  HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
#elif defined(STM32U595xx)
  if (hpcd->Instance != USB_OTG_HS)
  {
    return;
  }

  __HAL_RCC_USBPHYC_CLK_DISABLE();
  __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
  HAL_PWREx_DisableUSBHSTranceiverSupply();
  HAL_PWREx_DisableVddUSB();
  HAL_NVIC_DisableIRQ(OTG_HS_IRQn);
#endif
}
#endif
