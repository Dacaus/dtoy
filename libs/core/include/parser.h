#pragma once
#include "expr.h"
#include "stmt.h"
#include "token.h"
#include <memory>
#include <vector>

namespace dtoy {
namespace parser {
using namespace dtoy::expr;
using namespace dtoy::stmt;

class Parser {
public:
  Parser(std::vector<token::Token> tokens) : tokens_(tokens) {};
  std::vector<std::unique_ptr<Stmt>> parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!isAtEnd()) {
      statements.push_back(statement());
    }
    return statements;
  };

public:
  std::unique_ptr<Expr> evaluate();
  std::unique_ptr<Expr> expression();
  std::unique_ptr<Stmt> statement();

private:
  std::unique_ptr<Expr> equality();
  std::unique_ptr<Expr> comparison();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> unary();
  std::unique_ptr<Expr> primary();
  std::unique_ptr<Stmt> printStatement();
  std::unique_ptr<Stmt> exprStatement();
  std::unique_ptr<Stmt> declaration();
private:
  bool match(std::initializer_list<token::TokenType> types);
  bool check(token::TokenType type);
  token::Token advance();
  bool isAtEnd();
  token::Token peek();
  token::Token previous();

private:
  int current_ = 0;
  std::vector<token::Token> tokens_;
};

} // namespace parser
} // namespace dtoy