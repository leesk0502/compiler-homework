cmake_minimum_required (VERSION 2.6)

project(regexp)

file(GLOB regex_SRC
  "${PROJECT_SOURCE_DIR}/src/*.h"
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

include_directories(${PROJECT_SOURCE_DIR}/src)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(regexp ${regex_SRC})
