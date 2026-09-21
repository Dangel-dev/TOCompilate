cmake_minimum_required(VERSION 3.20)
project(TOCompilate CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the build type (Debug or Release)" FORCE)
endif()

add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

if(MINGW)
    add_link_options(-static-libgcc -static-libstdc++)
endif()

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# tools: graph & tree
add_library(graph_lib INTERFACE)
target_include_directories(graph_lib INTERFACE ${CMAKE_SOURCE_DIR}/tools)

add_executable(test_graph tools/tests/test_graph.cpp)
target_link_libraries(test_graph PRIVATE graph_lib)

add_executable(test_tree tools/tests/test_tree.cpp)
target_link_libraries(test_tree PRIVATE graph_lib)

enable_testing()
add_test(NAME graph COMMAND test_graph)
add_test(NAME tree COMMAND test_tree)
add_subdirectory(frontend)
