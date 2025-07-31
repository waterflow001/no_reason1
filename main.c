#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "data.h" // Include the header file for data structures
#include <semaphore.h>
#include "types.h"     // Include the header file for types
#include "constants.h" // Include constants for the server
#include "functions.h" // Include function declarations

int main(int argc, char **argv)
{

    // create variables for socket, address, and buffer
    int server_fd;                         // Server socket file descriptor
    struct sockaddr_in address;            // Server address structure
    int address_length = sizeof(address);  // Length of the address structure
    sem_init(&client_sem, 0, MAX_CLIENTS); // Initialize semaphore for client management

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("\033[31mError: Socket creation failed!\033[0m\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the address structure
    address.sin_family = AF_INET;         // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
    address.sin_port = htons(PORT);       // Set port number

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, address_length) < 0)
    {
        perror("\033[31mError: Binding failed!\033[0m\n");
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("\033[31mError: Listening failed!\033[0m\n");
        exit(EXIT_FAILURE);
    }
    printf("\033[32mServer is listening on port %d...\033[0m\n", PORT);

    pthread_t chefs[3];
    for (int i = 0; i < 3; i++)
    {
        pthread_create(&chefs[i], NULL, chef_thread, NULL);
    }

    while (1)
    {
        sem_wait(&client_sem); // Wait if max clients reached
        int *client_socket = malloc(sizeof(int));
        if ((*client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&address_length)) < 0)
        {
            perror("\033[31mError: Accepting connection failed!\033[0m\n");
            free(client_socket);
            sem_post(&client_sem); // Release slot on accept failure
            continue;
        }

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, client_socket) != 0)
        {
            perror("\033[31mError: Thread creation failed!\033[0m\n");
            free(client_socket);
            sem_post(&client_sem); // Release slot if thread creation fails
        }
        pthread_detach(thread_id); // Automatically reclaim thread resources
    }

    return 0;
}
