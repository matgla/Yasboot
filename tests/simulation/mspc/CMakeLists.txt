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

message(STATUS "Adding external project with firmware examples for MSPC")

include(ExternalProject)

ExternalProject_Add(mspc_firmwares
  CMAKE_ARGS -Dyaspem_SOURCE_DIR=${yaspem_SOURCE_DIR} -Dyaspem_BINARY_DIR=${PROJECT_BINARY_DIR}/packages
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/firmware
  BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/firmware
  INSTALL_COMMAND ""
  BUILD_ALWAYS 1
)
