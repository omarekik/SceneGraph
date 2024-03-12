cmake_minimum_required(VERSION 3.26)
project(Start)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

include (scripts/compiler_warnings.cmake)
set_compiler_warnings(compiler_warnings)

include (scripts/static_checking.cmake)
#set_clang_tidy_cmd(clang_tidy_cmd)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GoogleTest)
enable_testing ()

add_subdirectory(SceneNode)
add_subdirectory(SceneGraph)
add_subdirectory(Client)

#install(TARGETS Start DESTINATION bin)
