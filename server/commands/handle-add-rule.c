#include <stdio.h>
#include <stdbool.h>

#include "../utils/function-utils.h"
#include "../utils/list-utils.h"
#include "../utils/network-utils.h"

#include "../lib/rule.h"
// #include "../lib/query.h"

#include "../types/command.h"

/**
 * IMPORTANT: rule_id can't be 0
 */
int generate_id_for_rule(List *rules_list_head)
{
    int resulting_id;
    int rules_list_length;

    rules_list_length = get_list_length(rules_list_head);

    do
    {
        // TODO: replace 1000 with 100*RULES_LIST_LENGTH
        resulting_id = random_integer(1, 1000 * (rules_list_length == 0 ? 1 : rules_list_length));
    } while (get_rule_from_rules_list_by_id(rules_list_head, resulting_id) != NULL);

    return resulting_id;
}

const char COMMAND_NAME_VARIABLE_NAME[MAX_COMMAND_NAME_LENGTH] = "add-rule";

int COMMAND_HANDLER_FUNCTION_NAME(Context context)
{
    printf("[%s]: EXECUTED\n", command_name);

    if (context.rules_list == NULL)
    {
        printf("[COMMAND-%s]: rules_list is NULL\n", COMMAND_NAME_VARIABLE_NAME);
        return 1;
    }

    if (context.args_count != 2)
    {
        printf("[COMMAND-%s]: requires 2 args but got %d\n", COMMAND_NAME_VARIABLE_NAME, context.args_count);
        return 1;
    }

    RuleId rule_id = generate_id_for_rule(context.rules_list);
    char *rule_ip = context.args_values[0];
    RulePort rule_port = string_to_port_integer(context.args_values[1]);

    if (!is_valid_ip(rule_ip))
    {
        printf("[COMMAND-%s]: arg-1 (%s) isn't a valid ip\n", COMMAND_NAME_VARIABLE_NAME, context.args_values[0]);
        return 1;
    }

    if (!is_valid_port(rule_port))
    {
        printf("[COMMAND-%s]: arg-2 (%s) isn't a valid port\n", COMMAND_NAME_VARIABLE_NAME, context.args_values[1]);
        return 1;
    }

    Rule *rule = create_rule(rule_id, rule_port, rule_ip);

    if (rule == NULL)
    {
        printf("[COMMAND-%s]: failed while creating rule\n", COMMAND_NAME_VARIABLE_NAME);
        return 1;
    }

    /**
     * TODO: replace the node creation & insertion with a single function: add_rule_to_rules_list(rules_list, rule)
     * Function already exist and work properly
     */

    Node *node = create_node((void *)rule);

    if (node == NULL)
    {
        printf("[COMMAND-%s]: failed while creating node.\n", COMMAND_NAME_VARIABLE_NAME);
        free_rule(rule);
        return 1;
    }

    bool rule_insertion_response = add_rule_to_rules_list(context.rules_list, rule);

    if (rule_insertion_response == false)
    {
        printf("[COMMAND-%s]: rule's node insertion failed.\n", COMMAND_NAME_VARIABLE_NAME);
        free_rule(rule);
        free_node(node);
        return 1;
    }

    return 0;
}