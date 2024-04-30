module;
/**
 * cpu.cc
 *
 * Copyright (C) 2024 Mateusz Stadnik <matgla@live.com>
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

#include <cstdlib>

#include <hardware/structs/scb.h>

export module hal.cpu;

export namespace hal::cpu
{

std::size_t load_vector_table(std::size_t firmware)
{
  std::size_t *stack_pointer = reinterpret_cast<std::size_t *>(firmware);
  asm(R"(
    cpsid i
    msr msp, %0)"
      :
      : "r"(stack_pointer));
  scb_hw->vtor = firmware;

  return 4;
}

void jump(std::size_t address)
{
  void (**reset_handler)(void) = (void (**)(void))(address);
  (*reset_handler)();
}

} // namespace hal::cpu
