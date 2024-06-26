#
# CMakeLists.txt
# Copyright (C) 2023 Mateusz Stadnik <matgla@live.com>
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

cmake_minimum_required(VERSION 3.28)

find_program(yaspem yaspem.py)

set (YASPEM_NO_COMPATIBILITY_CHECK ON)

if (NOT yaspem OR $ENV{YASPEM_FETCH_FROM_GIT}})
  include(FetchContent)

  FetchContent_Declare(
    yaspem
    GIT_REPOSITORY https://github.com/matgla/Yaspem.git
    GIT_TAG master)

  FetchContent_MakeAvailable(yaspem)

else ()
  cmake_path(GET yaspem PARENT_PATH yaspem_dir)
  message(STATUS "Populating locally available Yaspem: ${yaspem_dir}")
  set (yaspem_SOURCE_DIR ${yaspem_dir})
endif ()
    
list (APPEND CMAKE_MODULE_PATH ${yaspem_SOURCE_DIR}/cmake) 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(yaspem)

setup_yaspem(
  YASPEM_SOURCE ${yaspem_SOURCE_DIR} OUTPUT_DIRECTORY
  ${CMAKE_CURRENT_BINARY_DIR}/packages PACKAGE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/preconfigure_packages.json)


find_package(CMakeUtils REQUIRED)


include(virtualenv)
create_virtualenv(kconfig ${CMAKE_CURRENT_SOURCE_DIR}/kconfig_scripts/requirements.txt
                  ${CMAKE_CURRENT_BINARY_DIR}/venvs)

include(kconfig_scripts/GenerateConfig.cmake)
generate_config(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

if(${configuration_is_ready})
  message(STATUS "Configuration is processed, starting initialization")
  add_subdirectory(cpu)
  
  list(APPEND CMAKE_MODULE_PATH "${CPU_MODULE_PATH}")
  list(APPEND CMAKE_MODULE_PATH "${ARCHITECTURE_MODULE_PATH}")

  include(McuInit)
  
  set(packagesToFetch ${CMAKE_CURRENT_SOURCE_DIR}/packages.json)
  if(CPU_PACKAGES_TO_FETCH)
    set(packagesToFetch ${packagesToFetch} ${CPU_PACKAGES_TO_FETCH})
  endif()

  setup_yaspem(
    YASPEM_SOURCE ${yaspem_SOURCE_DIR} OUTPUT_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/packages PACKAGE_FILES ${packagesToFetch})

  set(CMAKE_CXX_SCAN_FOR_MODULES ON) 

  preinitialize_cpu()
  project(Yasboot ASM C CXX)

  if (YASLD_LOGGER_ENABLED)
    add_compile_definitions(-DYASLD_ENABLE_LOGGER)
  endif ()


  # Global flags that breaks ABI are specified globally  
  set(CMAKE_CPPFRONT_FLAGS "-fno-exceptions -fno-rtti -no-c -no-n -no-s")
 
  include(cppfront)
  
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
  set(common_release_flags "-O2") 
  set(common_flags "-fanalyzer -Wno-analyzer-infinite-loop -Werror -pedantic -Wall -Wextra -Wconversion -Wunused -Wshadow -Wpointer-arith -Wcast-qual -Wdouble-promotion -Wno-missing-braces -ffunction-sections -fdata-sections")
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

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections") 
  
  set(CMAKE_CXX_SCAN_FOR_MODULES ON) 

  include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/libstd)
  # This will be extracted to external repository
  set (CMAKE_CXX_STANDARD 23)
  set (CMAKE_CXX_STANDARD_REQUIRED ON)
  set (CMAKE_CXX_EXTENSIONS OFF)
  initialize_cpu()

  add_library(yasboot_public_flags INTERFACE)
  target_compile_options(yasboot_public_flags 
    INTERFACE 
      -Wall 
      -Wextra
      
      $<$<COMPILE_LANGUAGE:CXX>: -fmodules-ts>
  )  
  target_link_libraries(yasboot_public_flags 
    INTERFACE 
      board
  )

  add_library(yasboot_private_flags INTERFACE)
  target_link_libraries(yasboot_private_flags INTERFACE yasboot_public_flags)

  include(clang_tidy)
  add_clang_tidy(".*packages/.*")

  if(${BOARD} STREQUAL "host")
    include(sanitizers)
    add_sanitizers()

    include(coverage)
    add_coverage()

    target_link_libraries(yasboot_public_flags INTERFACE enable_sanitizers
                                                         coverage_flags)

    target_link_libraries(yasboot_private_flags INTERFACE enable_sanitizers
                                                          coverage_flags)
  endif()

  add_subdirectory(board)
  add_subdirectory(common)
  add_subdirectory(hal)
  add_subdirectory(source)

  enable_testing()
  list (APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

  if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    set(renode_executable mono /Applications/Renode.app/Contents/MacOS/bin/Renode.exe)
  else () 
    find_program(renode_executable renode)
  endif ()

  if (NOT renode_executable)
    message(FATAL_ERROR "Can't find renode executable")
  endif ()

  if (NOT YASBOOT_DISABLE_TESTS)
    add_subdirectory(tests)
  endif ()

  if (DEFINED BOARD)
    add_subdirectory(simulation/${BOARD})
  endif ()

endif()

create_virtualenv(scripts ${CMAKE_CURRENT_SOURCE_DIR}/scripts/requirements.txt
                  ${CMAKE_CURRENT_BINARY_DIR}/venvs)

add_custom_target(
  disk
  COMMAND
    ${scripts_python_executable} ${PROJECT_SOURCE_DIR}/scripts/create_disk.py
    --output ${PROJECT_BINARY_DIR}/${BOARD}_disk.img --layout
    ${PROJECT_SOURCE_DIR}/board/${BOARD}/disk_layout.json
  COMMAND
    ${scripts_python_executable} ${PROJECT_SOURCE_DIR}/scripts/write_yasboot.py
    --drive ${PROJECT_BINARY_DIR}/${BOARD}_disk.img --yasboot
    ${PROJECT_BINARY_DIR}/yasboot.bin --bootcode ${PROJECT_BINARY_DIR}/boot.bin
  DEPENDS yasboot ${PROJECT_SOURCE_DIR}/scripts/create_disk.py mspc_firmwares 
  VERBATIM)

add_custom_target(
  flash
  COMMAND ${scripts_python_executable} ${PROJECT_SOURCE_DIR}/scripts/flash.py -f
          ${PROJECT_BINARY_DIR}/${BOARD}_disk_with_app.img
  DEPENDS disk
  VERBATIM)

add_custom_target(
  reset
  COMMAND ${scripts_python_executable} ${PROJECT_SOURCE_DIR}/scripts/reset.py
  VERBATIM)


