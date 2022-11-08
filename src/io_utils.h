#ifndef IO_UTILS_H
#define IO_UTILS_H

#include "tree.h"

const char AKINATOR_DB_FILE[] = "akinator.db.txt";

typedef int (*main_function)(binary_tree* tree);

/**
 * @brief Initialize akinator tree
 * 
 * @return Initialized tree
 */
binary_tree akinator_init(void);

/**
 * @brief Choose function to be run upon user input
 * 
 * @return Pointer to mode main function
 */
main_function choose_mode(void);

/**
 * @brief Print definition of a given word
 * 
 * @param[inout] tree Akinator tree state
 * 
 * @return 0 upon success, -1 otherwise
 */
int print_definition(binary_tree* tree);

/**
 * @brief Compare definitons of two words
 * 
 * @param[inout] tree Akinator tree state
 * 
 * @return 0 upon success, -1 otherwise
 */
int compare_words(binary_tree* tree);

/**
 * @brief Guess word interactively
 * 
 * @param[inout] tree Akinator tree state
 * 
 * @return 0 upon success, -1 otherwise
 */
int guess_word(binary_tree* tree);

/**
 * @brief Print akinator dictionary
 * 
 * @param[inout] tree Akinator tree state
 * 
 * @return 0 upon success, -1 otherwise
 */
int show_word_list(binary_tree* tree);

/**
 * @brief Dump tree as image with user-specified name
 * 
 * @param[inout] tree Akinator tree state
 * 
 * @return 0 upon success, -1 otherwise
 */
int dump_tree(binary_tree* tree);

/**
 * @brief 
 * Save akinator state and exit program
 * 
 * @param[inout] tree Akinator tree state
 * 
 */
[[noreturn]]
int quit(binary_tree* tree);

/**
 * @brief Print message to `stdout` and pronounce it using
 * festival
 * 
 */
void print_and_say(const char* format, ...)
    __attribute__((format (printf, 1, 2)));


#endif