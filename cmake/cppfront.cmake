#
# CMakeLists.txt
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

cmake_minimum_required(VERSION 3.28)

set (cppfront_flags "")

message(STATUS "Adding cppfront toolchain generation: ${CMAKE_CURRENT_LIST_DIR}/cppfront")

set (CPPFRONT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/cppfront)

file (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cppfront)

if (NOT DEFINED ENV{CPPFRONT_COMPILER_PATH})
    execute_process(
        COMMAND cmake ${CMAKE_CURRENT_LIST_DIR}/cppfront
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cppfront
        COMMAND_ERROR_IS_FATAL ANY)

    execute_process(
        COMMAND cmake --build .
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cppfront
        COMMAND_ERROR_IS_FATAL ANY)
    execute_process(
        COMMAND cmake --install .
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cppfront
        COMMAND_ERROR_IS_FATAL ANY)

else ()
    message(STATUS "Setting cppfront compiler path to: $ENV{CPPFRONT_COMPILER_PATH}/cppfront")
endif ()

macro(cppfront_set_flags flags)
    set (cppfront_flags ${flags})
endmacro()

macro(cppfront_enable_pure target)
    set (ENV{yasboot_cpp2_flags} -p)
endmacro()

macro(cppfront_generate_source input output target)
    add_custom_command(
        OUTPUT ${output}
        COMMAND ${CPPFRONT_BINARY_DIR}/cppfront ${input} -fno-rtti $ENV{yasboot_cpp2_flags} -fno-exceptions -o ${output}
        DEPENDS ${input}
        VERBATIM
    )
    
  get_source_file_property(was_generated ${output} GENERATED)
  
  if (output MATCHES ".*h")
    target_sources(${target} PUBLIC ${output})
  else ()
    target_sources(${target} PRIVATE ${output})
  endif ()
endmacro()

macro(cppfront_generate_sources)
    set(prefix CPPFRONT_GENERATOR)
    set(optionArgs "")
    set(singleValueArgs ROOT_DIRECTORY TARGET)
    set(multiValueArgs SOURCES)
    
    include(CMakeParseArguments)
    cmake_parse_arguments(
        ${prefix}
        "${optionArgs}"
        "${singleValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    set (generated_sources "") 
    add_library(${CPPFRONT_GENERATOR_TARGET}_cpp2 INTERFACE)
    target_include_directories(${CPPFRONT_GENERATOR_TARGET}
      PRIVATE ${PROJECT_BINARY_DIR}/cppfront/include)
      # $<TARGET_PROPERTY:${target},INCLUDE_DIRECTORIES>)
    
    # target_link_libraries(${CPPFRONT_GENERATOR_TARGET}_cpp2 
    #     ${cppfront_flags}
    # )
  
    foreach(source IN LISTS CPPFRONT_GENERATOR_SOURCES)
        cmake_path(IS_ABSOLUTE source is_absolute)
        if (NOT ${is_absolute})
            cmake_path(ABSOLUTE_PATH source BASE_DIRECTORY ${CPPFRONT_GENERATOR_ROOT_DIRECTORY} OUTPUT_VARIABLE source)
        endif ()
        cmake_path(NORMAL_PATH source OUTPUT_VARIABLE ${source}) 
        
        get_filename_component(source_name ${source} NAME)
        string(LENGTH ${source_name} name_length)
        math(EXPR name_length "${name_length} - 1")
        string(SUBSTRING ${source_name} 0 ${name_length} source_name)
        cppfront_generate_source(${source} ${CMAKE_CURRENT_BINARY_DIR}/${source_name} ${CPPFRONT_GENERATOR_TARGET})
    endforeach()
endmacro()

macro(use_cppfront_with_root target root_dir)
    message(VERBOSE "Adding cppfront for target: ${target}")
    get_target_property(sources ${target} SOURCES)
    list(FILTER sources INCLUDE REGEX "\\.(cpp|h)(2|2p)$")
    if (sources)
        message("Original sources: ${sources}, ${root_dir}") 
        cppfront_generate_sources(
            TARGET ${target} 
            ROOT_DIRECTORY ${root_dir}
            OUTPUT generated_cpp_sources 
            SOURCES ${sources}
        )
        target_link_libraries(${target} PRIVATE ${target}_cpp2)
    endif()
endmacro()

macro(use_cppfront target)
  use_cppfront_with_root(${target} ${CMAKE_CURRENT_SOURCE_DIR})
endmacro()

# macro (enable_cppfront_for_directory directory root_dir)
#     get_property(targets DIRECTORY ${directory} PROPERTY BUILDSYSTEM_TARGETS)
#     foreach (target IN LISTS targets)
#         use_cppfront(${target} ${root_dir})
#     endforeach ()

#     get_property(subdirectories DIRECTORY ${directory} PROPERTY SUBDIRECTORIES)
#     foreach (subdirectory IN LISTS subdirectories)
#         enable_cppfront_for_directory(${subdirectory} ${subdirectory})
#     endforeach ()
# endmacro ()

# macro (_enable_cppfront_root)
#     enable_cppfront_for_directory(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
# endmacro ()

# cmake_language(DEFER DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} CALL _enable_cppfront_root)

