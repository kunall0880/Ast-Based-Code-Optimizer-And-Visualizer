#include <stdio.h>
#include <stdlib.h>
#include "optimizer.h"
#include "codegen.h"

extern int yyparse();
extern FILE* yyin;
extern ASTNode* ast_root;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // Open input file
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror(argv[1]);
        return 1;
    }

    // Parse input file
    if (yyparse()) {
        fprintf(stderr, "Parse error!\n");
        return 1;
    }

    // Set optimization flags
    OptimizationFlags flags = {
        .constant_folding = 1,
        .dead_code_elimination = 1,
        .loop_unrolling = 1
    };

    // Optimize the AST
    ast_root = optimize_ast(ast_root, flags);

    // Open output file
    FILE* output = fopen(argv[2], "w");
    if (!output) {
        perror(argv[2]);
        free_ast(ast_root);
        return 1;
    }

    // Generate optimized code
    generate_code(ast_root, output, 0);

    // Cleanup
    free_ast(ast_root);
    fclose(yyin);
    fclose(output);

    return 0;
} 