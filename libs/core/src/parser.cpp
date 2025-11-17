#include "parser.h"

#include <memory>
#include <stdexcept>

#include "expr.h"
#include "token.h"
#include "stmt.h"
using namespace dtoy::expr;
namespace dtoy {
namespace parser {
using namespace dtoy::expr;
using namespace dtoy::stmt;
std::unique_ptr<Expr> Parser::expression() { return equality(); }
std::unique_ptr<Expr> Parser::equality() {
  std::unique_ptr<Expr> expr = comparison();
  while (match({token::TokenType::BANG_EQUAL, token::TokenType::EQUAL_EQUAL})) {
    token::Token op = previous();
    std::unique_ptr<Expr> right = comparison();
    expr = std::make_unique<Expr>(
        BinaryExpr(std::move(expr), op, std::move(right)));
  }
  return expr;
}
std::unique_ptr<Expr> Parser::comparison() {
  std::unique_ptr<Expr> expr = term();
  while (match({token::TokenType::GREATER, token::TokenType::GREATER_EQUAL,
                token::TokenType::LESS, token::TokenType::LESS_EQUAL})) {
    token::Token op = previous();
    std::unique_ptr<Expr> right = term();
    expr = std::make_unique<Expr>(
        BinaryExpr(std::move(expr), op, std::move(right)));
  }
  return expr;
}
std::unique_ptr<Expr> Parser::term() {
  std::unique_ptr<Expr> expr = factor();
  while (match({token::TokenType::MINUS, token::TokenType::PLUS})) {
    token::Token op = previous();
    std::unique_ptr<Expr> right = factor();
    expr = std::make_unique<Expr>(
        BinaryExpr(std::move(expr), op, std::move(right)));
  }
  return expr;
}
std::unique_ptr<Expr> Parser::factor() {
  std::unique_ptr<Expr> expr = unary();
  while (match({token::TokenType::SLASH, token::TokenType::STAR})) {
    token::Token op = previous();
    std::unique_ptr<Expr> right = unary();
    expr = std::make_unique<Expr>(
        BinaryExpr(std::move(expr), op, std::move(right)));
  }
  return expr;
}
std::unique_ptr<Expr> Parser::unary() {
  while (match({token::TokenType::BANG, token::TokenType::MINUS})) {
    token::Token op = previous();
    std::unique_ptr<Expr> right = unary();
    return std::make_unique<Expr>(UnaryExpr(op, std::move(right)));
  }
  return primary();
}
std::unique_ptr<Expr> Parser::primary() {
  if (match({token::TokenType::FALSE})) {
    return std::make_unique<Expr>(LiteralExpr(previous().literal()));
  } else if (match({token::TokenType::TRUE})) {
    return std::make_unique<Expr>(LiteralExpr(previous().literal()));
  } else if (match({token::TokenType::NIL})) {
    return std::make_unique<Expr>(LiteralExpr(previous().literal()));
  }
  if (match({token::TokenType::NUMBER, token::TokenType::STRING})) {
    return std::make_unique<Expr>(LiteralExpr(previous().literal()));
  }
  if (match({token::TokenType::LEFT_PAREN})) {
    std::unique_ptr<Expr> expr = expression();
    if (!match({token::TokenType::RIGHT_PAREN})) {
      throw std::runtime_error("Expect ')' after expression.");
    }
    // 这里需要创建 GroupingExpr，而不是直接返回 expr
    return std::make_unique<Expr>(GroupingExpr(std::move(expr)));
  }
  // 如果没有匹配任何primary表达式，抛出错误
  throw std::runtime_error("line: " + std::to_string(peek().line()) +
                           "lexeme:" + peek().lexeme() + "Expect expression.");
}

bool Parser::match(std::initializer_list<token::TokenType> types) {
  for (auto type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }
  return false;
}
bool Parser::check(token::TokenType type) {
  return !isAtEnd() && peek().type() == type;
}
token::Token Parser::advance() {
  if (!isAtEnd())
    current_++;
  return previous();
}
bool Parser::isAtEnd() {
  if (current_ >= tokens_.size())
    return true;
  return tokens_[current_].type() == token::TokenType::EOF_;
}
token::Token Parser::peek() { return tokens_[current_]; }
token::Token Parser::previous() { return tokens_[current_ - 1]; }

std::unique_ptr<Stmt> Parser::statement(){
  if(match({token::TokenType::PRINT})){
    return printStatement();
  }
  return exprStatement();
}

std::unique_ptr<Stmt> Parser::printStatement(){
  std::unique_ptr<Expr> value = expression();
  if(!match({token::TokenType::SEMICOLON})){
    throw std::runtime_error("Expect ';' after value.");
  }
  return std::make_unique<Stmt>(PrintStmt(std::move(value)));
}

std::unique_ptr<Stmt> Parser::exprStatement(){
  std::unique_ptr<Expr> expr = expression();
  if(!match({token::TokenType::SEMICOLON})){
    throw std::runtime_error("Expect ';' after expression.");
  }
  return std::make_unique<Stmt>(ExpressionStmt(std::move(expr)));   
}


} // namespace parser
} // namespace dtoy