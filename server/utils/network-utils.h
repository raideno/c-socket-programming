#ifndef __NETWORK_UTILS_H__

#include <stdbool.h>

#define __NETWORK_UTILS_H__

#define MAX_IP_LENGTH 15
#define MAX_PORT_LENGTH 5

#define MIN_PORT_VALUE 0
#define MAX_PORT_VALUE 65535

#define LOGGING true

bool is_valid_port(int port);

int string_to_port_integer(const char input[MAX_PORT_LENGTH]);

bool is_valid_ip(const char input[MAX_IP_LENGTH]);

#endif