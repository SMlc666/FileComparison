cmake_minimum_required(VERSION 3.5.0)
project(FileComparison VERSION 0.1.0 LANGUAGES C CXX)
add_executable(FileComparison 
main.cpp
src/File.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
