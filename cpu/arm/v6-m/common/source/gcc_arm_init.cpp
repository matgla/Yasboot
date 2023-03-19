/**
 * gcc_arm_init.cpp
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

#include <cstdint>

/*
 * IMPORTANT: We can't use any external library, i.e. libc, libstdc++
 *            They may contain constructors that will be started by this code
 */

extern "C"
{

  extern std::size_t __bss_start__;
  extern std::size_t __bss_end__;

  extern std::size_t __data_start__;
  extern std::size_t __data_end__;
  extern std::size_t __data_start_flash__;

  void _init()
  {
  }

  void __libc_init_array();
} // extern "C"

void initialize_bss()
{
  std::size_t *current_bss = &__bss_start__;
  while (current_bss != &__bss_end__)
  {
    *current_bss = 0;
    ++current_bss;
  }
}

void initialize_data()
{
  std::size_t *current_data = &__data_start__;
  std::size_t *current_data_in_flash = &__data_start_flash__;
  while (current_data != &__data_end__)
  {
    *current_data = *current_data_in_flash;
    ++current_data;
    ++current_data_in_flash;
  }
}

void initialize_constructors()
{
  __libc_init_array();
}
