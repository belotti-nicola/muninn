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


add_munin_test(TestInstantiatorGZIPCompressionIdempotency test_gzip_idempotency.c)

#add_munin_test(TestInstantiatorZSTDCompressionIdempotency test_zstd_idempotency.c)
#add_munin_test(TestInstantiatorLZ4CompressionIdempotency  test_lz4_idempotency.c)
#add_munin_test(TestInstantiatorXZCompressionIdempotency   test_xz_idempotency.c)
