#include <stdio.h>
#include <stdbool.h>

// #include "../utils/function-utils.h"
#include "../utils/list-utils.h"
// #include "../utils/network-utils.h"

#include "../lib/rule.h"
// #include "../lib/query.h"

#include "../types/command.h"

const char COMMAND_NAME_VARIABLE_NAME[MAX_COMMAND_NAME_LENGTH] = "list-rules";

int COMMAND_HANDLER_FUNCTION_NAME(Context context)
{
    printf("[COMMAND-%s]: EXECUTING\n", COMMAND_NAME_VARIABLE_NAME);

    // printf("[COMMAND-%s]:(args): %d\n", COMMAND_NAME_VARIABLE_NAME, context.args_count);

    // for (int i = 0; i < context.args_count; i++)
    // {
    //     printf("[COMMAND-%s]:(args-%d): %s\n", COMMAND_NAME_VARIABLE_NAME, i, context.args_values[i]);
    // }

    if (context.rules_list == NULL)
    {
        printf("[COMMAND-%s]: rules_list is NULL\n", COMMAND_NAME_VARIABLE_NAME);
        return 1;
    }

    Node *head = get_list_head(context.rules_list);

    while (head != NULL)
    {
        Rule *rule = (Rule *)head->data;
        print_rule(rule);
        head = get_next_node(head);
    }

    printf("[COMMAND-%s]: EXECUTED\n", COMMAND_NAME_VARIABLE_NAME);

    return 1;
}