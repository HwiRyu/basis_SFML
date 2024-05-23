cmake_minimum_required(VERSION 3.27)
project(basis_SFML)

# C++23 표준 설정
set(CMAKE_CXX_STANDARD 23)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(basis_SFML main.cpp
        one_variable_function.h)

target_link_libraries(basis_SFML sfml-graphics sfml-window sfml-system)
