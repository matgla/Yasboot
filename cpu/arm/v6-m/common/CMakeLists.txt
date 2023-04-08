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
#
# <https://www.gnu.org/licenses/>.

add_library(arm_v6m_common OBJECT)

set (include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/arm/v6-m/common)

target_sources(arm_v6m_common
  PUBLIC 
    ${include_dir}/gcc_arm_init.hpp 
  PUBLIC 
    source/gcc_arm_init.cpp
    source/system_stubs.cpp
    source/vector_table.S
)

target_link_options(arm_v6m_common 
  PUBLIC 
    -L${CMAKE_CURRENT_SOURCE_DIR}/linker_scripts
)

set_target_properties(arm_v6m_common 
  PROPERTIES 
  LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker_scripts/v6m_linker_script.ld
)

target_include_directories(arm_v6m_common 
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE 
    ${include_dir}
)
