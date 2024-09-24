cmake_minimum_required(VERSION 3.0)
project(Monopoly)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

add_executable(monopoly main.cpp)

target_link_libraries(monopoly sfml-system sfml-window sfml-graphics)
