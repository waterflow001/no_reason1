#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000
#define BUF_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    ssize_t n;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address setup
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", SERVER_IP, SERVER_PORT);

    // Main loop: receive from server, send user input
    while (1)
    {
        // Receive from server
        n = recv(sock, buffer, BUF_SIZE - 1, 0);
        if (n <= 0)
        {
            printf("Server closed connection or error occurred.\n");
            break;
        }
        buffer[n] = '\0';
        printf("%s", buffer);

        // Get user input
        if (fgets(buffer, BUF_SIZE, stdin) == NULL)
        {
            printf("Input error.\n");
            break;
        }
        // Send to server
        send(sock, buffer, strlen(buffer), 0);
    }

    close(sock);
    return 0;
}
