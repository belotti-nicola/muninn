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


add_munin_test(TestInstantiatorCompThApi       test_comp_th_api.c)
add_munin_test(TestInstantiatorCompThRoutine   test_comp_th_routine.c)
add_munin_test(TestInstantiatorLogThApi        test_comp_th_routine.c)
add_munin_test(TestInstantiatorLogThRoutine    test_comp_th_routine.c)