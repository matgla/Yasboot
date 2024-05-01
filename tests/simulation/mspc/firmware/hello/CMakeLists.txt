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

add_executable(mspc_firmware_hello
  main.cpp
)

target_link_libraries(mspc_firmware_hello 
  PRIVATE 
    pico_stdlib
)

pico_set_linker_script(mspc_firmware_hello ${CMAKE_CURRENT_SOURCE_DIR}/../board/mspc_yasboot_firmware.ld)

pico_add_extra_outputs(mspc_firmware_hello)

add_custom_target (print_mspc_firmware_size ALL arm-none-eabi-size ${CMAKE_CURRENT_BINARY_DIR}/mspc_firmware_hello.elf DEPENDS mspc_firmware_hello)
