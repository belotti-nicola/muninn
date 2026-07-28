function(add_munin_test test_name source_file)
    add_executable(${test_name} ${source_file})

    target_link_libraries(${test_name}
        PRIVATE
            muninn
            muninn_test_utils
            ${ARGN}
    )

    add_test(NAME ${test_name}
             COMMAND ${test_name})

    set_tests_properties(${test_name}
        PROPERTIES TIMEOUT 120
    )

endfunction()

add_munin_test(TestInstantiatorMPEncHeader  test_encode_header.c)
add_munin_test(TestInstantiatorMPDecHeader  test_decode_header.c)

add_munin_test(TestInstantiatorMPEncPayload test_encode_payload.c)
add_munin_test(TestInstantiatorMPDecPayload test_decode_payload.c)

add_munin_test(TestInstantiatorMPEncCSV     test_encode_csv.c)
add_munin_test(TestInstantiatorMPDecCSV     test_decode_csv.c)

muninn_copy_test_assets(TestInstantiatorMPEncCSV  "data")