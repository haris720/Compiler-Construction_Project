#include "Parser.h"
#include <cstdlib>

Parser::Parser(const std::vector<Token>& toks) : tokens(toks), pos(0) {}

const Token& Parser::peek() const { 
    if (pos >= tokens.size()) throw ParseException(ParseErrorKind::UnexpectedEOF,"Unexpected EOF");
    return tokens[pos]; 
}
const Token& Parser::previous() const { 
    if (pos==0) throw ParseException(ParseErrorKind::UnexpectedEOF,"No previous token");
    return tokens[pos-1]; 
}
bool Parser::isAtEnd() const { return pos >= tokens.size(); }
bool Parser::check(TokenType t) const { return !isAtEnd() && tokens[pos].type == t; }
const Token& Parser::advance() { 
    if (!isAtEnd()) ++pos; 
    return tokens[pos-1];
}
bool Parser::match(std::initializer_list<TokenType> types){
    for (auto t: types){
        if (check(t)){ advance(); return true; }
    }
    return false;
}
const Token& Parser::consume(TokenType t, const char* msg){
    if (check(t)) return advance();
    throw ParseException(ParseErrorKind::UnexpectedToken, std::string(msg) + " Found: " + tokens[pos].value, tokens[pos]);
}

// program := (fnDecl | varDecl)* EOF
std::shared_ptr<Program> Parser::parseProgram(){
    auto prog = std::make_shared<Program>();
    while (!isAtEnd()){
        // tolerate stray ERROR tokens from lexer
        if (check(TokenType::ERROR)) throw ParseException(ParseErrorKind::UnexpectedToken, "Lexer error token encountered", peek());
        prog->items.push_back(declaration());
    }
    return prog;
}

StmtPtr Parser::declaration(){
    if (match({TokenType::FUNCTION})) {
        return fnDeclaration();
    }
    // allow top-level variable declarations: type ident = expr ;
    if (!isAtEnd() && isTypeToken(peek().type)) {
        Token typeTok = advance();
        return varDeclaration(typeTok.type);
    }
    // Otherwise try an expression statement to avoid deadlock
    return exprStatement();
}

// fnDecl := "fn" [type]? IDENT "(" paramList? ")" "{" block "}"
StmtPtr Parser::fnDeclaration(){
    // optional return type
    TokenType returnType = TokenType::ERROR; // "unspecified"
    if (isTypeToken(peek().type)){
        returnType = advance().type;
    }
    const Token& nameTok = consume(TokenType::IDENTIFIER, "Expected function name after 'fn' (or return type).");

    consume(TokenType::PARENL, "Expected '(' after function name.");
    std::vector<Param> params;
    if (!check(TokenType::PARENR)){
        params = parameterList();
    }
    consume(TokenType::PARENR, "Expected ')' after parameter list.");
    consume(TokenType::BRACEL, "Expected '{' to start function body.");
    auto bodyBlock = block();
    consume(TokenType::BRACER, "Expected '}' to close function body.");

    return std::make_shared<FnDeclStmt>(returnType, nameTok.value, std::move(params), bodyBlock);
}

// paramList := type IDENT ("," type IDENT)*
std::vector<Param> Parser::parameterList(){
    std::vector<Param> ps;
    
    // Handle empty parameter list
    if (!isTypeToken(peek().type)) {
        return ps;
    }
    
    // Parse parameters
    while (true){
        if (!isTypeToken(peek().type))
            throw ParseException(ParseErrorKind::ExpectedTypeToken, "Expected parameter type.", peek());
        TokenType pt = advance().type;
        const Token& pn = consume(TokenType::IDENTIFIER, "Expected parameter name.");
        ps.push_back(Param{pt, pn.value});
        if (!match({TokenType::COMMA})) break;
    }
    return ps;
}

// block := { statement* }   (caller has already consumed '{')
std::shared_ptr<BlockStmt> Parser::block(){
    auto blk = std::make_shared<BlockStmt>();
    while (!isAtEnd() && !check(TokenType::BRACER)){
        blk->statements.push_back(statement());
    }
    return blk;
}

StmtPtr Parser::statement(){
    if (isTypeToken(peek().type)) {
        Token t = advance();
        return varDeclaration(t.type);
    }
    if (match({TokenType::RETURN})) return returnStatement();
    return exprStatement();
}

// varDecl := type IDENT "=" expression ";"
StmtPtr Parser::varDeclaration(TokenType typeTok){
    const Token& nameTok = consume(TokenType::IDENTIFIER, "Expected variable name.");
    consume(TokenType::ASSIGNOP, "Expected '=' in variable declaration.");
    ExprPtr initExpr = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
    return std::make_shared<VarDeclStmt>(typeTok, nameTok.value, initExpr);
}

// returnStmt := "return" expression ";"
StmtPtr Parser::returnStatement(){
    ExprPtr e = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after return value.");
    return std::make_shared<ReturnStmt>(e);
}

// exprStmt := expression ";"
StmtPtr Parser::exprStatement(){
    ExprPtr e = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression.");
    return std::make_shared<ExprStmt>(e);
}

// ---------- Expressions ----------
ExprPtr Parser::expression(){ return equality(); }

// equality := addition ( "==" addition )*
ExprPtr Parser::equality(){
    ExprPtr expr = addition();
    while (match({TokenType::EQUALSOP})){
        TokenType op = previous().type;
        ExprPtr right = addition();
        expr = std::make_shared<BinaryExpr>(op, expr, right);
    }
    return expr;
}

// addition := multiplication ( ("+"|"-") multiplication )*
ExprPtr Parser::addition(){
    ExprPtr expr = multiplication();
    while (match({TokenType::ADDOP, TokenType::SUBOP})){
        TokenType op = previous().type;
        ExprPtr right = multiplication();
        expr = std::make_shared<BinaryExpr>(op, expr, right);
    }
    return expr;
}

// multiplication := unary ( ("*"|"/") unary )*
ExprPtr Parser::multiplication(){
    ExprPtr expr = unary();
    while (match({TokenType::MULOP, TokenType::DIVOP})){
        TokenType op = previous().type;
        ExprPtr right = unary();
        expr = std::make_shared<BinaryExpr>(op, expr, right);
    }
    return expr;
}

// unary := ("-" ) unary | call
ExprPtr Parser::unary(){
    if (match({TokenType::SUBOP})){
        TokenType op = previous().type;
        return std::make_shared<UnaryExpr>(op, unary());
    }
    return call();
}

// call := primary ("(" argList? ")")*
ExprPtr Parser::call(){
    ExprPtr expr = primary();
    while (true){
        if (match({TokenType::PARENL})){
            expr = finishCall(expr);
        } else {
            break;
        }
    }
    return expr;
}

ExprPtr Parser::finishCall(ExprPtr callee){
    // Only identifiers can be called
    auto* id = dynamic_cast<IdentExpr*>(callee.get());
    if (!id)
        throw ParseException(ParseErrorKind::UnexpectedToken, "Can only call identifiers (e.g., foo(...)).", peek());

    std::vector<ExprPtr> args;
    if (!check(TokenType::PARENR)){
        do {
            args.push_back(expression());
        } while (match({TokenType::COMMA}));
    }
    consume(TokenType::PARENR, "Expected ')' after arguments.");
    return std::make_shared<CallExpr>(id->name, std::move(args));
}

// primary := INTLIT | FLOATLIT | STRINGLIT | IDENT | "(" expression ")"
ExprPtr Parser::primary(){
    if (match({TokenType::INTLIT})){
        long long v = std::stoll(previous().value);
        return std::make_shared<IntLitExpr>(v);
    }
    if (match({TokenType::FLOATLIT})){
        double v = std::stod(previous().value);
        return std::make_shared<FloatLitExpr>(v);
    }
    if (match({TokenType::STRINGLIT})){
        return std::make_shared<StringLitExpr>(previous().value);
    }
    if (match({TokenType::IDENTIFIER})){
        return std::make_shared<IdentExpr>(previous().value);
    }
    if (match({TokenType::PARENL})){
        ExprPtr e = expression();
        consume(TokenType::PARENR, "Expected ')' after expression.");
        return e;
    }
    throw ParseException(ParseErrorKind::ExpectedExpr, "Expected expression.", peek());
}
