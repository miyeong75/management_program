#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct user {
    int period;
    int num_people;
} user_t;

typedef struct schedule {
    int day;
    char place[40];
} schedule_t;

typedef struct node_1 {
    schedule_t s;
    struct node_1* next;
} node_s;

typedef struct info {
    char place[40];
    int admission;
} info_t;

typedef struct node_2 {
    info_t i;
    struct node_2* next;
} node_i;

