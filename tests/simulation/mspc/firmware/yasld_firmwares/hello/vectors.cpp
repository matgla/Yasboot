/**
 * vectors.cpp
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

#include "hardware/structs/scb.h"
#include <cstdint>

extern "C"
{
  void __unhandled_user_irq()
  {
    asm volatile("bkpt #0");
  }

  extern uint32_t app_stack_pointer;
}

using InterruptHandler = void (*)();

struct __attribute__((aligned(4))) VectorTable
{
  uint32_t stack_pointer;
  InterruptHandler handler[47];
};

void initialize_default_vector_table()
{
  static const VectorTable vectors = {
    .stack_pointer = app_stack_pointer,
    .handler = {
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq, &__unhandled_user_irq,
      &__unhandled_user_irq, &__unhandled_user_irq,
    }};
  scb_hw->vtor = reinterpret_cast<uintptr_t>(&vectors);
}
