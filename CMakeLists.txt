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

cmake_minimum_required (VERSION 3.19)

include (FetchContent)

FetchContent_Declare (
  yaspem
  GIT_REPOSITORY https://github.com/matgla/Yaspem.git
  GIT_TAG master 
)

FetchContent_MakeAvailable (yaspem)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${yaspem_SOURCE_DIR}/cmake CACHE INTERNAL "")
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

include (yaspem)

include (cmake/VirtualEnv.cmake)
create_virtualenv(kconfig ${CMAKE_CURRENT_SOURCE_DIR}/kconfig/requirements.txt ${CMAKE_CURRENT_BINARY_DIR}/venvs)

include (kconfig/GenerateConfig.cmake)
generate_config(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

if (${configuration_is_ready})
  message (STATUS "Configuration is processed, starting initialization")
  add_subdirectory(cpu)
  
  include(McuInit)
  
  set (packagesToFetch ${CMAKE_CURRENT_SOURCE_DIR}/packages.json)
  if (CPU_PACKAGES_TO_FETCH)
    set (packagesToFetch "${packagesToFetch},${CPU_PACKAGES_TO_FETCH}")
  endif ()

  setup_yaspem (
    YASPEM_SOURCE ${yaspem_SOURCE_DIR} 
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/packages
    PACKAGE_FILES ${packagesToFetch} 
  )

  preinitialize_cpu()
  project(Yasboot ASM C CXX)
  set (CXX_STANDARD 20)
  initialize_cpu()

  add_library(yasboot_public_flags INTERFACE)
  add_library(yasboot_private_flags INTERFACE)

  if (${BOARD} STREQUAL "host")
    find_package(CMakeUtils)
    include(sanitizers)
    add_sanitizers()

    include(coverage)
    add_coverage()

    target_link_libraries(yasboot_public_flags 
      INTERFACE 
        enable_sanitizers
        coverage_flags
    )

    target_link_libraries(yasboot_private_flags
      INTERFACE 
        enable_sanitizers 
        coverage_flags
    )
  endif ()

  add_subdirectory(board)
  add_subdirectory(common)
  add_subdirectory(hal)
  add_subdirectory(source)

  if (${BOARD} STREQUAL "host")
    add_subdirectory(tests)
  endif ()
endif ()

