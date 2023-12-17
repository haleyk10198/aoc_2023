cmake_minimum_required(VERSION 3.20)
project(aoc_2023)

set(CMAKE_CXX_STANDARD 23)

find_package(range-v3 REQUIRED)

include_directories(include)

add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
add_subdirectory(day4)
add_subdirectory(day5)
