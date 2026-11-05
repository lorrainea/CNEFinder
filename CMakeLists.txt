cmake_minimum_required(VERSION 3.13)
project(CNEFinder
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
include(GNUInstallDirs)
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
if(${CMAKE_INSTALL_PREFIX} MATCHES linuxbrew)
  set(CMAKE_INSTALL_LIBDIR lib)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_FLAGS_DEV "-O2 -g")
add_compile_options(-Wall -Wextra -pedantic)

set(CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost REQUIRED)
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")

find_package(OpenMP REQUIRED)
message(STATUS "OpenMP_FOUND: ${OpenMP_FOUND}")
message(STATUS "OpenMP_CXX_FLAGS: ${OpenMP_CXX_FLAGS}")

add_executable(cnef cnef.cc)
target_compile_features(cnef PUBLIC cxx_std_11)
install(TARGETS cnef
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_library(objlib OBJECT
  extend.cc
  utils.cc
  qgrams.cc
  overlaps.cc
  edlib.cc
)
target_link_libraries(objlib PUBLIC OpenMP::OpenMP_CXX Boost::headers)
target_link_libraries(cnef PRIVATE objlib)
