/**
 * configure_clocks.cpp
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

#include "arm/v6-m/rp2040/configure_clocks.hpp"

#include <hardware/regs/clocks.h>
#include <hardware/regs/pll.h>
#include <hardware/regs/resets.h>
#include <hardware/regs/xosc.h>
#include <hardware/resets.h>
#include <hardware/structs/clocks.h>
#include <hardware/structs/pll.h>
#include <hardware/structs/xosc.h>

#include "board_setup/xosc.hpp"

namespace
{

struct rp2040_vco
{
  constexpr static uint64_t min_frequency = 750_MHz;
  constexpr static uint64_t max_frequency = 1600_MHz;
};

static uint32_t configured_clocks[CLK_COUNT];

bool has_clock_glitchless_multiplexer(clock_index index)
{
  return index == clk_sys || index == clk_ref;
}

} // namespace

void initialize_xosc()
{
  // This supports both XIN driven directly by some source (up to 50MHz)
  // but for external quartz limit is 15MHz
  static_assert(board_config::xosc_frequency >= 1_MHz && board_config::xosc_frequency <= 50_MHz,
                "Invalid external oscillator used");
  // Magical formula from RP2040 datasheet
  constexpr uint64_t startup_delay = board_config::xosc_frequency / (256 * 1000);
  static_assert(startup_delay <= (1 << 13), "startup delay must fit within 14-bit");

  xosc_hw->ctrl = XOSC_CTRL_FREQ_RANGE_VALUE_1_15MHZ;
  xosc_hw->startup = startup_delay;

  hw_set_bits(&xosc_hw->ctrl, XOSC_CTRL_ENABLE_VALUE_ENABLE << XOSC_CTRL_ENABLE_LSB);
  while (!(xosc_hw->status & XOSC_STATUS_STABLE_BITS))
  {
  }
}

void initialize_pll(pll_hw_t *pll, uint32_t refdiv, uint32_t vco_frequency, uint32_t post_div1,
                    uint32_t post_div2)
{
  const uint64_t ref_mhz = board_config::xosc_frequency / refdiv;
  if (vco_frequency < rp2040_vco::min_frequency || vco_frequency > rp2040_vco::max_frequency)
  {
    // TODO: add panic here
    return;
  }

  const uint64_t fbdiv = vco_frequency / ref_mhz;

  if (fbdiv < 16 || fbdiv > 320)
  {
    // TODO: add panic here
    return;
  }

  if (post_div1 < 1 || post_div1 > 7)
  {
    // TODO: add panic here
    return;
  }

  if (post_div2 < 1 || post_div2 > 7)
  {
    // TODO: add panic here
    return;
  }

  if (ref_mhz > (vco_frequency / 16))
  {
    // TODO: add panic here
    return;
  }

  const uint32_t pdiv = (post_div1 << PLL_PRIM_POSTDIV1_LSB) | (post_div2 << PLL_PRIM_POSTDIV2_LSB);

  if ((pll->cs & PLL_CS_LOCK_BITS) && (refdiv == (pll->cs & PLL_CS_REFDIV_BITS)) &&
      (fbdiv == (pll->fbdiv_int & PLL_FBDIV_INT_BITS)) &&
      (pdiv == (pll->prim & (PLL_PRIM_POSTDIV1_LSB | PLL_PRIM_POSTDIV2_LSB))))
  {
    // PLL is already configured
    return;
  }

  const uint32_t reset_pll =
      (pll_sys_hw == pll) ? RESETS_RESET_PLL_SYS_BITS : RESETS_RESET_PLL_USB_BITS;

  reset_block(reset_pll);
  unreset_block_wait(reset_pll);

  pll->cs = refdiv;
  pll->fbdiv_int = static_cast<uint32_t>(fbdiv);
  const uint32_t poweron = PLL_PWR_PD_BITS | PLL_PWR_VCOPD_BITS;
  hw_clear_bits(&pll->pwr, poweron);
  while (!(pll->cs & PLL_CS_LOCK_BITS))
  {
  }
  pll->prim = pdiv;
  hw_clear_bits(&pll->pwr, PLL_PWR_POSTDIVPD_BITS);
}

void configure_clock(clock_index index, uint32_t source, uint32_t auxiliary_source,
                     uint64_t source_frequency, uint64_t frequency)
{
  if (frequency > source_frequency)
  {
    // TODO: panic
    return;
  }

  const uint64_t div = (source_frequency << 8) / frequency;
  clock_hw_t *clock = &clocks_hw->clk[index];

  if (div > clock->div)
  {
    clock->div = static_cast<uint32_t>(div);
  }

  if (has_clock_glitchless_multiplexer(index) &&
      source == CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX)
  {
    hw_clear_bits(&clock->ctrl, CLOCKS_CLK_REF_CTRL_SRC_BITS);
    while (!(clock->selected & 0x1))
    {
    }
  }
  else
  {
    hw_clear_bits(&clock->ctrl, CLOCKS_CLK_GPOUT0_CTRL_ENABLE_BITS);
    if (configured_clocks[index] != 0)
    {
      uint32_t delay = configured_clocks[clk_sys] / configured_clocks[index] + 1;
      asm volatile(".syntax unified\n"
                   "1:\n"
                   "subs %0, #1\n"
                   "bne 1b\n"
                   : "+r"(delay));
    }
  }

  hw_write_masked(&clock->ctrl, (auxiliary_source << CLOCKS_CLK_SYS_CTRL_AUXSRC_LSB),
                  CLOCKS_CLK_SYS_CTRL_AUXSRC_BITS);

  if (has_clock_glitchless_multiplexer(index))
  {
    hw_write_masked(&clock->ctrl, source << CLOCKS_CLK_REF_CTRL_SRC_LSB,
                    CLOCKS_CLK_REF_CTRL_SRC_BITS);
    while (!(clock->selected & (1u << source)))
    {
    }
  }
  hw_set_bits(&clock->ctrl, CLOCKS_CLK_GPOUT0_CTRL_ENABLE_BITS);
  clock->div = static_cast<uint32_t>(div);

  configured_clocks[index] = static_cast<uint32_t>((source_frequency << 8) / div);
}

void initialize_clocks()
{
  clocks_hw->resus.ctrl = 0;
  initialize_xosc();

  hw_clear_bits(&clocks_hw->clk[clk_sys].ctrl, CLOCKS_CLK_SYS_CTRL_SRC_BITS);
  while (clocks_hw->clk[clk_sys].selected != 0x1)
  {
  }
  hw_clear_bits(&clocks_hw->clk[clk_ref].ctrl, CLOCKS_CLK_REF_CTRL_SRC_BITS);
  while (clocks_hw->clk[clk_ref].selected != 0x1)
  {
  }

  /* 125 MHz */
  initialize_pll(pll_sys_hw, 1, 1500_MHz, 6, 2);

  configure_clock(clk_ref, CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC, 0, 12_MHz, 12_MHz);
  configure_clock(clk_sys, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                  CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, 125_MHz, 125_MHz);
  configure_clock(clk_peri, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS, 125_MHz, 125_MHz);
}

uint32_t get_clock(clock_index clock)
{
  return configured_clocks[clock];
}
