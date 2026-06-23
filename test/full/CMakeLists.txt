function(add_munin_test test_name source_file)
    add_executable(${test_name} ${source_file})

    target_link_libraries(${test_name}
        PRIVATE
            muninn
            muninn_test_utils
            ${ARGN}
    )

    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${test_name}>
    )

    set_tests_properties(${test_name}
        PROPERTIES TIMEOUT 60
    )

endfunction()

add_munin_test(TestInstantiatorThApi           test_th_api.c)
add_munin_test(TestInstantiatorSimpleLog       test_simple_log.c)
add_munin_test(TestInstantiatorTestVariadic    test_variadic.c)
add_munin_test(TestInstantiatorPanicFlush      test_panic_flush.c)
add_munin_test(TestInstantiatorLogLevel        test_log_level.c)
add_munin_test(TestInstantiatorCompression     test_compression.c)
add_munin_test(TestInstantiatorStressingMuninn test_stressing_muninn.c)




