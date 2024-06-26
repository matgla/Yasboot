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

#include <iostream>

#include <eul/utils/unused.hpp>

namespace yasboot::hal
{

template <int uart_id>
Uart<uart_id>::Uart(uint32_t baudrate)
{
  UNUSED(baudrate);
}

template <int uart_id>
std::size_t Uart<uart_id>::write(const std::string_view &data)
{
  std::cout << data;
  return data.size();
}

template class Uart<0>;

} // namespace yasboot::hal
