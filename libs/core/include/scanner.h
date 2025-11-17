#pragma once

#include "token.h"
// #include <cinttypes>

#include <string>
#include <vector>

namespace dtoy {
namespace scanner {

class Scanner {
public:
  friend class ScannerTest;
  friend class ParserTest;

public:
  Scanner() = default;  // 添加默认构造函数
  explicit Scanner(std::string sources)
      : sources_(std::move(sources)), start_(0), current_(0), line_(1) {};
  ~Scanner() {};

public:
  const std::vector<token::Token> &show_tokens() const;
  std::vector<token::Token> scan_tokens();

private:
  inline bool is_at_end() const;
  inline char advance();
  void skip_whitespace();
  void add_token(token::TokenType type);
  void add_token(token::TokenType type, std::string lexeme);
  void add_token(token::TokenType type, token::Literal literal);
  void add_token(token::TokenType type, std::string lexeme,
                 token::Literal literal);

  std::pair<char, char> process_escape_sequence(char escape_char);
  char peek() const;
  char peek_next() const;
  bool match(char expected);
  void scan_token();
  void string();
  void number();
  void identifierAndKeywords();

private:
  std::string sources_;
  std::vector<token::Token> tokens_;
  int current_;
  int start_;
  int line_;
};
}  // namespace scanner
}  // namespace dtoy