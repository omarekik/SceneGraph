project(Client)

add_subdirectory(tests)

find_package(Boost COMPONENTS boost program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/Client.h 
)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/Client.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    SceneGraph
)

target_include_directories(${PROJECT_NAME} PRIVATE 
${SceneGraph_SOURCE_DIR}/include
${SceneNode_SOURCE_DIR}/include
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
