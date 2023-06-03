#
# CMakeLists.txt
#
# Copyright (C) 2023 Mateusz Stadnik <matgla@live.com>
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program. If not, see <https://www.gnu.org/licenses/>.
#

add_executable(test_module)

target_sources(test_module PRIVATE main.cpp stubs.cpp)

target_link_libraries(test_module PRIVATE yasld_executable_flags)

target_compile_options(test_module PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=c++23
                                           -O2>)
include_directories(/home/mateusz/newlib/arm-none-eabi/include)

link_directories(/home/mateusz/newlib/arm-none-eabi/lib/thumb/v6-m/nofp/)
target_link_options(
  test_module
  PRIVATE
  /home/mateusz/newlib/arm-none-eabi/lib/thumb/v6-m/nofp/libc.a
  /home/mateusz/newlib/arm-none-eabi/lib/libc.a
  -L/home/mateusz/newlib/arm-none-eabi/lib/thumb/v6-m/nofp
  -nostdlib
  -lc
  -lm
  -specs=nano.specs)

target_link_libraries(
  test_module
  PRIVATE /home/mateusz/newlib/arm-none-eabi/lib/thumb/v6-m/nofp/libc.a)

include(convert_elf_to_yaff)

convert_elf_to_yaff(test_module)
