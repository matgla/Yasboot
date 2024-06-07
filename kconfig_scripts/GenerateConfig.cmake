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

macro (generate_config source_directory output_directory)
  file (MAKE_DIRECTORY ${output_directory}/config)

  find_file(kconfig_generator generate.py ${source_directory}/kconfig_scripts)

  if (USE_CONFIG)
    if(USE_CONFIG MATCHES ".*\.config")
      set(filepath ${USE_CONFIG})
    else ()
      set (filepath ${source_directory}/configs/${USE_CONFIG}/.config)
    endif ()
    
    if (NOT EXISTS ${filepath})
      message(FATAL_ERROR "Selected configuration ${USE_CONFIG} not exists in ${source_directory}/configs")
    endif()
    file (COPY ${filepath} DESTINATION ${output_directory}/config)
    message(STATUS "Configuration ${filepath} added")

    execute_process(
      COMMAND ${CMAKE_COMMAND} -E env
        srctree=${source_directory}
        ${kconfig_python_executable} ${kconfig_generator} --input ${output_directory}/config/.config --output ${output_directory}/config --kconfig ${source_directory}/Kconfig
      WORKING_DIRECTORY ${output_directory}
    )
  else ()
    if (NOT EXISTS ${output_directory}/config/.config)
      execute_process(COMMAND
        /usr/bin/tty
        OUTPUT_VARIABLE TTY_NAME
        OUTPUT_STRIP_TRAILING_WHITESPACE)
   
      execute_process(
        COMMAND
          ${CMAKE_COMMAND} -E env srctree=${source_directory} 
            bash -c "${kconfig_python_executable} -m menuconfig ${source_directory}/Kconfig"
         
        WORKING_DIRECTORY ${output_directory}/config
        OUTPUT_FILE ${TTY_NAME})
      
      execute_process(
        COMMAND ${CMAKE_COMMAND} -E env srctree=${source_directory}
          ${kconfig_python_executable} ${kconfig_generator} 
          --input ${output_directory}/config/.config 
          --output ${output_directory}/config 
          --kconfig ${source_directory}/Kconfig
        COMMAND ${CMAKE_COMMAND} -E env 
          srctree=${source_directory}
          ${kconfig_python_executable} -m genconfig ${source_directory}/Kconfig --header-path=${output_directory}/config/config.h

        WORKING_DIRECTORY ${output_directory}
      )
    endif ()
  endif ()

  add_custom_target(menuconfig
    COMMAND
      ${CMAKE_COMMAND} -E env srctree=${source_directory}/
      ${kconfig_python_executable} -m menuconfig 
      ${source_directory}/Kconfig 

    WORKING_DIRECTORY ${output_directory}/config
    USES_TERMINAL
    VERBATIM
  )

  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${output_directory}/.config)

  add_custom_command(TARGET menuconfig
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E env
      srctree=${source_directory}
      ${kconfig_python_executable} ${kconfig_generator} --input ${output_directory}/config/.config --output ${output_directory}/config --kconfig ${source_directory}/Kconfig
    COMMAND ${CMAKE_COMMAND} -E env 
      srctree=${source_directory}
      ${kconfig_python_executable} -m genconfig ${source_directory}/Kconfig --header-path=${output_directory}/config/config.h
    COMMAND cmake ${source_directory}
    WORKING_DIRECTORY ${output_directory}
    VERBATIM
  )

  if (EXISTS ${output_directory}/config/.config)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${GENERATE_CONFIG})
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${output_directory}/config/config.cmake)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${output_directory}/config/config.json)
    include(${output_directory}/config/config.cmake)
    set(configuration_is_ready TRUE CACHE INTERNAL "" FORCE)
  endif ()

endmacro()
