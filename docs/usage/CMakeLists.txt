cmake_minimum_required(VERSION 3.20)
project(pocM VERSION 1.0.0)

add_executable(pocM 
    main.c
)

target_compile_options(pocM PRIVATE -fsanitize=address -g)
target_link_options(pocM PRIVATE -fsanitize=address)

target_include_directories(pocM PRIVATE
    "/home/nicola/Projects/muninn/include"
    "/home/nicola/Projects/muninn/build"
)

add_library(muninn STATIC IMPORTED)
set_target_properties(muninn PROPERTIES
    IMPORTED_LOCATION "/home/nicola/Projects/muninn/build/libmuninn.a"
)

target_link_libraries(pocM 
    PRIVATE muninn
    lz4)