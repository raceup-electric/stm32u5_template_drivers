#pragma once

#include "common.hpp"
#include <cstdint>

#include "stm32u5xx_hal.h"

namespace ru::driver {

// GENERAL STUFF

constexpr result from_hal_status(const HAL_StatusTypeDef status) noexcept {
  switch (status) {
    case HAL_OK:
      return result::OK;
    case HAL_BUSY:
    case HAL_TIMEOUT:
      return result::RECOVERABLE_ERROR;
    case HAL_ERROR:
    default:
      return result::UNRECOVERABLE_ERROR;
  }
}

constexpr uint32_t us_to_ms(const Timestamp uS) noexcept {
  const auto ms = uS / 1000U + (uS % 1000U != 0U);
  return ms >= HAL_MAX_DELAY ? HAL_MAX_DELAY - 1U : static_cast<uint32_t>(ms);
}

// GPIO BEACUSE YES

static inline void enable_gpio_clock(const GPIO_TypeDef* port) noexcept {
  switch (reinterpret_cast<uintptr_t>(port)) {
#if defined(GPIOA_BASE)
    case GPIOA_BASE:
      __HAL_RCC_GPIOA_CLK_ENABLE();
      break;
#endif
#if defined(GPIOB_BASE)
    case GPIOB_BASE:
      __HAL_RCC_GPIOB_CLK_ENABLE();
      break;
#endif
#if defined(GPIOC_BASE)
    case GPIOC_BASE:
      __HAL_RCC_GPIOC_CLK_ENABLE();
      break;
#endif
#if defined(GPIOD_BASE)
    case GPIOD_BASE:
      __HAL_RCC_GPIOD_CLK_ENABLE();
      break;
#endif
#if defined(GPIOE_BASE)
    case GPIOE_BASE:
      __HAL_RCC_GPIOE_CLK_ENABLE();
      break;
#endif
#if defined(GPIOF_BASE)
    case GPIOF_BASE:
      __HAL_RCC_GPIOF_CLK_ENABLE();
      break;
#endif
#if defined(GPIOG_BASE)
    case GPIOG_BASE:
      __HAL_RCC_GPIOG_CLK_ENABLE();
      break;
#endif
#if defined(GPIOH_BASE)
    case GPIOH_BASE:
      __HAL_RCC_GPIOH_CLK_ENABLE();
      break;
#endif
#if defined(GPIOI_BASE)
    case GPIOI_BASE:
      __HAL_RCC_GPIOI_CLK_ENABLE();
      break;
#endif
    default:
      break;
  }
}

inline void init_pin(GPIO_TypeDef* const p_port, const uint32_t pin,
                     const uint32_t mode = GPIO_MODE_INPUT,
                     const uint32_t pull = GPIO_NOPULL,
                     const uint32_t speed = GPIO_SPEED_FREQ_LOW,
                     const uint32_t alternate = 0) noexcept {
  enable_gpio_clock(p_port);

  GPIO_InitTypeDef init{};
  init.Pin = pin;
  init.Mode = mode;
  init.Pull = pull;
  init.Speed = speed;
  init.Alternate = alternate;
  HAL_GPIO_Init(p_port, &init);
}

inline void init_pin(GPIO_TypeDef* const p_port, const GPIO_InitTypeDef& init) noexcept {
  enable_gpio_clock(p_port);
  auto init_copy = init;
  HAL_GPIO_Init(p_port, &init_copy);
}

// TIMER BEACUSE YES

static inline void enable_tim_clock(const TIM_TypeDef* instance) noexcept {
  switch (reinterpret_cast<uintptr_t>(instance)) {
#if defined(TIM1_BASE)
    case TIM1_BASE:
      __HAL_RCC_TIM1_CLK_ENABLE();
      break;
#endif
#if defined(TIM2_BASE)
    case TIM2_BASE:
      __HAL_RCC_TIM2_CLK_ENABLE();
      break;
#endif
#if defined(TIM3_BASE)
    case TIM3_BASE:
      __HAL_RCC_TIM3_CLK_ENABLE();
      break;
#endif
#if defined(TIM4_BASE)
    case TIM4_BASE:
      __HAL_RCC_TIM4_CLK_ENABLE();
      break;
#endif
#if defined(TIM5_BASE)
    case TIM5_BASE:
      __HAL_RCC_TIM5_CLK_ENABLE();
      break;
#endif
#if defined(TIM6_BASE)
    case TIM6_BASE:
      __HAL_RCC_TIM6_CLK_ENABLE();
      break;
#endif
#if defined(TIM7_BASE)
    case TIM7_BASE:
      __HAL_RCC_TIM7_CLK_ENABLE();
      break;
#endif
#if defined(TIM8_BASE)
    case TIM8_BASE:
      __HAL_RCC_TIM8_CLK_ENABLE();
      break;
#endif
#if defined(TIM12_BASE)
    case TIM12_BASE:
      __HAL_RCC_TIM12_CLK_ENABLE();
      break;
#endif
#if defined(TIM13_BASE)
    case TIM13_BASE:
      __HAL_RCC_TIM13_CLK_ENABLE();
      break;
#endif
#if defined(TIM14_BASE)
    case TIM14_BASE:
      __HAL_RCC_TIM14_CLK_ENABLE();
      break;
#endif
#if defined(TIM15_BASE)
    case TIM15_BASE:
      __HAL_RCC_TIM15_CLK_ENABLE();
      break;
#endif
#if defined(TIM16_BASE)
    case TIM16_BASE:
      __HAL_RCC_TIM16_CLK_ENABLE();
      break;
#endif
#if defined(TIM17_BASE)
    case TIM17_BASE:
      __HAL_RCC_TIM17_CLK_ENABLE();
      break;
#endif
    default:
      break;
  }
}

static inline void enable_lptim_clock(const LPTIM_TypeDef* instance) noexcept {
  switch (reinterpret_cast<uintptr_t>(instance)) {
#if defined(LPTIM1_BASE)
    case LPTIM1_BASE:
      __HAL_RCC_LPTIM1_CLK_ENABLE();
      break;
#endif
#if defined(LPTIM2_BASE)
    case LPTIM2_BASE:
      __HAL_RCC_LPTIM2_CLK_ENABLE();
      break;
#endif
#if defined(LPTIM3_BASE)
    case LPTIM3_BASE:
      __HAL_RCC_LPTIM3_CLK_ENABLE();
      break;
#endif
#if defined(LPTIM4_BASE)
    case LPTIM4_BASE:
      __HAL_RCC_LPTIM4_CLK_ENABLE();
      break;
#endif
#if defined(LPTIM5_BASE)
    case LPTIM5_BASE:
      __HAL_RCC_LPTIM5_CLK_ENABLE();
      break;
#endif
    default:
      break;
  }
}

inline bool is_apb2_timer(const TIM_TypeDef* instance) noexcept {
  return instance == TIM1 || instance == TIM8 || instance == TIM15 ||
         instance == TIM16 || instance == TIM17;
}

inline uint32_t timer_input_clock_hz(const TIM_TypeDef* instance) noexcept {
  RCC_ClkInitTypeDef clock_config{};
  uint32_t flash_latency = 0U;
  HAL_RCC_GetClockConfig(&clock_config, &flash_latency);

  const auto pclk_hz = is_apb2_timer(instance) ? HAL_RCC_GetPCLK2Freq() : HAL_RCC_GetPCLK1Freq();
  if (pclk_hz == 0U) {
    return SystemCoreClock == 0U ? 64000000U : SystemCoreClock;
  }

  const auto divider = is_apb2_timer(instance) ? clock_config.APB2CLKDivider
                                               : clock_config.APB1CLKDivider;
  return divider == RCC_HCLK_DIV1 ? pclk_hz : (pclk_hz * 2U);
}

inline uint32_t timer_auto_reload_max(const TIM_TypeDef* instance) noexcept {
  return IS_TIM_32B_COUNTER_INSTANCE(instance) ? 0xFFFFFFFFU : 0xFFFFU;
}

}  // namespace ru::driver
