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
        PROPERTIES TIMEOUT 5)
endfunction()


add_munin_test(TestInstantiatorTSRBEncoder  test_ts_encoder.c)
add_munin_test(TestInstantiatorTSRBDecoder  test_ts_decoder.c)
add_munin_test(TestInstantiatorTSRBPush     test_ts_ring_buffer_push.c)
add_munin_test(TestInstantiatorTSRBPop      test_ts_ring_buffer_pop.c)
