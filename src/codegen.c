#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"

void write_indentation(FILE* output, int indent) {
    for (int i = 0; i < indent; i++) {
        fprintf(output, "    ");
    }
}

void generate_binary_expr(ASTNode* node, FILE* output) {
    if (!node) return;
    
    if (node->left) {
        if (node->left->type == NODE_BINOP) fprintf(output, "(");
        generate_code(node->left, output, 0);
        if (node->left->type == NODE_BINOP) fprintf(output, ")");
    }
    
    fprintf(output, " %s ", node->value);
    
    if (node->right) {
        if (node->right->type == NODE_BINOP) fprintf(output, "(");
        generate_code(node->right, output, 0);
        if (node->right->type == NODE_BINOP) fprintf(output, ")");
    }
}

void generate_declaration(ASTNode* node, FILE* output, int indent) {
    if (!node || !node->value) return;
    
    write_indentation(output, indent);
    fprintf(output, "int %s = ", node->value);
    if (node->left) {
        generate_code(node->left, output, 0);
    } else {
        fprintf(output, "0");  // Default initialization
    }
    fprintf(output, ";\n");
}

void generate_if_statement(ASTNode* node, FILE* output, int indent) {
    if (!node) return;
    
    write_indentation(output, indent);
    fprintf(output, "if (");
    if (node->left) {
        generate_code(node->left, output, 0);
    }
    fprintf(output, ") {\n");
    
    if (node->right) {
        generate_code(node->right, output, indent + 1);
    }
    
    write_indentation(output, indent);
    fprintf(output, "}\n");
}

void generate_for_loop(ASTNode* node, FILE* output, int indent) {
    if (!node) return;
    
    write_indentation(output, indent);
    fprintf(output, "for (");
    
    // Init
    if (node->left) generate_code(node->left, output, 0);
    fprintf(output, "; ");
    
    // Condition
    if (node->right) generate_code(node->right, output, 0);
    fprintf(output, "; ");
    
    // Update
    if (node->right && node->right->next) 
        generate_code(node->right->next, output, 0);
    
    fprintf(output, ") {\n");
    
    // Body
    if (node->right && node->right->next && node->right->next->next)
        generate_code(node->right->next->next, output, indent + 1);
    
    write_indentation(output, indent);
    fprintf(output, "}\n");
}

void generate_function(ASTNode* node, FILE* output, int indent) {
    if (!node || !node->value) return;
    
    write_indentation(output, indent);
    fprintf(output, "int %s() {\n", node->value);
    if (node->left) {
        generate_code(node->left, output, indent + 1);
    }
    write_indentation(output, indent);
    fprintf(output, "}\n");
}

void generate_return(ASTNode* node, FILE* output, int indent) {
    if (!node) return;
    
    write_indentation(output, indent);
    fprintf(output, "return ");
    if (node->left) {
        generate_code(node->left, output, 0);
    } else {
        fprintf(output, "0");  // Default return value
    }
    fprintf(output, ";\n");
}

void generate_code(ASTNode* node, FILE* output, int indent) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_INT:
        case NODE_VAR:
            if (node->value) {
                fprintf(output, "%s", node->value);
            }
            break;
            
        case NODE_BINOP:
            generate_binary_expr(node, output);
            break;
            
        case NODE_DECL:
            generate_declaration(node, output, indent);
            break;
            
        case NODE_IF:
            generate_if_statement(node, output, indent);
            break;
            
        case NODE_FOR:
            generate_for_loop(node, output, indent);
            break;
            
        case NODE_FUNC_DEF:
            generate_function(node, output, indent);
            break;
            
        case NODE_RETURN:
            generate_return(node, output, indent);
            break;
            
        case NODE_SEQ:
            if (node->left) generate_code(node->left, output, indent);
            if (node->right) generate_code(node->right, output, indent);
            break;
            
        default:
            fprintf(stderr, "Warning: Unknown node type %d\n", node->type);
            break;
    }
    
    // Handle siblings
    if (node->next) {
        generate_code(node->next, output, indent);
    }
} 