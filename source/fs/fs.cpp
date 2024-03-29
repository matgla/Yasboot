/**
 * mbr.cpp
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

#include "yasboot/mbr/mbr.hpp"

namespace yasboot
{

MbrParser::MbrParser(const hal::Disk &disk) : disk_{disk}, mbr_{}
{
  disk_.read_sector(0, &mbr_);
}

bool MbrParser::isValidMbr() const
{
  return mbr_.signature == 0xaa55;
}

const MbrHeader &MbrParser::mbr() const
{
  return mbr_;
}

const MbrPartitionEntry *MbrParser::getBootablePartition() const
{
  for (const auto &partition : mbr_.partitions)
  {
    if ((partition.status & static_cast<uint8_t>(0x80)) != 0)
    {
      return &partition;
    }
  }
  return nullptr;
}

} // namespace yasboot
