#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "logger.h"

#include "akinator.h"

tree_node* next_question(tree_node* node, int answer)
{
    LOG_ASSERT(node, return NULL);
    LOG_ASSERT(node->left, return NULL);
    LOG_ASSERT(node->right, return NULL);

    return answer ? node->left : node->right;
}

int is_terminal(tree_node* node)
{
    LOG_ASSERT(node, return -1);

    return node->left == NULL && node->right == NULL;
}

tree_node* find_terminal(binary_tree* tree, const char* name)
{
    LOG_ASSERT(tree, return NULL);
    LOG_ASSERT(name, return NULL);

    for (tree_node* it = tree->root; it != NULL; next_iterator(&it))
        if (is_terminal(it) && strcasecmp(name, it->data))
            return it;

    return NULL;
}

int get_all_terminals(binary_tree* tree, tree_node* terminals[])
{
    LOG_ASSERT(tree, return -1);
    LOG_ASSERT(terminals, return -1);

    int cnt = 0;

    for (tree_node* it = tree->root; it != NULL; next_iterator(&it))
        if (is_terminal(it))
            terminals[cnt++] = it;
    
    return cnt;
}

int get_definition(tree_node* node, tree_node* questions[], int answers[])
{
    LOG_ASSERT(node, return -1);
    LOG_ASSERT(questions, return -1);
    LOG_ASSERT(answers, return -1);

    int cnt = 0;

    tree_node* tmp = node;
    while ((tmp = tmp->parent))
        tmp++;
    
    for (int i = cnt - 1; i >= 0; i--)
    {
        questions[i] = node->parent;
        answers[i] = (node == node->parent->left);
        node = node->parent;
    }

    return cnt;
}

tree_node* get_difference(binary_tree* tree, tree_node* first, tree_node* second, int* first_true)
{
    LOG_ASSERT(first, return NULL);
    LOG_ASSERT(second, return NULL);
    LOG_ASSERT(first_true, return NULL);

    if (first == second)
        return NULL;

    tree_node** fs_path = (tree_node**) calloc(tree->size, sizeof(*fs_path));
    tree_node** sc_path = (tree_node**) calloc(tree->size, sizeof(*sc_path));
    int* fs_ans         = (int*)        calloc(tree->size, sizeof(*fs_ans));
    int* sc_ans         = (int*)        calloc(tree->size, sizeof(*sc_ans));

    get_definition(first,  fs_path, fs_ans);
    get_definition(second, sc_path, sc_ans);

    int it = 0;
    while (fs_ans[it] == sc_ans[it])
        it++;
    
    tree_node* result = fs_path[it];
    *first_true = fs_ans[it];
    
    free(fs_path);
    free(sc_path);
    free(fs_ans);
    free(sc_ans);

    return result;
}

void split_with_question(binary_tree* tree, tree_node* node, const char* question, const char* terminal)
{
    static char buffer[16] = "";

    LOG_ASSERT(tree, return);
    LOG_ASSERT(node, return);
    LOG_ASSERT(question, return);
    LOG_ASSERT(terminal, return);
    
    int n_read = 0;
    int invert = 0;

    sscanf(buffer, " %16s %*c%n", buffer, &n_read);

    LOG_ASSERT(n_read > 0, return);

    if (strcasecmp("not", buffer) == 0)
    {
        question += n_read - 1;
        invert = 1;
    }

    node->left = make_node(terminal, node->left);
    node->right = make_node(node->data, node->right);
    free(node->data);
    node->data = strdup(question);
    tree->size+=2;

    if (invert)
    {
        tree_node* tmp = node->left;
        node->left = node->right;
        node->right = tmp;
    }
}

static void print_subtree(tree_node* root, FILE* file, int level = 0);

void save_tree(binary_tree* tree, FILE* file)
{
    print_subtree(tree->root, file);
}

static void indent(FILE* file, int tab_cnt);

static void print_subtree(tree_node* root, FILE* file, int level)
{
    indent(file, level);
    fputs("{\n", file);

    indent(file, level+1);
    fputs(root->data, file);
    
    if (is_terminal(root))
    {
        fputs(" }\n", file);
        return;
    }

    fputc('\n', file);

    if (root->left)
        print_subtree(root->left, file, level + 1);
    if (root->right)
        print_subtree(root->right, file, level + 1);

    indent(file, level);
    fputs("}\n", file);
}

static void indent(FILE* file, int tab_cnt)
{
    for(int i = 0; i < tab_cnt; i++)
        fputc('\t', file);
}

static tree_node* load_node(FILE* file, size_t* size);

binary_tree load_tree(FILE* file)
{
    size_t size = 0;
    tree_node* root = load_node(file, &size);

    return {
        .root = root,
        .size = size
    };
}

static tree_node* load_node(FILE* file, size_t* size)
{
    static const int BUFFER_SIZE = 1024;
    static char buffer[BUFFER_SIZE] = "";
    char fst = '\0';

    fscanf(file, " %c", &fst);

    LOG_ASSERT(fst == '{' || fst == '}', return NULL);

    if (fst == '}')
    {
        *size = 1;
        return NULL;
    }

    LOG_ASSERT(fscanf(file, "\"%[^\"]\"", buffer), return NULL);
    tree_node* result = make_node(buffer);
    size_t l_size = 0;
    size_t r_size = 0;

    result->left = load_node(file, &l_size);
    result->right= load_node(file, &r_size);

    if (result->left)
        result->left->parent = result;

    if(result->right)
        result->right->parent = result;

    *size = l_size + r_size;
    return result;
}
