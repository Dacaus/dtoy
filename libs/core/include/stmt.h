#pragma once
#include "expr.h"
#include "token.h"
#include <vector>
namespace dtoy {
namespace stmt {
class Stmt;
class ExpressionStmt {
public:
    std::unique_ptr<expr::Expr> expression;
    ExpressionStmt(std::unique_ptr<expr::Expr> expr) : expression(std::move(expr)) {}
};
class PrintStmt {
public:
    std::unique_ptr<expr::Expr> expression;
    PrintStmt(std::unique_ptr<expr::Expr> expr) : expression(std::move(expr)) {}
};
class VarStmt {
public:
    token::Token name;
    std::unique_ptr<expr::Expr> initializer;
    VarStmt(token::Token name, std::unique_ptr<expr::Expr> init)
        : name(name), initializer(std::move(init)) {}
};

class BlockStmt {
public:
    std::vector<std::unique_ptr<Stmt>> statements;
    BlockStmt(std::vector<std::unique_ptr<Stmt>> stmts)
        : statements(std::move(stmts)) {}
};



using StmtBase = std::variant<ExpressionStmt, PrintStmt, VarStmt, BlockStmt>;
class Stmt : public StmtBase {
public:
    using StmtBase::StmtBase;
    using StmtBase::operator=;
};

}
}