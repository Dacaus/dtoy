#include "token.h"
#include <gtest/gtest.h>

using namespace dtoy;

// 两种构造方案
//       Token(TokenType type, std ::string lexeme, Literal literal, int line)
//       : type_(type), lexeme_(lexeme), literal_(literal), line_(line) {};
//   Token(TokenType type, std::string lexeme, int line)
//       : type_(type), lexeme_(lexeme), literal_(), line_(line) {};


// 无literal
TEST(Init, Initdentifier) {
  token::Token token1(dtoy::token::TokenType::IDENTIFIER, "varName", 1);
  EXPECT_EQ(token1.type(), dtoy::token::TokenType::IDENTIFIER);
  EXPECT_EQ(token1.lexeme(), "varName");
  EXPECT_EQ(std::get<std::monostate>(token1.literal()), std::monostate{});
  EXPECT_EQ(token1.line(), 1);
}

// 有literal
TEST(Init, InitNumber) {
  dtoy::token::Token token2(dtoy::token::TokenType::NUMBER, "123", 123, 2);
  EXPECT_EQ(token2.type(), dtoy::token::TokenType::NUMBER);
  EXPECT_EQ(token2.lexeme(), "123");
  EXPECT_EQ(std::get<int>(token2.literal()), 123);
  EXPECT_EQ(token2.line(), 2);
}
