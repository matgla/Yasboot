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
  PRIVATE 
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
  VERBATIM
)


add_custom_target (second_stage_bootloader_size ALL arm-none-eabi-size ${CMAKE_CURRENT_BINARY_DIR}/second_stage_bootloader.elf DEPENDS second_stage_bootloader)


# TODO support for different flash drives 

# add_executable(mbr)

add_executable(mbr)

target_sources(mbr 
  PRIVATE 
    source/test.S    
)

target_link_libraries(mbr
  PRIVATE 
    hardware_regs
)

target_link_options(mbr
  PRIVATE 
    --specs=nosys.specs -nostartfiles
    LINKER:--script=${CMAKE_CURRENT_SOURCE_DIR}/linker_scripts/mbr.ld
)

set_target_properties(mbr 
  PROPERTIES 
  LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker_scripts/mbr.ld
)


set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/resize.py)

add_dependencies(mbr second_stage_bootloader)

add_custom_command(
  TARGET mbr 
  POST_BUILD 
  COMMAND 
    ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:mbr> ${CMAKE_CURRENT_BINARY_DIR}/mbr_second_part.bin
  COMMAND
    cat ${CMAKE_CURRENT_BINARY_DIR}/second_stage_bootloader_with_crc.bin ${CMAKE_CURRENT_BINARY_DIR}/mbr_second_part.bin > mbr.bin
  COMMAND 
    ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/resize.py 
    --input ${CMAKE_CURRENT_BINARY_DIR}/mbr.bin --output ${CMAKE_CURRENT_BINARY_DIR}/mbr.bin --size 512
 
  DEPENDS 
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/resize.py
    second_stage_bootloader
  VERBATIM
)

set (MBR_IMAGE ${CMAKE_CURRENT_BINARY_DIR}/mbr.bin CACHE INTERNAL "")

add_library(rp2040 STATIC)

# TODO this public may be probably removed and only system_init provided as public 
set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/arm/v6-m/rp2040)
target_sources(rp2040 
  PUBLIC 
    ${include_dir}/configure_clocks.hpp 

  PRIVATE
    source/configure_mcu.cpp 
    source/configure_clocks.cpp
    source/hal/uart.cpp
)

target_link_libraries(rp2040 
  PUBLIC 
    arm_v6m_common
    yasboot_hal_interface
  PRIVATE 
    board_setup
    hardware_regs
    hardware_structs
)

target_link_options(rp2040
  PUBLIC
    --specs=nano.specs -nostartfiles
    LINKER:--script=${CMAKE_CURRENT_SOURCE_DIR}/linker_scripts/yasboot.ld
    -Wl,--gc-sections
    -flto
)

target_compile_options(rp2040 
  PUBLIC 
    -ffunction-sections -fdata-sections
    $<$<COMPILE_LANGUAGE:CXX>:-std=c++23 -fno-threadsafe-statics -fno-exceptions>
  PRIVATE 
  $<$<COMPILE_LANGUAGE:CXX>:-Werror -Wall -Wextra -Wconversion -Wunused -Wshadow -Wpointer-arith -Wcast-qual -Wmissing-braces>
)

target_include_directories(rp2040
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE 
    $<TARGET_PROPERTY:hardware_resets,INTERFACE_INCLUDE_DIRECTORIES>
    ${include_dir}
)

