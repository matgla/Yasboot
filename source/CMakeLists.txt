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

add_subdirectory(fs)
add_subdirectory(mbr)

add_executable(yasboot)

target_sources(yasboot 
  PRIVATE 
    main.cpp2
    ram_manager.cpp2
  PUBLIC 
    FILE_SET CXX_MODULES FILES
      ram_layout.cc
)

use_cppfront(
  TARGET yasboot
  MODULE_SOURCES 
    ram_manager.cpp2
  BASE_DIRS  
    ${CMAKE_CURRENT_BINARY_DIR}
  # Internal compiler bug in file below, when not in main file 
  #    app.cpp2
)

find_package(eul REQUIRED)
find_package(littlefs REQUIRED)
find_package(yasconf REQUIRED)
find_package(sml REQUIRED)
find_package(yasld REQUIRED)
target_link_libraries(
  yasboot
  PUBLIC eul::utils newlib_stubs
  PRIVATE yasld yasboot_hal_interface yasboot_mbr yasboot::filesystem yasconf sml)

target_compile_options(
  yasboot 
  PRIVATE -Wno-shadow -Wno-unused-variable
)

include(generate_binary)
generate_binary(yasboot)
