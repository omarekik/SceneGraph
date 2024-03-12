project(SceneNodeTests)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

set(SOURCES SceneNode_tests.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    GTest::gtest 
    GTest::gtest_main
    SceneNode
)

set_target_properties (${PROJECT_NAME}
    PROPERTIES
        CXX_CLANG_TIDY "${clang_tidy_cmd}"
)

target_compile_options (${PROJECT_NAME} PRIVATE
    ${compiler_warnings}
)

target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include)

gtest_discover_tests(${PROJECT_NAME})
