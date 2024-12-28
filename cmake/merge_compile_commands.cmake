set(SERVER_FILE "${CMAKE_BINARY_DIR}/compile_commands.json")
set(CLIENT_FILE "${CMAKE_BINARY_DIR}/client/compile_commands.json")
set(OUTPUT_FILE "${CMAKE_BINARY_DIR}/all/compile_commands.json")

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/all")

if(EXISTS "${SERVER_FILE}" AND EXISTS "${CLIENT_FILE}")
    execute_process(
        COMMAND jq -s ".[0] + .[1]" "${SERVER_FILE}" "${CLIENT_FILE}"
        OUTPUT_FILE "${OUTPUT_FILE}"
        RESULT_VARIABLE result
    )
    if(result EQUAL 0)
        message(STATUS "Successfully merged compile_commands.json files")
    else()
        message(STATUS "Failed to merge compile_commands.json files")
    endif()
else()
    message(STATUS "One or both compile_commands.json files do not exist yet")
endif()

