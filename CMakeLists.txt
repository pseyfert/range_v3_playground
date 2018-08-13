# Copyright (C) 2018  CERN for the benefit of the LHCb collaboration
# Author: Paul Seyfert <pseyfert@cern.ch>
#
# This software is distributed under the terms of the GNU General Public
# Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".

# In applying this licence, CERN does not waive the privileges and immunities
# granted to it by virtue of its status as an Intergovernmental Organization
# or submit itself to any jurisdiction.

cmake_minimum_required(VERSION 3.11.4)
project(masked_range VERSION 0.0 LANGUAGES CXX)

# most of this file largely inspired by https://github.com/pabloariasal/modern-cmake-sample

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

find_package(range-v3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_EXAMPLES "Build the example" OFF)

if(BUILD_EXAMPLES)
  foreach(target IN ITEMS masked_int masked_bool wrap_user)
    add_executable(${target} examples/${target}.cpp)
    target_include_directories(${target}
      PUBLIC
      include
      )
    target_link_libraries(${target}
      PUBLIC # does that even make sense for an executable?
      range-v3
      )
  endforeach(target)
endif()

add_library(masked_range INTERFACE)
target_include_directories(masked_range
  SYSTEM INTERFACE
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)
target_link_libraries(masked_range
  INTERFACE
  range-v3
)

install(TARGETS masked_range
  EXPORT masked_range-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.hpp")

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/masked_range)

install(EXPORT masked_range-targets
  FILE
    masked_range-targets.cmake
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/masked_range-config-version.cmake
  DESTINATION ${INSTALL_CONFIGDIR})

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/masked_range-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY ExactVersion
)


# So downstream packages call find_package(range-v3)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/masked_range-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/masked_range-config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/masked_range-config.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)
