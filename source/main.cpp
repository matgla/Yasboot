/**
 * main.cpp
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

#include <cstdio>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>

#include <yasld/loader.hpp>

#include "hal/disk.hpp"
#include "hal/flow.hpp"
#include "hal/panic.hpp"
#include "hal/system_stubs.hpp"
#include "hal/uart.hpp"

#include "common/filesystem/filesystem_mount_points.hpp"

#include "yasboot/fs/littlefs.hpp"
#include "yasboot/mbr/mbr.hpp"

extern std::size_t YASBOOT_RAM_LOT;
extern std::size_t YASBOOT_RAM_LOT_SIZE;
extern std::byte YASBOOT_RAM_APP;
extern std::byte YASBOOT_RAM_APP_SIZE;

int main()
{
  yasboot::hal::Uart<0> uart(115200);

  yasboot::hal::setGlobalWrite([&uart](const std::string_view &str) {
    return uart.write(str);
  });

  const yasboot::hal::Disk disk;
  const yasboot::MbrParser mbr(disk);

  printf("\n\n");
  printf("================ YASBOOT =================\n");

  printf("Parsing MBR of primary drive\n");
  printf("Drive 0 contains: ");

  if (mbr.isValidMbr())
  {
    printf("valid MBR\n");
  }
  else
  {
    const auto &header = mbr.mbr();
    printf("Invalid (0x%x) MBR\n", header.signature);
  }

  const auto *bootablePartition = mbr.getBootablePartition();
  if (bootablePartition == nullptr)
  {
    printf("No bootable partition found\n");
    while (true)
    {
    }
  }

  printf("Found bootable partition at address: 0x%x\n",
         bootablePartition->lba_start);
  std::unique_ptr<yasboot::fs::Filesystem> lfs =
    std::make_unique<yasboot::fs::LittleFS>(
      // TODO(matgla): make this configurable
      yasboot::fs::DiskParameters{
        .read_size = 4,
        .write_size = 4,
        .block_size = 4096, // Winbond W25Q16JV
        .block_count =
          bootablePartition->number_of_sectors / 8 // MBR sector size has 512 bytes
      },
      [&bootablePartition](std::size_t address, std::span<uint8_t> buffer) {
        const std::size_t physical_address =
          0x10000000 + bootablePartition->lba_start * 512 + address;
        std::memcpy(buffer.data(), reinterpret_cast<const void *>(physical_address),
                    buffer.size());
        return 0;
      },
      [&bootablePartition](std::size_t address, std::span<const uint8_t> buffer) {
        return buffer.size();
      },
      [&bootablePartition](std::size_t block) {
        return 0;
      },
      [] {
        return 0;
      });

  if (!lfs->mount())
  {
    hal::panic("Boot partition mounting failure\n");
  }

  yasboot::fs::FilesystemMountPoints::get().register_mount_point("/",
                                                                 std::move(lfs));

  int fd = open("/boot.conf", O_RDONLY);
  if (fd < 0)
  {
    printf("Boot configuration opening failure\n");
  }

  printf("Configuration file dump:\n");
  std::array<char, 32> buf = {};
  while (read(fd, buf.data(), 31) == 31)
  {
    printf("%s", buf.data());
    buf = {};
  }
  printf("%s\n", buf.data());
  printf("======= Config End =========\n");

  printf("Creation of dynamic loader\n");
  printf("RAM memory for LOT: %p, %d\n", &YASBOOT_RAM_LOT, &YASBOOT_RAM_LOT_SIZE);

  while (!hal::should_exit())
  {
  }

  return 0;
}
