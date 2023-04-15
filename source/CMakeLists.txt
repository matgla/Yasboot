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

add_subdirectory(mbr)

add_executable(yasboot)

target_sources(yasboot 
  PRIVATE 
    main.cpp
)

target_link_libraries(yasboot
  PUBLIC 
    yasboot_public_flags
  PRIVATE
    yasboot_hal_interface
    board
)

# Change this to some function provided by cpu
# add_custom_command(
#   TARGET yasboot 
#   POST_BUILD 
#   COMMAND 
#     ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:yasboot> ${CMAKE_CURRENT_BINARY_DIR}/yasboot.bin
#   VERBATIM
# )
# 
# add_custom_target(flash_image ALL
#   COMMAND 
#     cat ${MBR_IMAGE} ${CMAKE_CURRENT_BINARY_DIR}/yasboot.bin > flash_image.bin
#   DEPENDS 
#     mbr 
#     yasboot 
#   VERBATIM 
# )
# 
# add_custom_target (printsize ALL arm-none-eabi-size ${CMAKE_CURRENT_BINARY_DIR}/yasboot.elf DEPENDS yasboot)

