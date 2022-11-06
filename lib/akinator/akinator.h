#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>

#include "tree.h"

/**
 * @brief Get next question depending on the
 * truthiness of answer to current question
 * 
 * @param[in] node Current question
 * @param[in] answer Truthiness of answer
 * 
 * @return Next question
 */
tree_node* next_question(tree_node* node, int answer);

/**
 * @brief Check if current node is a terminal
 * (i.e not a question)
 * 
 * @param[in] node Current node
 * @return 1 if node is terminal, 0 otherwise
 */
int is_terminal(tree_node* node);

/**
 * @brief Find terminal in tree by its name
 * 
 * @param[in] tree Akinator tree
 * @param[in] name Terminal name
 * @return Found terminal node or `NULL`
 */
tree_node* find_terminal(binary_tree* tree, const char* name);

/**
 * @brief Get the list of all terminals in a tree
 * 
 * @param[in] tree Akinator tree
 * @param[out] terminals Buffer for pointers to terminals
 * @return Number of elements in `terminals` buffer
 */
int get_all_terminals(binary_tree* tree, tree_node* terminals[]);

/**
 * @brief Get list of questions and answers leading
 * to terminal
 * 
 * @param[in] node Terminal node
 * @param[out] questions Buffer for pointers to question nodes
 * @param[out] answers Buffer for answers to questions. 0 is no, 1 is yes
 * @return Number of questions leading to current terminal
 */
int get_definition(tree_node* node, tree_node* questions[], int answers[]);

/**
 * @brief Get first question differentiating between terminals
 * 
 * @param[in] tree Akinator tree
 * @param[in] first Terminal node 
 * @param[in] second Terminal node
 * @param[out] first_true 1 if answer for returned question for `first`
 *                          is "yes", 0 otherwise
 * @return Differentiating question or `NULL` if `first` and `second`
 *          point to the same terminal
 */
tree_node* get_difference(binary_tree* tree, tree_node* first, tree_node* second, int* first_true);

/**
 * @brief Add new terminal node
 * 
 * @param[in] tree Akinator tree
 * @param[in] node Terminal node with same definition
 * @param[in] question Differentiating question
 * @param[in] terminal New terminal name
 */
void split_with_question(binary_tree* tree, tree_node* node, const char* question, const char* terminal);

/**
 * @brief Print tree contents to a text file
 * 
 * @param[in] tree Akinator tree
 * @param[in] file Output file
 */
void save_tree(binary_tree* tree, FILE* file);

/**
 * @brief Restore tree contents from a text file
 * 
 * @param[in] file Input file
 * @return Loaded tree
 */
binary_tree load_tree(FILE* file);

#endif