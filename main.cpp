#include "lexer.h"
#include <iostream>

int main() {
    // Sample source code
    std::string sourceCode = R"(
        fn my_fn(int x, float y) {
            string my_str = "hello";
            int a = 10;
            return a;
        }
    )";

    // Create lexer object
    Lexer lexer(sourceCode);

    // Tokenize the source code
    std::vector<Token> tokens = lexer.tokenize();

    // Output the tokenized result
    for (const auto& token : tokens) {
        std::cout << "Token Type: " << static_cast<int>(token.type) << ", Value: " << token.value << std::endl;
    }

    return 0;
}