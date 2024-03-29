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

function(cppfront_generate_source input output)
    add_custom_command(
        OUTPUT ${output}
        COMMAND ${CPPFRONT_BINARY_DIR}/cppfront ${input} -fno-rtti -fno-exceptions -o ${output}
        VERBATIM
    )
endfunction()

function(cppfront_generate_sources output)
    set (generated_sources "") 
    foreach(source IN LISTS ARGN)
        get_filename_component(source_name ${source} NAME)
        get_filename_component(source ${source} ABSOLUTE) 
        string(LENGTH ${source_name} name_length)
        math(EXPR name_length "${name_length} - 1")
        string(SUBSTRING ${source_name} 0 ${name_length} source_name)
        cppfront_generate_source(${source} ${CMAKE_CURRENT_BINARY_DIR}/${source_name})
        list (APPEND generated_sources ${CMAKE_CURRENT_BINARY_DIR}/${source_name}) 
    endforeach()
    #message(FATAL_ERROR "output: ${output}, sources: ${sources}")
    set (${output} ${generated_sources} PARENT_SCOPE)
endfunction()

function(use_cppfront target)
    message(STATUS "Adding cppfront for target: ${target}")
    get_target_property(sources ${target} SOURCES)
    list(FILTER sources INCLUDE REGEX "\\.(cpp|h|hpp)2$")
    if (sources)
        cppfront_generate_sources(generated_cpp_sources ${sources})
        target_sources(${target} PRIVATE ${generated_cpp_sources})
        target_include_directories(${target} PRIVATE ${CPPFRONT_BINARY_DIR}/include)
    endif()
endfunction()

