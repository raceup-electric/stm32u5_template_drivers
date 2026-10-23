#include "opaque_common.hpp"
#include "stm_common.hpp"
#include "board_support.hpp"

namespace ru::driver {
result opaque_common::start() const noexcept {
  const auto status = HAL_Init();
  if (status != HAL_OK) {
    return from_hal_status(status);
  }
  return stm32u5xx::configure_board();
}
}  // namespace ru::driver
