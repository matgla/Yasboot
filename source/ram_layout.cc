module;
/**
 * ram_layout.cc
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

#include <cmath>
#include <cstdint>

export module yasboot.ram_layout;

extern "C"
{
  extern char YASBOOT_RAM_APP;
  extern char YASBOOT_RAM_APP_SIZE;
}

export namespace yasboot
{

struct ApplicationData
{
  void *lot_address;
  uint32_t lot_size;
  void *init_address;
  uint32_t init_size;
  void *data_address;
  uint32_t data_size;
  void *modules_root;
};

void *application_ram_start = &YASBOOT_RAM_APP;
std::size_t application_ram_size =
  reinterpret_cast<std::size_t>(&YASBOOT_RAM_APP_SIZE);

std::size_t pointer_offset(const void *a, const void *b)
{
  return std::abs(static_cast<const char *>(a) - static_cast<const char *>(b));
}

} // namespace yasboot
