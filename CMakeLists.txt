cmake_minimum_required(VERSION 3.1...3.12)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(install_utils LANGUAGES NONE)

set(INSTALL_DIR ${CMAKE_SOURCE_DIR})
configure_file(${CMAKE_SOURCE_DIR}/templates/md.sh ${CMAKE_SOURCE_DIR}/md_test.sh @ONLY)