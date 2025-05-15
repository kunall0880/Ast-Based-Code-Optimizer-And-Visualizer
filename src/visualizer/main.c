#include <stdio.h>
#include <stdlib.h>
#include "visualizer.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // For now, create a dummy AST for testing
    ASTNode* root = create_node(NODE_FUNC_DEF, "main");
    ASTNode* body = create_node(NODE_BINOP, "+");
    add_child(root, body);
    add_child(body, create_node(NODE_INT, "1"));
    add_child(body, create_node(NODE_INT, "2"));

    // Generate visualization
    generate_html_visualization(root, argv[2]);

    // Cleanup
    free_ast(root);

    return 0;
} 