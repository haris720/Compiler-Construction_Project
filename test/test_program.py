import sys
import os

# Add the root directory of the project to sys.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

# Sample source code to test the lexer and parser
source_code = """
fn int my_fn(int x, float y) {
    string my_str = "hmm";
    bool my_bool = x == 40;
    return x;
}
"""

# Tokenize the source code using the lexer
from lexer.lexer_with_regex import LexerWithRegex

lexer = LexerWithRegex(source_code)
tokens = lexer.tokenize()  # Ensure that tokens are generated before passing to parser

# Now, parse the token stream using the parser
# from parser.parser import Parser

# parser = Parser(tokens)
# ast = parser.parse()

# Print the resulting AST
print(ast)
