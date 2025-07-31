#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <semaphore.h>
#include <pthread.h>
#include "types.h" // for Order

// Constants
#define PORT 9000
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 36
#define MAX_ORDERS 128

// Message strings
extern const char *welcome_message;
extern char *options;
extern const char *food_menu;

// Global shared data
extern Order order_queue[MAX_ORDERS];
extern int order_front;
extern int order_rear;
extern int order_count;

// Synchronization
extern sem_t client_sem;
extern pthread_mutex_t order_mutex;
extern pthread_cond_t order_cond;

#endif // CONSTANTS_H
