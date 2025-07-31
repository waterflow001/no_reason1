#include "constants.h"

// Message strings
const char *welcome_message = "\033[32mWelcome to the KingsPaw Restraunt!\033[0m\n";
char *options = "\033[33mPlease choose an option (1, 2, 3):\n1. View Menu\n2. Place Order\n3. Exit\n\033[0m";
const char *food_menu = "\033[34mMain Menu (1, 2, 3, 4):\n1. Heavy Meals\n3. Salads\n4. Drinks\n\033[0m";

// Global order queue
Order order_queue[MAX_ORDERS];
int order_front = 0;
int order_rear = 0;
int order_count = 0;

// Synchronization
sem_t client_sem;
pthread_mutex_t order_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t order_cond = PTHREAD_COND_INITIALIZER;
