class LexerWithoutRegex:
    def __init__(self, source_code):
        self.source_code = source_code
        self.tokens = []
        self.current_pos = 0

    def advance(self):
        """ Move to the next character in the input string """
        self.current_pos += 1
        if self.current_pos < len(self.source_code):
            return self.source_code[self.current_pos]
        return None  # Gracefully return None at the end

    def current_char(self):
        """ Return the current character in the input string """
        if self.current_pos < len(self.source_code):
            return self.source_code[self.current_pos]
        return None  # Gracefully return None at the end

    def skip_whitespace(self):
        """ Skip whitespace characters """
        while self.current_char() is not None and self.current_char().isspace():
            self.advance()

    def lex_identifier(self):
        """ Lex identifiers (variable names, function names) """
        start_pos = self.current_pos
        while self.current_char() is not None and (self.current_char().isalnum() or self.current_char() == '_'):
            self.advance()
        ident = self.source_code[start_pos:self.current_pos]
        
        # Check if the identifier starts with a digit
        if ident[0].isdigit():
            self.tokens.append(('T_ERROR', ident))
            return None
        return ident

    def lex_number(self):
        """ Lex integers """
        start_pos = self.current_pos
        while self.current_char() is not None and self.current_char().isdigit():
            self.advance()
        return int(self.source_code[start_pos:self.current_pos])

    def lex_string(self):
        """ Lex string literals """
        self.advance()  # Skip the opening quote
        start_pos = self.current_pos
        while self.current_char() is not None and self.current_char() != '"':
            self.advance()
        value = self.source_code[start_pos:self.current_pos]
        self.advance()  # Skip the closing quote
        return value

    def tokenize(self):
        while self.current_pos < len(self.source_code):
            self.skip_whitespace()
            char = self.current_char()
            
            if char is None:  # Stop if the end of the string is reached
                break

            if char == 'f' and self.source_code[self.current_pos:self.current_pos + 2] == 'fn':
                self.tokens.append(('T_FUNCTION', 'fn'))
                self.advance()
                self.advance()

            elif char == 'i' and self.source_code[self.current_pos:self.current_pos + 3] == 'int':
                self.tokens.append(('T_INT', 'int'))
                self.advance()
                self.advance()
                self.advance()

            elif char == 'f' and self.source_code[self.current_pos:self.current_pos + 5] == 'float':
                self.tokens.append(('T_FLOAT', 'float'))
                self.advance()
                self.advance()
                self.advance()
                self.advance()

            elif char == 'b' and self.source_code[self.current_pos:self.current_pos + 4] == 'bool':
                self.tokens.append(('T_BOOL', 'bool'))
                self.advance()
                self.advance()
                self.advance()
                self.advance()

            elif char == '"':
                value = self.lex_string()
                self.tokens.append(('T_STRINGLIT', value))

            elif char.isalpha():
                ident = self.lex_identifier()
                if ident:  # Only append if valid identifier
                    self.tokens.append(('T_IDENTIFIER', ident))

            elif char.isdigit():
                num = self.lex_number()
                self.tokens.append(('T_INTLIT', num))

            elif char == '=':
                self.tokens.append(('T_ASSIGNOP', '='))
                self.advance()

            elif char == ';':
                self.tokens.append(('T_SEMICOLON', ';'))
                self.advance()

            elif char == ',':
                self.tokens.append(('T_COMMA', ','))
                self.advance()

            elif char == '(':
                self.tokens.append(('T_PARENL', '('))
                self.advance()

            elif char == ')':
                self.tokens.append(('T_PARENR', ')'))
                self.advance()

            elif char == '{':
                self.tokens.append(('T_BRACEL', '{'))
                self.advance()

            elif char == '}':
                self.tokens.append(('T_BRACER', '}'))
                self.advance()

            else:
                self.tokens.append(('T_ERROR', char))
                self.advance()

        return self.tokens

# Test lexer without regex
source_code = """
fn int my_fn(int x, float y) {
    string my_str = "hmm";
    bool my_bool = x == 40;
    return x;
}
"""
lexer_no_regex = LexerWithoutRegex(source_code)
tokens_no_regex = lexer_no_regex.tokenize()
for token in tokens_no_regex:
    print(token)
