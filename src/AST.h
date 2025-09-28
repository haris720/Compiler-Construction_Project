#pragma once
#include <memory>
#include <string>
#include <vector>
#include "lexer.h"

// ---------- AST Base ----------
struct Expr;
struct Stmt;

using ExprPtr = std::shared_ptr<Expr>;
using StmtPtr = std::shared_ptr<Stmt>;

enum class NodeKind {
    // Expr
    Binary, Unary, Identifier, IntLit, FloatLit, StringLit, Call,
    // Stmt
    VarDecl, ReturnStmt, ExprStmt, Block, FnDecl, Program
};

struct Expr {
    NodeKind kind;
    explicit Expr(NodeKind k) : kind(k) {}
    virtual ~Expr() = default;
};

struct Stmt {
    NodeKind kind;
    explicit Stmt(NodeKind k) : kind(k) {}
    virtual ~Stmt() = default;
};

// ---------- Expr Nodes ----------
struct BinaryExpr : Expr {
    TokenType op;
    ExprPtr left, right;
    BinaryExpr(TokenType op, ExprPtr l, ExprPtr r)
        : Expr(NodeKind::Binary), op(op), left(std::move(l)), right(std::move(r)) {}
};

struct UnaryExpr : Expr {
    TokenType op;
    ExprPtr expr;
    UnaryExpr(TokenType op, ExprPtr e)
        : Expr(NodeKind::Unary), op(op), expr(std::move(e)) {}
};

struct IdentExpr : Expr {
    std::string name;
    explicit IdentExpr(std::string n)
        : Expr(NodeKind::Identifier), name(std::move(n)) {}
};

struct IntLitExpr : Expr {
    long long value;
    explicit IntLitExpr(long long v)
        : Expr(NodeKind::IntLit), value(v) {}
};

struct FloatLitExpr : Expr {
    double value;
    explicit FloatLitExpr(double v)
        : Expr(NodeKind::FloatLit), value(v) {}
};

struct StringLitExpr : Expr {
    std::string value;
    explicit StringLitExpr(std::string v)
        : Expr(NodeKind::StringLit), value(std::move(v)) {}
};

struct CallExpr : Expr {
    std::string callee;               // expects identifier callee
    std::vector<ExprPtr> args;
    CallExpr(std::string c, std::vector<ExprPtr> a)
        : Expr(NodeKind::Call), callee(std::move(c)), args(std::move(a)) {}
};

// ---------- Stmt Nodes ----------
struct VarDeclStmt : Stmt {
    TokenType typeTok;      // INT/FLOAT/STRING
    std::string name;
    ExprPtr init;
    VarDeclStmt(TokenType t, std::string n, ExprPtr e)
        : Stmt(NodeKind::VarDecl), typeTok(t), name(std::move(n)), init(std::move(e)) {}
};

struct ReturnStmt : Stmt {
    ExprPtr expr;
    explicit ReturnStmt(ExprPtr e)
        : Stmt(NodeKind::ReturnStmt), expr(std::move(e)) {}
};

struct ExprStmt : Stmt {
    ExprPtr expr;
    explicit ExprStmt(ExprPtr e)
        : Stmt(NodeKind::ExprStmt), expr(std::move(e)) {}
};

struct BlockStmt : Stmt {
    std::vector<StmtPtr> statements;
    BlockStmt() : Stmt(NodeKind::Block) {}
};

struct Param {
    TokenType typeTok;      // INT/FLOAT/STRING
    std::string name;
};

struct FnDeclStmt : Stmt {
    // returnType optional: if none, set to TokenType::ERROR as "void/unspecified"
    TokenType returnType;   // INT/FLOAT/STRING or ERROR for "no explicit type"
    std::string name;
    std::vector<Param> params;
    std::shared_ptr<BlockStmt> body;
    FnDeclStmt(TokenType rt, std::string n, std::vector<Param> p, std::shared_ptr<BlockStmt> b)
        : Stmt(NodeKind::FnDecl), returnType(rt), name(std::move(n)),
          params(std::move(p)), body(std::move(b)) {}
};

struct Program : Stmt {
    std::vector<StmtPtr> items; // FnDecl | VarDecl (top-level)
    Program() : Stmt(NodeKind::Program) {}
};

// ---------- AST Pretty Printer ----------
void printAST(const StmtPtr& node, int indent = 0);
void printAST(const std::vector<StmtPtr>& nodes, int indent = 0);
