#ifndef __DYN_LIST_H__
#define __DYN_LIST_H__

#include <stdint.h>
#include <math.h>

typedef struct {
    uint16_t size;
    uint16_t count;
    float_t* data;
} list_f;

list_f* init_list();
void append_el(list_f* list, float_t el);
void pop_back(list_f* list);
void free_list(list_f* list);

#endif // __DYN_LIST_H__