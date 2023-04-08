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

namespace
{
volatile std::sig_atomic_t signalStatus;
}

void signal_handler(int signal)
{
  signalStatus = signal;
}

bool register_signal()
{
  std::signal(SIGINT, signal_handler);
  return true;
}

bool should_exit()
{
  static bool registered_signal = register_signal();
  UNUSED(registered_signal);

  return signalStatus == SIGINT;
}

} // namespace hal
