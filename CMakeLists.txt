cmake_minimum_required(VERSION 3.19)
project(MC504_projeto2 C)

set(CMAKE_C_STANDARD 99)

add_executable(MC504_projeto2 main.c monks.h linked_list.h)
target_link_libraries(MC504_projeto2 PRIVATE m pthread ncurses)