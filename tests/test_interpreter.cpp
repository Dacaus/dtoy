#include "interpreter.h"
#include <gtest/gtest.h>
#include "scanner.h"
#include "parser.h"

namespace dtoy {
namespace interpreter {
TEST(Interpreter, Literal) {
    // Integer literal test
    {
        // expression: 42
        scanner::Scanner scanner("42");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens); 
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<int>(result));
        EXPECT_EQ(std::get<int>(result), 42);
    }
    // Double literal test
    {
        // expression: 3.14
        scanner::Scanner scanner("3.14");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<double>(result));
        EXPECT_EQ(std::get<double>(result), 3.14);
    }
    // String literal test
    {
        // expression: "hello"
        scanner::Scanner scanner("\"hello\"");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<std::string>(result));
        EXPECT_EQ(std::get<std::string>(result), "hello");
    }

    // Boolean literal test
    {
        // expression: true
        scanner::Scanner scanner("true");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<bool>(result));
        EXPECT_EQ(std::get<bool>(result), true);
    }

    // Nil literal test
    {
        // expression: nil
        scanner::Scanner scanner("nil");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<std::nullptr_t>(result) ||
                    std::holds_alternative<std::monostate>(result));
    }

    // Additional test for false boolean literal
    {
        // expression: false
        scanner::Scanner scanner("false");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<bool>(result));
        EXPECT_EQ(std::get<bool>(result), false);
    }

    // Additional test for empty string literal
    {
        // expression: ""
        scanner::Scanner scanner("\"\"");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<std::string>(result));
        EXPECT_EQ(std::get<std::string>(result), "");
    }
}
TEST(Interpreter, UnaryExpression) {
    // Test for negation of an integer: -5
    {
        scanner::Scanner scanner("-5");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<int>(result));
        EXPECT_EQ(std::get<int>(result), -5);
    }

    // Test for negation of a double: -3.14
    {
        scanner::Scanner scanner("-3.14");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<double>(result));
        EXPECT_EQ(std::get<double>(result), -3.14);
    }

    // Test for logical NOT of a boolean: !true
    {
        scanner::Scanner scanner("!true");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<bool>(result));
        EXPECT_EQ(std::get<bool>(result), false);
    }
}


TEST(Interpreter, BinaryExpression) {
    // Test for addition of two integers: 1 + 2
    {
        scanner::Scanner scanner("1 + 2");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<int>(result));
        EXPECT_EQ(std::get<int>(result), 3);
    }
    // Test for addition of two doubles: 1.5 + 2.5
    {
        scanner::Scanner scanner("1.5 + 2.5");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<double>(result));
        EXPECT_EQ(std::get<double>(result), 4.0);
    }
    // Test for addition of two strings: "foo" + "bar"
    {
        scanner::Scanner scanner("\"foo\" + \"bar\"");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<std::string>(result));
        EXPECT_EQ(std::get<std::string>(result), "foobar");
    }

    // Test for subtraction of two integers: 5 - 3
    {
        scanner::Scanner scanner("5 - 3");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<int>(result));
        EXPECT_EQ(std::get<int>(result), 2);
    }

    // Test for multiplication of two doubles: 2.0 * 3.5
    {
        scanner::Scanner scanner("2.0 * 3.5");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<double>(result));
        EXPECT_EQ(std::get<double>(result), 7.0);
    }

    // Test for division of two integers: 8 / 2
    {
        scanner::Scanner scanner("8 / 2");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<int>(result));
        EXPECT_EQ(std::get<int>(result), 4);
    }

    // Test for division by zero
    {
        scanner::Scanner scanner("5 / 0");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        try {
            auto result = interpreter.evaluate(*expr);
            FAIL() << "Expected RuntimeError for division by zero.";
        } catch (const RuntimeError& e) {
            EXPECT_STREQ(e.what(), "Division by zero.");
        } catch (...) {
            FAIL() << "Expected RuntimeError for division by zero.";
        }
    }

    // Test for equality: 3 == 3
    {
        scanner::Scanner scanner("3 == 3");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<bool>(result));
        EXPECT_EQ(std::get<bool>(result), true);
    }

    // Test for inequality: 4 != 5
    {
        scanner::Scanner scanner("4 != 5");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<bool>(result));
        EXPECT_EQ(std::get<bool>(result), true);
    }

    // Test for invalid addition: 1 + "string"
    {
        scanner::Scanner scanner("1 + \"string\"");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        try {
            auto result = interpreter.evaluate(*expr);
            FAIL() << "Expected RuntimeError for invalid addition.";
        } catch (const RuntimeError& e) {
            EXPECT_STREQ(e.what(), "Operands must be two numbers or two strings.");
        } catch (...) {
            FAIL() << "Expected RuntimeError for invalid addition.";
        }
    }

    // Test for invalid subtraction: "string" - 2
    {
        scanner::Scanner scanner("\"string\" - 2");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        try {
            auto result = interpreter.evaluate(*expr);
            FAIL() << "Expected RuntimeError for invalid subtraction.";
        } catch (const RuntimeError& e) {
            EXPECT_STREQ(e.what(), "Operands must be numbers.");
        } catch (...) {
            FAIL() << "Expected RuntimeError for invalid subtraction.";
        }
    }
}

TEST(Interpreter, GroupingExpression) {
    // Test for grouping expression: (1 + 2) * 3
    {
        scanner::Scanner scanner("(1 + 2) * 3");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<int>(result));
        EXPECT_EQ(std::get<int>(result), 9);
    }

    // test for nested grouping expression: ((2 + 3) * (4 - 1))
    {
        scanner::Scanner scanner("((2 + 3) * (4 - 1))");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<int>(result));
        EXPECT_EQ(std::get<int>(result), 15);
    }
}

TEST(Interpreter, ComplexExpression) {
    // Test for complex expression: -(3 + 4) * (2 - 5) / 7 
    {
        scanner::Scanner scanner("-(3 + 4) * (2 - 5) / 7");
        auto tokens = scanner.scan_tokens();
        parser::Parser parser1(tokens);
        auto expr = parser1.expression();
        EXPECT_NE(expr, nullptr);
        Interpreter interpreter;
        auto result = interpreter.evaluate(*expr);
        EXPECT_TRUE(std::holds_alternative<int>(result));
        EXPECT_EQ(std::get<int>(result), 3);
    }
}
} // namespace interpreter
} // namespace dtoy