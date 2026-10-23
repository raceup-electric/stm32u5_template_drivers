#pragma once
#include "common.hpp"

#if !defined(STM32U545xx)
#error "This backend targets STM32U545xx (STM32U545CET6Q)."
#endif

namespace ru::driver::stm32u5xx {
// Implement in the board project. Called after HAL_Init and before drivers init.
// Configure supply, system/peripheral clocks and peripheral MSP callbacks there.
// No weak default: a missing board configuration must fail at link time.
result configure_board() noexcept;
}

// With RU_STM32U5XX_EXTERNAL_DMA_IRQ_HANDLERS, forward ADC DMA IRQs here.
#include "stm32u5xx_hal.h"
namespace ru::driver {
void adc_dma_channel_irq(DMA_Channel_TypeDef* channel) noexcept;
}
