#include "dyn_list.h"
#include <malloc.h>

list_f* init_list() {
    list_f* list = malloc(sizeof(list_f));

    list->size = 5;
    list->count = 0;
    list->data = calloc(list->size, sizeof(float_t));

    return list;

}

void append_el(list_f* list, float_t el) {
    if (list->size <= list->count) {
        list->size *= 2;
        float_t* temp = realloc(list->data, sizeof(float_t) * list->size);

        if (!temp) {
            printf("Error while reallocating memory!\n");
            return;
        }

        list->data = temp;
    }

    list->data[list->count++] = el;

}

void pop_back(list_f* list) {
    if (list->count >= 0) {
        list->count--;
    }
    else {
        printf("No list lmao\n");
    }
}

void free_list(list_f* list) {
    free(list->data);
    free(list);
}
