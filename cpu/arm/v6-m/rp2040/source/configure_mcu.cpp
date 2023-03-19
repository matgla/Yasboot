/**
 * configure_mcu.cpp
 *
 * Copyright (C) 2023 Mateusz Stadnik <matgla@live.com>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version
 * 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General
 * Public License along with this program. If not, see
 * <https://www.gnu.org/licenses/>.
 */

#include "arm/v6-m/common/gcc_arm_init.hpp"

#include "arm/v6-m/rp2040/configure_clocks.hpp"
#include "hardware/resets.h"

#include <hardware/regs/resets.h>

extern "C"
{
  void system_init()
  {
    reset_block(~(RESETS_RESET_IO_QSPI_BITS | RESETS_RESET_PADS_QSPI_BITS |
                  RESETS_RESET_PLL_USB_BITS | RESETS_RESET_USBCTRL_BITS | RESETS_RESET_SYSCFG_BITS |
                  RESETS_RESET_PLL_SYS_BITS));

    unreset_block_wait(RESETS_RESET_BITS &
                       ~(RESETS_RESET_ADC_BITS | RESETS_RESET_RTC_BITS | RESETS_RESET_SPI0_BITS |
                         RESETS_RESET_SPI1_BITS | RESETS_RESET_UART0_BITS |
                         RESETS_RESET_UART1_BITS | RESETS_RESET_USBCTRL_BITS));
    initialize_data();
    initialize_bss();
    initialize_constructors();
    initialize_clocks();
  }
}
