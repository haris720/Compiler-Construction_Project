import re

class TokenType:
    T_FUNCTION = "T_FUNCTION"
    T_INT = "T_INT"
    T_FLOAT = "T_FLOAT"
    T_BOOL = "T_BOOL"
    T_STRING = "T_STRING"
    T_IDENTIFIER = "T_IDENTIFIER"
    T_ASSIGNOP = "T_ASSIGNOP"
    T_EQUALSOP = "T_EQUALSOP"
    T_COMMA = "T_COMMA"
    T_SEMICOLON = "T_SEMICOLON"
    T_PARENL = "T_PARENL"
    T_PARENR = "T_PARENR"
    T_BRACEL = "T_BRACEL"
    T_BRACER = "T_BRACER"
    T_QUOTES = "T_QUOTES"
    T_STRINGLIT = "T_STRINGLIT"
    T_INTLIT = "T_INTLIT"
    T_BOOLLIT = "T_BOOLLIT"
    T_ERROR = "T_ERROR"


class LexerWithRegex:
    def __init__(self, source_code):
        self.source_code = source_code
        self.tokens = []
        self.token_patterns = {
            'T_FUNCTION': r'\bfn\b',
            'T_INT': r'\bint\b',
            'T_FLOAT': r'\bfloat\b',
            'T_BOOL': r'\bbool\b',
            'T_STRING': r'\bstring\b',
            'T_IDENTIFIER': r'[a-zA-Z_][a-zA-Z0-9_]*',
            'T_ASSIGNOP': r'=',
            'T_EQUALSOP': r'==',
            'T_COMMA': r',',
            'T_SEMICOLON': r';',
            'T_PARENL': r'\(',
            'T_PARENR': r'\)',
            'T_BRACEL': r'\{',
            'T_BRACER': r'\}',
            'T_QUOTES': r'"',
            'T_STRINGLIT': r'"[^"\\]*(\\.[^"\\]*)*"',  # For string literals with escaped characters
            'T_INTLIT': r'\b\d+\b',  # Integer literal
            'T_BOOLLIT': r'\b(true|false)\b',  # Boolean literals
        }

    def tokenize(self):
        pos = 0
        while pos < len(self.source_code):
            match = None
            for token_type, pattern in self.token_patterns.items():
                regex = re.compile(pattern)
                match = regex.match(self.source_code, pos)
                if match:
                    value = match.group(0)

                    # Skip whitespace characters (spaces, tabs, newlines)
                    if token_type in ['T_ERROR']:
                        pos = match.end()
                        continue
                    
                    # Handle string literals
                    if token_type == 'T_STRINGLIT':
                        value = value[1:-1]  # Remove surrounding quotes from string literal
                    elif token_type == 'T_INTLIT':
                        value = int(value)
                    elif token_type == 'T_BOOLLIT':
                        value = value.lower() == 'true'

                    self.tokens.append((token_type, value))
                    pos = match.end()
                    break

            # Skip over invalid characters that are not recognized by the regex patterns
            if not match:
                pos += 1

        return self.tokens


# Test the lexer with the example code
source_code = """
fn int my_fn(int x, float y) {
    string my_str = "hmm";
    bool my_bool = x == 40;
    return x;
}
"""

lexer = LexerWithRegex(source_code)
tokens = lexer.tokenize()
for token in tokens:
    print(token)
