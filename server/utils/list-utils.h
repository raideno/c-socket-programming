#ifndef __LIST_UTILS_H__
#define __LIST_UTILS_H__

#include <stdio.h>
#include <stdbool.h>

struct Node
{
    /**
     * TODO: the data also have to be freed
     */
    void *data;
    struct Node *next;
};

typedef struct Node Node;

struct List
{
    size_t size;
    Node *head;
};

typedef struct List List;

Node *get_next_node(Node *node);

List *create_list();
Node *create_node(void *data);
bool add_node_to_list(List *list, Node *node);
bool delete_node_from_list(List *list, Node *node);

bool free_list(List *list);
bool free_node(Node *node);

Node *get_previous_node_of_node_from_list(List *list, Node *node);
Node *get_list_tail(List *list);
Node *get_list_head(List *list);

int get_list_length(List *list);

#endif