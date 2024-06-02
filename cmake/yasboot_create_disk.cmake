#
# create_disk.cmake
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
set (FILE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
macro (yasboot_create_disk)
    set(prefix YASBOOT)
    set(optionArgs "")
    set(singleValueArgs TARGET OUTPUT LAYOUT YASBOOT_CONFIG TOOLCHAIN_FILE YASLD_BINARY_DIR YASLD_SOURCE_DIR)
    set(multiValueArgs DEPENDS)
    
    include(CMakeParseArguments)
    cmake_parse_arguments(
        ${prefix}
        "${optionArgs}"
        "${singleValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    message (STATUS "Adding disk target created from: ${YASBOOT_LAYOUT}")

    if (NOT TARGET yasboot)
        include(yasboot_import_project)
        yasboot_import_project(
            CONFIG ${YASBOOT_YASBOOT_CONFIG}
        )

    endif ()

    add_custom_target(${YASBOOT_TARGET} 
        COMMAND
            ${scripts_python_executable} ${ROOT_DIR}/scripts/create_disk.py
            --output ${YASBOOT_OUTPUT} 
            --layout ${YASBOOT_LAYOUT}
        COMMAND
            ${scripts_python_executable} ${ROOT_DIR}/scripts/write_yasboot.py
            --drive ${YASBOOT_OUTPUT} 
            --yasboot ${FILE_BINARY_DIR}/../yasboot.bin 
            --bootcode ${FILE_BINARY_DIR}/../boot.bin
        DEPENDS 
            yasboot 
            ${ROOT_DIR}/scripts/create_disk.py 
            ${YASBOOT_DEPENDS} 
        VERBATIM
    )
endmacro ()
