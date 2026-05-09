function(add_munin_test test_name source_file)
    add_executable(${test_name} ${source_file})

    target_link_libraries(${test_name}
        PRIVATE
            muninn
            ${ARGN}
    )

    # Directory del file sorgente senza estensione
    get_filename_component(test_dir ${source_file} DIRECTORY)

    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${test_name}>
    )

    set_tests_properties(${test_name}
        PROPERTIES TIMEOUT 20)

endfunction()

add_munin_test(TestInstantiatorFull            test_full.c)
add_munin_test(TestInstantiatorStressingMuninm test_stressing_muninn.c)
