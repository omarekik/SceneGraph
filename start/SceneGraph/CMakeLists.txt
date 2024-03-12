project(SceneGraph)

add_subdirectory(tests)

set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/SceneGraph.h 
)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/SceneGraph.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    SceneNode 
    Client
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${SceneNode_SOURCE_DIR}/include 
    ${CMAKE_SOURCE_DIR}/Client/include
)


#Compilers will not issue warnings from header files originating from SYSTEM
#Header files will generate warning within the library project and not when used by other project
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE ${PROJECT_SOURCE_DIR}/include)

target_compile_options (${PROJECT_NAME} PRIVATE
    ${compiler_warnings}
)

set_target_properties (${PROJECT_NAME}
    PROPERTIES
        CXX_CLANG_TIDY "${clang_tidy_cmd}"
)
