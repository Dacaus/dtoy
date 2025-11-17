#include <gtest/gtest.h>

#include "scanner.h"

namespace dtoy {
namespace scanner {
class ScannerTest : public ::testing::Test {
public:
  void scan_token(Scanner &obj) {
    obj.scan_token();
  }

protected:
  Scanner scanner_1;
};

TEST_F(ScannerTest, show_tokens) {
  scanner_1 = Scanner("");  // 空输入
  EXPECT_EQ(scanner_1.show_tokens().size(), 0);
}

// Literal相关的
TEST_F(ScannerTest, AddTokenSingleCharacter) {
  scanner_1 = Scanner("(){},.+-*/");  // 空输入

  scan_token(scanner_1);  // (
  EXPECT_EQ(scanner_1.show_tokens().size(), 1);
  EXPECT_EQ(scanner_1.show_tokens()[0].type(), token::TokenType::LEFT_PAREN);

  scan_token(scanner_1);  // )
  EXPECT_EQ(scanner_1.show_tokens().size(), 2);
  EXPECT_EQ(scanner_1.show_tokens()[1].type(), token::TokenType::RIGHT_PAREN);

  scan_token(scanner_1);  // {
  EXPECT_EQ(scanner_1.show_tokens().size(), 3);
  EXPECT_EQ(scanner_1.show_tokens()[2].type(), token::TokenType::LEFT_BRACE);

  scan_token(scanner_1);  // }
  EXPECT_EQ(scanner_1.show_tokens().size(), 4);
  EXPECT_EQ(scanner_1.show_tokens()[3].type(), token::TokenType::RIGHT_BRACE);

  scan_token(scanner_1);  // ,
  EXPECT_EQ(scanner_1.show_tokens().size(), 5);
  EXPECT_EQ(scanner_1.show_tokens()[4].type(), token::TokenType::COMMA);

  scan_token(scanner_1);  // .
  EXPECT_EQ(scanner_1.show_tokens().size(), 6);
  EXPECT_EQ(scanner_1.show_tokens()[5].type(), token::TokenType::DOT);

  scan_token(scanner_1);  // +
  EXPECT_EQ(scanner_1.show_tokens().size(), 7);
  EXPECT_EQ(scanner_1.show_tokens()[6].type(), token::TokenType::PLUS);

  scan_token(scanner_1);  // -
  EXPECT_EQ(scanner_1.show_tokens().size(), 8);
  EXPECT_EQ(scanner_1.show_tokens()[7].type(), token::TokenType::MINUS);

  scan_token(scanner_1);  // *
  EXPECT_EQ(scanner_1.show_tokens().size(), 9);
  EXPECT_EQ(scanner_1.show_tokens()[8].type(), token::TokenType::STAR);

  scan_token(scanner_1);  // /
  EXPECT_EQ(scanner_1.show_tokens().size(), 10);
  EXPECT_EQ(scanner_1.show_tokens()[9].type(), token::TokenType::SLASH);
}


TEST_F(ScannerTest, AddToken_DoubleCharacter_SkipWhitespace) {
  scanner_1 = Scanner("!= ! == = >= <=");  // 空输入
  scan_token(scanner_1);                   // !=
  EXPECT_EQ(scanner_1.show_tokens().size(), 1);
  EXPECT_EQ(scanner_1.show_tokens()[0].type(), token::TokenType::BANG_EQUAL);

  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // !
  EXPECT_EQ(scanner_1.show_tokens().size(), 2);
  EXPECT_EQ(scanner_1.show_tokens()[1].type(), token::TokenType::BANG);

  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // ==
  EXPECT_EQ(scanner_1.show_tokens().size(), 3);
  EXPECT_EQ(scanner_1.show_tokens()[2].type(), token::TokenType::EQUAL_EQUAL);

  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // =
  EXPECT_EQ(scanner_1.show_tokens().size(), 4);
  EXPECT_EQ(scanner_1.show_tokens()[3].type(), token::TokenType::EQUAL);

  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // >=
  EXPECT_EQ(scanner_1.show_tokens().size(), 5);
  EXPECT_EQ(scanner_1.show_tokens()[4].type(), token::TokenType::GREATER_EQUAL);

  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // <=
  EXPECT_EQ(scanner_1.show_tokens().size(), 6);
  EXPECT_EQ(scanner_1.show_tokens()[5].type(), token::TokenType::LESS_EQUAL);
}


TEST_F(ScannerTest, AddTokenIdentifer) {
  scanner_1 = Scanner("varName anotherVar _privateVar var123");  // 空输入
  scan_token(scanner_1);                                         // varName
  EXPECT_EQ(scanner_1.show_tokens().size(), 1);
  EXPECT_EQ(scanner_1.show_tokens()[0].type(), token::TokenType::IDENTIFIER);
  EXPECT_EQ(scanner_1.show_tokens()[0].lexeme(), "varName");


  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // anotherVar
  EXPECT_EQ(scanner_1.show_tokens().size(), 2);
  EXPECT_EQ(scanner_1.show_tokens()[1].type(), token::TokenType::IDENTIFIER);
  EXPECT_EQ(scanner_1.show_tokens()[1].lexeme(), "anotherVar");


  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // _privateVar
  EXPECT_EQ(scanner_1.show_tokens().size(), 3);
  EXPECT_EQ(scanner_1.show_tokens()[2].type(), token::TokenType::IDENTIFIER);
  EXPECT_EQ(scanner_1.show_tokens()[2].lexeme(), "_privateVar");

  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // var123
  EXPECT_EQ(scanner_1.show_tokens().size(), 4);
  EXPECT_EQ(scanner_1.show_tokens()[3].type(), token::TokenType::IDENTIFIER);
  EXPECT_EQ(scanner_1.show_tokens()[3].lexeme(), "var123");
}

TEST_F(ScannerTest, AddTokenNumber) {
  scanner_1 = Scanner("123 45.67 0.89 1000");  // 空输入
  scan_token(scanner_1);                       // 123
  EXPECT_EQ(scanner_1.show_tokens().size(), 1);
  EXPECT_EQ(scanner_1.show_tokens()[0].type(), token::TokenType::NUMBER);
  EXPECT_EQ(scanner_1.show_tokens()[0].lexeme(), "123");

  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // 45.67
  EXPECT_EQ(scanner_1.show_tokens().size(), 2);
  EXPECT_EQ(scanner_1.show_tokens()[1].type(), token::TokenType::NUMBER);
  EXPECT_EQ(scanner_1.show_tokens()[1].lexeme(), "45.67");

  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // 0.89
  EXPECT_EQ(scanner_1.show_tokens().size(), 3);
  EXPECT_EQ(scanner_1.show_tokens()[2].type(), token::TokenType::NUMBER);
  EXPECT_EQ(scanner_1.show_tokens()[2].lexeme(), "0.89");


  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // 1000
  EXPECT_EQ(scanner_1.show_tokens().size(), 4);
  EXPECT_EQ(scanner_1.show_tokens()[3].type(), token::TokenType::NUMBER);
  EXPECT_EQ(scanner_1.show_tokens()[3].lexeme(), "1000");
}




TEST_F(ScannerTest, AddToken_EOF) {
  scanner_1 = Scanner("");  // 空输入
  scan_token(scanner_1);    // EOF
  EXPECT_EQ(scanner_1.show_tokens().size(), 1);
  EXPECT_EQ(scanner_1.show_tokens()[0].type(), token::TokenType::EOF_);
}


TEST_F(ScannerTest, AddTokenChar) {
  scanner_1 = Scanner("'a' 'Z' '0' '_' '\\n'   '\\t'" );  // 空输入
  scan_token(scanner_1);                   // 'a'
  EXPECT_EQ(scanner_1.show_tokens().size(), 1);
  EXPECT_EQ(scanner_1.show_tokens()[0].type(), token::TokenType::CHAR);
  EXPECT_EQ(std::get<char>(scanner_1.show_tokens()[0].literal()), 'a');
  EXPECT_EQ(scanner_1.show_tokens()[0].lexeme(), "'a'");


  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // 'Z'
  EXPECT_EQ(scanner_1.show_tokens().size(), 2);
  EXPECT_EQ(scanner_1.show_tokens()[1].type(), token::TokenType::CHAR);
  EXPECT_EQ(std::get<char>(scanner_1.show_tokens()[1].literal()), 'Z');
  EXPECT_EQ(scanner_1.show_tokens()[1].lexeme(), "'Z'");

  // skip whitespace
  scan_token(scanner_1);  // whitespace
  scan_token(scanner_1);  // '0'
  EXPECT_EQ(scanner_1.show_tokens().size(), 3);
  EXPECT_EQ(scanner_1.show_tokens()[2].type(), token::TokenType::CHAR);
  EXPECT_EQ(std::get<char>(scanner_1.show_tokens()[2].literal()), '0');
  EXPECT_EQ(scanner_1.show_tokens()[2].lexeme(), "'0'");
  // skip whitespace
  scan_token(scanner_1);  // whitespace
  scan_token(scanner_1);  // '_'
  EXPECT_EQ(scanner_1.show_tokens().size(), 4);
  EXPECT_EQ(scanner_1.show_tokens()[3].type(), token::TokenType::CHAR);
  EXPECT_EQ(std::get<char>(scanner_1.show_tokens()[3].literal()), '_');
  EXPECT_EQ(scanner_1.show_tokens()[3].lexeme(), "'_'");
  //skip whitespace
  scan_token(scanner_1);  // whitespace
  scan_token(scanner_1);  // '\n'
  EXPECT_EQ(scanner_1.show_tokens().size(), 5);
  EXPECT_EQ(scanner_1.show_tokens()[4].type(), token::TokenType::CHAR);
  EXPECT_EQ(std::get<char>(scanner_1.show_tokens()[4].literal()), '\n');
  EXPECT_EQ(scanner_1.show_tokens()[4].lexeme(), "'\\n'");
    //skip whitespace
  scan_token(scanner_1);  // whitespace
  scan_token(scanner_1);  // '\t'
  EXPECT_EQ(scanner_1.show_tokens().size(), 6);
  EXPECT_EQ(scanner_1.show_tokens()[5].type(), token::TokenType::CHAR);
  EXPECT_EQ(std::get<char>(scanner_1.show_tokens()[5].literal()), '\t');
  EXPECT_EQ(scanner_1.show_tokens()[5].lexeme(), "'\\t'");
}

TEST_F(ScannerTest, AddTokenStringWithoutEscape) {
  scanner_1 = Scanner("\"hello\" \"world\" \"test string\"");  // 空输入
  scan_token(scanner_1);                                       // "hello"
  EXPECT_EQ(scanner_1.show_tokens().size(), 1);
  EXPECT_EQ(scanner_1.show_tokens()[0].type(), token::TokenType::STRING);

  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // "world"
  EXPECT_EQ(scanner_1.show_tokens().size(), 2);
  EXPECT_EQ(scanner_1.show_tokens()[1].type(), token::TokenType::STRING);

  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // "test string"
  EXPECT_EQ(scanner_1.show_tokens().size(), 3);
  EXPECT_EQ(scanner_1.show_tokens()[2].type(), token::TokenType::STRING);
}

TEST_F(ScannerTest, AddTokenString){
  scanner_1 = Scanner("\"Hello, World!\" \"Line1\\nLine2\" \"Tab\\tCharacter\"");  // 空输入
  scan_token(scanner_1);                                       // "Hello, World!"
  EXPECT_EQ(scanner_1.show_tokens().size(), 1);
  EXPECT_EQ(scanner_1.show_tokens()[0].type(), token::TokenType::STRING);
  EXPECT_EQ(std::get<std::string>(scanner_1.show_tokens()[0].literal()), "Hello, World!");
  EXPECT_EQ(scanner_1.show_tokens()[0].lexeme(), "\"Hello, World!\"");

  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // "Line1\nLine2"
  EXPECT_EQ(scanner_1.show_tokens().size(), 2);
  EXPECT_EQ(scanner_1.show_tokens()[1].type(), token::TokenType::STRING);
  EXPECT_EQ(std::get<std::string>(scanner_1.show_tokens()[1].literal()), "Line1\nLine2");
  EXPECT_EQ(scanner_1.show_tokens()[1].lexeme(), "\"Line1\\nLine2\"");

  // skip whitespace
  scan_token(scanner_1);  // whitespace

  scan_token(scanner_1);  // "Tab\tCharacter"
  EXPECT_EQ(scanner_1.show_tokens().size(), 3);
  EXPECT_EQ(scanner_1.show_tokens()[2].type(), token::TokenType::STRING);
  EXPECT_EQ(std::get<std::string>(scanner_1.show_tokens()[2].literal()), "Tab\tCharacter");
  EXPECT_EQ(scanner_1.show_tokens()[2].lexeme(), "\"Tab\\tCharacter\"");

}


TEST_F(ScannerTest, addTokenIdentifierAndKeywords){
  scanner_1 = Scanner("if else for while return var fun class true false nil");  // 空输入

  std::vector<token::TokenType> expected_types = {
      token::TokenType::IF,     token::TokenType::ELSE,
      token::TokenType::FOR,    token::TokenType::WHILE,
      token::TokenType::RETURN, token::TokenType::VAR,
      token::TokenType::FUN,    token::TokenType::CLASS,
      token::TokenType::TRUE,   token::TokenType::FALSE,
      token::TokenType::NIL};

  for (const auto &expected_type : expected_types) {
    scan_token(scanner_1);  // scan each keyword
    EXPECT_EQ(scanner_1.show_tokens().back().type(), expected_type);
    // skip whitespace
    scan_token(scanner_1);  // whitespace
  }
  
}


}  // namespace scanner
}  // namespace dtoy