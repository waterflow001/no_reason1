#include <stdio.h>  // for printf, perror, snprintf
#include <string.h> // for memset, strlen
#include <unistd.h> // for read, close, sleep
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include "types.h"     // Include the header file for types
#include "constants.h" // Include constants for the server

void enqueue_order(int client_socket, int item_type, int item_id)
{
    pthread_mutex_lock(&order_mutex);
    if (order_count < MAX_ORDERS)
    {
        order_queue[order_rear].client_socket = client_socket;
        order_queue[order_rear].item_type = item_type;
        order_queue[order_rear].item_id = item_id;
        order_rear = (order_rear + 1) % MAX_ORDERS;
        order_count++;
        pthread_cond_signal(&order_cond); // Notify chefs that an order is available
    }
    pthread_mutex_unlock(&order_mutex);
}

void *chef_thread(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&order_mutex);
        while (order_count == 0)
        {
            pthread_cond_wait(&order_cond, &order_mutex); // Wait for orders
        }

        // Process the order
        Order order = order_queue[order_front];
        order_front = (order_front + 1) % MAX_ORDERS;
        order_count--;

        pthread_mutex_unlock(&order_mutex);

        // Simulate cooking time
        sleep(2); // Simulate time taken to prepare the order

        // Notify client that their order is ready
        const char *ready_message = "\033[32mYour order is ready!\033[0m\n";
        send(order.client_socket, ready_message, strlen(ready_message), 0);
    }
    return NULL;
}

void *handle_client(void *arg)
{
    int client_socket = *(int *)arg;
    free(arg); // free the allocated memory for client socket pointer

    char buffer[BUFFER_SIZE];

    send(client_socket, welcome_message, strlen(welcome_message), 0);
    memset(buffer, 0, BUFFER_SIZE);
    while (1)
    {
        send(client_socket, options, strlen(options), 0);
        memset(buffer, 0, BUFFER_SIZE);

        long valread = read(client_socket, buffer, BUFFER_SIZE);
        if (valread <= 0)
        {
            perror("\033[31mError: Failed to read from client!\033[0m\n");
            break; // Exit the loop if read fails
        }

        if ((atoi(buffer) == 1))
        {
            send(client_socket, food_menu, strlen(food_menu), 0);
            memset(buffer, 0, BUFFER_SIZE);
        }
        else if ((atoi(buffer) == 2))
        {
            const char *ask_type = "\033[33mOrder type? (1=Heavy Meal, 2=Salad, 3=Drink):\033[0m\n";
            send(client_socket, ask_type, strlen(ask_type), 0);
            memset(buffer, 0, BUFFER_SIZE);
            long valread = read(client_socket, buffer, BUFFER_SIZE);
            if (valread <= 0)
                break;
            int item_type = atoi(buffer);

            const char *ask_id = "\033[33mEnter item ID:\033[0m\n";
            send(client_socket, ask_id, strlen(ask_id), 0);
            memset(buffer, 0, BUFFER_SIZE);
            valread = read(client_socket, buffer, BUFFER_SIZE);
            if (valread <= 0)
                break;
            int item_id = atoi(buffer);

            enqueue_order(client_socket, item_type, item_id);
            const char *wait_msg = "\033[34mYour order is being prepared by our chefs...\033[0m\n";
            send(client_socket, wait_msg, strlen(wait_msg), 0);

            // Wait for chef to send "Your order is ready!"
            // (This will be received by the client automatically)

            // Now, wait for payment and tip from client
            const char *pay_msg = "\033[33mPlease enter payment amount:\033[0m\n";
            send(client_socket, pay_msg, strlen(pay_msg), 0);
            memset(buffer, 0, BUFFER_SIZE);
            valread = read(client_socket, buffer, BUFFER_SIZE);
            if (valread <= 0)
                break;
            float payment = atof(buffer);

            const char *tip_msg = "\033[33mPlease enter tip amount:\033[0m\n";
            send(client_socket, tip_msg, strlen(tip_msg), 0);
            memset(buffer, 0, BUFFER_SIZE);
            valread = read(client_socket, buffer, BUFFER_SIZE);
            if (valread <= 0)
                break;
            float tip = atof(buffer);

            char thanks[BUFFER_SIZE];
            snprintf(thanks, sizeof(thanks), "\033[32mThank you for your payment of %.2f and tip of %.2f! Goodbye!\033[0m\n", payment, tip);
            send(client_socket, thanks, strlen(thanks), 0);

            close(client_socket);
            sem_post(&client_sem);
            return NULL;
        }
        else if ((atoi(buffer) == 3))
        {
            const char *exit_message = "\033[32mThank you for visiting KingsPaw Restraunt!\033[0m\n";
            send(client_socket, exit_message, strlen(exit_message), 0);
            close(client_socket); // Close the client socket
            return NULL;          // Exit the thread
        }
        else
        {
            const char *invalid_input = "\033[31mInvalid input. Please enter a valid option.\033[0m\n";
            send(client_socket, invalid_input, strlen(invalid_input), 0);
            continue; // Skip to the next iteration for valid input
        }
    }
    sem_post(&client_sem); // Release slot for new clients
    return NULL;
}