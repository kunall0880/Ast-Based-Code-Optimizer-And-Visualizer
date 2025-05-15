#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "optimizer.h"

static LogLevel current_log_level = LOG_INFO;

void set_log_level(LogLevel level) {
    current_log_level = level;
}

void optimizer_log(LogLevel level, const char* format, ...) {
    if (level <= current_log_level) {
        va_list args;
        va_start(args, format);
        
        switch (level) {
            case LOG_ERROR:
                fprintf(stderr, "[ERROR] ");
                break;
            case LOG_WARN:
                fprintf(stderr, "[WARN] ");
                break;
            case LOG_INFO:
                fprintf(stderr, "[INFO] ");
                break;
            case LOG_DEBUG:
                fprintf(stderr, "[DEBUG] ");
                break;
        }
        
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");
        va_end(args);
    }
}

int validate_ast_node(ASTNode* node) {
    if (!node) return 1;  // NULL node is valid
    
    // Check for valid node type
    if (node->type < NODE_INT || node->type > NODE_TYPE) {
        optimizer_log(LOG_ERROR, "Invalid node type: %d", node->type);
        return 0;
    }
    
    // Check for required value in certain node types
    if ((node->type == NODE_INT || node->type == NODE_VAR) && !node->value) {
        optimizer_log(LOG_ERROR, "Missing value for node type %s", get_node_type_str(node->type));
        return 0;
    }
    
    // Check for valid operator in binary operations
    if (node->type == NODE_BINOP && node->value) {
        char op = node->value[0];
        if (op != '+' && op != '-' && op != '*' && op != '/' && op != '=') {
            optimizer_log(LOG_ERROR, "Invalid binary operator: %c", op);
            return 0;
        }
    }
    
    return 1;
}

ASTNode* optimize_ast(ASTNode* root, OptimizationFlags flags) {
    if (!root) {
        optimizer_log(LOG_DEBUG, "Optimize_ast called with NULL root");
        return NULL;
    }
    
    if (!validate_ast_node(root)) {
        optimizer_log(LOG_ERROR, "Invalid AST node detected during optimization");
        return root;  // Return unoptimized node rather than crashing
    }
    
    optimizer_log(LOG_DEBUG, "Optimizing node type: %s", get_node_type_str(root->type));
    
    // Apply optimizations in sequence
    if (flags.constant_folding) {
        optimizer_log(LOG_INFO, "Applying constant folding");
        root = constant_folding(root);
        if (!validate_ast_node(root)) {
            optimizer_log(LOG_ERROR, "Constant folding produced invalid AST");
            return NULL;
        }
    }
    
    if (flags.dead_code_elimination) {
        optimizer_log(LOG_INFO, "Applying dead code elimination");
        root = dead_code_elimination(root);
        if (!validate_ast_node(root)) {
            optimizer_log(LOG_ERROR, "Dead code elimination produced invalid AST");
            return NULL;
        }
    }
    
    if (flags.loop_unrolling) {
        optimizer_log(LOG_INFO, "Applying loop unrolling");
        root = loop_unrolling(root);
        if (!validate_ast_node(root)) {
            optimizer_log(LOG_ERROR, "Loop unrolling produced invalid AST");
            return NULL;
        }
    }
    
    // Recursively optimize children if they exist
    if (root->left) {
        optimizer_log(LOG_DEBUG, "Optimizing left child of %s", get_node_type_str(root->type));
        root->left = optimize_ast(root->left, flags);
    }
    
    if (root->right) {
        optimizer_log(LOG_DEBUG, "Optimizing right child of %s", get_node_type_str(root->type));
        root->right = optimize_ast(root->right, flags);
    }
    
    if (root->next) {
        optimizer_log(LOG_DEBUG, "Optimizing next sibling of %s", get_node_type_str(root->type));
        root->next = optimize_ast(root->next, flags);
    }
    
    return root;
}

int is_constant_expr(ASTNode* node) {
    if (!node) return 1;
    
    if (!validate_ast_node(node)) {
        optimizer_log(LOG_ERROR, "Invalid node in constant expression check");
        return 0;
    }
    
    if (node->type == NODE_INT) {
        return 1;
    }
    
    if (node->type == NODE_BINOP) {
        return is_constant_expr(node->left) && is_constant_expr(node->right);
    }
    
    return 0;
}

int evaluate_constant_expr(ASTNode* node) {
    if (!node) {
        optimizer_log(LOG_ERROR, "Attempt to evaluate NULL expression");
        return 0;
    }
    
    if (!validate_ast_node(node)) {
        optimizer_log(LOG_ERROR, "Invalid node in constant evaluation");
        return 0;
    }
    
    if (node->type == NODE_INT) {
        return atoi(node->value);
    }
    
    if (node->type == NODE_BINOP) {
        int left = evaluate_constant_expr(node->left);
        int right = evaluate_constant_expr(node->right);
        
        switch (node->value[0]) {
            case '+': return left + right;
            case '-': return left - right;
            case '*': return left * right;
            case '/':
                if (right == 0) {
                    optimizer_log(LOG_ERROR, "Division by zero detected");
                    return 0;
                }
                return left / right;
            default:
                optimizer_log(LOG_ERROR, "Unknown operator in constant expression");
                return 0;
        }
    }
    
    return 0;
}

ASTNode* constant_folding(ASTNode* node) {
    if (!node) return NULL;
    
    if (!validate_ast_node(node)) {
        optimizer_log(LOG_ERROR, "Invalid node during constant folding");
        return node;  // Return unoptimized node
    }
    
    // If this is a constant expression, evaluate it
    if (node->type == NODE_BINOP && is_constant_expr(node)) {
        optimizer_log(LOG_INFO, "Found constant expression to fold");
        int result = evaluate_constant_expr(node);
        char value[32];
        snprintf(value, sizeof(value), "%d", result);
        
        // Create a new constant node
        ASTNode* folded = create_node(NODE_INT, value);
        if (!folded) {
            optimizer_log(LOG_ERROR, "Failed to create folded node");
            return node;  // Return original if allocation fails
        }
        
        optimizer_log(LOG_INFO, "Folded expression to constant: %d", result);
        
        // Free the old subtree
        free_ast(node);
        return folded;
    }
    
    return node;
}

int is_dead_code(ASTNode* node) {
    if (!node) return 0;
    
    if (!validate_ast_node(node)) {
        optimizer_log(LOG_ERROR, "Invalid node in dead code check");
        return 0;
    }
    
    // Check for unreachable code after return
    if (node->type == NODE_RETURN && node->next != NULL) {
        optimizer_log(LOG_INFO, "Found dead code after return statement");
        return 1;
    }
    
    // Check for if statements with constant conditions
    if (node->type == NODE_IF && is_constant_expr(node->left)) {
        int condition = evaluate_constant_expr(node->left);
        if (!condition) {
            optimizer_log(LOG_INFO, "Found dead code in if statement with constant false condition");
        }
        return !condition;
    }
    
    return 0;
}

ASTNode* dead_code_elimination(ASTNode* node) {
    if (!node) return NULL;
    
    if (!validate_ast_node(node)) {
        optimizer_log(LOG_ERROR, "Invalid node during dead code elimination");
        return node;
    }
    
    if (is_dead_code(node)) {
        optimizer_log(LOG_INFO, "Eliminating dead code");
        ASTNode* next = node->next;
        node->next = NULL;  // Prevent freeing the next node
        free_ast(node);
        return next;
    }
    
    return node;
}

ASTNode* loop_unrolling(ASTNode* node) {
    if (!node) return NULL;
    
    if (!validate_ast_node(node)) {
        optimizer_log(LOG_ERROR, "Invalid node during loop unrolling");
        return node;
    }
    
    if (node->type != NODE_FOR) return node;
    
    optimizer_log(LOG_DEBUG, "Analyzing loop for unrolling");
    
    // For now, we only unroll loops with constant bounds
    if (node->right && is_constant_expr(node->right)) {
        int bound = evaluate_constant_expr(node->right);
        if (bound <= 4) { // Only unroll small loops
            optimizer_log(LOG_INFO, "Unrolling loop with %d iterations", bound);
            
            // Create a sequence of the loop body
            ASTNode* sequence = NULL;
            for (int i = 0; i < bound; i++) {
                if (!sequence) {
                    sequence = node->right->next->next; // Loop body
                } else {
                    // Clone the body and add it to sequence
                    ASTNode* clone = create_node(sequence->type, sequence->value);
                    if (!clone) {
                        optimizer_log(LOG_ERROR, "Failed to clone loop body");
                        return node;
                    }
                    add_sibling(sequence, clone);
                }
            }
            
            if (sequence) {
                optimizer_log(LOG_INFO, "Successfully unrolled loop");
                return sequence;
            }
        } else {
            optimizer_log(LOG_DEBUG, "Loop not unrolled - too many iterations (%d)", bound);
        }
    }
    
    return node;
} 