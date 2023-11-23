#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "../utils/list-utils.h"

#define MAX_COMMAND_NAME_LENGTH 128
#define MAX_COMMAND_PATH_LENGTH 512

#define MAX_ARGS_COUNT 16
#define MAX_ARGV_LENGTH 64

#define COMMAND_NAME_VARIABLE_NAME command_name
#define COMMAND_HANDLER_FUNCTION_NAME command_handler

#define S_COMMAND_NAME_VARIABLE_NAME "command_name"
#define S_COMMAND_HANDLER_FUNCTION_NAME "command_handler"

struct Context
{
    int client_socket;
    List *rules_list;
    int args_count;
    char args_values[MAX_ARGV_LENGTH][MAX_ARGS_COUNT];
    char command_name[MAX_COMMAND_NAME_LENGTH];
};
typedef struct Context Context;

typedef int(Handler)(Context context);
// typedef int (*Handler)(Context context);

struct Command
{
    void *file;
    Handler *handler;
    char name[MAX_COMMAND_NAME_LENGTH];
    char path[MAX_COMMAND_PATH_LENGTH];
};
typedef struct Command Command;

#endif