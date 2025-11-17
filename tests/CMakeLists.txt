# 查找系统安装的 Google Test
find_package(GTest REQUIRED)

# 添加测试可执行文件 并链接
add_executable(test_token test_token.cpp)
target_link_libraries(test_token 
    libcore 
    GTest::gtest 
    GTest::gtest_main
)

add_executable(test_scanner test_scanner.cpp)
target_link_libraries(test_scanner
    libcore 
    GTest::gtest 
    GTest::gtest_main
)

add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser
    libcore 
    GTest::gtest 
    GTest::gtest_main
)

add_executable(test_interpreter test_interpreter.cpp)
target_link_libraries(test_interpreter
    libcore 
    GTest::gtest 
    GTest::gtest_main
)


# add_test(NAME test_token COMMAND test_token)
# 自动发现测试
include(GoogleTest)
gtest_discover_tests(test_token PROPERTIES LABELS "libcore" )
gtest_discover_tests(test_scanner LABELS "libcore" )
gtest_discover_tests(test_parser LABELS "libcore" )
gtest_discover_tests(test_interpreter LABELS "libcore" )