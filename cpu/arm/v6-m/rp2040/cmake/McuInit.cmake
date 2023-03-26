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

set (CPU_PACKAGES_TO_FETCH ${CMAKE_CURRENT_LIST_DIR}/packages.json CACHE INTERNAL "" FORCE)
set (MCU_INIT_DIR ${CMAKE_CURRENT_LIST_DIR})

macro (preinitialize_cpu)
  find_package (picosdk REQUIRED)
  include (pico_sdk_init)
endmacro ()

macro (initialize_cpu)
  pico_sdk_init()
  add_subdirectory(${MCU_INIT_DIR}/../../common) 
  add_subdirectory(${MCU_INIT_DIR}/..)
endmacro ()


