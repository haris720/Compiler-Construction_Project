class ParseError(Exception):
    pass

class ASTNode:
    def __init__(self, node_type, value=None):
        self.node_type = node_type
        self.value = value

    def __repr__(self):
        return f"{self.node_type}({self.value!r})"

class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.current_token_index = 0
        self.current_token = None
        self.advance()  # Initialize the first token

    def advance(self):
        """ Move to the next token in the token list """
        if self.current_token_index < len(self.tokens):
            self.current_token = self.tokens[self.current_token_index]
            self.current_token_index += 1
        else:
            self.current_token = None  # End of token stream

    def parse(self):
        """ Start parsing and return the AST """
        ast = []
        while self.current_token is not None:
            ast.append(self.parse_statement())
        return ast

    def parse_statement(self):
        """ Parse statements (variable declarations, function declarations, etc.) """
        if self.current_token[0] == 'T_FUNCTION':
            return self.parse_function_declaration()
        elif self.current_token[0] in ['T_INT', 'T_FLOAT', 'T_BOOL', 'T_STRING']:
            return self.parse_variable_declaration()
        elif self.current_token[0] == 'T_RETURN':
            return self.parse_return_statement()
        else:
            raise ParseError(f"Unexpected token: {self.current_token}")
    
    def parse_function_declaration(self):
        """ Parse a function declaration (e.g., fn int my_fn(int x, float y)) """
        self.advance()  # Skip 'fn'
        
        return_type = self.parse_type()
        func_name = self.parse_identifier()
        
        self.expect('T_PARENL')  # Expect '('
        params = self.parse_parameter_list()
        self.expect('T_PARENR')  # Expect ')'
        
        self.expect('T_BRACEL')  # Expect '{'
        body = self.parse_block()
        self.expect('T_BRACER')  # Expect '}'
        
        # Create a function declaration AST node
        fn_decl_node = ASTNode('FnDecl', {'type': return_type, 'name': func_name, 'params': params, 'body': body})
        return fn_decl_node
    
    def parse_variable_declaration(self):
        """ Parse a variable declaration (e.g., int x = 5 or string my_str = "hmm") """
        var_type = self.parse_type()
        var_name = self.parse_identifier()
        
        self.expect('T_ASSIGNOP')  # Expect '='
        
        # Parse the assignment expression, which could be a string literal, int literal, etc.
        expr = self.parse_expression()
        
        self.expect('T_SEMICOLON')  # Expect ';' at the end of the statement
        
        # Create a variable declaration AST node
        var_decl_node = ASTNode('VarDecl', {'type': var_type, 'name': var_name, 'expr': expr})
        return var_decl_node

    
    def parse_expression(self):
        """ Parse an expression (e.g., x + 2 or y == 10) """
        if self.current_token[0] == 'T_IDENTIFIER':
            left_operand = self.parse_identifier()
        elif self.current_token[0] == 'T_INTLIT':
            left_operand = self.parse_int_literal()
        elif self.current_token[0] == 'T_QUOTES':  # Handle string literals
            left_operand = self.parse_string_literal()
        else:
            raise ParseError(f"Unexpected token in expression: {self.current_token}")

        if self.current_token[0] in ['T_PLUS', 'T_MINUS', 'T_MUL', 'T_DIV']:
            op = self.current_token
            self.advance()
            right_operand = self.parse_expression()
            expr_node = ASTNode('BinaryOp', {'left': left_operand, 'op': op, 'right': right_operand})
            return expr_node
        return left_operand

    def parse_return_statement(self):
        """ Parse a return statement (e.g., return x; or return 5;) """
        self.advance()  # Skip 'return'
        expr = self.parse_expression()
        self.expect('T_SEMICOLON')  # Expect ';'
        return ASTNode('ReturnStmt', {'expr': expr})

    def parse_type(self):
        """ Parse a type token (e.g., int, float) """
        if self.current_token[0] in ['T_INT', 'T_FLOAT', 'T_BOOL', 'T_STRING']:
            type_node = self.current_token
            self.advance()
            return type_node
        raise ParseError(f"Expected a type, but got: {self.current_token}")

    def parse_identifier(self):
        """ Parse an identifier (e.g., x, my_fn) """
        if self.current_token[0] == 'T_IDENTIFIER':
            ident = self.current_token
            self.advance()
            return ident
        raise ParseError(f"Expected an identifier, but got: {self.current_token}")
    
    def parse_parameter_list(self):
        """ Parse the parameter list (e.g., int x, float y) """
        params = []
        while self.current_token[0] != 'T_PARENR':
            param_type = self.parse_type()
            param_name = self.parse_identifier()
            params.append({'type': param_type, 'name': param_name})
            if self.current_token[0] == 'T_COMMA':
                self.advance()  # Skip the comma
        return params

    def parse_block(self):
        """ Parse a block of statements (e.g., { ... }) """
        block = []
        while self.current_token[0] != 'T_BRACER':
            block.append(self.parse_statement())
        return block
    
    def expect(self, token_type):
        """ Helper function to ensure the expected token type """
        if self.current_token[0] != token_type:
            raise ParseError(f"Expected {token_type}, but got {self.current_token[0]}")
        self.advance()
    
    def parse_string_literal(self):
        """ Parse a string literal (e.g., "hmm") """
        if self.current_token[0] == 'T_QUOTES':  # Check for opening quote
            self.advance()  # Consume the opening quote
            string_value = self.current_token[1]  # The actual string value inside the quotes
            self.advance()  # Consume the closing quote
            return ASTNode('StringLiteral', {'value': string_value})
        raise ParseError(f"Expected a string literal, but got: {self.current_token}")
