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

add_executable (hello_from_relocatable_module)

target_sources (hello_from_relocatable_module 
  PUBLIC 
  PRIVATE
    main.cpp
    vector_table.S
)

target_link_libraries(hello_from_relocatable_module 
  PRIVATE 
    pico_stdlib
    pico_runtime
    yasld_standalone_executable_flags 
)

target_link_options(hello_from_relocatable_module
  PUBLIC
   -T${CMAKE_CURRENT_SOURCE_DIR}/executable.ld
)

set_target_properties(hello_from_relocatable_module 
  PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/executable.ld)

include (ConvertElfToYasiff)

convert_elf_to_yasiff(
  TARGET hello_from_relocatable_module
  TYPE   executable)

