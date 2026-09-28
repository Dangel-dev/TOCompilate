find_program(FLEX_EXECUTABLE NAMES win_flex flex PATHS "C:/win_flex_bison-2.5.25")
find_program(BISON_EXECUTABLE NAMES win_bison bison PATHS "C:/win_flex_bison-2.5.25")

set(GEN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated)
file(MAKE_DIRECTORY ${GEN_DIR})

if(FLEX_EXECUTABLE AND BISON_EXECUTABLE)
    message(STATUS "Found Flex: ${FLEX_EXECUTABLE}")
    message(STATUS "Found Bison: ${BISON_EXECUTABLE}")

    add_custom_command(
        OUTPUT ${GEN_DIR}/grammar.cpp ${GEN_DIR}/grammar.hpp
        COMMAND ${BISON_EXECUTABLE} -d -v -o ${GEN_DIR}/grammar.cpp ${CMAKE_CURRENT_SOURCE_DIR}/parser/grammar.y
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser/grammar.y
        COMMENT "Generating Bison parser..."
    )

    add_custom_command(
        OUTPUT ${GEN_DIR}/lexer.cpp
        COMMAND ${FLEX_EXECUTABLE} -o ${GEN_DIR}/lexer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/parser/lexer.l
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser/lexer.l ${GEN_DIR}/grammar.hpp
        COMMENT "Generating Flex lexer..."
    )
else()
    if(EXISTS "${GEN_DIR}/grammar.cpp" AND EXISTS "${GEN_DIR}/lexer.cpp")
        message(STATUS "Flex/Bison not found, using pre-generated files in ${GEN_DIR}")
    else()
        message(FATAL_ERROR "win_flex or win_bison not found! Please check PATH or C:/win_flex_bison-2.5.25")
    endif()
endif()

add_library(frontend_lib STATIC
    ast/ast.cpp
    ast/ast_dot.cpp
    parser/parser.cpp
    semantics/symbol_table.cpp
    semantics/semantic_analyzer.cpp
    ${GEN_DIR}/grammar.cpp
    ${GEN_DIR}/lexer.cpp
)

target_include_directories(frontend_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ast
    ${CMAKE_CURRENT_SOURCE_DIR}/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/semantics
    ${GEN_DIR}
)

add_executable(frontend_cli src/main.cpp)
target_link_libraries(frontend_cli PRIVATE frontend_lib)

add_executable(test_frontend tests/test_frontend.cpp)
target_link_libraries(test_frontend PRIVATE frontend_lib)
add_test(NAME frontend COMMAND test_frontend WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(test_semantics tests/test_semantics.cpp)
target_link_libraries(test_semantics PRIVATE frontend_lib)
add_test(NAME semantics COMMAND test_semantics WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
