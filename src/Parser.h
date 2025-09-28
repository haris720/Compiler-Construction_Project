#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include <optional>
#include "lexer.h"
#include "AST.h"

enum class ParseErrorKind {
    UnexpectedEOF,
    FailedToFindToken,
    ExpectedTypeToken,
    ExpectedIdentifier,
    UnexpectedToken,
    ExpectedFloatLit,
    ExpectedIntLit,
    ExpectedStringLit,
    ExpectedExpr
};

struct ParseException : std::runtime_error {
    ParseErrorKind kind;
    std::optional<Token> token;
    ParseException(ParseErrorKind k, const std::string& msg, std::optional<Token> t = std::nullopt)
        : std::runtime_error(msg), kind(k), token(std::move(t)) {}
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& toks);
    std::shared_ptr<Program> parseProgram();

private:
    const std::vector<Token>& tokens;
    size_t pos;

    // utilities
    const Token& peek() const;
    const Token& previous() const;
    bool isAtEnd() const;
    bool check(TokenType t) const;
    const Token& advance();
    bool match(std::initializer_list<TokenType> types);
    const Token& consume(TokenType t, const char* msg);

    // top-level
    StmtPtr declaration();

    // statements
    StmtPtr fnDeclaration();
    std::vector<Param> parameterList(); // in fn sig
    std::shared_ptr<BlockStmt> block();
    StmtPtr varDeclaration(TokenType typeTok);
    StmtPtr statement();
    StmtPtr returnStatement();
    StmtPtr exprStatement();

    // expressions (Pratt / precedence climbing)
    ExprPtr expression();            // -> equality
    ExprPtr equality();              // == 
    ExprPtr addition();              // + -
    ExprPtr multiplication();        // * /
    ExprPtr unary();                 // (-)x
    ExprPtr call();                  // primary ( ... )*
    ExprPtr finishCall(ExprPtr callee);
    ExprPtr primary();               // identifiers, literals, ( )

    // helpers
    bool isTypeToken(TokenType t) const {
        return t == TokenType::INT || t == TokenType::FLOAT || t == TokenType::STRING;
    }
};
