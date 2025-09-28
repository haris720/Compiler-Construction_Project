#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "lexer.h"
#include "Parser.h"
#include "AST.h"

// Pretty name for TokenType (for readable token dumps)
const char* tokenTypeName(TokenType t) {
    switch (t) {
        case TokenType::FUNCTION:  return "FUNCTION";
        case TokenType::INT:       return "INT";
        case TokenType::FLOAT:     return "FLOAT";
        case TokenType::STRING:    return "STRING";
        case TokenType::RETURN:    return "RETURN";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::ASSIGNOP:  return "ASSIGNOP";
        case TokenType::EQUALSOP:  return "EQUALSOP";
        case TokenType::ADDOP:     return "ADDOP";
        case TokenType::SUBOP:     return "SUBOP";
        case TokenType::MULOP:     return "MULOP";
        case TokenType::DIVOP:     return "DIVOP";
        case TokenType::COMMA:     return "COMMA";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::PARENL:    return "PARENL";
        case TokenType::PARENR:    return "PARENR";
        case TokenType::BRACEL:    return "BRACEL";
        case TokenType::BRACER:    return "BRACER";
        case TokenType::STRINGLIT: return "STRINGLIT";
        case TokenType::INTLIT:    return "INTLIT";
        case TokenType::FLOATLIT:  return "FLOATLIT";
        case TokenType::ERROR:     return "ERROR";
        default:                   return "UNKNOWN";
    }
}

// Read whole file into a string
std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char** argv) {
    try {
        // 1) Load source (file path arg optional)
        std::string sourceCode;
        if (argc > 1) {
            sourceCode = readFile(argv[1]);
        } else {
            // Sample program
            sourceCode = R"(
                fn my_fn(int x, float y) {
                    string my_str = "hello";
                    int a = 10;
                    return a;
                }
            )";
        }

        std::cout << "=== SOURCE CODE ===\n";
        std::cout << sourceCode << "\n";

        // 2) Lex
        Lexer lexer(sourceCode);
        std::vector<Token> tokens = lexer.tokenize();

        // 3) Print tokens (optional but handy)
        std::cout << "=== TOKENS ===\n";
        for (const auto& t : tokens) {
            std::cout << tokenTypeName(t.type) << " \"" << t.value << "\"\n";
        }

        // 4) Parse
        Parser parser(tokens);
        auto program = parser.parseProgram();

        // 5) Print AST
        std::cout << "\n=== AST ===\n";
        printAST(program);

        std::cout << "\n=== SUCCESS ===\n";
        std::cout << "Parsing completed successfully!\n";

        return 0;

    } catch (const ParseException& ex) {
        std::cerr << "Parse error: " << ex.what() << "\n";
        if (ex.token) {
            std::cerr << "At token: (" << tokenTypeName(ex.token->type)
                      << ", \"" << ex.token->value << "\")\n";
        }
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}
