function(add_munin_test test_name source_file)
    cmake_parse_arguments(ARG "" "PASS_REGEX" "" ${ARGN})    

    add_executable(${test_name} ${source_file})

    target_link_libraries(${test_name}
        PRIVATE
            muninn
            muninn_test_utils
            ${ARG_UNPARSED_ARGUMENTS}
    )

    get_filename_component(test_dir ${source_file} DIRECTORY)

    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${test_name}>
    )

    set_tests_properties(${test_name}
        PROPERTIES TIMEOUT 120)

    if(ARG_PASS_REGEX)
        set_tests_properties(${test_name} PROPERTIES
            PASS_REGULAR_EXPRESSION "${ARG_PASS_REGEX}"
        )
    endif()
endfunction()

add_munin_test(TestInstantiatorMuninnWorkerRB  test_muninn_worker_rb.c)
add_munin_test(TestInstantiatorMuninnWorkerQ   test_muninn_worker_q.c)
add_munin_test(TestInstantiatorGWThApi         test_gw_th_api.c)
add_munin_test(TestInstantiatorGWThRoutine     test_gw_th_routine.c)
add_munin_test(TestInstantiatorGWThStress      test_gw_th_stress.c)
add_munin_test(TestInstantiatorLogThApi        test_log_th_api.c)
add_munin_test(TestInstantiatorLogThRoutine    test_log_th_routine.c)
add_munin_test(TestInstantiatorLogThStress     test_log_th_stress.c)
add_munin_test(TestInstantiatorCompThApi       test_comp_th_api.c)
add_munin_test(TestInstantiatorCompThRoutine   test_comp_th_routine.c)
add_munin_test(TestInstantiatorConsThApi       test_cons_th_api.c)

#check output with cmake
add_munin_test(TestInstantiatorConsThRoutine   test_cons_th_routine.c 
    PASS_REGEX "Hello.*World")