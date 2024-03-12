project(Start)

find_package(Microsoft.GSL CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SceneNode 
    SceneGraph
    Client
    Microsoft.GSL::GSL
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${Config_SOURCE_DIR}/include
)

target_compile_options (${PROJECT_NAME} PRIVATE
    ${compiler_warnings}
)

set_target_properties (${PROJECT_NAME}
    PROPERTIES
        CXX_CLANG_TIDY "${clang_tidy_cmd}"
)
