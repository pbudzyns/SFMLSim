cmake_minimum_required(VERSION 3.25)

project(SFML_Apps VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANGARD_REQUIRED TRUE)

include_directories(src)

find_package(
    SFML
    COMPONENTS graphics
)

add_library(Vector STATIC src/Vector.hpp)
target_include_directories(Vector INTERFACE src/)
set_target_properties(Vector PROPERTIES LINKER_LANGUAGE CXX)

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE Vector sfml-graphics)
# target_link_libraries(app PUBLIC Vector)
