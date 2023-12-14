cmake_minimum_required(VERSION 3.22)
project(lab4 C)

set(CMAKE_C_STANDARD 99)

add_library(lib1 STATIC first_realization.c realization.h)
add_library(lib2 STATIC second_realization.c realization.h)
add_executable(program1 static_main.c)
add_executable(program2 static_main.c)
target_link_libraries(program1 lib1)
target_link_libraries(program2 lib2)

add_library(lib1d SHARED first_realization.c realization.h)
add_library(lib2d SHARED second_realization.c realization.h)
add_executable(programd dynamic_main.c)
target_link_libraries(lib1d)
target_link_libraries(lib2d)
target_link_libraries(programd ${CMAKE_DL_LIBS})