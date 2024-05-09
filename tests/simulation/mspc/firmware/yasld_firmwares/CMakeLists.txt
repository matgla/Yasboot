#
# CMakeLists.txt
#
# Copyright (C) 2024 Mateusz Stadnik <matgla@live.com>
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

cmake_minimum_required(VERSION 3.25)

list (APPEND CMAKE_MODULE_PATH ${yaspem_SOURCE_DIR}/cmake)

include (yaspem)

setup_yaspem (
  YASPEM_SOURCE ${yaspem_SOURCE_DIR}
  OUTPUT_DIRECTORY ${yaspem_BINARY_DIR}
  PACKAGE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/packages.json
)

find_package (picosdk REQUIRED)
include (pico_sdk_init)

list (APPEND PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../board)
set (PICO_BOARD mspc)

pico_sdk_init()

find_package (yasld REQUIRED)
