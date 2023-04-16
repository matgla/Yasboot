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

#include "hal/disk.hpp"
#include "hal/flow.hpp"
#include "hal/uart.hpp"

#include "yasboot/mbr/mbr.hpp"

#include <eul/utils/string.hpp>

template <typename UartType> void dump_memory(const uint8_t *data, std::size_t size, UartType &uart)
{
  for (std::size_t offset = 0; offset < size; ++offset)
  {
    std::array<char, 3> buf{};
    if (offset % 16 == 0)
    {
      uart.write("\r\n");
    }
    eul::utils::itoa<16>(*(data + offset), buf); // NOLINT
    if (eul::utils::strlen(std::span<char>(buf.data(), buf.size())) == 1)
    {
      buf[1] = '0';
      buf[2] = 0;
    }
    uart.write("0x");
    uart.write(std::string_view(buf.data(), buf.size()));
    uart.write(", ");
  }
  uart.write("\r\n");
}

int main()
{
  yasboot::hal::Uart<0> uart(115200);

  // load MBR partition header
  // verify magic signature
  // search bootable partition
  // second stage bootloader is put in 32K block after MBR and represented as non bootable
  // partition, that partition doesn't contain any filesystem, it's just a row of bytes second stage
  // may contain dynamic loader to load to RAM in future

  const yasboot::hal::Disk disk;
  const yasboot::MbrParser mbr(disk);

  uart.write("Parsing MBR of primary drive\n\r");
  uart.write("Drive 0 contains ");

  if (mbr.isValidMbr())
  {
    uart.write("valid MBR\r\n");
  }
  else
  {
    const auto &header = mbr.mbr();
    std::array<char, 5> buf{};
    eul::utils::itoa<16>(header.signature, std::span<char>(buf.data(), buf.size()));
    uart.write("invalid (0x");
    uart.write(std::string_view(buf.data(), buf.size()));
    uart.write(") MBR\r\n");

    dump_memory(reinterpret_cast<const uint8_t *>(&header), 512, uart); // NOLINT
  }

  while (!hal::should_exit())
  {
  }

  uart.write("Yasboot exit\n\r");
  return 0;
}
