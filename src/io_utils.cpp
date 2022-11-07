#include <stdio.h>
#include <stdarg.h>
#include <strings.h>
#include <string.h>

#define NTRACE
#include "logger.h"
#include "akinator.h"

#include "io_utils.h"

binary_tree akinator_init(void)
{
    FILE* database = fopen(AKINATOR_DB_FILE, "r");
    binary_tree tree = {};
    if (database)
    {
        tree = load_tree(database);
        fclose(database);
    }
    else
        tree = tree_ctor();

    return tree;
}

static void skip_line(void);

static void print_and_say(const char* format, ...)
    __attribute__((format (printf, 1, 2)));

main_function choose_mode(void)
{
    static char BUFFER[3] = "";

    print_and_say("What would you like me to do?\n");
    
    while(1) 
    {
        puts("[L]ist words, [D]efine word, [C]ompare words, [G]uess word");
        puts("[S]ave tree image, [Q]uit");

        LOG_ASSERT(scanf("%2s", BUFFER) == 1, return NULL);

        if (strlen(BUFFER) == 1) break;

        print_and_say("Invalid input. Please, try again.\n");
        skip_line();
    }

    while (1)
    {
        switch (BUFFER[0])
        {
        case 'L':
        case 'l':
            return show_word_list;
        case 'D':
        case 'd':
            return print_definition;
        case 'C':
        case 'c':
            return compare_words;
        case 'G':
        case 'g':
            return guess_word;
        case 'S':
        case 's':
            return dump_tree;
        case 'Q':
        case 'q':
            return quit;
        default:
            print_and_say("Invalid input. Please, try again.\n");
            skip_line();
            break;
        }
    }

    return NULL;
}

int print_definition(binary_tree* tree)
{
    char* word = NULL;
    size_t word_len = 0;

    if(getline(&word, &word_len, stdin) == -1)
        return 0;
    *strchr(word, '\n') = '\0';

    tree_node* node = find_terminal(tree, word);

    if (!node)
    {
        print_and_say("I don't know the word \"%s\".\n", word);
        return 0;
    }

    tree_node** questions = (tree_node**) calloc(tree->size, sizeof(*questions));
    int* answers = (int*) calloc(tree->size, sizeof(*answers));

    int def_length = get_definition(node, questions, answers);
    print_and_say("Definition of \"%s\" is following: %s", word, word);

    for (int i = 0; i < def_length; i++)
        print_and_say("is %s%s%s",
            answers[i] ? "" : "not ",
            questions[i]->data,
            i < def_length - 1 ? ", " : "."
            );
    
    putc('\n', stdout);
    free(questions);
    free(answers);
    free(word);
    
    return 0;
}

int compare_words(binary_tree* tree)
{
    char *word1 = NULL, *word2 = NULL;
    size_t word_len = 0;

    skip_line();
    if(getline(&word1, &word_len, stdin) == -1)
        return 0;
    word_len = 0;
    skip_line();
    if(getline(&word2, &word_len, stdin) == -1)
    {
        free(word1);
        return 0;
    }
    *strchr(word1, '\n') = '\0';
    *strchr(word2, '\n') = '\0';
    
    tree_node* node1 = find_terminal(tree, word1);
    tree_node* node2 = find_terminal(tree, word2);

    if (!node1)
    {
        print_and_say("I don't know the word \"%s\".\n", word1);
        return 0;
    }
    if (!node2)
    {
        print_and_say("I don't know the word \"%s\".\n", word2);
        return 0;
    }
    if (strcasecmp(word1, word2) == 0)
    {
        print_and_say("It's no fun comparing the word with itself. "
                        "Here's a hint: try comparing two different words.\n");
        return 0;
    }

    int first_true = 0;
    tree_node* diff = get_difference(tree, node1, node2, &first_true);

    print_and_say("The main difference between %s and %s is following:\n", word1, word2);
    print_and_say("%s is %s%s, while %s is%s.\n",
        word1, first_true ? "" : "not ", diff->data,
        word2, first_true ? " not" : "");

    free(word1);
    free(word2);
    return 0;
}

static int get_answer(void);

int guess_word(binary_tree* tree)
{
    tree_node* current = tree->root;

    if (!current)
    {
        print_and_say("Sadly, I don't know any words yet. Please, teach me my first word.\n");
        char* word = NULL;
        size_t word_len = 0;

        skip_line();
        if(getline(&word, &word_len, stdin) == -1)
            return 0;
        
        *strchr(word, '\n') = '\0';
        
        tree->root = make_node(word);
        tree->size = 1;

        free(word);

        return 0;
    }

    print_and_say("Please provide a 'yes' or 'no' answer to the following questions.\n");

    while(!is_terminal(current))
    {
        print_and_say("Is it %s?\n", current->data);

        int answer = get_answer();

        if(answer == -1)
            return 0;

        current = answer ? current->left : current->right;
    }

    print_and_say("Was your word \"%s\"?\n", current->data);

    int answer = 0;
    LOG_PRINT_TRACE(
        answer = get_answer(),
        "answer = %d",
        answer
    );

    if(answer == -1)
        return 0;

    if (answer == 1)
    {
        print_and_say("Artificial intelligence superior, pesky humans - inferior.\n");
        return 0;
    }

    print_and_say("Which word was it?\n");

    char* word = NULL;
    size_t word_len = 0;

    skip_line();
    if(getline(&word, &word_len, stdin) == -1)
        return 0;
    *strchr(word, '\n') = '\0';
    
    if(find_terminal(tree, word))
    {
        print_and_say("Word \"%s\" is already defined.\n", word);
        free(word);
        return 0;
    }
    
    print_and_say("What is the difference between %s and %s?\n",
        current->data, word);
    
    printf("Unlike %s, %s is\n", current->data, word);

    char* question = NULL;
    word_len = 0;
    if (getline(&question, &word_len, stdin) == -1)
    {
        free(word);
        return 0;
    }

    *strchr(question, '\n') = '\0';
    char* q = strchr(question, '.');
    if (q) *q = '\0';
    split_with_question(tree, current, question, word);

    free(word);
    free(question);

    return 0;
}

int show_word_list(binary_tree* tree)
{
    if (!tree->root)
    {
        print_and_say("I don't know any words yet.\n");
        return 0;
    }

    tree_node** nodes = (tree_node**) calloc(tree->size, sizeof(*nodes));

    int cnt = get_all_terminals(tree, nodes);

    print_and_say("Here are all the words I know:\n");

    for (int i = 0; i < cnt; i++)
        print_and_say("%s\n", nodes[i]->data);

    free(nodes);

    return 0;
}

int dump_tree(binary_tree* tree)
{
    print_and_say("Please, specify image file name.\n");
    
    char* word = NULL;
    size_t word_len = 0;

    skip_line();
    if(getline(&word, &word_len, stdin) == -1)
        return 0;
    *strchr(word, '\n') = '\0';

    tree_dump(tree, word);
    
    free(word);

    return 0;
}

int quit(binary_tree* tree)
{
    if (tree->root)
    {
        FILE* database = fopen(AKINATOR_DB_FILE, "w+");

        LOG_ASSERT_FATAL(database != NULL,
            "Cannot save state to database: failed to open file '%s'",
            AKINATOR_DB_FILE);

        save_tree(tree, database);
        fclose(database);

        tree_dump(tree, "on_exit.png");
    }

    tree_dtor(tree);

    print_and_say("Goodbye!\n");

    exit(0);
}

static void print_and_say(const char* format, ...)
{
    va_list args = {};

    va_start(args, format);
    vprintf(format, args);
    va_end(args);
/*
    va_start(args, format);
    FILE* voice = popen("festival --tts", "w");
    vfprintf(voice, format, args);
    fclose(voice);
    va_end(args);
    */
}

static int get_answer(void)
{
    static char BUFFER[17] = "";

    while(1)
    {

        LOG_ASSERT(scanf("%16s", BUFFER) == 1, return -1);

        if (strcasecmp("y",     BUFFER) == 0    ||
            strcasecmp("yes",   BUFFER) == 0    ||
            strcasecmp("true",  BUFFER) == 0    )
            return 1;

        if (strcasecmp("n",     BUFFER) == 0    ||
            strcasecmp("no",    BUFFER) == 0    ||
            strcasecmp("false", BUFFER) == 0    )
            return 0;

        skip_line();
        print_and_say("Please, provide a 'yes' or 'no' answer.\n");
    }

    return -1;
}

static void skip_line(void)
{
    int c = '\0';
    while((c = getchar()) != EOF && c != '\n')
        continue;
}
