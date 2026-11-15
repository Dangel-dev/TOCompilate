add_library(lir_lib STATIC
    lir.cpp
    generator.cpp
)

target_include_directories(lir_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/hir
    ${CMAKE_SOURCE_DIR}/frontend
    ${CMAKE_SOURCE_DIR}/frontend/ast
    ${CMAKE_SOURCE_DIR}/frontend/parser
    ${CMAKE_SOURCE_DIR}/frontend/semantics
    ${CMAKE_SOURCE_DIR}/tools
)

target_link_libraries(lir_lib PUBLIC hir_lib frontend_lib)

add_executable(test_lir tests/test_lir.cpp)
target_link_libraries(test_lir PRIVATE lir_lib)

add_executable(lir_cli src/main.cpp)
target_link_libraries(lir_cli PRIVATE lir_lib)

add_test(NAME lir COMMAND test_lir WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

