module;
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

#include <functional>
#include <string_view>

export module hal.system.io;

export namespace yasboot::hal::system::io
{
using WriteCallback = std::function<size_t(const std::string_view &data)>;
}

export namespace yasboot::hal::system::io
{

WriteCallback &get_global_write()
{
  static WriteCallback writer;
  return writer;
}

void set_global_write(const WriteCallback &write)
{
  get_global_write() = write;
}

} // namespace yasboot::hal::system::io
