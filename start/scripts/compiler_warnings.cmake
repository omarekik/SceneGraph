function ( set_compiler_warnings    warnings)
    # Define warning level.
    if (    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"
        OR  "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
    )
        set (
            compiler_warnings
                -Wall
                -Wextra
                -Werror
                -pedantic-errors
				-Wno-unused-value
                -Wconversion
                -Wsign-conversion
        )

    elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

        set (
            compiler_warnings
                /W4
                /WX
                /EHsc
        )

    endif()

    set ( ${warnings} ${compiler_warnings} PARENT_SCOPE )
endfunction()
