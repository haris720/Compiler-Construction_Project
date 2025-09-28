class ASTNode:
    def __init__(self, node_type, value=None):
        self.node_type = node_type  # e.g., 'FnDecl', 'VarDecl', 'Expr', 'StringLiteral'
        self.value = value  # Stores the value of the node (e.g., function name, operator, string value)
        self.children = []  # List of child nodes (subtrees)

    def add_child(self, child_node):
        """ Adds a child node to this node. """
        self.children.append(child_node)

    def __repr__(self):
        # For StringLiteral, it represents just the value, otherwise it shows the type and value
        if self.node_type == 'StringLiteral':
            return f"{self.node_type}({self.value!r})"
        return f"{self.node_type}({self.value})"
    
    def __str__(self):
        """ Display the node and its children for debugging """
        return f"{self.node_type}({self.value}) -> Children: [{', '.join(str(child) for child in self.children)}]"

