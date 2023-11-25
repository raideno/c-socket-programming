#include "list-utils.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

List *create_list()
{
    List *list = NULL;

    list = malloc(1 * sizeof(List));

    if (list == NULL)
    {
        /**
         * TODO: signal problem while allocating
         */
        return NULL;
    }

    list->head = NULL;
    list->size = 0;

    return list;
}

Node *create_node(void *data)
{
    Node *node = NULL;

    node = malloc(1 * sizeof(Node));

    if (node == NULL)
    {
        /**
         * TODO: signal problem while allocating
         */
        return NULL;
    }

    node->data = data;
    node->next = NULL;

    return node;
}

Node *get_next_node(Node *node)
{
    if (node == NULL)
        return NULL;

    return node->next;
}

bool add_node_to_list(List *list, Node *node)
{
    if (list == NULL || node == NULL)
        return false;

    Node *head = get_list_head(list);
    Node *tail = get_list_tail(list);

    if (tail == NULL || head == NULL)
    {
        list->size++;
        list->head = node;
        return true;
    }

    list->size++;
    tail->next = node;

    return true;
}

bool delete_node_from_list(List *list, Node *node)
{
    if (list == NULL || list->head == NULL || node == NULL)
        return false;

    if (list->head == node)
    {
        // Node *temp = node->next;
        list->head = list->head->next;
        free_node(node);
        return true;
    }

    Node *previous = get_previous_node_of_node_from_list(list, node);

    if (previous == NULL)
        return false;

    previous->next = node->next;

    free_node(node);

    return true;
}

bool free_list(List *list)
{
    if (list == NULL)
        return false;

    Node *node = get_list_head(list);

    while (node != NULL)
    {
        free_node(node);
        node = node->next;
    }

    list->head = NULL;
    list->size = 0;

    free(list);

    return true;
}

/**
 * should we free the given data here too ?
 */
bool free_node(Node *node)
{
    if (node == NULL)
        return false;

    node->next = NULL;
    node->data = NULL;

    free(node);

    return true;
}

Node *get_previous_node_of_node_from_list(List *list, Node *node)
{
    if (list == NULL || node == NULL)
        return NULL;

    Node *head = get_list_head(list);

    /**
     * head don't have a precedent
     */
    if (head == node)
        return NULL;

    while (head != NULL)
    {
        if (head->next == node)
            return node;
        head = head->next;
    }

    /**
     * given node isn't present in the given list
     */
    return NULL;
}

Node *get_list_tail(List *list)
{
    if (list == NULL)
        return NULL;

    Node *head = get_list_head(list);

    if (head == NULL)
        return NULL;

    while (head->next != NULL)
        head = head->next;

    return head;
}

Node *get_list_head(List *list)
{
    return list->head;
}

int get_list_length(List *list)
{
    int length;

    if (list == NULL)
        return 0;

    Node *head = get_list_head(list);

    length = 0;

    while (head != NULL)
    {
        length++;
        head = head->next;
    }

    return length;
}