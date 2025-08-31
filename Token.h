//Token.h


#pragma once
#include <string>
using namespace std;

struct Token {
    string type;
    string value;

    Token(string t, string v = "") : type(std::move(t)), value(std::move(v)) {}
};
