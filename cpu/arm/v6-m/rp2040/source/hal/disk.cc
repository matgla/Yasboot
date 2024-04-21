module;

/**
 * disk.cpp
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

#include <cstdint>
#include <cstring>

#include <hardware/regs/addressmap.h>

export module hal.system.disk;

export namespace yasboot::hal::system
{

class Disk
{
public:
  void read_sector(uint32_t sector_number, void *buffer) const;

  template <typename T>
  T read(std::size_t address) const;
};

} // namespace yasboot::hal::system

/* This implementation is valid only for drive 0 (main memory) */

namespace yasboot::hal::system
{

void Disk::read_sector(uint32_t sector_number, void *buffer) const
{
  constexpr std::size_t sector_size = 512;
  std::memcpy(buffer,
              reinterpret_cast<void *>(XIP_BASE + sector_number * sector_size),
              sector_size);
}

template <typename T>
T Disk::read(std::size_t address) const
{
  T d;
  std::memcpy(&d, reinterpret_cast<void *>(XIP_BASE + address), sizeof(T));
  return d;
}

} // namespace yasboot::hal::system
