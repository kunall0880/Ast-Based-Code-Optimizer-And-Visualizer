#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

// Generate C code from AST
void generate_code(ASTNode* node, FILE* output, int indent);

// Utility functions
void write_indentation(FILE* output, int indent);
void generate_binary_expr(ASTNode* node, FILE* output);
void generate_declaration(ASTNode* node, FILE* output, int indent);
void generate_if_statement(ASTNode* node, FILE* output, int indent);
void generate_for_loop(ASTNode* node, FILE* output, int indent);
void generate_function(ASTNode* node, FILE* output, int indent);

#endif 