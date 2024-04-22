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

set (CMAKE_CPPFRONT_FLAGS "")

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

macro(cppfront_generate_source input output target is_module flags)
    set(global_flags ${CMAKE_CPPFRONT_FLAGS})
    string(REPLACE " " ";" global_flags ${global_flags})
    add_custom_command(
        OUTPUT ${output}
        COMMAND ${CPPFRONT_BINARY_DIR}/cppfront ${flags} ${global_flags} ${input} -o ${output}
        DEPENDS ${input}
        VERBATIM
    )
 
  if (${is_module})
      message(STATUS "${input}, :o ${output}")
    target_sources(${target}
        PUBLIC FILE_SET CXX_MODULES FILES ${output}
        BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
    )
  else ()
      if (output MATCHES ".*h")
            target_sources(${target} PUBLIC ${output})
      else ()
            target_sources(${target} PRIVATE ${output})
      endif ()
    endif()
endmacro()

macro(cppfront_generate_sources)
    set(prefix CPPFRONT_GENERATOR)
    set(optionArgs "")
    set(singleValueArgs ROOT_DIRECTORY TARGET)
    set(multiValueArgs SOURCES MODULE_SOURCES FLAGS)
    
    include(CMakeParseArguments)
    cmake_parse_arguments(
        ${prefix}
        "${optionArgs}"
        "${singleValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    set (generated_sources "") 
    target_include_directories(${CPPFRONT_GENERATOR_TARGET}
      PRIVATE ${PROJECT_BINARY_DIR}/cppfront/include)
          
  
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
        cppfront_generate_source(${source} ${CMAKE_CURRENT_BINARY_DIR}/${source_name} ${CPPFRONT_GENERATOR_TARGET} OFF "${CPPFRONT_GENERATOR_FLAGS}")
    endforeach()

    foreach(source IN LISTS CPPFRONT_GENERATOR_MODULE_SOURCES)
        cmake_path(IS_ABSOLUTE source is_absolute)
        if (NOT ${is_absolute})
          cmake_path(ABSOLUTE_PATH source BASE_DIRECTORY ${CPPFRONT_GENERATOR_ROOT_DIRECTORY} OUTPUT_VARIABLE source)
        endif ()
        cmake_path(NORMAL_PATH source OUTPUT_VARIABLE ${source}) 
        
        get_filename_component(source_name ${source} NAME)
        string(LENGTH ${source_name} name_length)
        math(EXPR name_length "${name_length} - 1")
        string(SUBSTRING ${source_name} 0 ${name_length} source_name)
        # message(FATAL_ERROR "${CPPFRONT_GENERATOR_FLAGS}")
        cppfront_generate_source(${source} ${CMAKE_CURRENT_BINARY_DIR}/${source_name} ${CPPFRONT_GENERATOR_TARGET} ON "${CPPFRONT_GENERATOR_FLAGS}")
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
    endif()
endmacro()

macro(use_cppfront)
    set(prefix CPP2)
    set(optionArgs PURE)
    set(singleValueArgs TARGET)
    set(multiValueArgs MODULE_SOURCES FLAGS)
    
    include(CMakeParseArguments)
    cmake_parse_arguments(
        ${prefix}
        "${optionArgs}"
        "${singleValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    message(VERBOSE "Adding cppfront for target: ${}")
   
    get_target_property(sources ${CPP2_TARGET} SOURCES)
    list(FILTER sources INCLUDE REGEX "\\.(cpp|h)(2|2p)$")
    if (sources OR (NOT "${CPP2_MODULE_SOURCES}" STREQUAL ""))
        if (${CPP2_PURE})
            set (CPP2_FLAGS ${CPP2_FLAGS} -p)
        endif ()
        cppfront_generate_sources(
            TARGET ${CPP2_TARGET} 
            ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            SOURCES ${sources}
            MODULE_SOURCES ${CPP2_MODULE_SOURCES}
            FLAGS ${CPP2_FLAGS}
        ) 
    endif()
endmacro()


