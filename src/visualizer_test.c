#include <stdio.h>
#include "ast.h"
#include "visualizer/visualizer.h"

int main() {
    // Create a simple AST
    ASTNode* root = create_node(NODE_FUNC_DEF, "main");
    ASTNode* body = create_node(NODE_BINOP, "+");
    add_child(root, body);
    add_child(body, create_node(NODE_INT, "1"));
    add_child(body, create_node(NODE_INT, "2"));

    // Generate visualization
    generate_html_visualization(root, "test_output.html");

    // Cleanup
    free_ast(root);
    return 0;
} 