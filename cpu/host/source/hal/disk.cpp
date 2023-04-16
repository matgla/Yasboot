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

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

namespace yasboot::hal
{

namespace
{
std::vector<uint8_t> disk; // NOLINT

bool initialize_disk()
{
  if (const char *env_p = std::getenv("DISK"))
  {
    std::filesystem::path disk_path(env_p);
    if (std::filesystem::exists(disk_path))
    {
      std::ifstream file(disk_path, std::ios::binary);
      disk = std::vector<uint8_t>((std::istreambuf_iterator<char>(file)),
                                  std::istreambuf_iterator<char>());
      std::cerr << "<HOST_INFO> successfuly readed disk: " << disk_path.native()
                << ", with size: " << disk.size() << std::endl;
      return true;
    }
    std::cerr << "<HOST_ERROR> unable to open file: " << disk_path.native() << std::endl;
  }
  return false;
}

} // namespace

void Disk::read_sector(uint32_t sector_number, void *buffer) const
{
  static const bool readed = initialize_disk();
  if (!readed)
  {
    std::cerr << "<HOST_ERROR> Disk not provided" << std::endl;
    exit(-1);
  }
  constexpr std::size_t sector_size = 512;
  if ((sector_number + 1) * sector_size < disk.size())
  {
    std::cerr << "<HOST_INFO> reading from sector: " << sector_number << std::endl;
    std::memcpy(buffer, &disk.at(sector_number * sector_size), sector_size);
  }
}

} // namespace yasboot::hal
