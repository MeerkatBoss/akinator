#ifndef TREE_H
#define TREE_H

#include <stddef.h>

struct tree_node
{
    const char* data;
    int leaf;
    tree_node* parent;
    tree_node* left;
    tree_node* right;
};

struct binary_tree
{
    tree_node* root;
    size_t size;
};

/**
 * @brief Create new tree node with specified parent
 * 
 * @param[in] data Data stored in node
 * @param[in] parent Parent node
 * @return Allocated `tree_node` instance
 */
tree_node* make_node(const char* data, tree_node* parent = NULL);

/**
 * @brief Delete created node and its subtree.
 * Free associated resources
 * 
 * @param[inout] node `tree_node` instance to be deleted
 */
void delete_node(tree_node* node);

/**
 * @brief Create `binary_tree` instance
 * 
 * @return Constructed `binary_tree` instance
 */
binary_tree tree_ctor(void);

/**
 * @brief Destroy `binary_tree`
 * 
 * @param[inout] tree `binary_tree` instance to be destroyed
 */
void tree_dtor(binary_tree* tree);

/**
 * @brief Get iterator to next tree node
 * 
 * @param[inout] node Current tree node
 */
void next_iterator(tree_node **node);

/**
 * @brief Get iterator to previous tree node
 * 
 * @param[inout] node Current tree node
 */
void prev_iterator(tree_node **node);

#endif