#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// #include "../utils/function-utils.h"
#include "../utils/list-utils.h"
// #include "../utils/network-utils.h"

#include "../lib/rule.h"
// #include "../lib/query.h"

#include "../types/command.h"

const char COMMAND_NAME_VARIABLE_NAME[MAX_COMMAND_NAME_LENGTH] = "server-shutdown-rule";

int COMMAND_HANDLER_FUNCTION_NAME(Context context)
{
    printf("[%s]: EXECUTED\n", context.command_name);

    printf("Edited content\n");

    return 1;
}