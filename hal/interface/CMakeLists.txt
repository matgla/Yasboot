#
# CMakeLists.txt
#
# Copyright (C) 2023 Mateusz Stadnik <matgla@live.com>
#
# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General
# Public License along with this program. If not, see
# <https://www.gnu.org/licenses/>.
#

add_library(yasboot_hal_interface INTERFACE)

set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/hal)

target_sources(yasboot_hal_interface 
  INTERFACE 
    ${include_dir}/uart.hpp
)

target_compile_options(yasboot_hal_interface 
  INTERFACE 
    $<$<COMPILE_LANGUAGE:CXX>:-std=c++23>
)

target_include_directories(yasboot_hal_interface 
  INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
)

