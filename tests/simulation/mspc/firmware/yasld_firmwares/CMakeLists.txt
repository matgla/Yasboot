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

cmake_minimum_required(VERSION 3.25)

list (APPEND CMAKE_MODULE_PATH ${yaspem_SOURCE_DIR}/cmake)

include (yaspem)

setup_yaspem (
  YASPEM_SOURCE ${yaspem_SOURCE_DIR}
  OUTPUT_DIRECTORY ${yaspem_BINARY_DIR}
  PACKAGE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/packages.json
)

# libgcc wrappers inside picoSDK are not relocatable
# libgcc can be used instead, 
# but it won't be as efficient as hardware wrappers
add_definitions(
  -DPICO_DISABLE_SHARED_IRQ_HANDLERS
  -DPICO_NO_BINARY_INFO
)
add_library(pico_bit_ops INTERFACE)
add_library(pico_mem_ops INTERFACE)
add_library(pico_mem_ops_headers INTERFACE)
add_library(pico_double INTERFACE)
add_library(pico_double_headers INTERFACE)
add_library(pico_float INTERFACE)
add_library(pico_float_headers INTERFACE)
# end of libgcc hacks 

add_library(pico_standard_link INTERFACE)
add_library(pico_standard_link_headers INTERFACE)
target_link_libraries(pico_standard_link INTERFACE pico_binary_info_headers)

find_package (picosdk REQUIRED)
include (pico_sdk_init)

list (APPEND PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../board)
set (PICO_BOARD mspc)

pico_sdk_init()

project (yasld_firmwares ASM C CXX)

set(common_flags "-Werror -Wall -Wextra -Wconversion -Wno-unused -Wshadow -Wpointer-arith -Wcast-qual -Wdouble-promotion -Wmissing-braces -fno-function-sections -fno-data-sections")
set(common_release_flags "") 
set(common_debug_flags "-g")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions -fmodules-ts -fno-threadsafe-statics -fno-module-lazy  ${common_flags}")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${common_release_flags}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${common_debug_flags}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${common_flags}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${common_release_flags}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${common_debug_flags}")

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${common_flags}")
set(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} ${common_release_flags}")
set(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} ${common_debug_flags}")
 

set (YASLD_USE_CUSTOM_LINKER_SCRIPT ON)
find_package (yasld REQUIRED)
list (APPEND CMAKE_MODULE_PATH ${yasld_SOURCE_DIR}/cmake)

add_subdirectory(hello)


