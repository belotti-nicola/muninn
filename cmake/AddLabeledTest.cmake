# add_labeled_test(<test_name> <sorgente1> [sorgente2 ...] [LINK_LIBRARIES lib1 lib2 ...])
#
# Compila i sorgenti in un eseguibile chiamato <test_name>, lo registra
# come test con add_test, e lo etichetta automaticamente con il percorso
# relativo (e tutti i suoi livelli di directory ancestor) rispetto alla
# root del progetto.
function(add_labeled_test test_name)

    #message(STATUS "Registrando test: ${test_name}")


    cmake_parse_arguments(ARG "" "" "LINK_LIBRARIES" ${ARGN})

    # ARG_UNPARSED_ARGUMENTS contiene i file sorgente
    add_executable(${test_name} ${ARG_UNPARSED_ARGUMENTS})

    target_link_libraries(${test_name} PRIVATE muninn muninn_test_utils)

    if(ARG_LINK_LIBRARIES)
        target_link_libraries(${test_name} PRIVATE ${ARG_LINK_LIBRARIES})
    endif()

    add_test(NAME ${test_name} COMMAND ${test_name})

    file(RELATIVE_PATH rel_dir "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

    set(labels "")
    string(REPLACE "/" ";" path_parts "${rel_dir}")
    set(accum "")
    foreach(part ${path_parts})
        if(accum STREQUAL "")
            set(accum "${part}")
        else()
            set(accum "${accum}/${part}")
        endif()
        list(APPEND labels "${accum}")
    endforeach()

    #message(STATUS "${labels}")

    set_tests_properties(${test_name} PROPERTIES LABELS "${labels}")

endfunction()