cmake_minimum_required(VERSION 3.8)
project(cpp-conway-gol VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(MAIN_EXE conway-gol)

add_executable(${MAIN_EXE} src/main.cpp)

add_custom_target(run
  COMMAND ${MAIN_EXE}
  DEPENDS ${MAIN_EXE}
  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
