add_library(muninn_test_utils STATIC utils/test_utils.c)
target_include_directories(muninn_test_utils PUBLIC utils)

function(muninn_copy_test_assets TARGET_NAME ASSET_FOLDER)
    file(GLOB_RECURSE TEST_ASSETS "${CMAKE_CURRENT_SOURCE_DIR}/${ASSET_FOLDER}/*")
    
    set(COPIED_FILES "")
    
    foreach(SRC_FILE ${TEST_ASSETS})
        if(NOT IS_DIRECTORY ${SRC_FILE})
            # Calcola il percorso relativo rispetto alla sottocartella corrente
            file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${SRC_FILE}")
            
            # Destinazione speculare nella cartella di build corrente
            set(DST_FILE "${CMAKE_CURRENT_BINARY_DIR}/${REL_PATH}")
            get_filename_component(DST_DIR "${DST_FILE}" DIRECTORY)
            
            # Comando di copia atomico
            add_custom_command(
                OUTPUT ${DST_FILE}
                COMMAND ${CMAKE_COMMAND} -E make_directory ${DST_DIR}
                COMMAND ${CMAKE_COMMAND} -E copy ${SRC_FILE} ${DST_FILE}
                DEPENDS ${SRC_FILE}
                COMMENT "Copying asset for ${TARGET_NAME}: ${REL_PATH}"
            )
            list(APPEND COPIED_FILES ${DST_FILE})
        endif()
    endforeach()
    
    set(CUSTOM_TARGET_NAME "${TARGET_NAME}_CopyAssets")
    add_custom_target(${CUSTOM_TARGET_NAME} ALL DEPENDS ${COPIED_FILES})
    
    add_dependencies(${TARGET_NAME} ${CUSTOM_TARGET_NAME})
endfunction()
# ==============================================================================

# Ora le sottocartelle vedranno la funzione!
add_subdirectory(queue)
add_subdirectory(threadsafe_queue)
add_subdirectory(ring_buffer)
add_subdirectory(threadsafe_ring_buffer)
add_subdirectory(compression)
add_subdirectory(threads)
add_subdirectory(full)