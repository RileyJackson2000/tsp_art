cmake_minimum_required(VERSION 3.1.5)
project(tsp)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall)
# add_compile_options(-Wextra)
# add_compile_options(-Wshadow)
# add_compile_options(-Wpedantic)
add_compile_options(-g -O3)

include_directories(./)

add_executable(tsp tsp.cpp)
