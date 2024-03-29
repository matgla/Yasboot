/**
 * system_stubs.cpp
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

#include <unistd.h>

#include "hal/system_stubs.hpp"

extern "C"
{
  void panic(const char *, ...)
  {
    while (true)
    {
    }
  }

  void __unhandled_user_irq()
  {
    while (true)
    {
    }
  }

  void hard_assertion_failure()
  {
    while (true)
    {
    }
  }

  int _kill(pid_t, int)
  {
    return 0;
  }

  pid_t _getpid(void)
  {
    return 0;
  }

  int _fstat(int, struct stat *)
  {
    return 0;
  }

  int _isatty(int)
  {
    return 0;
  }

  void _exit(int)
  {
    while (true)
    {
    }
  }

  int _close(int)
  {
    return 0;
  }

  off_t _lseek(int, off_t, int)
  {
    return 0;
  }

  ssize_t _read(int, void *, size_t)
  {
    return 0;
  }

  ssize_t _write(int fd, const char *buf, size_t count)
  {
    yasboot::hal::WriteCallback write = yasboot::hal::getGlobalWrite();
    if ((fd == 1 || fd == 2) && write)
    {
      return write(std::string_view(buf, count));
    }
    return 0;
  }

  extern char __heap_start__;
  extern char __heap_end__;

  static char *current_heap_end = &__heap_start__;

  void *_sbrk(intptr_t incr)
  {
    if (current_heap_end + incr > &__heap_end__)
    {
      return nullptr;
    }

    char *previous_heap_end = current_heap_end;
    current_heap_end += incr;
    return static_cast<caddr_t>(previous_heap_end);
  }
}
