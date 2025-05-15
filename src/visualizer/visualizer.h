#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "../ast.h"

// Generate DOT file for Graphviz visualization
void generate_dot(ASTNode* root, const char* filename);

// Generate HTML with embedded visualization
void generate_html_visualization(ASTNode* root, const char* filename);

// Utility functions
void write_node_to_dot(FILE* fp, ASTNode* node, int* id);
void write_edge_to_dot(FILE* fp, int from_id, int to_id);
const char* get_node_color(NodeType type);

#endif // VISUALIZER_H 