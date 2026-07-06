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
        PROPERTIES TIMEOUT 120
    )

endfunction()

add_munin_test(TestInstantiatorMPEnc test_encode.c)
add_munin_test(TestInstantiatorMPDec test_decode.c)