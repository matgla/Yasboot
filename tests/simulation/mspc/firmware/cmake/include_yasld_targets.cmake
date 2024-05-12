#
# import_other_projects.cmake
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

set (SCRIPT_DIR ${CMAKE_CURRENT_LIST_DIR})

macro (include_yasld_targets)
  include (ExternalProject)
 
  set (options "")
  set (oneValueArgs YASPEM_SOURCE_DIR YASPEM_BINARY_DIR TOOLCHAINS_PATH)
  set (multiValueArgs)

  cmake_parse_arguments(I "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  ExternalProject_Add(mspc_yasld_firmwares 
    CMAKE_ARGS 
      -Dyaspem_SOURCE_DIR=${I_YASPEM_SOURCE_DIR}
      -Dyaspem_BINARY_DIR=${I_YASPEM_BINARY_DIR}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DCMAKE_TOOLCHAIN_FILE=${I_TOOLCHAINS_PATH}/arm-none-eabi-with-pic.cmake
    SOURCE_DIR ${SCRIPT_DIR}/../yasld_firmwares
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/yasld_firmwares 
    INSTALL_COMMAND "" 
    BUILD_ALWAYS 1
  )
endmacro()


