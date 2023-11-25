#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#include <dlfcn.h>

#include <dirent.h>

#include "utils/list-utils.h"
#include "utils/network-utils.h"
#include "utils/function-utils.h"

#include "lib/rule.h"
#include "lib/query.h"

#include "types/command.h"

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

#define LOGGING true

List *rules_list = NULL;
List *commands_list = NULL;

void print_command(Command *command)
{
    printf("[command](%s)\n", command->name);
}

Command *get_command_from_commands_list_by_name(List *commands_list, const char name[MAX_COMMAND_NAME_LENGTH])
{
    if (commands_list == NULL)
        return NULL;

    Node *head = commands_list->head;

    if (head == NULL)
        return NULL;

    while (head != NULL)
    {
        Command *command = (Command *)head->data;

        if (strncmp(command->name, name, MAX_COMMAND_NAME_LENGTH) == 0)
            return command;

        head = get_next_node(head);
    }

    return NULL;
}

void command_handler(int client_socket, char *input)
{
    Context context;

    context.args_count = 0;
    context.client_socket = client_socket;
    context.rules_list = rules_list;

    char input_copy[MAX_BUFFER_SIZE];

    strcpy(input_copy, input);

    char *token = strtok(input_copy, " ");

    if (token == NULL)
    {
        printf("[command_handler()]: no command provided\n");
        return;
    }

    strcpy(context.command_name, token);

    printf("[command_handler()]: %s\n", context.command_name);

    token = strtok(NULL, " ");

    int i = 0;

    printf("[command_handler()]: started processing args\n");

    while (token != NULL && i < MAX_ARGS_COUNT)
    {
        strcpy(context.args_values[i], token);
        i++;
        token = strtok(NULL, " ");
    }

    context.args_count = i;

    printf("[command_handler()]: completed args processing\n");

    for (int j = 0; j < context.args_count; j++)
    {
        printf("args[%d]: %s\n", j, context.args_values[j]);
    }

    Command *command = get_command_from_commands_list_by_name(commands_list, context.command_name);

    if (command == NULL)
    {
        printf("[command_handler()]: invalid command name(%s)\n", context.command_name);
        return;
    }

    print_command(command);

    printf("[command_handler()]: going to execute command\n");

    (command->handler)(context);

    printf("[command_handler()]: command executed\n");

    return;
}

void *process_request(void *args)
{
    int client_socket = *((int *)args);

    printf("[thread]: started-processing\n");

    char request_buffer[MAX_BUFFER_SIZE] = {0};
    char response_buffer[MAX_BUFFER_SIZE] = {0};

    memset(request_buffer, 0, sizeof(request_buffer));
    memset(response_buffer, 0, sizeof(response_buffer));

    int bytes_read = read(client_socket, request_buffer, MAX_BUFFER_SIZE);

    if (bytes_read == 0)
    {
        printf("[thread](main-loop): exit\n");
    }

    if (bytes_read < 0)
    {
        printf("[thread](main-loop): error\n");
    }

    printf("[thread](client-message): %s\n", request_buffer);

    command_handler(client_socket, request_buffer);

    // size_t remaining_space = sizeof(response_buffer) - strlen("ECHO: ");s

    // // snprintf(response_buffer, sizeof(response_buffer), "ECHO: %s", request_buffer);
    // snprintf(response_buffer, sizeof(response_buffer), "ECHO: %.*s", (int)remaining_space, request_buffer);

    // write(client_socket, response_buffer, strlen(response_buffer));

    printf("[thread](response-sent)\n");
    // } while (running);

    printf("[thread](closing-socket)\n");
    close(client_socket);

    printf("[thread]: completed-processing\n");

    return NULL;
}

Command *create_command()
{
    Command *command = NULL;

    command = malloc(1 * sizeof(Command));

    if (command == NULL)
    {
        printf("[system]: unable to allocate a command\n");
        return NULL;
    }

    command->handler = NULL;

    return command;
}

bool free_command(Command *command)
{
    if (command == NULL)
    {
        printf("[free_command()]: trying to free NULL command\n");
        return true;
    }

    if (command->file != NULL)
    {
        int command_file_close_response = dlclose(command->file);
        if (command_file_close_response != 0)
        {
            printf("[free_command()]: unable to close command file with dlclose.\n");
        }
        command->file = NULL;
        command->handler = NULL;
    }

    free(command);

    // useless
    command = NULL;

    return true;
}

bool free_commands_list(List *commands_list)
{
    if (commands_list == NULL)
        return false;

    Node *head = commands_list->head;

    while (head != NULL)
    {
        Command *command = (Command *)head->data;
        free_command(command);
        head->data = NULL;
        /*---*/
        head = head->next;
    }

    return free_list(commands_list);
}

List *load_commands_list()
{
    List *commands_list = NULL;

    if (commands_list == NULL)
    {
        printf("[load_commands_list()]: failed to create commands_list\n");
        return NULL;
    }

    DIR *commands_directory;
    struct dirent *directory_entry;

    // open the commands directory
    commands_directory = opendir("./commands");

    if (commands_directory == NULL)
    {
        perror("[load_commands_list] ERROR, unable to open the commands directory.");
        return NULL;
    }

    commands_list = create_list();

    // for (size_t i = 0; i < sizeof(commands_paths) / sizeof(commands_paths[0]); i++)
    while ((directory_entry = readdir(commands_directory)) != NULL)
    {
        if (string_ends_with(directory_entry->d_name, ".so") == false)
        {
            printf("[load_commands_list]: ignored (./commands/%s), not a shared object file.\n", directory_entry->d_name);
            continue;
        }

        Command *command = NULL;

        command = create_command();

        char command_path[512];

        snprintf(command_path, 512, "./commands/%s", directory_entry->d_name);

        if (command == NULL)
        {
            printf("[load_commands_list()]: failed to create_command() for %s\n", command_path);
            continue;
        }

        snprintf(command->path, MAX_COMMAND_PATH_LENGTH, "%s", command_path);
        // command->name;
        // snprintf(command->name, MAX_COMMAND_NAME_LENGTH, "%s", command_path);
        command->handler = NULL;
        command->file = NULL;

        command->file = dlopen(command_path, RTLD_LAZY);

        if (command->file == NULL)
        {
            printf("[load_commands_list()]: command_file for (%s) NOT FOUND\n", command_path);
            free_command(command);
            continue;
        }

// Warning: ISO C forbids conversion of object pointer to function pointer type
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
        command->handler = (Handler *)dlsym(command->file, S_COMMAND_HANDLER_FUNCTION_NAME);
#pragma GCC diagnostic pop

        const char *command_name = (const char *)dlsym(command->file, S_COMMAND_NAME_VARIABLE_NAME);

        if (command->name == NULL)
        {
            printf("[load_commands_list()]: command (%s) don't have a name\n", command_path);
            free_command(command);
            continue;
        }

        /**
         * TODO: check if there is not already a function with that name
         */

        bool is_command_name_already_used = get_command_from_commands_list_by_name(commands_list, command_name) != NULL;

        if (is_command_name_already_used == true)
        {
            /**
             * TODO: give more details about the command that is using this name.
             */
            printf("[load_commands_list()]: command name (%s) is already used by another command, aborting actual command.\n", command_path);
            free_command(command);
            continue;
        }

        snprintf(command->name, MAX_COMMAND_NAME_LENGTH, "%s", command_name);

        if (command->handler == NULL)
        {
            printf("[load_commands_list()]: handler for (%s) NOT FOUND\n", command_path);
            free_command(command);
            continue;
        }

        Node *node = create_node((void *)command);

        if (node == NULL)
        {
            printf("[load_commands_list()]: node allocation for (%s) FAILED\n", command_path);
            free_command(command);
            continue;
        }

        bool insertion_result = add_node_to_list(commands_list, node);

        if (insertion_result == false)
        {
            printf("[load_commands_list()]: command insertion for (%s) into commands_list failed\n", command_path);
            free_command(command);
            free_node(node);
            continue;
        }

        printf("[load_commands_list]: loaded command (%s).\n", command->name);

        /**
         * IMPORTANT: when you free a command you have to close the command_file
         */
        // dlclose(command->file);
    }

    closedir(commands_directory);

    return commands_list;
}

void list_commands(List *commands_list)
{
    if (commands_list == NULL)
    {
        printf("[list_commands()]: commands_list is NULL\n");
        return;
    }

    Node *head = get_list_head(commands_list);

    while (head != NULL)
    {
        Command *command = (Command *)head->data;
        print_command(command);
        head = get_next_node(head);
    }
}

int main()
{
    rules_list = create_list();
    /**
     * linux: dlopen, dlsym, dlclose
     * windows: LoadLibrary, GetProcAddress, FreeLibrary
     */

    /**
     * load-commands -> get all the files in the commands folder and store them in an array (path only)
     *
     * loop through the commands
     * * load the command
     * * extract the handler command
     * * store it in the commands array
     */

    if (rules_list == NULL)
    {
        printf("[system]: something went wrong while allocating rules_list\n");
        exit(EXIT_FAILURE);
    }

    commands_list = load_commands_list();

    if (commands_list == NULL)
    {
        printf("[system]: something went wrong while loading commands.\n");
        exit(EXIT_FAILURE);
    }

    list_commands(commands_list);

    bool running;
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    int addrlen = sizeof(server_address);

    // Create a socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[system]: socket creation failed.");
        exit(EXIT_FAILURE);
    }

    // Initialize server details
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind the socket
    int socket_binding_response = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    if (socket_binding_response == -1)
    {
        perror("[system]: Socket bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    int initiate_socket_listening_response = listen(server_socket, 3);

    if (initiate_socket_listening_response == -1)
    {
        perror("[system]: Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("[server]: %d\n", PORT);

    printf("[server]: main-loop\n");

    running = true;

    while (running)
    {
        pthread_t thread;

        client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&addrlen);

        if (client_socket == -1)
        {
            perror("[server]: socket-accept: FAILED");
            exit(EXIT_FAILURE);
        }

        printf("[server]: accepted connection.\n");

        int request_processing_response = pthread_create(&thread, NULL, process_request, (void *)&client_socket);

        if (request_processing_response != 0)
        {
            fprintf(stderr, "[server]: socket-request-failed: %s\n", strerror(errno));
            // TODO: write to socket client that the request failed
            close(client_socket);
        }
    }

    close(server_socket);

    free_commands_list(commands_list);

    return 0;
}
