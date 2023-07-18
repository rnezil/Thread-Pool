#Specify minimum cmake version requirement
CMAKE_MINIMUM_REQUIRED(VERSION 3.1 FATAL_ERROR)

#Specify project name and languages
project(concurrency LANGUAGES CXX)

#Find Catch2 package
find_package(Catch2 REQUIRED)

#Include Catch header
include(Catch)

#Add some compiler options
add_compile_options(-std=c++20 -pthread)

#Globally include 'include' directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#Add executable target
add_executable(test_queue ${CMAKE_CURRENT_SOURCE_DIR}/app/test_queue.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/ra/queue.hpp)

