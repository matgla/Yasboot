/**
 * file_descriptors.hpp
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

#pragma once

#include <bitset>
#include <cstdint>

namespace yasboot::fs
{

class FileDescriptors
{
public:
  static FileDescriptors &get();

  int8_t allocate();
  void release(int8_t fd);

private:
  FileDescriptors();

  std::bitset<8> file_descriptors_;
};

} // namespace yasboot::fs
