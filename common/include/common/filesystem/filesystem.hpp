/**
 * filesystem.hpp
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

#include <eul/functional/function.hpp>

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

namespace yasboot::fs
{

class Filesystem
{
public:
  virtual ~Filesystem() = default;

  using ReadFromDisk =
    eul::function<int(std::size_t address, std::span<uint8_t> buffer),
                  sizeof(void *)>;
  using WriteToDisk =
    eul::function<int(std::size_t address, std::span<const uint8_t> buffer),
                  sizeof(void *)>;
  using Erase = eul::function<int(std::size_t block), sizeof(void *)>;
  using Sync = eul::function<int(), sizeof(void *)>;

  virtual bool mount() = 0;
  virtual int open(std::string_view path, int flags) = 0;
  virtual int close(int fd) = 0;

  [[nodiscard]] virtual bool has_fd(int fd) const = 0;
  [[nodiscard]] virtual int read_file(int fd, std::span<uint8_t> buffer) = 0;
};

} // namespace yasboot::fs
