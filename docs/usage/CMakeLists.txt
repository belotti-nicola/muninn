cmake_minimum_required(VERSION 3.20)

project(pocM VERSION 1.0.0 DESCRIPTION "Poc using muninn")

add_executable(pocM
  main.c
)

find_package(muninn REQUIRED)
target_link_libraries(pocM PRIVATE
  muninn::muninn)


