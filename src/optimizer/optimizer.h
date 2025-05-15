#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "../ast.h"
#include <stdio.h>

// Debug levels
typedef enum {
    LOG_ERROR = 0,
    LOG_WARN = 1,
    LOG_INFO = 2,
    LOG_DEBUG = 3
} LogLevel;

// Optimization flags
typedef struct {
    int constant_folding;
    int dead_code_elimination;
    int loop_unrolling;
    LogLevel log_level;  // Added logging level
} OptimizationFlags;

// Logging functions
void optimizer_log(LogLevel level, const char* format, ...);
void set_log_level(LogLevel level);

// Main optimization function
ASTNode* optimize_ast(ASTNode* root, OptimizationFlags flags);

// Individual optimization passes
ASTNode* constant_folding(ASTNode* node);
ASTNode* dead_code_elimination(ASTNode* node);
ASTNode* loop_unrolling(ASTNode* node);

// Utility functions
int is_constant_expr(ASTNode* node);
int evaluate_constant_expr(ASTNode* node);
int is_dead_code(ASTNode* node);
int validate_ast_node(ASTNode* node);  // Added validation function

#endif // OPTIMIZER_H 