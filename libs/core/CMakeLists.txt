
add_library(libcore
    "src/token.cpp"
    "src/scanner.cpp"
    "src/parser.cpp"
)

target_include_directories(libcore PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)