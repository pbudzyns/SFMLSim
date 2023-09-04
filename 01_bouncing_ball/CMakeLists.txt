cmake_minimum_required(VERSION 3.25)

project(SFML_Apps VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANGARD_REQUIRED TRUE)

find_package(
    SFML
    COMPONENTS graphics
)

# add_library(Ball STATIC src/Ball.hpp src/Ball.cpp)
# target_include_directories(Ball INTERFACE src/)
# set_target_properties(Ball PROPERTIES LINKER_LANGUAGE CXX)

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE sfml-graphics)
