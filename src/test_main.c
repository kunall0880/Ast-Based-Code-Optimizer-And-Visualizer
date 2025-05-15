#include <stdio.h>
#include "ast.h"

int main() {
    // Create a simple AST
    ASTNode* root = create_node(NODE_FUNC_DEF, "main");
    ASTNode* body = create_node(NODE_BINOP, "+");
    add_child(root, body);
    add_child(body, create_node(NODE_INT, "1"));
    add_child(body, create_node(NODE_INT, "2"));

    // Print the AST
    print_ast(root, stdout, 0);

    // Cleanup
    free_ast(root);
    return 0;
} 