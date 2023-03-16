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

add_executable(second_stage_bootloader)

target_sources(second_stage_bootloader
  PUBLIC
    source/second_stage.S
)

target_link_libraries(second_stage_bootloader 
  PRIVATE 
    hardware_regs
)

target_link_options(second_stage_bootloader
  PRIVATE 
    --specs=nosys.specs -nostartfiles
    LINKER:--script=${CMAKE_CURRENT_SOURCE_DIR}/linker_scripts/second_stage.ld
)

set_target_properties(second_stage_bootloader 
  PROPERTIES 
  LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker_scripts/second_stage.ld
)


find_package(Python3 COMPONENTS Interpreter REQUIRED)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/append_checksum.py)

add_custom_command(
  TARGET second_stage_bootloader 
  POST_BUILD 
  COMMAND 
    ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:second_stage_bootloader> second_stage_bootloader.bin
  COMMAND 
    ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/append_checksum.py 
    --input ${CMAKE_CURRENT_BINARY_DIR}/second_stage_bootloader.bin --output ${CMAKE_CURRENT_BINARY_DIR}/second_stage_bootloader_with_crc.bin
  DEPENDS 
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/append_checksum.py
)



# TODO support for different flash drives 

# add_executable(mbr)


