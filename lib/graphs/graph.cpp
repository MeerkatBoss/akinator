#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

#include "graph.h"

#define TMPFILE "tmp.dot"

graph_builder get_builder(void)
{
    //FILE* graph_file = tmpfile();
    FILE* graph_file = fopen(TMPFILE, "w+");
    fputs("digraph G{\n", graph_file);
    fputs(  "layout=dot\n"
            "rankdir=TD\n"
            "splines=ortho\n"
            "nodesep=0.1\n"
            "overlap=voronoi\n",
            graph_file);
    return {
        .stream = graph_file,
        .is_built = 0,
        .node_cnt = 0
    };
}

void builder_dtor(graph_builder* builder)
{
    fclose(builder->stream);
    *builder = {};
}

void build(graph_builder* builder)
{
    if (builder->is_built) return;

    fputc('}', builder->stream);
    builder->is_built = 1;
    fflush(builder->stream);
}

static const char* get_file_name(FILE* stream);

void draw(graph_builder* builder, const char* filename)
{
    static char cmd_buf[FILENAME_MAX * 2] = "";

    if (!builder->is_built) build(builder);

    //const char *src_name = get_file_name(builder->stream);
    sprintf(cmd_buf, "dot -Tpng -o %s %s", filename, TMPFILE);

    system(cmd_buf);
}

void add_property(graph_builder* builder, const char* format, ...)
{
    va_list list = {};
    va_start(list, format);
    vfprintf(builder->stream, format, list);
    va_end(list);
    fputc('\n', builder->stream);
}

size_t add_node(graph_builder* builder, const char* attrs, ...)
{
    fprintf(builder->stream, "node%zu [", builder->node_cnt);

    va_list list = {};
    va_start(list, attrs);
    vfprintf(builder->stream, attrs, list);
    va_end(list);

    fputs("]\n", builder->stream);

    return builder->node_cnt++;
}

void add_edge(graph_builder* builder, size_t src, size_t dst, const char *attrs, ...)
{
    fprintf(builder->stream, "node%zu->node%zu [", src, dst);

    va_list list = {};
    va_start(list, attrs);
    vfprintf(builder->stream, attrs, list);
    va_end(list);

    fputs("]\n", builder->stream);
}

static const char* get_file_name(FILE* stream)
{
    static char fname[FILENAME_MAX] = "";
    static char link[FILENAME_MAX] = "";

    sprintf(fname, "/proc/self/fd/%d", fileno(stream));
    if(readlink(fname, link, sizeof(link - 1)) > 0)
        return link;
    return NULL;
}


