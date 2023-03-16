#
# FindArmToolchain.cmake
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

find_program(c_compiler NAMES "arm-none-eabi-gcc")
find_program(cxx_compiler NAMES "arm-none-eabi-g++")
find_program(objcopy NAMES "arm-none-eabi-objcopy")
find_program(objdump NAMES "arm-none-eabi-objdump")
find_program(size NAMES "arm-none-eabi-size")
find_program(ar NAMES "arm-none-eabi-gcc-ar")
find_program(ranlib NAMES "arm-none-eabi-ranlib")

message(STATUS "asm compiler: ${c_compiler}")
message(STATUS "c compiler: ${c_compiler}")
message(STATUS "cxx compiler: ${cxx_compiler}")
message(STATUS "objcopy: ${objcopy}")
message(STATUS "objdump: ${objdump}")
message(STATUS "size: ${size}")
message(STATUS "ar: ${ar}")
message(STATUS "ranlib: ${ranlib}")

# Executable building test will failed 
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(CMAKE_ASM_COMPILER ${c_compiler} CACHE STRING "ASM compiler" FORCE)
set(CMAKE_C_COMPILER ${c_compiler} CACHE STRING "C compiler" FORCE)
set(CMAKE_CXX_COMPILER ${cxx_compiler} CACHE STRING "CXX compiler" FORCE)
set(CMAKE_OBJCOPY ${objcopy} CACHE STRING "Objcopy" FORCE)
set(CMAKE_CXX_LINK_EXECUTABLE ${cxx_compiler} CACHE STRING "CXX linker" FORCE)
set(CMAKE_C_LINK_EXECUTABLE ${c_compiler} CACHE STRING "C linker" FORCE)
set(CMAKE_AR ${ar} CACHE STRING "AR" FORCE)
set(CMAKE_RANLIB ${ranlib} CACHE STRING "Ranlib" FORCE)
set(CMAKE_C_COMPILER_RANLIB ${ranlib} CACHE STRING "C ranlib" FORCE)

find_package(PackageHandleStandardArgs)

find_package_handle_standard_args(
  ArmToolchain 
  DEFAULT_MSG
  c_compiler
  cxx_compiler
  objcopy 
  objdump 
  size 
  ar
  ranlib
)

