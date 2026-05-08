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
        PROPERTIES TIMEOUT 10)
endfunction()


add_munin_test(TestInstantiatorQPush test_queue_push.c)
add_munin_test(TestInstantiatorQPop  test_queue_pop.c)