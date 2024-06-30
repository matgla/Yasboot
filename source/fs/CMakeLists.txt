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

add_library(yasboot_filesystem STATIC)
add_library(yasboot::filesystem ALIAS yasboot_filesystem)

set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/yasboot/fs)

target_sources(yasboot_filesystem
  PUBLIC
    ${include_dir}/littlefs.hpp
)

use_cppfront(
  TARGET yasboot_filesystem
  MODULE_SOURCES littlefs.cpp2
  BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
)

find_package(littlefs REQUIRED)

target_link_libraries(yasboot_filesystem 
  PUBLIC 
    common
    littlefs
  PRIVATE 
    yasboot_private_flags 
)

target_include_directories(yasboot_filesystem
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
