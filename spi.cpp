#include <array>
#include <algorithm>
#include <limits>

#include "spi.hpp"

#include "mapping.hpp"
#include "stm_common.hpp"
#include "stm32u5xx_hal_spi.h"

using namespace ru::driver;

namespace ru::driver {
namespace {
const stm32u5xx::cfg::spi_config* config_for(const SpiId id) noexcept {
  switch (id) {
#define RU_STM32U5XX_SPI_CONFIG(name, config) \
    case SpiId::name:                         \
      return &config;
    RU_STM32U5XX_SPI_MAP(RU_STM32U5XX_SPI_CONFIG)
#undef RU_STM32U5XX_SPI_CONFIG
    default:
      return nullptr;
  }
}

SPI_HandleTypeDef& handle_for(const SpiId id) noexcept {
  static std::array<SPI_HandleTypeDef, static_cast<size_t>(SpiId::COUNT)> handles{};
  return handles[static_cast<size_t>(id)];
}

}  // namespace

result Spi::start() noexcept {
  return result::OK;
}

result Spi::init() noexcept {
  const auto* const p_config = config_for(m_id);
  if (p_config == nullptr) {
    return result::UNRECOVERABLE_ERROR;
  }

  auto& handle = handle_for(m_id);
  if (handle.Instance != nullptr) return result::OK;
  if (p_config->init.DataSize != SPI_DATASIZE_8BIT) return result::UNRECOVERABLE_ERROR;
  handle.Instance = p_config->instance();
  handle.Init = p_config->init;
  const auto status = HAL_SPI_Init(&handle);
  if (status != HAL_OK) handle = {};
  return from_hal_status(status);
}

result Spi::stop() noexcept {
  const auto* const p_config = config_for(m_id);
  if (p_config == nullptr) {
    return result::UNRECOVERABLE_ERROR;
  }

  auto& handle = handle_for(m_id);
  if (handle.Instance == nullptr) return result::OK;
  const auto status = HAL_SPI_DeInit(&handle);
  if (status == HAL_OK) handle = {};
  return from_hal_status(status);
}

result Spi::transfer(const uint8_t* tx_data, uint8_t* rx_data, const size_t len,
                     const Timestamp timeout_uS) noexcept {
  if (len != 0U && (tx_data == nullptr || rx_data == nullptr)) {
    return result::UNRECOVERABLE_ERROR;
  }

  const auto* const p_config = config_for(m_id);
  if (p_config == nullptr) {
    return result::UNRECOVERABLE_ERROR;
  }

  auto& handle = handle_for(m_id);
  if (handle.Instance == nullptr) return result::UNRECOVERABLE_ERROR;
  const uint32_t timeout_ms = std::max<uint32_t>(us_to_ms(timeout_uS), 1U);
  const uint32_t start_ms = HAL_GetTick();
  size_t remaining = len;
  while (remaining != 0U) {
    const uint16_t chunk = static_cast<uint16_t>(std::min<size_t>(
        remaining, static_cast<size_t>(std::numeric_limits<uint16_t>::max())));
    const auto elapsed = static_cast<uint32_t>(HAL_GetTick() - start_ms);
    if (elapsed > timeout_ms) return result::RECOVERABLE_ERROR;
    const auto status = HAL_SPI_TransmitReceive(&handle, tx_data, rx_data, chunk, timeout_ms - elapsed);
    if (status != HAL_OK) {
      return from_hal_status(status);
    }

    remaining -= chunk;
    tx_data += chunk;
    rx_data += chunk;
  }

  return result::OK;
}
}  // namespace ru::driver
