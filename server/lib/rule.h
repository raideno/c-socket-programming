#ifndef __RULE_H__
#define __RULE_H__

#include "../utils/list-utils.h"
#include "../utils/network-utils.h"

#include "./query.h"

typedef int RuleId;
typedef int RulePort;
typedef char RuleIp[MAX_IP_LENGTH];

struct Rule
{
    RuleId id;
    // TODO: transform this to array of 2 ips
    RulePort port;
    // TODO: transform this to array of 2 ips
    RuleIp ip;
    /*---*/
    // Query *queries;
};

typedef struct Rule Rule;

List *create_rules_list();

void sprint_rule(char *__stream, Rule *rule);

bool add_rule_to_rules_list(List *rules_list, Rule *rule);
bool delete_rule_from_rules_list(List *rules_list, Rule *rule);

Node *get_rule_node_from_rules_list_by_id(List *rules_list, RuleId rule_id);
Rule *get_rule_from_rules_list_by_id(List *rules_list, RuleId rule_id);

Node *get_rule_node_from_rules_list(List *rules_list, Rule *rule);

Rule *create_rule(RuleId id, RulePort port, RuleIp ip);

bool free_rule(Rule *rule);

void print_rule(Rule *rule);

#endif