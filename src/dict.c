#include "dict.h"

#define MAX_SIZE 64

struct dict {
    char keys[MAX_SIZE][100];
    int values[MAX_SIZE];
    int size;
};

int get_index(struct dict* dictionary, char* key){
    for (int i = 0; i < dictionary->size; i++){
        if (strcmp(dictionary->keys[i], key) == 0){
            return i;
        }
    }

    return -1;
}

void insert(struct dict* dictionary, char* key, int value){
    int index = get_index(dictionary, key);
    if (index == -1){
        strcpy(dictionary->keys[dictionary->size], key);
        dictionary->values[dictionary->size] = value;
        dictionary->size++;
    } else {
        dictionary->values[index] = value;
    }
}

int get(struct dict* dictionary, char* key){
    int index = get_index(dictionary, key);
    if (index == -1){
        return -1;
    }

    return dictionary->values[index];
}    
