#include <stdio.h>

#include "logger.h"
#include "tree.h"

int main()
{
    add_default_console_logger();
    add_logger({
        .name = "HTML logs",
        .stream = fopen("logs.html", "a"),
        .logging_level = LOG_ALL,
        .settings_mask = LGS_USE_HTML
    });

    binary_tree tree = tree_ctor();
    tree.root = make_node("0");
    tree.root->left = make_node("1", tree.root);
    tree.root->right = make_node("2", tree.root);
    tree.root->right->left = make_node("3", tree.root->right);
    tree.root->right->right = make_node("4", tree.root->right);
    tree.size = 5;

    tree_dump(&tree, "dump1.png");

    log_message(MSG_TRACE, "Dumping tree: <h2>Tree built</h2><br/>"
            "<img src=\"dump1.png\" style=\"height: 50em\"/><hr/>");

    return 0;
}