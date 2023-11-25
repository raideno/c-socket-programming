#include "function-utils.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int random_integer(int min, int max)
{
    return (rand() % (min - max)) + max;
}

bool string_ends_with(const char *str, const char *suffix)
{
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    if (str_len < suffix_len)
    {
        return false;
    }

    return strncmp(str + (str_len - suffix_len), suffix, suffix_len) == 0;
}
