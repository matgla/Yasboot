#
# CMakeLists.txt
#
# Copyright (C) 2024 Mateusz Stadnik <matgla@live.com>
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program. If not, see <https://www.gnu.org/licenses/>.
#

message (STATUS "Adding MSPC Simulator Board")

find_package(renode_rp2040 REQUIRED)

set (RENODE_BOARD_DIR ${renode_rp2040_SOURCE_DIR})
set (RENODE_BINARY ${PROJECT_BINARY_DIR}/${BOARD}_disk.img)

configure_file(mspc.repl ${CMAKE_CURRENT_BINARY_DIR}/mspc.repl)

add_custom_target(
  run 
  COMMAND ${renode_executable} --console --disable-xwt ${CMAKE_CURRENT_BINARY_DIR}/mspc.repl 
  DEPENDS disk ${CMAKE_CURRENT_BINARY_DIR}/mspc.repl 
)

