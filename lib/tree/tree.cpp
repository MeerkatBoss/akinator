#include <stdlib.h>

#include "logger.h"
#include "graph.h"

#include "tree.h"

#define BGCOLOR "lightsteelblue1"
#define NODE_COLOR "springgreen"
#define NODE_HEAD_COLOR "springgreen4"
#define HEAD_FONT_COLOR "azure"
#define BORDER_COLOR "darkslategray"
#define EDGE_COLOR "midnightblue"
#define ROOT_COLOR "navajowhite"
#define ROOT_HEAD_COLOR "peru"
#define SPEC_HEAD_COLOR "darkslategrey"

#define NODE_FORMAT(name, bg, head) \
                        "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\" BGCOLOR=\"" bg "\">\n"\
                        "<TR><TD ALIGN=\"CENTER\" COLSPAN=\"2\" BGCOLOR=\"" head "\">"\
                            "<FONT COLOR=\"" HEAD_FONT_COLOR "\"><B>" name " [%zu]</B></FONT></TD></TR>\n"\
                        "<TR><TD COLSPAN=\"2\"><I>data: </I> %s</TD></TR>\n"\
                        "<TR><TD><I>left: </I>%zu</TD><TD><I>right: </I>%zu</TD></TR>\n"\
                        "</TABLE>"

#define NODE_ARGS(node) node->node_number, node->data,\
                            (node->left ? node->left ->node_number  : 0),\
                            (node->right? node->right->node_number  : 0)

tree_node* make_node(element_t data, tree_node* parent)
{
    LOG_ASSERT(data != NULL, return NULL);

    tree_node* node = (tree_node*)calloc(1, sizeof(*node));
    *node = {
        .data = data,
        .node_number = 0,
        .parent = parent,
        .left = NULL,
        .right = NULL
    };

    return node;
}

void delete_node(tree_node* node)
{
    LOG_ASSERT(node != NULL, return);

    if (node->left) delete_node(node->left);
    if (node->right) delete_node(node->right);

    free(node);
}

binary_tree tree_ctor(void)
{
    return {
        .root = NULL,
        .size = 0
    };
}

void tree_dtor(binary_tree* tree)
{
    LOG_ASSERT(tree != NULL, return);

    if (tree->root) delete_node(tree->root);
}

void next_iterator(tree_node **node)
{
    LOG_ASSERT(node, return);
    LOG_ASSERT(*node, return);

    if ((*node)->left)
    {
        *node = (*node)->left;
        return;
    }

    if ((*node)->right)
    {
        *node = (*node)->right;
        return;
    }

    /* find first ancestor with right child not visited*/
    while (*node && (*node)->parent
            && (*node == (*node)->parent->right || !(*node)->parent->right))
        *node = (*node)->parent;
    
    if ((*node)->parent)
        (*node) = (*node)->parent->right;
    else
        *node = NULL;
}

void prev_iterator(tree_node **node)
{
    LOG_ASSERT(node, return);
    LOG_ASSERT(*node, return);
    LOG_ASSERT((*node)->parent, return);

    if (*node == (*node)->parent->left)
    {
        *node = (*node)->parent;
        return;
    }

    *node = (*node)->parent->left;

    LOG_ASSERT(*node, return);

    while ((*node)->right)
        *node = (*node)->right;
}

void tree_dump(const binary_tree* tree, const char* filename)
{
    size_t *node_id = (size_t*)calloc(tree->size, sizeof(*node_id));

    /* assign numbers to nodes */
    size_t cnt = 0;
    for (tree_node* it = tree->root; it != NULL; next_iterator(&it))
        it->node_number = cnt++;

    graph_builder builder = get_builder();
    add_property(&builder, "bgcolor=%s", BGCOLOR);
    add_property(&builder, "node [shape=plaintext color=%s]", BORDER_COLOR);

    for (tree_node* it = tree->root; it != NULL; next_iterator(&it))
    {
        if (it->node_number == 0)
            node_id[it->node_number] 
                = add_node(&builder,
                        "label=<"NODE_FORMAT("root", ROOT_COLOR, ROOT_HEAD_COLOR)">",
                        NODE_ARGS(it));
        else if(!it->left && !it->right)
            node_id[it->node_number] 
                = add_node(&builder,
                        "label=<"NODE_FORMAT("leaf", NODE_COLOR, SPEC_HEAD_COLOR)">",
                        NODE_ARGS(it));
        else
            node_id[it->node_number] 
                = add_node(&builder,
                        "label=<"NODE_FORMAT("node", NODE_COLOR, NODE_HEAD_COLOR)">",
                        NODE_ARGS(it));
    }
    for (tree_node* it = tree->root; it != NULL; next_iterator(&it))
    {
        if (it->left)
            add_edge(&builder,
                node_id[it->node_number],
                node_id[it->left->node_number],
                "color="EDGE_COLOR);
        if (it->right)
            add_edge(&builder,
                node_id[it->node_number],
                node_id[it->right->node_number],
                "color="EDGE_COLOR);
    }

    draw(&builder, filename);

    for (tree_node* it = tree->root; it != NULL; next_iterator(&it))
        it->node_number = 0;
    free(node_id);
    builder_dtor(&builder);
}
