#Specify minimum cmake version requirement
CMAKE_MINIMUM_REQUIRED(VERSION 3.1 FATAL_ERROR)

#Specify project name and languages
project(concurrency LANGUAGES CXX)

#Require C++20 to be used
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#Find threads library and indicate preference for pthread library
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#Find Catch2 package
find_package(Catch2 REQUIRED)

#Include Catch header
include(Catch)

#Globally include 'include' directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#Add executable target
add_executable(test_queue ${CMAKE_CURRENT_SOURCE_DIR}/app/test_queue.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/ra/queue.hpp)

#Link against Threads library
target_link_libraries(test_queue Threads::Threads)
