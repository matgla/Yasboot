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

project(cppfront CXX)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
    cppfront 
    GIT_REPOSITORY https://github.com/matgla/cppfront.git
    GIT_TAG fixNoRtti 
)

FetchContent_MakeAvailable(cppfront)

add_executable(cppfront 
    ${cppfront_SOURCE_DIR}/source/cppfront.cpp
)

target_include_directories(cppfront
    PUBLIC 
        ${cppfront_SOURCE_DIR}/include
    PRIVATE 
        ${cppfront_SOURCE_DIR}/source
)

install(DIRECTORY ${cppfront_SOURCE_DIR}/include/ DESTINATION ${PROJECT_BINARY_DIR}/include)