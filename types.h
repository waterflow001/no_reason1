#ifndef TYPES_H
#define TYPES_H

struct HeavyMeal
{
    int id;
    char *name;
    char *description;
    int calories;
    float cooking_time; // in minutes
    float price;
};
struct Salad
{
    int id;
    char *name;
    char *description;
    int calories;
    float price;
    float preparing_time; // in minutes
};
struct Drink
{
    int id;
    char *name;
    char *description;
    int calories;
    float price;
};

typedef struct
{
    int client_socket;
    int item_type; // 1=Heavy Meal, 2=Salad, 3=Drink
    int item_id;
} Order;

#endif