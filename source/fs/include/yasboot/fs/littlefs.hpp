/**
 * littlefs.hpp
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

#include <cstddef>
#include <map>
#include <span>
#include <string_view>

#include <lfs.h>

#include <eul/functional/function.hpp>

#include "common/filesystem/disk_parameters.hpp"
#include "common/filesystem/filesystem.hpp"

namespace yasboot::fs
{

class LittleFS : public Filesystem
{
public:
  static int lfs_read_callback(const struct lfs_config *c, lfs_block_t block,
                               lfs_off_t off, void *buffer, lfs_size_t size);
  static int lfs_write_callback(const struct lfs_config *c, lfs_block_t block,
                                lfs_off_t off, const void *buffer, lfs_size_t size);
  static int lfs_erase_callback(const struct lfs_config *c, lfs_block_t block);
  static int lfs_sync_callback(const struct lfs_config *c);

  using ReadFromDisk =
    eul::function<int(std::size_t, std::span<uint8_t>), sizeof(void *)>;
  using WriteToDisk =
    eul::function<int(std::size_t, std::span<const uint8_t>), sizeof(void *)>;
  using Erase = eul::function<int(std::size_t), sizeof(void *)>;
  using Sync = eul::function<int(), sizeof(void *)>;

  LittleFS(const DiskParameters &disk, ReadFromDisk read, WriteToDisk write,
           Erase erase, Sync sync);

  bool mount() override;
  int open(std::string_view path, int flags) override;

  [[nodiscard]] bool has_fd(int fd) const override;
  [[nodiscard]] int read_file(int fd, std::span<uint8_t> buffer) override;
  [[nodiscard]] int close(int fd) override;

private:
  ReadFromDisk read_from_disk_;
  WriteToDisk write_to_disk_;
  Erase erase_;
  Sync sync_;

  struct lfs_config lfs_config_;
  lfs_t lfs_;

  std::map<int8_t, lfs_file_t> files_;
};

} // namespace yasboot::fs
