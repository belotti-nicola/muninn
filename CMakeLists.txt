cmake_minimum_required(VERSION 3.20)

project(muninn VERSION 1.0.0 DESCRIPTION "Logger for C")

find_package(Threads REQUIRED)

include(CTest)
include(CMakePackageConfigHelpers)

set(MUNINN_SOURCES
    src/queue/queue_message.c
    src/queue/queue_rb.c
    src/thread_safe_queue/ts_queue.c
    src/muninn.c
)

add_subdirectory(test)

add_library(muninn STATIC
    ${MUNINN_SOURCES}
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/muninnConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

target_include_directories(muninn
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

target_link_libraries(muninn
    PRIVATE
        Threads::Threads
)

install(TARGETS muninn
    EXPORT muninnTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(EXPORT muninnTargets
    FILE muninnTargets.cmake
    NAMESPACE muninn::
    DESTINATION lib/cmake/muninn
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/muninnConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/muninnConfigVersion.cmake"
    DESTINATION lib/cmake/muninn
)


include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/muninnConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/muninnConfig.cmake"
    INSTALL_DESTINATION lib/cmake/muninn
)

install(DIRECTORY include/
    DESTINATION include
)
