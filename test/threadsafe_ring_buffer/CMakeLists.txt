include(${CMAKE_SOURCE_DIR}/cmake/AddLabeledTest.cmake)

add_labeled_test(TestInstantiatorTSRBPush     test_ts_ring_buffer_push.c)
add_labeled_test(TestInstantiatorTSRBPop      test_ts_ring_buffer_pop.c)