set(HEADER_LIST "${cpp-conway-gol_SOURCE_DIR}/include/conway_gol/gol.hpp")

add_library(conway_gol gol.cpp ${HEADER_LIST})
target_include_directories(conway_gol PUBLIC ../include)
target_compile_features(conway_gol PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST}
)
