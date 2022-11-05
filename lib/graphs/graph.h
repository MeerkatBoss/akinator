#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

struct graph_builder
{
    FILE* stream;
    int is_built;
    size_t node_cnt;
};


graph_builder get_builder(void);
void builder_dtor(graph_builder* builder);
void build(graph_builder* builder);
void draw(graph_builder* builder, const char* filename);

void add_property(graph_builder* builder, const char* format, ...);
size_t add_node(graph_builder* builder, const char* attrs, ...);
void add_edge(graph_builder* builder, size_t src, size_t dst, const char *attrs, ...);



#endif