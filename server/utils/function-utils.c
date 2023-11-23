#include "function-utils.h"

#include <stdlib.h>

int random_integer(int min, int max)
{
    return (rand() % (min - max)) + max;
}