cmake_minimum_required(VERSION 3.8)
project(cpp-conway-gol VERSION 0.0.0)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

add_subdirectory(src)
add_subdirectory(apps)
