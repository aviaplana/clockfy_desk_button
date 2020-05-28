#ifndef PROJECT_H
#define PROJECT_H

#include <types.h>

typedef struct Project {
    char id[30];
    char name[50];
    Color color;
} Project;

#endif