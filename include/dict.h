#pragma once
#include <stdlib.h>
#include <string.h>

typedef struct dict dict;

int get_index(struct dict* dictionary, char* key);
void insert(struct dict* dictionary, char* key, int value);
int get(struct dict* dictionary, char* key);
