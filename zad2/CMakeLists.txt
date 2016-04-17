cmake_minimum_required(VERSION 3.4)
project(zad2 C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lrt")

add_executable(zad2 main.c)
add_executable(client client.c)