add_library(hir_lib STATIC
    hir.cpp
    verifier.cpp
    generator.cpp
    cfg.cpp
)

target_include_directories(hir_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/frontend
    ${CMAKE_SOURCE_DIR}/frontend/ast
    ${CMAKE_SOURCE_DIR}/frontend/parser
    ${CMAKE_SOURCE_DIR}/frontend/semantics
    ${CMAKE_SOURCE_DIR}/tools
)

target_link_libraries(hir_lib PUBLIC frontend_lib graph_lib)

add_executable(test_hir tests/test_hir.cpp)
target_link_libraries(test_hir PRIVATE hir_lib)

add_executable(hir_cli src/main.cpp)
target_link_libraries(hir_cli PRIVATE hir_lib)

add_test(NAME hir COMMAND test_hir WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
