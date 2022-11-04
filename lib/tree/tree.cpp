#include <stdlib.h>

#include "logger.h"

#include "tree.h"

tree_node* make_node(const char* data, tree_node* parent)
{
    LOG_ASSERT(data != NULL, return NULL);

    tree_node* node = (tree_node*)calloc(1, sizeof(*node));
    *node = {
        .data = data,
        .leaf = 1,
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
        *node = (*node)->right;
        return;
    }

    if ((*node)->right)
    {
        *node = (*node)->right;
        return;
    }

    /* find first ancestor with right child not visited*/
    while (*node && *node == (*node)->parent->right)
        *node = (*node)->parent;
    
    if ((*node))
        (*node) = (*node)->parent->right;
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
