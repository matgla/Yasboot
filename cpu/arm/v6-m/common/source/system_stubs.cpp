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

#include "common/filesystem/filesystem_mount_points.hpp"

import hal.system.io;

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

  int _close(int fd)
  {
    auto fs = yasboot::fs::FilesystemMountPoints::get().get_filesystem_for_fd(fd);
    if (fs == nullptr)
    {
      return -1;
    }
    return fs->close(fd);
  }

  off_t _lseek(int, off_t, int)
  {
    return 0;
  }

  ssize_t _read(int fd, void *buf, size_t size)
  {
    auto fs = yasboot::fs::FilesystemMountPoints::get().get_filesystem_for_fd(fd);
    if (fs == nullptr)
    {
      return 0;
    }
    return fs->read_file(fd, std::span<uint8_t>(static_cast<uint8_t *>(buf), size));
  }

  ssize_t _write(int fd, const char *buf, size_t count)
  {
    const auto write = yasboot::hal::system::io::get_global_write();
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

  int _open(const char *pathname, int flags)
  {
    auto [fs, path] =
      yasboot::fs::FilesystemMountPoints::get().get_mount_point(pathname);

    if (!fs)
    {
      printf("Responding with -1\n");
      errno = ENOENT;
      return -1;
    }

    return fs->open(path, flags);
  }
}
