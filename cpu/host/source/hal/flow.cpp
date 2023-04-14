/**
 * flow.cpp
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

#include <hal/flow.hpp>

#include <atomic>
#include <csignal>
#include <iostream>

#include <eul/utils/unused.hpp>

namespace hal
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
volatile static bool should_close = false;
void signal_handler(int signal)
{
  switch (signal)
  {
  case SIGINT:
  case SIGHUP:
  case SIGKILL:
    should_close = true;
  default:
    break;
  }
}

bool register_signal()
{
  std::signal(SIGINT, signal_handler); // NOLINT(cert-err33-c)
  std::signal(SIGHUP, signal_handler); // NOLINT(cert-err33-c)
  return true;
}

bool should_exit()
{
  static const bool registered_signal = register_signal();
  UNUSED(registered_signal);

  return should_close;
}

} // namespace hal
