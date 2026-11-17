#include "opaque_common.hpp"

#include "stm_common.hpp"

using namespace ru::driver;

namespace ru::driver {
static result configure_system_clock() noexcept;

result opaque_common::start() const noexcept {
  const auto hal_status = HAL_Init();
  if (hal_status != HAL_OK) {
    return from_hal_status(hal_status);
  }

  return configure_system_clock();
}

static result configure_system_clock() noexcept {
#if defined(STM32U595xx)
  constexpr uint32_t power_supply = PWR_SMPS_SUPPLY;
#else
  constexpr uint32_t power_supply = PWR_LDO_SUPPLY;
#endif
  if (HAL_PWREx_ConfigSupply(power_supply) != HAL_OK ||
      HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
    return result::RECOVERABLE_ERROR;
  }

  RCC_OscInitTypeDef oscillator{};
  oscillator.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48;
#if defined(STM32U595xx)
  // The U595 USB HS PHY uses HSE as its reference clock. Keep it running for
  // the peripheral even though SYSCLK remains sourced from HSI/PLL1.
  oscillator.OscillatorType |= RCC_OSCILLATORTYPE_HSE;
  oscillator.HSEState = RCC_HSE_ON;
#endif
  oscillator.HSIState = RCC_HSI_ON;
  oscillator.HSI48State = RCC_HSI48_ON;
  oscillator.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  oscillator.PLL.PLLState = RCC_PLL_ON;
  oscillator.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  oscillator.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV1;
  oscillator.PLL.PLLM = 4U;
  oscillator.PLL.PLLN = 80U;
  oscillator.PLL.PLLP = 2U;
  oscillator.PLL.PLLQ = 2U;
  oscillator.PLL.PLLR = 2U;
  oscillator.PLL.PLLRGE = RCC_PLLVCIRANGE_0;
  oscillator.PLL.PLLFRACN = 0U;
  if (HAL_RCC_OscConfig(&oscillator) != HAL_OK) {
    return result::RECOVERABLE_ERROR;
  }

  RCC_ClkInitTypeDef clocks{};
  clocks.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                    RCC_CLOCKTYPE_PCLK3;
  clocks.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clocks.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clocks.APB1CLKDivider = RCC_HCLK_DIV1;
  clocks.APB2CLKDivider = RCC_HCLK_DIV1;
  clocks.APB3CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&clocks, FLASH_LATENCY_4) != HAL_OK) {
    return result::RECOVERABLE_ERROR;
  }

  RCC_PeriphCLKInitTypeDef peripheral{};
  peripheral.PeriphClockSelection = RCC_PERIPHCLK_ADCDAC | RCC_PERIPHCLK_FDCAN1;
  peripheral.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_HSI;
  peripheral.Fdcan1ClockSelection = RCC_FDCAN1CLKSOURCE_PLL1;
  if (HAL_RCCEx_PeriphCLKConfig(&peripheral) != HAL_OK) {
    return result::RECOVERABLE_ERROR;
  }

  return result::OK;
}
}  // namespace ru::driver
