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

add_library(other_module)

target_sources(other_module 
  PUBLIC 
    FILE_SET CXX_MODULES FILES other.cc
)

add_library(hello)

target_sources(hello
  PUBLIC 
    FILE_SET CXX_MODULES FILES hello.cc
) 

target_link_libraries(other_module PRIVATE yasboot_private_flags)

target_link_libraries(hello PRIVATE yasboot_private_flags other_module)

add_executable(yasboot)

target_sources(yasboot 
  PRIVATE 
  #main.cpp
    main.cpp2
)

use_cppfront(
  TARGET yasboot
  FLAGS -p -fno-rtti -fno-exceptions)

find_package(eul REQUIRED)
find_package(littlefs REQUIRED)
find_package(yasconf REQUIRED)

# note: 
# additional manual linking to libc enforces newlib stubs propagation from static library
target_link_libraries(
  yasboot
  PUBLIC c yasboot_public_flags eul::utils
  PRIVATE yasld yasboot_hal_interface yasboot_mbr yasboot::filesystem yasconf hello c)

target_compile_options(yasboot 
  PUBLIC -fmodules-ts
)

include(generate_binary)
generate_binary(yasboot)
