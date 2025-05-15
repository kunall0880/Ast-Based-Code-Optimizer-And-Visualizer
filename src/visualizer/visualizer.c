#include <stdio.h>
#include <stdlib.h>
#include "visualizer.h"

const char* get_node_color(NodeType type) {
    switch (type) {
        case NODE_INT:
        case NODE_STRING:
            return "lightblue";
        case NODE_VAR:
            return "lightgreen";
        case NODE_BINOP:
            return "pink";
        case NODE_FUNC_DEF:
        case NODE_FUNC_CALL:
            return "orange";
        case NODE_IF:
        case NODE_FOR:
            return "yellow";
        default:
            return "white";
    }
}

void write_node_to_dot(FILE* fp, ASTNode* node, int* id) {
    if (!node) return;
    
    int current_id = (*id)++;
    
    // Write node
    fprintf(fp, "    node%d [label=\"%s\\n%s\", shape=box, style=filled, fillcolor=\"%s\"];\n",
            current_id,
            get_node_type_str(node->type),
            node->value ? node->value : "",
            get_node_color(node->type));
    
    // Process children
    if (node->left) {
        int left_id = *id;
        write_node_to_dot(fp, node->left, id);
        write_edge_to_dot(fp, current_id, left_id);
    }
    
    if (node->right) {
        int right_id = *id;
        write_node_to_dot(fp, node->right, id);
        write_edge_to_dot(fp, current_id, right_id);
    }
    
    if (node->next) {
        int next_id = *id;
        write_node_to_dot(fp, node->next, id);
        write_edge_to_dot(fp, current_id, next_id);
        fprintf(fp, "    {rank=same; node%d; node%d;}\n", current_id, next_id);
    }
}

void write_edge_to_dot(FILE* fp, int from_id, int to_id) {
    fprintf(fp, "    node%d -> node%d;\n", from_id, to_id);
}

void generate_dot(ASTNode* root, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }
    
    // Write DOT file header
    fprintf(fp, "digraph AST {\n");
    fprintf(fp, "    node [fontname=\"Arial\"];\n");
    fprintf(fp, "    edge [fontname=\"Arial\"];\n");
    fprintf(fp, "    rankdir=TB;\n");
    
    // Generate nodes and edges
    int id = 0;
    write_node_to_dot(fp, root, &id);
    
    // Close DOT file
    fprintf(fp, "}\n");
    fclose(fp);
}

void generate_html_visualization(ASTNode* root, const char* filename) {
    // Generate DOT file first
    char dot_filename[256];
    snprintf(dot_filename, sizeof(dot_filename), "%s.dot", filename);
    generate_dot(root, dot_filename);
    
    // Create HTML file
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }
    
    // Write HTML header with embedded visualization
    fprintf(fp, "<!DOCTYPE html>\n");
    fprintf(fp, "<html>\n<head>\n");
    fprintf(fp, "    <title>AST Visualization</title>\n");
    fprintf(fp, "    <script src=\"https://d3js.org/d3.v5.min.js\"></script>\n");
    fprintf(fp, "    <script src=\"https://unpkg.com/@hpcc-js/wasm@0.3.11/dist/index.min.js\"></script>\n");
    fprintf(fp, "    <script src=\"https://unpkg.com/d3-graphviz@3.0.5/build/d3-graphviz.js\"></script>\n");
    fprintf(fp, "</head>\n<body>\n");
    
    // Add visualization div
    fprintf(fp, "    <div id=\"graph\" style=\"text-align: center;\"></div>\n");
    
    // Add JavaScript to load and render the DOT file
    fprintf(fp, "    <script>\n");
    fprintf(fp, "        d3.select(\"#graph\").graphviz()\n");
    fprintf(fp, "            .renderDot('");
    
    // Copy DOT file contents
    FILE* dot_fp = fopen(dot_filename, "r");
    if (dot_fp) {
        char buffer[1024];
        size_t bytes;
        while ((bytes = fread(buffer, 1, sizeof(buffer), dot_fp)) > 0) {
            fwrite(buffer, 1, bytes, fp);
        }
        fclose(dot_fp);
    }
    
    fprintf(fp, "');\n");
    fprintf(fp, "    </script>\n");
    fprintf(fp, "</body>\n</html>\n");
    
    fclose(fp);
} 