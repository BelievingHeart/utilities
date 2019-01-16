cmake_minimum_required(VERSION 3.1...3.12)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(install_utils LANGUAGES NONE)
include(${CMAKE_SOURCE_DIR}/cmake/install_sh.cmake)
set(INSTALL_DIR ${CMAKE_SOURCE_DIR})

if(APPLE)
install_sh(INFILE md-mac.sh OUTFILE md)
else()
install_sh(INFILE cppinit.sh OUTFILE cppinit)
install_sh(INFILE md.sh OUTFILE md)
install_sh(INFILE getpath.sh OUTFILE getpath)
install_sh(INFILE run-valgrind.sh OUTFILE run-valgrind)
install_sh(INFILE asm.sh OUTFILE asm)
install_sh(INFILE test-concurrency.sh OUTFILE test-concurrency)
endif()


