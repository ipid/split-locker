cmake_minimum_required(VERSION 3.0)
project(split-locker VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(split-locker main.cpp)

# Add threads library
find_package(Threads REQUIRED)
target_link_libraries(split-locker Threads::Threads)
