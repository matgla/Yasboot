/**
 * uart.cpp
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

#include "hal/uart.hpp"

#include "arm/v6-m/rp2040/configure_clocks.hpp"

#include <hardware/regs/io_bank0.h>
#include <hardware/regs/resets.h>
#include <hardware/regs/uart.h>
#include <hardware/resets.h>
#include <hardware/structs/clocks.h>
#include <hardware/structs/iobank0.h>
#include <hardware/structs/padsbank0.h>
#include <hardware/structs/uart.h>

namespace yasboot::hal
{

template <int uart_id> void uart_reset()
{
  if constexpr (uart_id == 0)
  {
    reset_block(RESETS_RESET_UART0_BITS);
  }
  else
  {
    reset_block(RESETS_RESET_UART1_BITS);
  }
}

template <int uart_id> void uart_unreset()
{
  if constexpr (uart_id == 0)
  {
    unreset_block(RESETS_RESET_UART0_BITS);
  }
  else
  {
    unreset_block(RESETS_RESET_UART1_BITS);
  }
}

template <int uart_id> uart_hw_t *get_uart_instance()
{
  if constexpr (uart_id == 0)
    return uart0_hw;
  else
    return uart1_hw;
}

void set_format(uart_hw_t *uart, uint32_t data_bits, uint32_t stop_bits, Parity parity)
{
  hw_write_masked(&uart->lcr_h,
                  ((data_bits - 5u) << UART_UARTLCR_H_WLEN_LSB) |
                      ((stop_bits - 1u) << UART_UARTLCR_H_STP2_LSB) |
                      (bool_to_bit(parity != Parity::None) << UART_UARTLCR_H_PEN_LSB) |
                      (bool_to_bit(parity == Parity::Even) << UART_UARTLCR_H_EPS_LSB),
                  UART_UARTLCR_H_WLEN_BITS | UART_UARTLCR_H_STP2_BITS | UART_UARTLCR_H_PEN_BITS |
                      UART_UARTLCR_H_EPS_BITS);
}

void configure_gpio(int gpio)
{
  constexpr uint32_t gpio_uart_function = 2;
  hw_write_masked(&padsbank0_hw->io[gpio], PADS_BANK0_GPIO0_IE_BITS,
                  PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS);
  iobank0_hw->io[gpio].ctrl = gpio_uart_function << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
}

template <int uart_id> void configure_uart_gpio()
{
  if constexpr (uart_id == 0)
  {
    configure_gpio(0);
    configure_gpio(1);
  }
}

template <int uart_id> Uart<uart_id>::Uart(uint32_t baudrate)
{
  uart_reset<uart_id>();
  uart_unreset<uart_id>();
  set_baudrate(baudrate);
  set_format(get_uart_instance<uart_id>(), 8, 1, Parity::None);

  get_uart_instance<uart_id>()->cr =
      UART_UARTCR_UARTEN_BITS | UART_UARTCR_TXE_BITS | UART_UARTCR_RXE_BITS;
  hw_set_bits(&get_uart_instance<uart_id>()->lcr_h, UART_UARTLCR_H_FEN_BITS);
  get_uart_instance<uart_id>()->dmacr = UART_UARTDMACR_TXDMAE_BITS | UART_UARTDMACR_RXDMAE_BITS;
  configure_uart_gpio<uart_id>();
}

template <int uart_id> void Uart<uart_id>::set_baudrate(uint32_t baudrate)
{
  const uint32_t baudrate_div = (8 * get_clock(clk_peri) / baudrate);
  uint32_t baudrate_ibrd = baudrate_div >> 7;
  uint32_t baudrate_fbrd;

  if (baudrate_ibrd == 0)
  {
    baudrate_ibrd = 1;
    baudrate_fbrd = 0;
  }
  else if (baudrate_ibrd >= 65535)
  {
    baudrate_ibrd = 65535;
    baudrate_fbrd = 0;
  }
  else
  {
    baudrate_fbrd = ((baudrate_div & 0x7f) + 1) / 2;
  }

  get_uart_instance<uart_id>()->ibrd = baudrate_ibrd;
  get_uart_instance<uart_id>()->fbrd = baudrate_fbrd;

  hw_set_bits(&get_uart_instance<uart_id>()->lcr_h, 0);
}

bool is_uart_writable(uart_hw_t *uart)
{
  return !(uart->fr & UART_UARTFR_TXFF_BITS);
}

template <int uart_id> std::size_t Uart<uart_id>::write(const std::string_view &data)
{
  uart_hw_t *uart = get_uart_instance<uart_id>();
  for (char byte : data)
  {
    while (!is_uart_writable(uart))
    {
    }
    uart->dr = byte;
  }
  return data.size();
}

template class Uart<0>;
template class Uart<1>;

} // namespace yasboot::hal
