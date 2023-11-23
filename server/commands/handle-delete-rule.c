#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// #include "../utils/function-utils.h"
#include "../utils/list-utils.h"
// #include "../utils/network-utils.h"

#include "../lib/rule.h"
// #include "../lib/query.h"

#include "../types/command.h"

const char COMMAND_NAME_VARIABLE_NAME[MAX_COMMAND_NAME_LENGTH] = "delete-rule";

int COMMAND_HANDLER_FUNCTION_NAME(Context context)
{
    printf("[%s]: EXECUTED\n", command_name);

    if (context.args_count != 2)
    {
        printf("[COMMAND-%s]: requires 1 arg(s) but got %d\n", COMMAND_NAME_VARIABLE_NAME, context.args_count);
        return 1;
    }

    RuleId rule_id = atoi(context.args_values[0]);

    if (rule_id == 0)
    {
        printf("[COMMAND-%s]: provided invalid rule_id (%s)\n", COMMAND_NAME_VARIABLE_NAME, context.args_values[0]);
        return 1;
    }

    Node *rule_node = get_rule_node_from_rules_list_by_id(context.rules_list, rule_id);

    if (rule_node == NULL || rule_node->data == NULL)
    {
        printf("[COMMAND-%s]: rule with id (%s) don't exist\n", COMMAND_NAME_VARIABLE_NAME, context.args_values[0]);
        return 1;
    }

    bool rule_free_response = free_rule(rule_node->data);

    if (rule_free_response == false)
    {
        printf("[COMMAND-%s]: something went wrong while trying to free the rule.\n", COMMAND_NAME_VARIABLE_NAME);
    }

    bool node_delete_response = delete_node_from_list(context.rules_list, rule_node);

    if (node_delete_response == false)
    {
        printf("[COMMAND-%s]: something went wrong while trying to delete the rule_node.\n", COMMAND_NAME_VARIABLE_NAME);
    }

    return 1;
}