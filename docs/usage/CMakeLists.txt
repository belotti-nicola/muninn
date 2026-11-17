cmake_minimum_required(VERSION 3.20)
project(pocM VERSION 1.0.0)

add_executable(pocM main.c)

find_package(muninn REQUIRED)

target_compile_options(pocM PRIVATE -fsanitize=address -g)
target_link_options(pocM PRIVATE -fsanitize=address)

target_link_libraries(pocM PRIVATE muninn::muninn)
