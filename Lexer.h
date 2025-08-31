//Lexer.h:

#include "Token.h"
#include <regex>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

class Lexer {
private:
    vector<pair<string, string>> tokenSpecs;
    unordered_map<string, string> keywords;

public:
    Lexer() {
        // Regex patterns for tokens (priority matters: longer matches first!)
        tokenSpecs = {
            {"T_WHITESPACE", "(\\s+)"},
            {"T_COMMENT", "(//[^\\n]|/\\[\\s\\S]?\\/)"},
            {"T_STRINGLIT", "\"(?:\\\\.|[^\"\\\\])*\""},
            {"T_CHARLIT", "'(?:\\\\.|[^'\\\\])'"},
            {"T_FLOATLIT", "\\b\\d+\\.\\d+\\b"},
            {"T_INTLIT", "\\b\\d+\\b"},
            {"T_IDENTIFIER", "\\b[a-zA-Z_][a-zA-Z0-9_]*\\b"},
            {"T_OPERATOR", "(==|!=|<=|>=|&&|\\|\\||\\+\\+|--|<<|>>|&|\\||\\^|~|\\+|\\-|\\*|/|%|=|<|>)"},
            {"T_PAREN", "[\\(\\)]"},
            {"T_BRACE", "[\\{\\}]"},
            {"T_BRACKET", "[\\[\\]]"},
            {"T_COMMA", ","},
            {"T_SEMICOLON", ";"}
        };

        // Reserved keywords
        keywords = {
            {"fn", "T_FUNCTION"}, {"int", "T_INT"}, {"float", "T_FLOAT"},
            {"string", "T_STRING"}, {"char", "T_CHAR"}, {"bool", "T_BOOL"},
            {"return", "T_RETURN"}, {"if", "T_IF"}, {"else", "T_ELSE"},
            {"while", "T_WHILE"}, {"for", "T_FOR"}, {"break", "T_BREAK"},
            {"continue", "T_CONTINUE"}, {"true", "T_TRUE"}, {"false", "T_FALSE"},
            {"void", "T_VOID"}
        };
    }

