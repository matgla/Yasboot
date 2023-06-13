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

#include <array>
#include <cstdio>

#include <yasld/loader.hpp>

#include "hal/disk.hpp"
#include "hal/flow.hpp"
#include "hal/system_stubs.hpp"
#include "hal/uart.hpp"

#include "yasboot/mbr/mbr.hpp"

extern std::size_t YASBOOT_RAM_LOT;
extern std::size_t YASBOOT_RAM_LOT_SIZE;
extern std::byte YASBOOT_RAM_APP;
extern std::byte YASBOOT_RAM_APP_SIZE;
int main()
{
  yasboot::hal::Uart<0> uart(115200);

  // load MBR partition header
  // verify magic signature
  // search bootable partition
  // second stage bootloader is put in 32K block after MBR and represented as
  // non bootable partition, that partition doesn't contain any filesystem, it's
  // just a row of bytes second stage may contain dynamic loader to load to RAM
  // in future

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
  if (bootablePartition != nullptr)
  {
    printf("Found bootable partition at address: 0x%x\n", bootablePartition->lba_start);
  }
  else
  {
    printf("Any bootable paritition found!\n");
  }

  // loaded binary must not be dependent on any yasboot symbols
  // but for now we are just testing loader
  printf("Creation of dynamic loader\n");
  printf("RAM memory for LOT: %p, %d\n", &YASBOOT_RAM_LOT, &YASBOOT_RAM_LOT_SIZE);
  yasld::Loader loader(std::span<std::size_t>(&YASBOOT_RAM_LOT, &YASBOOT_RAM_LOT_SIZE),
                       std::span<std::byte>(&YASBOOT_RAM_APP, &YASBOOT_RAM_APP_SIZE));

  // for now let's put image just after disk image
  // image size is hardcoded to 64 KB
  const void *module_address = reinterpret_cast<const uint8_t *>(0x10008000);
  loader.load_module(module_address);

  while (!hal::should_exit())
  {
  }

  return 0;
}
