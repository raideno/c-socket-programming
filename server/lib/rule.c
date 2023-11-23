#include "rule.h"

#include "../utils/list-utils.h"
#include "../utils/network-utils.h"

#include <stdlib.h>
#include <stdio.h>

List *create_rules_list()
{
    return create_list();
}

bool add_rule_to_rules_list(List *rules_list, Rule *rule)
{
    if (rules_list == NULL || rule == NULL)
        return false;

    Node *node = create_node((void *)rule);

    if (node == NULL)
        return false;

    return add_node_to_list(rules_list, node);
}

bool delete_rule_from_rules_list(List *rules_list, Rule *rule)
{
    Node *node = get_rule_node_from_rules_list(rules_list, rule);

    if (node == NULL)
        return false;

    free_rule(rule);

    return delete_node_from_list(rules_list, node);
}

Node *get_rule_node_from_rules_list_by_id(List *rules_list, RuleId rule_id)
{
    if (rules_list == NULL)
        return NULL;

    Node *head = get_list_head(rules_list);

    if (head == NULL)
        return NULL;

    while (head != NULL)
    {
        Rule *rule = (Rule *)head->data;
        if (rule->id == rule_id)
            return head;
        head = head->next;
    }

    return NULL;
}

Rule *get_rule_from_rules_list_by_id(List *rules_list, RuleId rule_id)
{
    if (rules_list == NULL)
        return NULL;

    Node *rule = get_rule_node_from_rules_list_by_id(rules_list, rule_id);

    if (rule == NULL)
        return NULL;

    return (Rule *)rule->data;
}

Node *get_rule_node_from_rules_list(List *rules_list, Rule *rule)
{
    if (rules_list == NULL || rule == NULL)
        return NULL;

    Node *head = get_list_head(rules_list);

    if (head == NULL)
        return NULL;

    while (head != NULL)
    {
        Rule *ruleI = (Rule *)head->data;
        if (rule == ruleI)
            return head;
        head = head->next;
    }

    return NULL;
}

Rule *create_rule(RuleId id, RulePort port, RuleIp ip)
{
    Rule *rule = NULL;

    rule = malloc(1 * sizeof(Rule));

    if (rule == NULL)
        return rule;

    rule->id = id;
    rule->port = port;
    // TODO: check the return to know if the copy have been done correctly
    snprintf(rule->ip, MAX_IP_LENGTH, "%s", ip);

    return rule;
}

bool free_rule(Rule *rule)
{
    // TODO: complete and add try-catch for the free

    // TODO: free the queries

    free(rule);

    return true;
}

void print_rule(Rule *rule)
{
    printf("[rule-%d]: %s:%d\n", rule->id, rule->ip, rule->port);
}

void sprint_rule(char *__stream, Rule *rule)
{
    sprintf(__stream, "[rule-%d]: %s:%d", rule->id, rule->ip, rule->port);
}