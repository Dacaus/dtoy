#include "parser.h"
#include "scanner.h"
#include "gtest/gtest.h"

namespace dtoy {
namespace parser {

TEST(parserTest, testPrimary) {
  // Test Literal Expressions
  {
    // true literal
    scanner::Scanner scanner("true");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*expr));
    const auto &literal = std::get<expr::LiteralExpr>(*expr);
    EXPECT_EQ(std::get<bool>(literal.value), true);
  }

  {
    // false literal
    scanner::Scanner scanner("false");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*expr));
    const auto &literal = std::get<expr::LiteralExpr>(*expr);
    EXPECT_EQ(std::get<bool>(literal.value), false);
  }

  {
    // nil literal
    scanner::Scanner scanner("nil");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*expr));
    const auto &literal = std::get<expr::LiteralExpr>(*expr);
    EXPECT_TRUE(std::holds_alternative<std::nullptr_t>(literal.value));
  }

  {
    // integer literal
    scanner::Scanner scanner("123");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*expr));
    const auto &literal = std::get<expr::LiteralExpr>(*expr);
    EXPECT_EQ(std::get<int>(literal.value), 123);
  }

  {
    // float literal
    scanner::Scanner scanner("45.67");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*expr));
    const auto &literal = std::get<expr::LiteralExpr>(*expr);
    EXPECT_DOUBLE_EQ(std::get<double>(literal.value), 45.67);
  }

  {
    // string literal
    scanner::Scanner scanner("\"hello\"");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*expr));
    const auto &literal = std::get<expr::LiteralExpr>(*expr);
    EXPECT_EQ(std::get<std::string>(literal.value), "hello");
  }

  {
    // empty string literal
    scanner::Scanner scanner("\"\"");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*expr));
    const auto &literal = std::get<expr::LiteralExpr>(*expr);
    EXPECT_EQ(std::get<std::string>(literal.value), "");
  }

  {
    // grouping with number
    scanner::Scanner scanner("(123)");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::GroupingExpr>(*expr));
  }

  {
    // grouping with boolean
    scanner::Scanner scanner("(true)");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::GroupingExpr>(*expr));
  }

  {
    // nested grouping
    scanner::Scanner scanner("((123))");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::GroupingExpr>(*expr));

    // 检查嵌套结构
    const auto &outerGroup = std::get<expr::GroupingExpr>(*expr);
    EXPECT_TRUE(
        std::holds_alternative<expr::GroupingExpr>(*outerGroup.expression));

    const auto &innerGroup =
        std::get<expr::GroupingExpr>(*outerGroup.expression);
    EXPECT_TRUE(
        std::holds_alternative<expr::LiteralExpr>(*innerGroup.expression));

    const auto &literal = std::get<expr::LiteralExpr>(*innerGroup.expression);
    EXPECT_EQ(std::get<int>(literal.value), 123);
  }

  {
    // invalid: only plus
    scanner::Scanner scanner("+");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    EXPECT_THROW(parser.expression(), std::runtime_error);
  }

  {
    // invalid: only right parenthesis
    // scanner::Scanner scanner(")");
    // auto tokens = scanner.scan_tokens();
    // Parser parser(tokens);
    // EXPECT_THROW(parser.expression(), std::runtime_error);
    scanner::Scanner scanner(")");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    EXPECT_THROW(parser.expression(), std::runtime_error);
  }

  {
    // invalid: unclosed parenthesis
    scanner::Scanner scanner("(123");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    EXPECT_THROW(parser.expression(), std::runtime_error);
  }

  {
    // invalid: empty input
    scanner::Scanner scanner("");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    EXPECT_THROW(parser.expression(), std::runtime_error);
  }

  // {
  //   // invalid: unknown identifier
  //   scanner::Scanner scanner("unknown");
  //   auto tokens = scanner.scan_tokens();
  //   Parser parser(tokens);
  //   EXPECT_THROW(parser.expression(), std::runtime_error);
  // }
  {
    // identifier: parser 应当接受标识符，语义错误由后续阶段报告
    scanner::Scanner scanner("unknown");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::VariableExpr>(*expr));
    const auto &varExpr = std::get<expr::VariableExpr>(*expr);
    EXPECT_EQ(varExpr.name.lexeme(), "unknown");
  }
}

TEST(parserTest, testUnary) {
  {
    // unary not
    scanner::Scanner scanner("!false");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::UnaryExpr>(*expr));
    const auto &unaryExpr = std::get<expr::UnaryExpr>(*expr);
    EXPECT_EQ(unaryExpr.op.type(), token::TokenType::BANG);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*unaryExpr.right));
    const auto &rightLiteral = std::get<expr::LiteralExpr>(*unaryExpr.right);
    EXPECT_EQ(std::get<bool>(rightLiteral.value), false);
  }

  {
    // unary minus
    scanner::Scanner scanner("-123");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::UnaryExpr>(*expr));
    const auto &unaryExpr = std::get<expr::UnaryExpr>(*expr);
    EXPECT_EQ(unaryExpr.op.type(), token::TokenType::MINUS);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*unaryExpr.right));
    const auto &rightLiteral = std::get<expr::LiteralExpr>(*unaryExpr.right);

    // 检查数值类型，可能是 int 或 double
    if (std::holds_alternative<int>(rightLiteral.value)) {
      EXPECT_EQ(std::get<int>(rightLiteral.value), 123);
    } else if (std::holds_alternative<double>(rightLiteral.value)) {
      EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 123.0);
    } else {
      FAIL() << "Expected int or double literal";
    }
  }
}

TEST(parserTest, testFactor) {
  {
    // multiplication
    scanner::Scanner scanner("3 * 4");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::BinaryExpr>(*expr));
    const auto &binaryExpr = std::get<expr::BinaryExpr>(*expr);
    EXPECT_EQ(binaryExpr.op.type(), token::TokenType::STAR);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.left));
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.right));

    const auto &leftLiteral = std::get<expr::LiteralExpr>(*binaryExpr.left);
    const auto &rightLiteral = std::get<expr::LiteralExpr>(*binaryExpr.right);

    // 检查数值类型
    if (std::holds_alternative<int>(leftLiteral.value)) {
      EXPECT_EQ(std::get<int>(leftLiteral.value), 3);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 3.0);
    }

    if (std::holds_alternative<int>(rightLiteral.value)) {
      EXPECT_EQ(std::get<int>(rightLiteral.value), 4);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 4.0);
    }
  }

  {
    // division
    scanner::Scanner scanner("10 / 2");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::BinaryExpr>(*expr));
    const auto &binaryExpr = std::get<expr::BinaryExpr>(*expr);
    EXPECT_EQ(binaryExpr.op.type(), token::TokenType::SLASH);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.left));
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.right));

    const auto &leftLiteral = std::get<expr::LiteralExpr>(*binaryExpr.left);
    const auto &rightLiteral = std::get<expr::LiteralExpr>(*binaryExpr.right);

    // 检查数值类型
    if (std::holds_alternative<int>(leftLiteral.value)) {
      EXPECT_EQ(std::get<int>(leftLiteral.value), 10);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 10.0);
    }

    if (std::holds_alternative<int>(rightLiteral.value)) {
      EXPECT_EQ(std::get<int>(rightLiteral.value), 2);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 2.0);
    }
  }
}

TEST(parserTest, testTerm) {
  {
    // addition
    scanner::Scanner scanner("5 + 7");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::BinaryExpr>(*expr));
    const auto &binaryExpr = std::get<expr::BinaryExpr>(*expr);
    EXPECT_EQ(binaryExpr.op.type(), token::TokenType::PLUS);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.left));
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.right));

    const auto &leftLiteral = std::get<expr::LiteralExpr>(*binaryExpr.left);
    const auto &rightLiteral = std::get<expr::LiteralExpr>(*binaryExpr.right);

    // 检查数值类型
    if (std::holds_alternative<int>(leftLiteral.value)) {
      EXPECT_EQ(std::get<int>(leftLiteral.value), 5);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 5.0);
    }

    if (std::holds_alternative<int>(rightLiteral.value)) {
      EXPECT_EQ(std::get<int>(rightLiteral.value), 7);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 7.0);
    }
  }

  {
    // subtraction
    scanner::Scanner scanner("20 - 8");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::BinaryExpr>(*expr));
    const auto &binaryExpr = std::get<expr::BinaryExpr>(*expr);
    EXPECT_EQ(binaryExpr.op.type(), token::TokenType::MINUS);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.left));
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.right));

    const auto &leftLiteral = std::get<expr::LiteralExpr>(*binaryExpr.left);
    const auto &rightLiteral = std::get<expr::LiteralExpr>(*binaryExpr.right);

    // 检查数值类型
    if (std::holds_alternative<int>(leftLiteral.value)) {
      EXPECT_EQ(std::get<int>(leftLiteral.value), 20);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 20.0);
    }

    if (std::holds_alternative<int>(rightLiteral.value)) {
      EXPECT_EQ(std::get<int>(rightLiteral.value), 8);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 8.0);
    }
  }
}

TEST(parserTest, testComparison) {
  {
    // greater than
    scanner::Scanner scanner("10 > 5");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::BinaryExpr>(*expr));
    const auto &binaryExpr = std::get<expr::BinaryExpr>(*expr);
    EXPECT_EQ(binaryExpr.op.type(), token::TokenType::GREATER);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.left));
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.right));

    const auto &leftLiteral = std::get<expr::LiteralExpr>(*binaryExpr.left);
    const auto &rightLiteral = std::get<expr::LiteralExpr>(*binaryExpr.right);

    // 检查数值类型
    if (std::holds_alternative<int>(leftLiteral.value)) {
      EXPECT_EQ(std::get<int>(leftLiteral.value), 10);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 10.0);
    }

    if (std::holds_alternative<int>(rightLiteral.value)) {
      EXPECT_EQ(std::get<int>(rightLiteral.value), 5);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 5.0);
    }
  }
}

TEST(parserTest, testEquality) {
  {
    // equality
    scanner::Scanner scanner("3 == 3");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::BinaryExpr>(*expr));
    const auto &binaryExpr = std::get<expr::BinaryExpr>(*expr);
    EXPECT_EQ(binaryExpr.op.type(), token::TokenType::EQUAL_EQUAL);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.left));
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.right));

    const auto &leftLiteral = std::get<expr::LiteralExpr>(*binaryExpr.left);
    const auto &rightLiteral = std::get<expr::LiteralExpr>(*binaryExpr.right);

    // 检查数值类型
    if (std::holds_alternative<int>(leftLiteral.value)) {
      EXPECT_EQ(std::get<int>(leftLiteral.value), 3);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 3.0);
    }

    if (std::holds_alternative<int>(rightLiteral.value)) {
      EXPECT_EQ(std::get<int>(rightLiteral.value), 3);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 3.0);
    }
  }

  {
    // inequality
    scanner::Scanner scanner("4 != 5");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);
    EXPECT_TRUE(std::holds_alternative<expr::BinaryExpr>(*expr));
    const auto &binaryExpr = std::get<expr::BinaryExpr>(*expr);
    EXPECT_EQ(binaryExpr.op.type(), token::TokenType::BANG_EQUAL);
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.left));
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*binaryExpr.right));

    const auto &leftLiteral = std::get<expr::LiteralExpr>(*binaryExpr.left);
    const auto &rightLiteral = std::get<expr::LiteralExpr>(*binaryExpr.right);

    // 检查数值类型
    if (std::holds_alternative<int>(leftLiteral.value)) {
      EXPECT_EQ(std::get<int>(leftLiteral.value), 4);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 4.0);
    }

    if (std::holds_alternative<int>(rightLiteral.value)) {
      EXPECT_EQ(std::get<int>(rightLiteral.value), 5);
    } else {
      EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 5.0);
    }
  }
}

TEST(parserTest, testExpression) {
  {
    // 简单表达式测试: 3 + 4 * 2
    scanner::Scanner scanner("3 + 4 * 2");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);

    // 结构应该是：
    // BinaryExpr(+)
    //   ├── LiteralExpr(3)
    //   └── BinaryExpr(*)
    //         ├── LiteralExpr(4)
    //         └── LiteralExpr(2)

    EXPECT_TRUE(std::holds_alternative<expr::BinaryExpr>(*expr));
    const auto &additionExpr = std::get<expr::BinaryExpr>(*expr);
    EXPECT_EQ(additionExpr.op.type(), token::TokenType::PLUS);

    // 左操作数应该是字面量 3
    EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*additionExpr.left));

    // 右操作数应该是乘法表达式
    EXPECT_TRUE(std::holds_alternative<expr::BinaryExpr>(*additionExpr.right));
    const auto &multiplicationExpr =
        std::get<expr::BinaryExpr>(*additionExpr.right);
    EXPECT_EQ(multiplicationExpr.op.type(), token::TokenType::STAR);
  }
}

TEST(parserTest, testExprPrinter) {
  {
    // expression: !(3 + 4 * 2 == 11)
    scanner::Scanner scanner("!(3 + 4 * 2 == 11)");
    auto tokens = scanner.scan_tokens();
    Parser parser(tokens);
    auto expr = parser.expression();
    EXPECT_NE(expr, nullptr);

    // Print the expression
    expr::ExprPrinter printer;
    std::cout << "Expression Printer Output: ";
    printer.print(*expr);
    std::cout << std::endl;
  }
}

} // namespace parser
} // namespace dtoy

// #include "gtest/gtest.h"
// #include "parser.h"
// #include "scanner.h"

// namespace dtoy {
// namespace parser {

// TEST(parserTest, testPrimary) {
//   // Test Literal Expressions
//   {
//     // true literal
//     scanner::Scanner scanner("true");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*expr));
//     const auto& literal = std::get<LiteralExpr>(*expr);
//     EXPECT_EQ(std::get<bool>(literal.value), true);
//   }

//   {
//     // false literal
//     scanner::Scanner scanner("false");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*expr));
//     const auto& literal = std::get<LiteralExpr>(*expr);
//     EXPECT_EQ(std::get<bool>(literal.value), false);
//   }

//   {
//     // nil literal
//     scanner::Scanner scanner("nil");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*expr));
//     const auto& literal = std::get<LiteralExpr>(*expr);
//     EXPECT_TRUE(std::holds_alternative<std::nullptr_t>(literal.value));
//   }

//   {
//     // integer literal
//     scanner::Scanner scanner("123");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*expr));
//     const auto& literal = std::get<LiteralExpr>(*expr);
//     EXPECT_DOUBLE_EQ(std::get<int>(literal.value), 123);
//   }

//   {
//     // float literal
//     scanner::Scanner scanner("45.67");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*expr));
//     const auto& literal = std::get<LiteralExpr>(*expr);
//     EXPECT_DOUBLE_EQ(std::get<double>(literal.value), 45.67);
//   }

//   {
//     // string literal
//     scanner::Scanner scanner("\"hello\"");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*expr));
//     const auto& literal = std::get<LiteralExpr>(*expr);
//     EXPECT_EQ(std::get<std::string>(literal.value), "hello");
//   }

//   {
//     // empty string literal
//     scanner::Scanner scanner("\"\"");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*expr));
//     const auto& literal = std::get<LiteralExpr>(*expr);
//     EXPECT_EQ(std::get<std::string>(literal.value), "");
//   }

//   {
//     // grouping with number
//     scanner::Scanner scanner("(123)");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//   }

//   {
//     // grouping with boolean
//     scanner::Scanner scanner("(true)");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//   }

//   {
//     // nested grouping
//     scanner::Scanner scanner("((123))");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//   }

//   {
//     // invalid: only plus
//     scanner::Scanner scanner("+");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     EXPECT_THROW(parser.expression(), std::runtime_error);
//   }

//   {
//     // invalid: only right parenthesis
//     scanner::Scanner scanner(")");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     EXPECT_THROW(parser.expression(), std::runtime_error);
//   }

//   {
//     // invalid: unclosed parenthesis
//     scanner::Scanner scanner("(123");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     EXPECT_THROW(parser.expression(), std::runtime_error);
//   }

//   {
//     // invalid: empty input
//     scanner::Scanner scanner("");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     EXPECT_THROW(parser.expression(), std::runtime_error);
//   }

//   {
//     // invalid: unknown identifier
//     scanner::Scanner scanner("unknown");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     EXPECT_THROW(parser.expression(), std::runtime_error);
//   }
// }

// TEST(parserTest, testUnary) {
//   {
//     // unary not
//     scanner::Scanner scanner("!false");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<UnaryExpr>(*expr));
//     const auto& unaryExpr = std::get<UnaryExpr>(*expr);
//     EXPECT_EQ(unaryExpr.op.type(), token::TokenType::BANG);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*unaryExpr.right));
//     const auto& rightLiteral = std::get<LiteralExpr>(*unaryExpr.right);
//     EXPECT_EQ(std::get<bool>(rightLiteral.value), false);
//   }

//   {
//     // unary minus
//     scanner::Scanner scanner("-123");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<UnaryExpr>(*expr));
//     const auto& unaryExpr = std::get<UnaryExpr>(*expr);
//     EXPECT_EQ(unaryExpr.op.type(), token::TokenType::MINUS);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*unaryExpr.right));
//     const auto& rightLiteral = std::get<LiteralExpr>(*unaryExpr.right);
//     EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 123.0);
//   }
// }

// TEST(parserTest, testFactor) {
//   {
//     // multiplication
//     scanner::Scanner scanner("3 * 4");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<BinaryExpr>(*expr));
//     const auto& binaryExpr = std::get<BinaryExpr>(*expr);
//     EXPECT_EQ(binaryExpr.op.type(), token::TokenType::STAR);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.left));
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.right));
//     const auto& leftLiteral = std::get<LiteralExpr>(*binaryExpr.left);
//     const auto& rightLiteral = std::get<LiteralExpr>(*binaryExpr.right);
//     EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 3.0);
//     EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 4.0);
//   }

//   {
//     // division
//     scanner::Scanner scanner("10 / 2");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<BinaryExpr>(*expr));
//     const auto& binaryExpr = std::get<BinaryExpr>(*expr);
//     EXPECT_EQ(binaryExpr.op.type(), token::TokenType::SLASH);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.left));
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.right));
//     const auto& leftLiteral = std::get<LiteralExpr>(*binaryExpr.left);
//     const auto& rightLiteral = std::get<LiteralExpr>(*binaryExpr.right);
//     EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 10.0);
//     EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 2.0);
//   }
// }

// TEST(parserTest, testTerm) {
//   {
//     // addition
//     scanner::Scanner scanner("5 + 7");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<BinaryExpr>(*expr));
//     const auto& binaryExpr = std::get<BinaryExpr>(*expr);
//     EXPECT_EQ(binaryExpr.op.type(), token::TokenType::PLUS);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.left));
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.right));
//     const auto& leftLiteral = std::get<LiteralExpr>(*binaryExpr.left);
//     const auto& rightLiteral = std::get<LiteralExpr>(*binaryExpr.right);
//     EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 5.0);
//     EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 7.0);
//   }

//   {
//     // subtraction
//     scanner::Scanner scanner("20 - 8");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<BinaryExpr>(*expr));
//     const auto& binaryExpr = std::get<BinaryExpr>(*expr);
//     EXPECT_EQ(binaryExpr.op.type(), token::TokenType::MINUS);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.left));
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.right));
//     const auto& leftLiteral = std::get<LiteralExpr>(*binaryExpr.left);
//     const auto& rightLiteral = std::get<LiteralExpr>(*binaryExpr.right);
//     EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 20.0);
//     EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 8.0);
//   }
// }

// TEST(parserTest, testComparison) {
//   {
//     // greater than
//     scanner::Scanner scanner("10 > 5");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<BinaryExpr>(*expr));
//     const auto& binaryExpr = std::get<BinaryExpr>(*expr);
//     EXPECT_EQ(binaryExpr.op.type(), token::TokenType::GREATER);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.left));
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.right));
//     const auto& leftLiteral = std::get<LiteralExpr>(*binaryExpr.left);
//     const auto& rightLiteral = std::get<LiteralExpr>(*binaryExpr.right);
//     EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 10.0);
//     EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 5.0);
//   }
// }

// TEST(parserTest, testEquality) {
//   {
//     // equality
//     scanner::Scanner scanner("3 == 3");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<BinaryExpr>(*expr));
//     const auto& binaryExpr = std::get<BinaryExpr>(*expr);
//     EXPECT_EQ(binaryExpr.op.type(), token::TokenType::EQUAL_EQUAL);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.left));
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.right));
//     const auto& leftLiteral = std::get<LiteralExpr>(*binaryExpr.left);
//     const auto& rightLiteral = std::get<LiteralExpr>(*binaryExpr.right);
//     EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 3.0);
//     EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 3.0);
//   }

//   {
//     // inequality
//     scanner::Scanner scanner("4 != 5");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);
//     EXPECT_TRUE(std::holds_alternative<BinaryExpr>(*expr));
//     const auto& binaryExpr = std::get<BinaryExpr>(*expr);
//     EXPECT_EQ(binaryExpr.op.type(), token::TokenType::BANG_EQUAL);
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.left));
//     EXPECT_TRUE(std::holds_alternative<LiteralExpr>(*binaryExpr.right));
//     const auto& leftLiteral = std::get<LiteralExpr>(*binaryExpr.left);
//     const auto& rightLiteral = std::get<LiteralExpr>(*binaryExpr.right);
//     EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 4.0);
//     EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 5.0);
//   }
// }

// TEST(parserTest, testExpression) {
//   {
//     // 修改表达式以获得期望的结构: !(3 + 4 * 2) == 11
//     scanner::Scanner scanner("!(3 + 4 * 2) == 11");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);

//     // 现在结构应该是：
//     // BinaryExpr(==)
//     //   ├── UnaryExpr(!)
//     //   │     └── GroupingExpr
//     //   │           └── BinaryExpr(+)
//     //   │                 ├── LiteralExpr(3)
//     //   │                 └── BinaryExpr(*)
//     //   │                       ├── LiteralExpr(4)
//     //   │                       └── LiteralExpr(2)
//     //   └── LiteralExpr(11)

//     // Test the outermost binary expression (equality)
//     EXPECT_TRUE(std::holds_alternative<expr::BinaryExpr>(*expr));
//     const auto& equalityExpr = std::get<expr::BinaryExpr>(*expr);
//     EXPECT_EQ(equalityExpr.op.type(), token::TokenType::EQUAL_EQUAL);

//     // Test the left side of equality (unary expression)
//     EXPECT_TRUE(std::holds_alternative<expr::UnaryExpr>(*equalityExpr.left));
//     const auto& unaryExpr = std::get<expr::UnaryExpr>(*equalityExpr.left);
//     EXPECT_EQ(unaryExpr.op.type(), token::TokenType::BANG);

//     // Test the grouping expression inside unary
//     EXPECT_TRUE(std::holds_alternative<expr::GroupingExpr>(*unaryExpr.right));
//     const auto& groupingExpr =
//     std::get<expr::GroupingExpr>(*unaryExpr.right);
//     EXPECT_TRUE(std::holds_alternative<expr::BinaryExpr>(*groupingExpr.expression));
//     const auto& additionExpr =
//     std::get<expr::BinaryExpr>(*groupingExpr.expression);
//     EXPECT_EQ(additionExpr.op.type(), token::TokenType::PLUS);
//     // Test the left side of addition (literal 3)
//     EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*additionExpr.left));
//     const auto& leftLiteral =
//     std::get<expr::LiteralExpr>(*additionExpr.left);
//     EXPECT_DOUBLE_EQ(std::get<double>(leftLiteral.value), 3.0);
//     // Test the right side of addition (multiplication)
//     EXPECT_TRUE(std::holds_alternative<expr::BinaryExpr>(*additionExpr.right));
//     const auto& multiplicationExpr =
//     std::get<expr::BinaryExpr>(*additionExpr.right);
//     EXPECT_EQ(multiplicationExpr.op.type(), token::TokenType::STAR);
//     // Test the left side of multiplication (literal 4)
//     EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*multiplicationExpr.left));
//     const auto& multLeftLiteral =
//     std::get<expr::LiteralExpr>(*multiplicationExpr.left);
//     EXPECT_DOUBLE_EQ(std::get<double>(multLeftLiteral.value), 4.0);
//     // Test the right side of multiplication (literal 2)
//     EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*multiplicationExpr.right));
//     const auto& multRightLiteral =
//     std::get<expr::LiteralExpr>(*multiplicationExpr.right);
//     EXPECT_DOUBLE_EQ(std::get<double>(multRightLiteral.value), 2.0);
//     // Test the right side of equality (literal 11)
//     EXPECT_TRUE(std::holds_alternative<expr::LiteralExpr>(*equalityExpr.right));
//     const auto& rightLiteral =
//     std::get<expr::LiteralExpr>(*equalityExpr.right);
//     EXPECT_DOUBLE_EQ(std::get<double>(rightLiteral.value), 11.0);

//   }
// }

// TEST(parserTest, testExprPrinter) {
//   {
//     // expression: !(3 + 4 * 2 == 11)
//     scanner::Scanner scanner("!(3 + 4 * 2 == 11)");
//     auto tokens = scanner.scan_tokens();
//     Parser parser(tokens);
//     auto expr = parser.expression();
//     EXPECT_NE(expr, nullptr);

//     // Print the expression
//     ExprPrinter printer;
//     std::cout << "Expression Printer Output: ";
//     printer.print(*expr);
//     std::cout << std::endl;
//   }
// }

// }
// }