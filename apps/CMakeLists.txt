add_executable(app app.cpp)
target_compile_features(app PRIVATE cxx_std_17)

target_link_libraries(app PRIVATE conway-gol)

add_custom_target(run
  COMMAND app
  DEPENDS app
  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
