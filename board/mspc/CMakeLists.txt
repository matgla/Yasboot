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

add_library(board INTERFACE)

target_link_libraries(board 
  INTERFACE 
    rp2040
)

add_library(board_setup INTERFACE)

set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_sources(board_setup 
  INTERFACE 
    ${include_dir}/board_setup/xosc.hpp
)

target_link_libraries(board_setup 
  INTERFACE 
    common
)

target_include_directories(board_setup
  INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


