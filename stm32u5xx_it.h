#ifndef __STM32U5xx_IT_H
#define __STM32U5xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#if defined(STM32U545xx)
void USB_IRQHandler(void);
#elif defined(STM32U585xx)
void OTG_FS_IRQHandler(void);
#elif defined(STM32U595xx)
void OTG_HS_IRQHandler(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __STM32U5xx_IT_H */
