cmake_minimum_required(VERSION 3.1...3.12)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(install_utils LANGUAGES NONE)

set(INSTALL_DIR ${CMAKE_SOURCE_DIR})
set(MD_EXECUTABLE ${CMAKE_SOURCE_DIR}/bin/md)
configure_file(${CMAKE_SOURCE_DIR}/templates/md.sh ${MD_EXECUTABLE} @ONLY)
execute_process(COMMAND chmod +x ${MD_EXECUTABLE})