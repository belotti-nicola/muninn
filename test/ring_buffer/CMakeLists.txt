function(add_munin_test test_name source_file)
    add_executable(${test_name} ${source_file})

    target_link_libraries(${test_name}
        PRIVATE
            muninn
            ${ARGN}
    )

    add_test(NAME ${test_name}
             COMMAND ${test_name})

    set_tests_properties(${test_name}
        PROPERTIES TIMEOUT 5)
endfunction()


add_munin_test(TestInstantiatorRBSize           test_rb_size.c)
add_munin_test(TestInstantiatorRBStart          test_rb_start.c)
add_munin_test(TestInstantiatorRBEnd            test_rb_end.c)
add_munin_test(TestInstantiatorRBPeek           test_rb_peek.c)
add_munin_test(TestInstantiatorRBOverFlowEnd    test_rb_overflow_end.c)
add_munin_test(TestInstantiatorRBOverFlowStart  test_rb_overflow_start.c)
add_munin_test(TestInstantiatorRBFull           test_rb_full.c)
