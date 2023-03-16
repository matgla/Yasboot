#
# GenerateConfig.cmake
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

function (generate_config source_directory output_directory)
  file (MAKE_DIRECTORY ${output_directory}/config)

  add_custom_target(menuconfig
    COMMAND
      ${CMAKE_COMMAND} -E env srctree=${source_directory}/
      ${kconfig_python_executable} -m menuconfig 
      ${source_directory}/Kconfig 

    WORKING_DIRECTORY ${output_directory}
  )

  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${output_directory}/.config)

  find_file(kconfig_generator generate.py ${source_directory}/kconfig)

  add_custom_command(TARGET menuconfig
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E env
      srctree=${source_directory}
      ${kconfig_python_executable} ${kconfig_generator} --input ${output_directory}/.config --output ${output_directory}/config --kconfig ${source_directory}/Kconfig
    COMMAND cmake ${source_directory}
    WORKING_DIRECTORY ${output_directory}
  )

  if (EXISTS ${output_directory}/.config)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${GENERATE_CONFIG})
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${output_directory}/config/config.cmake)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${output_directory}/config/config.json)
    include(${output_directory}/config/config.cmake)
    set(configuration_is_ready TRUE CACHE INTERNAL "" FORCE)
  endif ()

endfunction()
