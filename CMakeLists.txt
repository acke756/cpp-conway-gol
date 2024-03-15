cmake_minimum_required(VERSION 3.8)
project(conway_gol VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

set(SOURCES
  ${CMAKE_PROJECT_NAME}/main.cpp
  ${CMAKE_PROJECT_NAME}/gol.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE .)
