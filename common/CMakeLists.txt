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

add_library(common)

set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/common)

target_sources(common 
  PUBLIC 
    ${include_dir}/filesystem/disk_parameters.hpp
    ${include_dir}/units.hpp
  PRIVATE 
    source/units.cpp
  PUBLIC 
    FILE_SET CXX_MODULES FILES
      source/file_descriptors.cc
      source/filesystem_mount_points.cc
)

use_cppfront(
  TARGET common
  MODULE_SOURCES 
    source/disk_parameters.cpp2
    source/filesystem.cpp2
)

target_include_directories(common 
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(common 
  PUBLIC 
    eul::functional
  PRIVATE 
    yasboot_private_flags
)

