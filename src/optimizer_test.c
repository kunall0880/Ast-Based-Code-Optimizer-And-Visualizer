#include <stdio.h>
#include "ast.h"
#include "optimizer/optimizer.h"
#include "codegen.h"

int main() {
    // Set logging level to DEBUG for maximum information
    set_log_level(LOG_DEBUG);
    
    printf("\n=== Creating AST ===\n");
    
    // Create a simple AST with optimizable code
    ASTNode* root = create_node(NODE_FUNC_DEF, "main");
    if (!root) {
        fprintf(stderr, "Failed to create root node\n");
        return 1;
    }
    
    // Create a sequence of statements
    ASTNode* seq = NULL;
    
    // Add constant folding example: x = 1 + 2 * 3
    ASTNode* mult = make_binop_node('*', make_int_node(2), make_int_node(3));
    ASTNode* add = make_binop_node('+', make_int_node(1), mult);
    ASTNode* decl1 = make_decl_node("x", add);
    
    if (!decl1) {
        fprintf(stderr, "Failed to create declaration node\n");
        free_ast(root);
        return 1;
    }
    seq = make_seq_node(decl1, NULL);
    
    // Add dead code example: if (0) { y = 10; }
    ASTNode* if_stmt = make_if_node(
        make_int_node(0),
        make_binop_node('=', make_var_node("y"), make_int_node(10))
    );
    if (!if_stmt) {
        fprintf(stderr, "Failed to create if statement node\n");
        free_ast(root);
        return 1;
    }
    seq = make_seq_node(seq, if_stmt);
    
    // Add another constant folding example: z = (5 + 3) * 2
    ASTNode* add2 = make_binop_node('+', make_int_node(5), make_int_node(3));
    ASTNode* mult2 = make_binop_node('*', add2, make_int_node(2));
    ASTNode* decl2 = make_decl_node("z", mult2);
    seq = make_seq_node(seq, decl2);
    
    // Add return statement
    ASTNode* ret = make_return_node(make_binop_node('+', make_var_node("x"), make_var_node("z")));
    if (!ret) {
        fprintf(stderr, "Failed to create return node\n");
        free_ast(root);
        return 1;
    }
    seq = make_seq_node(seq, ret);
    
    // Set the function body
    root->left = seq;
    
    // Set optimization flags
    OptimizationFlags flags = {
        .constant_folding = 1,
        .dead_code_elimination = 1,
        .loop_unrolling = 0,  // Disable loop unrolling for now
        .log_level = LOG_DEBUG
    };
    
    // Print original AST
    printf("\n=== Original AST ===\n");
    print_ast(root, stdout, 0);
    
    // Optimize the AST
    printf("\n=== Optimizing AST ===\n");
    ASTNode* optimized = optimize_ast(root, flags);
    if (!optimized) {
        fprintf(stderr, "Optimization failed\n");
        free_ast(root);
        return 1;
    }
    
    // Print optimized AST
    printf("\n=== Optimized AST ===\n");
    print_ast(optimized, stdout, 0);
    
    // Generate code
    printf("\n=== Generating Code ===\n");
    FILE* output = fopen("test_output.c", "w");
    if (output) {
        generate_code(optimized, output, 0);
        fclose(output);
        printf("Optimized code written to test_output.c\n");
    } else {
        fprintf(stderr, "Failed to open output file\n");
        free_ast(optimized);
        return 1;
    }
    
    // Cleanup
    free_ast(optimized);
    printf("\n=== Test completed successfully ===\n");
    return 0;
} 