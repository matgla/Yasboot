#
# yasboot_import_project.cmake
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

set (ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
set (ROOT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

macro (yasboot_import_project)
    message (STATUS "Importing Yasboot project") 

    set(prefix YASBOOT)
    set(optionArgs "")
    set(multiValueArgs "")
    set(singleValueArgs CONFIG TOOLCHAIN_FILE)
    
    include(CMakeParseArguments)
    cmake_parse_arguments(
        ${prefix}
        "${optionArgs}"
        "${singleValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    include (ExternalProject) 
    ExternalProject_Add(yasboot
        CMAKE_ARGS 
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DUSE_CONFIG=${YASBOOT_CONFIG}
            -DYASBOOT_DISABLE_TESTS=ON
            #-DCMAKE_TOOLCHAIN_FILE=${I_TOOLCHAINS_PATH}/arm-none-eabi-with-pic.cmake
        
        SOURCE_DIR ${ROOT_DIR}
        BINARY_DIR ${ROOT_BINARY_DIR}/yasboot
        INSTALL_COMMAND "" 
        BUILD_ALWAYS 1
  )
endmacro ()
