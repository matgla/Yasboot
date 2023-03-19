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
  void panic(const char *fmt, ...)
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

  int _kill(pid_t pid, int sig)
  {
    return 0;
  }

  pid_t _getpid(void)
  {
    return 0;
  }

  int _fstat(int filedes, struct stat *buf)
  {
    return 0;
  }

  int _isatty(int fd)
  {
    return 0;
  }

  void _exit(int status)
  {
    while (true)
    {
    }
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
}

// #include <hardware/uart.h>
#include <hal/uart.hpp>

int main()
{
  //  uart_init(uart0, 115200);
  //  gpio_set_function(0, GPIO_FUNC_UART);
  //  gpio_set_function(1, GPIO_FUNC_UART);
  //
  yasboot::hal::Uart<0> uart(115200);
  while (true)
  {
    uart.write("Hello from Yasboot\n\r");
    //    uart_puts(uart0, "Hello from Yasboot, new bootloader is in use");
    //    sleep_ms(1000);
  }
  return 0;
}
