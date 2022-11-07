#include "logger.h"
#include "tree.h"

#include "io_utils.h"

int main()
{
    add_logger({
        .name = "HTML logs",
        .stream = fopen("logs.html", "a"),
        .logging_level = LOG_ALL,
        .settings_mask = LGS_USE_HTML
    });

    main_function run_program = NULL;
    binary_tree tree = akinator_init();

    while((run_program = choose_mode()))
        LOG_ASSERT_ERROR(
            run_program(&tree) == 0, return 1,
            "Failed to execute program: invalid input", NULL
        );

    tree_dtor(&tree);
    return 1;
}