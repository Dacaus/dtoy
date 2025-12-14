#pragma once

#include <iostream>
#include <memory>
#include <variant>

#include "token.h"
namespace dtoy {
namespace expr {

// class BinaryExpr;
// class LiteralExpr;
// class GroupingExpr;
// class UnaryExpr;
// class VariableExpr;
// class AssignExpr;
// class CallExpr;

class Expr;

class BinaryExpr {
public:
  std::unique_ptr<Expr> left;
  token::Token op;
  std::unique_ptr<Expr> right;
  BinaryExpr(std::unique_ptr<Expr> left, token::Token op,
             std::unique_ptr<Expr> right)
      : left(std::move(left)), op(op), right(std::move(right)) {}
};

class AssignExpr {
public:
  token::Token name;
  std::unique_ptr<Expr> value;
  AssignExpr(token::Token name, std::unique_ptr<Expr> value)
      : name(name), value(std::move(value)) {}
};


class UnaryExpr {
public:
  token::Token op;
  std::unique_ptr<Expr> right;
  UnaryExpr(token::Token op, std::unique_ptr<Expr> right)
      : op(op), right(std::move(right)) {}
};

class LiteralExpr {
public:
  token::Literal value;
  LiteralExpr(token::Literal value) : value(value) {}
};

class GroupingExpr {
public:
  std::unique_ptr<Expr> expression;
  GroupingExpr(std::unique_ptr<Expr> expression)
      : expression(std::move(expression)) {}
};

class VariableExpr {
public:
  token::Token name;
  VariableExpr(token::Token name) : name(name) {}
};

// 把 GroupingExpr 加入 variant
using ExprBase = std::variant<BinaryExpr, UnaryExpr, LiteralExpr, GroupingExpr,VariableExpr,AssignExpr>;

class Expr : public ExprBase {
public:
  using ExprBase::ExprBase;
  using ExprBase::operator=;
};
// ... 保持 Expr 定义不变 ...

struct ExprPrinter {
  void print(const Expr &expr) const { std::visit(*this, expr); }

  // 处理 BinaryExpr
  void operator()(const BinaryExpr &expr) const {
    std::cout << "(" << expr.op.lexeme();
    std::visit(*this, *expr.left);
    std::visit(*this, *expr.right);
    std::cout << ")";
  }

  // 处理 UnaryExpr
  void operator()(const UnaryExpr &expr) const {
    std::cout << "(" << expr.op.lexeme();
    std::visit(*this, *expr.right);
    std::cout << ")";
  }

  // 处理 GroupingExpr
  void operator()(const GroupingExpr &expr) const {
    std::cout << "(group";
    std::visit(*this, *expr.expression);
    std::cout << ")";
  }

  void operator() (const VariableExpr &expr) const {
    std::cout << "(var " << expr.name.lexeme() << ")";
  }
  // 处理 LiteralExpr
  void operator()(const LiteralExpr &expr) const {
    if (std::holds_alternative<int>(expr.value)) {
      std::cout << " " << std::get<int>(expr.value);
    } else if (std::holds_alternative<double>(expr.value)) {
      std::cout << " " << std::get<double>(expr.value);
    } else if (std::holds_alternative<std::string>(expr.value)) {
      std::cout << " \"" << std::get<std::string>(expr.value) << "\"";
    } else if (std::holds_alternative<bool>(expr.value)) {
      std::cout << " " << (std::get<bool>(expr.value) ? "true" : "false");
    } else {
      std::cout << " nil";
    }
  }

  void operator()(const AssignExpr &expr) const {
    std::cout << "(= " << expr.name.lexeme() << " ";
    std::visit(*this, *expr.value);
    std::cout << ")";
  }
};


} // namespace expr
} // namespace dtoy