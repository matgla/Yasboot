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

#include "hal/disk.hpp"

#include <cstring>

#include <hardware/regs/addressmap.h>

namespace yasboot::hal
{

/* This implementation is valid only for drive 0 (main memory) */

void Disk::read_sector(uint32_t sector_number, void *buffer) const
{
  constexpr std::size_t sector_size = 512;
  std::memcpy(buffer, reinterpret_cast<void *>(XIP_BASE + sector_number * sector_size),
              sector_size);
}

} // namespace yasboot::hal
