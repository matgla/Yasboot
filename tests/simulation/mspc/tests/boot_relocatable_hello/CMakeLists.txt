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

include (create_test_targets)

add_custom_target(boot_relocatable_hello)
add_dependencies(boot_relocatable_hello mspc_yasld_firmwares)

add_custom_command(TARGET boot_relocatable_hello
  POST_BUILD 
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/../../yasld_firmwares/hello/hello_from_relocatable_module.yaff ${CMAKE_CURRENT_BINARY_DIR}/disk/hello_from_relocatable_module.yaff
  VERBATIM
)
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/disk ${CMAKE_CURRENT_BINARY_DIR}/disk)

create_test_targets(
  NAME boot_relocatable_hello
  DISK_LAYOUT ${CMAKE_CURRENT_BINARY_DIR}/disk/disk_layout.json
)

