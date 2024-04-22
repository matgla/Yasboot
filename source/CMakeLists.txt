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
    ${newlib_stubs_file}
    wut.cpp
    #$<TARGET_OBJECTS:newlib_stubs>
)

use_cppfront(
  TARGET yasboot
  FLAGS -p -fno-rtti -fno-exceptions)

find_package(eul REQUIRED)
find_package(littlefs REQUIRED)
find_package(yasconf REQUIRED)

target_link_libraries(
  yasboot
  PUBLIC yasboot_public_flags eul::utils newlib_stubs
  PRIVATE yasld yasboot_hal_interface yasboot_mbr yasboot::filesystem yasconf)

target_compile_options(yasboot 
  PUBLIC -fmodules-ts
)

include(generate_binary)
generate_binary(yasboot)
