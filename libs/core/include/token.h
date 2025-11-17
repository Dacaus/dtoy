#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <variant>
namespace dtoy {
namespace token {

enum class TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals.
  IDENTIFIER,
  STRING,
  NUMBER,
  CHAR,
  BOOL,
  // Keywords.
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  EOF_,
};

using Literal = std::variant<std::string,bool, char, int, double, std::nullptr_t,std::monostate>;

class Token {
public:
  Token(TokenType type, std ::string lexeme, Literal literal, int line)
      : type_(type), lexeme_(lexeme), literal_(literal), line_(line) {};
  Token(TokenType type, std::string lexeme, int line)
      : type_(type), lexeme_(lexeme), literal_(std::monostate{}), line_(line) {
        };
  TokenType type() const {
    return type_;
  }
  Literal literal() const {
    return literal_;
  }
  std::string lexeme() const {
    return lexeme_;
  }
  int line() const {
    return line_;
  }
  std::string type_name() const {
    return type_names[type_];
  }

public:
  std::string to_string() const;
  std::string literalToString() const;
  static std::pair<bool,TokenType>  isKeyword(const std::string& str);


private:
  TokenType type_;
  Literal literal_;
  std::string lexeme_;
  int line_;
  static std::map<TokenType, std::string> type_names;
  static std::map<std::string, TokenType> keywords;
};

}  // namespace token
}  // namespace dtoy