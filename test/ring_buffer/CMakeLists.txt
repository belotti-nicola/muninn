include(${CMAKE_SOURCE_DIR}/cmake/AddLabeledTest.cmake)

add_labeled_test(TestInstantiatorRBSize           test_rb_size.c)
add_labeled_test(TestInstantiatorRBStart          test_rb_start.c)
add_labeled_test(TestInstantiatorRBEnd            test_rb_end.c)
add_labeled_test(TestInstantiatorRBAvailableSize  test_rb_available_size.c)
add_labeled_test(TestInstantiatorRBPeek           test_rb_peek.c)
add_labeled_test(TestInstantiatorRBAdvance        test_rb_advance.c)
add_labeled_test(TestInstantiatorRBOverFlowEnd    test_rb_overflow_end.c)
add_labeled_test(TestInstantiatorRBOverFlowStart  test_rb_overflow_start.c)
add_labeled_test(TestInstantiatorRBFull           test_rb_full.c)

muninn_copy_test_assets(TestInstantiatorRBFull  "data")
