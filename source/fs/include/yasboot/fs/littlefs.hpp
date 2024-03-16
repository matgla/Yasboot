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

#include "yasboot/fs/filesystem.hpp"

#include <lfs.h>

#include "yasboot/fs/disk_parameters.hpp"

namespace yasboot::fs
{

class LittleFS : public Filesystem
{
public:
  LittleFS(const DiskParameters &disk, ReadFromDisk read, WriteToDisk write,
           Erase erase, Sync sync);

  bool mount() override;

private:
  static int lfs_read_callback(const struct lfs_config *c, lfs_block_t block,
                               lfs_off_t off, void *buffer, lfs_size_t size);
  static int lfs_write_callback(const struct lfs_config *c, lfs_block_t block,
                                lfs_off_t off, const void *buffer, lfs_size_t size);
  static int lfs_erase_callback(const struct lfs_config *c, lfs_block_t block);
  static int lfs_sync_callback(const struct lfs_config *c);

  ReadFromDisk read_from_disk_;
  WriteToDisk write_to_disk_;
  Erase erase_;
  Sync sync_;

  const struct lfs_config lfs_config_;
  lfs_t lfs_;
};

} // namespace yasboot::fs
