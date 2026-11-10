function(add_munin_test test_name source_file)
    cmake_parse_arguments(ARG "" "PASS_REGEX" "LABELS;LIBS" ${ARGN})

    add_executable(${test_name} ${source_file})

    target_link_libraries(${test_name}
        PRIVATE
            muninn
            muninn_test_utils
            ${ARG_UNPARSED_ARGUMENTS}
    )

    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${test_name}>
    )

    set_tests_properties(${test_name}
        PROPERTIES TIMEOUT 120
    )

    if(ARG_LABELS)
        set_tests_properties(${test_name} PROPERTIES
            LABELS "${ARG_LABELS}")
    endif()

endfunction()

add_munin_test(TestInstantiatorThApi              test_th_api.c)
add_munin_test(TestInstantiatorSimpleLog          test_simple_log.c)
add_munin_test(TestInstantiatorSimpleExpandedLog  test_simple_expanded_log.c)
add_munin_test(TestInstantiatorMask               test_mask.c)
add_munin_test(TestInstantiatorLogInCSV           test_log_in_csv.c)
add_munin_test(TestInstantiatorCodec              test_codec.c)
add_munin_test(TestInstantiatorTestVariadic       test_variadic.c)
add_munin_test(TestInstantiatorPanicFlush         test_panic_flush.c)
add_munin_test(TestInstantiatorLogLevel           test_log_level.c)
add_munin_test(TestInstantiatorCompression        test_compression.c)
add_munin_test(TestInstantiatorStressingMuninn    test_stressing_muninn.c LABELS stress)


muninn_copy_test_assets(TestInstantiatorLogInCSV  "data")





