#include "lexer.h"
#include <iostream>
using namespace std;

void runTest(const string& title, const string& code) {
    cout << "\n--- " << title << " ---\n";
    Lexer lexer(code);
    while (true) {
        Token t = lexer.nextToken();
        if (t.type == TokenType::T_EOF) break;
        cout << t.toString();
    }
    cout << endl;
}

int main() {
    

    // Comments Test
    string commentsCode =
        "// This is a single-line comment\n"
        "int x = 10; /* This is a\n"
        "multi-line comment */ x = x + 1;\n";

    // Run tests
    
    runTest("Comments Test", commentsCode);


    

    return 0;
}
