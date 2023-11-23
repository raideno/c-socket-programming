#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int client_socket;
    struct sockaddr_in server_address;

    // Create a socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[client]: socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server details
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0)
    {
        perror("[client]: Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    char message[MAX_BUFFER_SIZE];

    if (argc < 2)
    {
        perror("[client]: please provide a command in the program arguments.");
        exit(EXIT_FAILURE);
    }

    char *command = argv[1];

    size_t totalLength = 0;
    for (int i = 1; i < argc; ++i)
    {
        totalLength += strlen(argv[i]) + 1; // +1 for space between arguments
    }

    // Allocate a buffer for the concatenated string
    char *concatenatedString = (char *)malloc(totalLength);
    if (!concatenatedString)
    {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }

    // Concatenate the arguments into the buffer
    concatenatedString[0] = '\0'; // Ensure the buffer is initially empty
    for (int i = 1; i < argc; ++i)
    {
        strcat(concatenatedString, argv[i]);
        if (i < argc - 1)
        {
            strcat(concatenatedString, " "); // Add space between arguments
        }
    }

    sprintf(message, "%s", concatenatedString);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("[client]: connection-failed, server probably off.");
        exit(EXIT_FAILURE);
    }

    write(client_socket, message, strlen(message));

    char buffer[1024] = {0};
    read(client_socket, buffer, sizeof(buffer));
    printf("[server-response]: %s\n", buffer);

    close(client_socket);

    return 0;
}
