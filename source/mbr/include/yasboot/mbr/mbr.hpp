/**
 * mbr.hpp
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
#pragma once

#include <cstdint>

#include "hal/disk.hpp"

namespace yasboot
{

struct __attribute__((packed)) MbrPartitionEntry
{
  uint8_t status;
  uint8_t chs_start[3];
  uint8_t type;
  uint8_t chs_end[3];
  uint32_t lba_start;
  uint32_t number_of_sectors;
};

struct __attribute__((packed)) MbrHeader
{
  uint8_t bootloader[446];
  MbrPartitionEntry partitions[4];
  uint16_t signature;
};

class MbrParser
{
public:
  explicit MbrParser(const hal::Disk &disk);

  bool isValidMbr() const;
  const MbrHeader &mbr() const;

  // Searching for bootable partition
  // returns first if there are more than one
  const MbrPartitionEntry *getBootablePartition() const;

private:
  const hal::Disk &disk_;
  MbrHeader mbr_;
};

} // namespace yasboot
