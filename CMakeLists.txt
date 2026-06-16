cmake_minimum_required(VERSION 3.20)

project(muninn VERSION 3.0.0 DESCRIPTION "Logger for C" LANGUAGES C)

find_package(Threads REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
endif()


include(CTest)
include(CMakePackageConfigHelpers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MUNINN_SOURCES
    src/workers/muninn_worker.c
    src/workers/gateway/gateway_th.c
    src/internal/panic_flusher.c
    src/internal/console_handler.c
    src/queue/queue_message.c
    src/queue/queue_rb.c
    src/thread_safe_queue/ts_queue.c
    src/compressor/format/lz4/lz4_wrapper.c
    src/compressor/compressor_th.c
    src/logger/logger_th.c
    src/ring_buffer/ring_buffer.c
    src/threadsafe_ring_buffer/ts_ring_buffer.c
    src/threadsafe_ring_buffer/ts_rb_encoder.c
    src/threadsafe_ring_buffer/ts_rb_decoder.c
    src/threadsafe_ring_buffer/ts_rb_message.c
    src/console/console_th.c
    src/timestamp/timestamp_gen.c
    src/muninn.c
)

add_subdirectory(test)


add_library(muninn STATIC ${MUNINN_SOURCES})

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
        lz4
)


#
# package version
#
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/muninnConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

#
# install target
#
install(TARGETS muninn
    EXPORT muninnTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
)

install(EXPORT muninnTargets
    FILE muninnTargets.cmake
    NAMESPACE muninn::
    DESTINATION lib/cmake/muninn
)

#
# config file
#
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/muninnConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/muninnConfig.cmake"
    INSTALL_DESTINATION lib/cmake/muninn
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/muninnConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/muninnConfigVersion.cmake"
    DESTINATION lib/cmake/muninn
)