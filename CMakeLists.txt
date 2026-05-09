cmake_minimum_required(VERSION 3.20)

project(muninn VERSION 1.0.0 DESCRIPTION "Logger for C")

find_package(Threads REQUIRED)

include(CTest)

set(MUNINN_SOURCES
    #src/logger.c
    #src/logger.h
    #src/log_queue.h
    #src/log_queue.c
    #src/log_message.h
    #src/log_message.c

    src/queue/queue_message.c
    src/queue/queue_rb.c
    src/thread_safe_queue/ts_queue.c
    src/muninn.c
)

add_subdirectory(test)

add_library(muninn
    ${MUNINN_SOURCES}
)

target_include_directories(muninn
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

target_link_libraries(muninn
    PRIVATE
        Threads::Threads
)