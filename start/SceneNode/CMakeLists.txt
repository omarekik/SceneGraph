project(SceneNode)

#add_subdirectory(tests)

set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/SceneNode.h
)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/SceneNode.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/SceneGraph/include 
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
