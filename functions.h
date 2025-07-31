#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void *handle_client(void *arg);

void enqueue_order(int client_socket, int item_type, int item_id);

void *chef_thread(void *arg);

#endif