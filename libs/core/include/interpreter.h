// #pragma once
// #include "expr.h"
// #include "stmt.h"
// #include "token.h"
// #include "enviroment.h"
// #include <memory>
// #include <stdexcept>
// #include <vector>
// namespace dtoy {
// namespace interpreter {
// class RuntimeError : public std::runtime_error {
// public:
//   token::Token token;
//   RuntimeError(token::Token token, const std::string &message)
//       : std::runtime_error(message), token(token) {}
// };
// using namespace dtoy::stmt;
// class Interpreter {
// public:
//   Interpreter() = default;
//   void interpret(const expr::Expr &expression) {
//     try {
//       Literal result = evaluate(expression);
//       // 这里需要实现一个将Literal转换为字符串的函数
//       // 暂时先用简单输出
//       std::cout << "Result: " << literalToString(result) << std::endl;
//     } catch (const RuntimeError &error) {
//       std::cerr << "Runtime error: " << error.what() << " [line "
//                 << error.token.line() << "]" << std::endl;
//     }
//   }

//   // void interpret(const std::vector<std::unique_ptr<Stmt>> statements) {

//   //   // try {
//   //   //   for (const auto &statement : statements) {
//   //   //     if (std::holds_alternative<PrintStmt>(statement)) {
//   //   //       visitPrintStmt(std::get<PrintStmt>(statement));
//   //   //     } else if (std::holds_alternative<ExpressionStmt>(statement)) {
//   //   //       visitExpressionStmt(std::get<ExpressionStmt>(statement));
//   //   //     }
//   //   //   }
//   //   // } catch (const RuntimeError &error) {
//   //   //   std::cerr << "Runtime error: " << error.what() << " [line "
//   //   //             << error.token.line() << "]" << std::endl;
//   //   // }
//   // }

//   // ...existing code...
//   void interpret(const std::vector<std::unique_ptr<Stmt>> &statements) {
//     try {
//       for (const auto &statement_ptr : statements) {
//         if (!statement_ptr)
//           continue;
//         // 使用 std::visit 在 variant 上分派到具体的 Stmt 类型
//         auto visitor = [this](auto &&stmt_node) -> void {
//           using T = std::decay_t<decltype(stmt_node)>;
//           if constexpr (std::is_same_v<T, PrintStmt>) {
//             this->visitPrintStmt(stmt_node);
//           } else if constexpr (std::is_same_v<T, ExpressionStmt>) {
//             this->visitExpressionStmt(stmt_node);
//           } else if constexpr (std::is_same_v<T, VarStmt>) {
//             // TODO: 处理 VarStmt（当前忽略或抛出）
//           } else if constexpr (std::is_same_v<T, BlockStmt>) {
//             // 递归执行块内语句
//             this->interpret(stmt_node.statements);
//           }
//         };
//         std::visit(visitor, static_cast<const Stmt &>(*statement_ptr));
//       }
//     } catch (const RuntimeError &error) {
//       std::cerr << "Runtime error: " << error.what() << " [line "
//                 << error.token.line() << "]" << std::endl;
//     }
//   }


  


//   using Literal = token::Literal;
//   Literal evaluate(const expr::Expr &expression) {
//     // 使用std::visit来分派到对应的visit方法
//     auto visitor = [this](const auto &expr_node) -> Literal {
//       if constexpr (std::is_same_v<std::decay_t<decltype(expr_node)>,
//                                    expr::BinaryExpr>) {
//         return this->visitBinaryExpr(expr_node);
//       } else if constexpr (std::is_same_v<std::decay_t<decltype(expr_node)>,
//                                           expr::UnaryExpr>) {
//         return this->visitUnaryExpr(expr_node);
//       } else if constexpr (std::is_same_v<std::decay_t<decltype(expr_node)>,
//                                           expr::LiteralExpr>) {
//         return this->visitLiteralExpr(expr_node);
//       } else if constexpr (std::is_same_v<std::decay_t<decltype(expr_node)>,
//                                           expr::GroupingExpr>) {
//         return this->visitGroupingExpr(expr_node);
//       }
//     };

//     return std::visit(visitor, expression);
//   }



//   void visitExpressionStmt(const ExpressionStmt &statement) {
//     evaluate(*statement.expression);
//   }
//   void visitPrintStmt(const PrintStmt &statement) {
//     Literal value = evaluate(*statement.expression);
//     std::cout << literalToString(value) << std::endl;
//   }

//   Literal visitBinaryExpr(const expr::BinaryExpr &expr) {
//     Literal left = evaluate(*expr.left);
//     Literal right = evaluate(*expr.right);

//     switch (expr.op.type()) {
//     case token::TokenType::PLUS: {
//       if (std::holds_alternative<int>(left) &&
//           std::holds_alternative<int>(right)) {
//         return std::get<int>(left) + std::get<int>(right);
//       } else if (std::holds_alternative<double>(left) &&
//                  std::holds_alternative<double>(right)) {
//         return std::get<double>(left) + std::get<double>(right);
//       }

//       else if (std::holds_alternative<std::string>(left) &&
//                std::holds_alternative<std::string>(right)) {
//         return std::get<std::string>(left) + std::get<std::string>(right);
//       } else {
//         throw RuntimeError(expr.op,
//                            "Operands must be two numbers or two strings.");
//       }
//     }
//     case token::TokenType::MINUS: {
//       if (std::holds_alternative<int>(left) &&
//           std::holds_alternative<int>(right)) {
//         return std::get<int>(left) - std::get<int>(right);
//       } else if (std::holds_alternative<double>(left) &&
//                  std::holds_alternative<double>(right)) {
//         return std::get<double>(left) - std::get<double>(right);
//       } else {
//         throw RuntimeError(expr.op, "Operands must be numbers.");
//       }
//     }
//     case token::TokenType::STAR: {
//       if (std::holds_alternative<int>(left) &&
//           std::holds_alternative<int>(right)) {
//         return std::get<int>(left) * std::get<int>(right);
//       } else if (std::holds_alternative<double>(left) &&
//                  std::holds_alternative<double>(right)) {
//         return std::get<double>(left) * std::get<double>(right);
//       } else {
//         throw RuntimeError(expr.op, "Operands must be numbers.");
//       }
//     }
//     case token::TokenType::SLASH: {
//       if (std::holds_alternative<int>(left) &&
//           std::holds_alternative<int>(right)) {
//         if (std::get<int>(right) == 0) {
//           throw RuntimeError(expr.op, "Division by zero.");
//         }
//         return std::get<int>(left) / std::get<int>(right);
//       } else if (std::holds_alternative<double>(left) &&
//                  std::holds_alternative<double>(right)) {
//         if (std::get<double>(right) == 0.0) {
//           throw RuntimeError(expr.op, "Division by zero.");
//         }
//         return std::get<double>(left) / std::get<double>(right);
//       } else {
//         throw RuntimeError(expr.op, "Operands must be numbers.");
//       }
//     }
//     case token::TokenType::GREATER: {
//       if (std::holds_alternative<int>(left) &&
//           std::holds_alternative<int>(right)) {
//         return std::get<int>(left) > std::get<int>(right);
//       } else if (std::holds_alternative<double>(left) &&
//                  std::holds_alternative<double>(right)) {
//         return std::get<double>(left) > std::get<double>(right);
//       } else {
//         throw RuntimeError(expr.op, "Operands must be numbers.");
//       }
//     }
//     case token::TokenType::GREATER_EQUAL: {
//       if (std::holds_alternative<int>(left) &&
//           std::holds_alternative<int>(right)) {
//         return std::get<int>(left) >= std::get<int>(right);
//       } else if (std::holds_alternative<double>(left) &&
//                  std::holds_alternative<double>(right)) {
//         return std::get<double>(left) >= std::get<double>(right);
//       } else {
//         throw RuntimeError(expr.op, "Operands must be numbers.");
//       }
//     }
//     case token::TokenType::LESS: {
//       if (std::holds_alternative<int>(left) &&
//           std::holds_alternative<int>(right)) {
//         return std::get<int>(left) < std::get<int>(right);
//       } else if (std::holds_alternative<double>(left) &&
//                  std::holds_alternative<double>(right)) {
//         return std::get<double>(left) < std::get<double>(right);
//       } else {
//         throw RuntimeError(expr.op, "Operands must be numbers.");
//       }
//     }
//     case token::TokenType::LESS_EQUAL: {
//       if (std::holds_alternative<int>(left) &&
//           std::holds_alternative<int>(right)) {
//         return std::get<int>(left) <= std::get<int>(right);
//       } else if (std::holds_alternative<double>(left) &&
//                  std::holds_alternative<double>(right)) {
//         return std::get<double>(left) <= std::get<double>(right);
//       } else {
//         throw RuntimeError(expr.op, "Operands must be numbers.");
//       }
//     }
//     case token::TokenType::EQUAL_EQUAL: {
//       Literal left = evaluate(*expr.left);
//       Literal right = evaluate(*expr.right);
//       return left == right;
//     }
//     case token::TokenType::BANG_EQUAL: {
//       Literal left = evaluate(*expr.left);
//       Literal right = evaluate(*expr.right);
//       return left != right;
//     }
//     default:
//       throw RuntimeError(expr.op, "Unknown binary operator.");
//     }
//   };
//   Literal visitUnaryExpr(const expr::UnaryExpr &expr) {
//     Literal right = evaluate(*expr.right);
//     switch (expr.op.type()) {
//     case token::TokenType::MINUS: {
//       if (std::holds_alternative<int>(right)) {
//         return -std::get<int>(right);
//       } else if (std::holds_alternative<double>(right)) {
//         return -std::get<double>(right);
//       } else {
//         throw RuntimeError(expr.op, "Operand must be a number.");
//       }
//     }
//     case token::TokenType::BANG: {
//       if (std::holds_alternative<bool>(right)) {
//         return !std::get<bool>(right);
//       } else {
//         throw RuntimeError(expr.op, "Operand must be a boolean.");
//       }
//     }
//     default:
//       throw RuntimeError(expr.op, "Unknown unary operator.");
//     }
//   }
//   Literal visitLiteralExpr(const expr::LiteralExpr &expr) { return expr.value; }
//   Literal visitGroupingExpr(const expr::GroupingExpr &expr) {
//     return evaluate(*expr.expression);
//   }

// private:
//   std::string literalToString(const Literal &value) {
//     if (std::holds_alternative<int>(value)) {
//       return std::to_string(std::get<int>(value));
//     } else if (std::holds_alternative<double>(value)) {
//       return std::to_string(std::get<double>(value));
//     } else if (std::holds_alternative<std::string>(value)) {
//       return std::get<std::string>(value);
//     } else if (std::holds_alternative<bool>(value)) {
//       return std::get<bool>(value) ? "true" : "false";
//     } else if (std::holds_alternative<std::nullptr_t>(value)) {
//       return "nil";
//     } else if (std::holds_alternative<std::monostate>(value)) {
//       return "nil";
//     }
//     return "unknown";
//   }
//   private:
//     enviroment::Enviroment enviroment_;
// };

// } // namespace interpreter
// } // namespace dtoy










#pragma once
#include "enviroment.h"
#include "expr.h"
#include "stmt.h"
#include "token.h"
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream> // 需要添加这个头文件

namespace dtoy {
namespace interpreter {
class RuntimeError : public std::runtime_error {
public:
  token::Token token;
  RuntimeError(token::Token token, const std::string &message)
      : std::runtime_error(message), token(token) {}
};

using namespace dtoy::stmt;

class Interpreter {
public:
  Interpreter() = default;
  
  void interpret(const expr::Expr &expression) {
    try {
      Literal result = evaluate(expression);
      std::cout << "Result: " << literalToString(result) << std::endl;
    } catch (const RuntimeError &error) {
      std::cerr << "Runtime error: " << error.what() << " [line "
                << error.token.line() << "]" << std::endl;
    }
  }

  void interpret(const std::vector<std::unique_ptr<Stmt>> &statements) {
    try {
      for (const auto &statement_ptr : statements) {
        if (!statement_ptr)
          continue;
        execute(statement_ptr);
      }
    } catch (const RuntimeError &error) {
      std::cerr << "Runtime error: " << error.what() << " [line "
                << error.token.line() << "]" << std::endl;
    }
  }

  void execute(const std::unique_ptr<Stmt> &statement) {
    auto visitor = [this](auto &&stmt_node) -> void {
      using T = std::decay_t<decltype(stmt_node)>;
      if constexpr (std::is_same_v<T, PrintStmt>) {
        this->visitPrintStmt(stmt_node);
      } else if constexpr (std::is_same_v<T, ExpressionStmt>) {
        this->visitExpressionStmt(stmt_node);
      } else if constexpr (std::is_same_v<T, VarStmt>) {
        this->visitVarStmt(stmt_node); // 需要实现这个方法
      } else if constexpr (std::is_same_v<T, BlockStmt>) {
        this->visitBlockStmt(stmt_node); // 需要实现这个方法
      }
      // 可以添加其他语句类型的处理
    };
    
    std::visit(visitor, static_cast<const Stmt &>(*statement));
  }

  using Literal = token::Literal;
  
  Literal evaluate(const expr::Expr &expression) {
    auto visitor = [this](const auto &expr_node) -> Literal {
      using T = std::decay_t<decltype(expr_node)>;
      if constexpr (std::is_same_v<T, expr::BinaryExpr>) {
        return this->visitBinaryExpr(expr_node);
      } else if constexpr (std::is_same_v<T, expr::UnaryExpr>) {
        return this->visitUnaryExpr(expr_node);
      } else if constexpr (std::is_same_v<T, expr::LiteralExpr>) {
        return this->visitLiteralExpr(expr_node);
      } else if constexpr (std::is_same_v<T, expr::GroupingExpr>) {
        return this->visitGroupingExpr(expr_node);
      }
      // 可以添加其他表达式类型的处理
    };

    return std::visit(visitor, expression);
  }

  void visitExpressionStmt(const ExpressionStmt &statement) {
    evaluate(*statement.expression);
  }
  
  void visitPrintStmt(const PrintStmt &statement) {
    Literal value = evaluate(*statement.expression);
    std::cout << literalToString(value) << std::endl;
  }
  
  void visitVarStmt(const VarStmt &stmt) {
    // TODO: 实现变量声明
    // 例如：enviroment_.define(stmt.name.lexeme(), evaluate(*stmt.initializer));
    Literal value;
    if (stmt.initializer) {
      value = evaluate(*stmt.initializer);
    } else {
      value = std::monostate{}; // 或者其他默认值
    }
    enviroment_.define(stmt.name.lexeme(), value);

  }
  
  void visitBlockStmt(const BlockStmt &stmt) {
    // 递归执行块内语句
    interpret(stmt.statements);
  }

  Literal visitBinaryExpr(const expr::BinaryExpr &expr) {
    Literal left = evaluate(*expr.left);
    Literal right = evaluate(*expr.right);

    switch (expr.op.type()) {
    case token::TokenType::PLUS: {
      if (std::holds_alternative<int>(left) &&
          std::holds_alternative<int>(right)) {
        return std::get<int>(left) + std::get<int>(right);
      } else if (std::holds_alternative<double>(left) &&
                 std::holds_alternative<double>(right)) {
        return std::get<double>(left) + std::get<double>(right);
      } else if (std::holds_alternative<std::string>(left) &&
                 std::holds_alternative<std::string>(right)) {
        return std::get<std::string>(left) + std::get<std::string>(right);
      } else {
        throw RuntimeError(expr.op,
                           "Operands must be two numbers or two strings.");
      }
    }
    case token::TokenType::MINUS: {
      if (std::holds_alternative<int>(left) &&
          std::holds_alternative<int>(right)) {
        return std::get<int>(left) - std::get<int>(right);
      } else if (std::holds_alternative<double>(left) &&
                 std::holds_alternative<double>(right)) {
        return std::get<double>(left) - std::get<double>(right);
      } else {
        throw RuntimeError(expr.op, "Operands must be numbers.");
      }
    }
    case token::TokenType::STAR: {
      if (std::holds_alternative<int>(left) &&
          std::holds_alternative<int>(right)) {
        return std::get<int>(left) * std::get<int>(right);
      } else if (std::holds_alternative<double>(left) &&
                 std::holds_alternative<double>(right)) {
        return std::get<double>(left) * std::get<double>(right);
      } else {
        throw RuntimeError(expr.op, "Operands must be numbers.");
      }
    }
    case token::TokenType::SLASH: {
      if (std::holds_alternative<int>(left) &&
          std::holds_alternative<int>(right)) {
        if (std::get<int>(right) == 0) {
          throw RuntimeError(expr.op, "Division by zero.");
        }
        return std::get<int>(left) / std::get<int>(right);
      } else if (std::holds_alternative<double>(left) &&
                 std::holds_alternative<double>(right)) {
        if (std::get<double>(right) == 0.0) {
          throw RuntimeError(expr.op, "Division by zero.");
        }
        return std::get<double>(left) / std::get<double>(right);
      } else {
        throw RuntimeError(expr.op, "Operands must be numbers.");
      }
    }
    case token::TokenType::GREATER: {
      if (std::holds_alternative<int>(left) &&
          std::holds_alternative<int>(right)) {
        return std::get<int>(left) > std::get<int>(right);
      } else if (std::holds_alternative<double>(left) &&
                 std::holds_alternative<double>(right)) {
        return std::get<double>(left) > std::get<double>(right);
      } else {
        throw RuntimeError(expr.op, "Operands must be numbers.");
      }
    }
    case token::TokenType::GREATER_EQUAL: {
      if (std::holds_alternative<int>(left) &&
          std::holds_alternative<int>(right)) {
        return std::get<int>(left) >= std::get<int>(right);
      } else if (std::holds_alternative<double>(left) &&
                 std::holds_alternative<double>(right)) {
        return std::get<double>(left) >= std::get<double>(right);
      } else {
        throw RuntimeError(expr.op, "Operands must be numbers.");
      }
    }
    case token::TokenType::LESS: {
      if (std::holds_alternative<int>(left) &&
          std::holds_alternative<int>(right)) {
        return std::get<int>(left) < std::get<int>(right);
      } else if (std::holds_alternative<double>(left) &&
                 std::holds_alternative<double>(right)) {
        return std::get<double>(left) < std::get<double>(right);
      } else {
        throw RuntimeError(expr.op, "Operands must be numbers.");
      }
    }
    case token::TokenType::LESS_EQUAL: {
      if (std::holds_alternative<int>(left) &&
          std::holds_alternative<int>(right)) {
        return std::get<int>(left) <= std::get<int>(right);
      } else if (std::holds_alternative<double>(left) &&
                 std::holds_alternative<double>(right)) {
        return std::get<double>(left) <= std::get<double>(right);
      } else {
        throw RuntimeError(expr.op, "Operands must be numbers.");
      }
    }
    case token::TokenType::EQUAL_EQUAL: {
      return left == right;
    }
    case token::TokenType::BANG_EQUAL: {
      return left != right;
    }
    default:
      throw RuntimeError(expr.op, "Unknown binary operator.");
    }
  }
  
  Literal visitUnaryExpr(const expr::UnaryExpr &expr) {
    Literal right = evaluate(*expr.right);
    switch (expr.op.type()) {
    case token::TokenType::MINUS: {
      if (std::holds_alternative<int>(right)) {
        return -std::get<int>(right);
      } else if (std::holds_alternative<double>(right)) {
        return -std::get<double>(right);
      } else {
        throw RuntimeError(expr.op, "Operand must be a number.");
      }
    }
    case token::TokenType::BANG: {
      if (std::holds_alternative<bool>(right)) {
        return !std::get<bool>(right);
      } else {
        throw RuntimeError(expr.op, "Operand must be a boolean.");
      }
    }
    default:
      throw RuntimeError(expr.op, "Unknown unary operator.");
    }
  }
  
  Literal visitVariableExpr(const expr::VariableExpr &expr) {
    return enviroment_.get(expr.name.lexeme());
  }

  Literal visitLiteralExpr(const expr::LiteralExpr &expr) { 
    return expr.value; 
  }
  
  Literal visitGroupingExpr(const expr::GroupingExpr &expr) {
    return evaluate(*expr.expression);
  }

  Literal visitAssignExpr(const expr::AssignExpr &expr) {
    Literal value = evaluate(*expr.value);
    enviroment_.assign(expr.name.lexeme(), value);
    return value;
  }
private:
  std::string literalToString(const Literal &value) {
    if (std::holds_alternative<int>(value)) {
      return std::to_string(std::get<int>(value));
    } else if (std::holds_alternative<double>(value)) {
      // 可以优化double的输出格式
      std::string result = std::to_string(std::get<double>(value));
      // 移除多余的0
      result.erase(result.find_last_not_of('0') + 1, std::string::npos);
      if (result.back() == '.') {
        result.pop_back();
      }
      return result;
    } else if (std::holds_alternative<std::string>(value)) {
      return std::get<std::string>(value);
    } else if (std::holds_alternative<bool>(value)) {
      return std::get<bool>(value) ? "true" : "false";
    } else if (std::holds_alternative<std::nullptr_t>(value)) {
      return "nil";
    } else if (std::holds_alternative<std::monostate>(value)) {
      return "nil";
    }
    return "unknown";
  }

private:
  enviroment::Enviroment enviroment_;
};

} // namespace interpreter
} // namespace dtoy