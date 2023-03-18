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

/* CRT will be moved somewhere */
#include <cstdint>
#include <unistd.h>
extern "C"
{
  void _exit(int status)
  {
  }

  int _close(int fd)
  {
    return 0;
  }

  off_t _lseek(int fd, off_t offset, int whence)
  {
    return 0;
  }

  ssize_t _read(int fd, void *buf, size_t count)
  {
    return 0;
  }

  ssize_t _write(int fd, const void *buf, size_t count)
  {
    return 0;
  }

  void *_sbrk(intptr_t increment)
  {
    return nullptr;
  }

  extern std::size_t *__bss_start__;
  extern std::size_t *__bss_end__;

  void system_init()
  {
    /* initialize bss */
    std::size_t *bss_current = __bss_start__;
    while (bss_current != __bss_end__)
    {
      *bss_current = 0;
      ++bss_current;
    }

    while (true)
    {
    }
  }
}

int main()
{
  while (true)
  {
  }
  return 0;
}
