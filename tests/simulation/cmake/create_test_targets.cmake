#
# create_disk_for_test.cmake
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

macro (create_test_targets)
  set (options "")
  set (oneValueArgs NAME DISK_LAYOUT)
  set (multiValueArgs "")

  cmake_parse_arguments(TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set (target_disk_path ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}_disk.img)
  
  message(STATUS "Adding image (${target_disk_path}) for test: ${TEST_NAME} with layout: ${TEST_DISK_LAYOUT}")
 
  add_custom_target(create_disk_for_${TEST_NAME}
    COMMAND 
      ${scripts_python_executable} ${PROJECT_SOURCE_DIR}/scripts/create_disk.py 
      --output ${target_disk_path} --layout 
      ${TEST_DISK_LAYOUT}
    COMMAND 
      ${scripts_python_executable} ${PROJECT_SOURCE_DIR}/scripts/write_yasboot.py 
      --drive ${target_disk_path} --yasboot ${PROJECT_BINARY_DIR}/yasboot.bin --bootcode ${PROJECT_BINARY_DIR}/boot.bin
    DEPENDS 
      yasboot ${TEST_NAME} ${PROJECT_SOURCE_DIR}/scripts/create_disk.py 
  )

  add_custom_target(run_${TEST_NAME} 
    COMMAND renode-test --debug-on-error ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.robot 
    VERBATIM 
    DEPENDS create_disk_for_${TEST_NAME} 
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
  
set (renode_test_image ${target_disk_path})
find_package(renode_rp2040 REQUIRED)
set (renode_rp2040_board_dir ${renode_rp2040_SOURCE_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}.robot ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.robot @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../common.robot ${CMAKE_CURRENT_BINARY_DIR}/common.robot @ONLY)

add_test(
  NAME ${TEST_NAME}_test 
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} -- run_${TEST_NAME})

endmacro ()
