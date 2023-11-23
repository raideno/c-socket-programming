#include "network-utils.h"

#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>

bool is_valid_port(int port)
{
    return port >= MIN_PORT_VALUE && port <= MAX_PORT_VALUE;
}

int string_to_port_integer(const char input[MAX_PORT_LENGTH])
{
    int port = atoi(input);

    if (input[0] != '0' && port == 0)
        return -1;

    if (!is_valid_port(port))
        return -1;

    return port;
}

bool is_valid_ip(const char input[MAX_IP_LENGTH])
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, input, &(sa.sin_addr));
    return result != 0;
}
