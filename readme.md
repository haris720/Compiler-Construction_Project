# Language Grammar - Backus-Naur Form (BNF)

## Overview
This document describes the grammar for a simple functional programming language that supports function declarations, variable declarations, and basic expressions.

## Lexical Elements

### Keywords

fn, int, float, string, return


### Operators

= (assignment)
== (equality)
+, -, *, / (arithmetic)


### Delimiters

( ) { } , ;


### Literals

INTLIT    ::= [0-9]+
FLOATLIT  ::= [0-9]+ '.' [0-9]+
STRINGLIT ::= '"' [^"]* '"'


### Identifiers

IDENTIFIER ::= [a-zA-Z_][a-zA-Z0-9_]*


## Grammar Rules (BNF)

### Program Structure
bnf
<program> ::= <declaration>*

<declaration> ::= <function_declaration>
                | <variable_declaration>


### Function Declarations
bnf
<function_declaration> ::= "fn" <identifier> "(" <parameter_list>? ")" "{" <statement>* "}"

<parameter_list> ::= <parameter> ("," <parameter>)*

<parameter> ::= <type> <identifier>

<type> ::= "int" | "float" | "string"


### Statements
bnf
<statement> ::= <variable_declaration>
              | <return_statement>
              | <expression_statement>
              | <block_statement>

<variable_declaration> ::= <type> <identifier> "=" <expression> ";"

<return_statement> ::= "return" <expression>? ";"

<expression_statement> ::= <expression> ";"

<block_statement> ::= "{" <statement>* "}"


### Expressions (with precedence)
bnf
<expression> ::= <equality>

<equality> ::= <addition> ("==" <addition>)*

<addition> ::= <multiplication> (("+" | "-") <multiplication>)*

<multiplication> ::= <primary> (("*" | "/") <primary>)*

<primary> ::= <identifier>
            | <literal>
            | <call_expression>
            | "(" <expression> ")"

<call_expression> ::= <identifier> "(" <argument_list>? ")"

<argument_list> ::= <expression> ("," <expression>)*

<literal> ::= INTLIT | FLOATLIT | STRINGLIT

<identifier> ::= IDENTIFIER


## Language Features

### Supported Data Types
- int: Integer numbers
- float: Floating-point numbers  
- string: String literals

### Supported Operations
- Arithmetic: +, -, *, /
- Equality: ==
- Assignment: =
- Function calls

### Sample Program
c
fn add(int a, int b) {
    int result = a + b;
    return result;
}

fn main() {
    int x = 5;
    int y = 10;
    int sum = add(x, y);
    return sum;
}


## Parser Implementation Notes

The parser implements:
- *Recursive Descent Parsing*: Top-down parsing approach
- *Precedence Climbing*: For expression parsing with operator precedence
- *Abstract Syntax Tree (AST)*: Builds a tree representation of the parsed program
- *Error Handling*: Provides meaningful error messages with token context

### AST Node Types
- *Program*: Root node containing all declarations
- *FnDecl*: Function declaration with parameters and body
- *VarDecl*: Variable declaration with type and initialization
- *Block*: Block statement containing multiple statements
- *Return*: Return statement with optional expression
- *ExprStmt*: Expression statement
- *Binary*: Binary operation (arithmetic, equality)
- *Call*: Function call with arguments
- *Identifier*: Variable or function name reference
- *Literals*: Integer, float, and string literals