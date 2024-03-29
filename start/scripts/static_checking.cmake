function (set_clang_tidy_cmd cmd)
    find_program (
        clang_tidy_tool
        NAMES
            clang-tidy
            clang-tidy.exe
    )

    if (NOT clang_tidy_tool)
        message (
            FATAL_ERROR
            "Static Checking requested but clang-tidy tool not found."
        )
    endif()

    set ( ${cmd}
            ${clang_tidy_tool};
            -config-file=${CMAKE_SOURCE_DIR}/../.clang-tidy;
            -p ${CMAKE_SOURCE_DIR}/build/compile_commands.json;
        PARENT_SCOPE
    )
endfunction ( set_clang_tidy_cmd)
