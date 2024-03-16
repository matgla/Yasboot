/**
 * littlefs.cpp
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

#include "yasboot/fs/littlefs.hpp"

#include <cstdio>

namespace yasboot::fs
{

int LittleFS::lfs_read_callback(const struct lfs_config *c, lfs_block_t block,
                                lfs_off_t off, void *buffer, lfs_size_t size)
{
  return static_cast<LittleFS *>(c->context)
    ->read_from_disk_(block * c->block_size + off,
                      std::span<uint8_t>(static_cast<uint8_t *>(buffer), size));
}

int LittleFS::lfs_write_callback(const struct lfs_config *c, lfs_block_t block,
                                 lfs_off_t off, const void *buffer, lfs_size_t size)
{
  return static_cast<LittleFS *>(c->context)
    ->write_to_disk_(
      block * c->block_size + off,
      std::span<const uint8_t>(static_cast<const uint8_t *>(buffer), size));
}

int LittleFS::lfs_erase_callback(const struct lfs_config *c, lfs_block_t block)
{
  return static_cast<LittleFS *>(c->context)->erase_(block);
}

int LittleFS::lfs_sync_callback(const struct lfs_config *c)
{
  return static_cast<LittleFS *>(c->context)->sync_();
}

LittleFS::LittleFS(const DiskParameters &disk, ReadFromDisk read, WriteToDisk write,
                   Erase erase, Sync sync)
  : read_from_disk_{std::move(read)}
  , write_to_disk_{std::move(write)}
  , erase_{std::move(erase)}
  , sync_{std::move(sync)}
  , lfs_config_{
      .context = this,
      .read = lfs_read_callback,
      .prog = lfs_write_callback,
      .erase = lfs_erase_callback,
      .sync = lfs_sync_callback,
      .read_size = disk.read_size,
      .prog_size = disk.write_size,
      .block_size = disk.block_size,
      .block_count = disk.block_count,
      .block_cycles = 600,
      .cache_size =
        256, // plenty of RAM on RP2040, but make this configurable via KConfig
      .lookahead_size = 16,
    }
  , lfs_{}
{
  printf("Block size: %d, blocks: %d\n", disk.block_size, disk.block_count);
}

bool LittleFS::mount()
{
  printf("Mounting LittleFS\n");
  int ec = lfs_mount(&lfs_, &lfs_config_);
  printf("Error code: %d\n", ec);
  return ec == LFS_ERR_OK;
}

} // namespace yasboot::fs
