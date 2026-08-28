#ifndef __SCREEN_BUFFER_H__
#define __SCREEN_BUFFER_H__

#include <stdint.h>

typedef struct {
    uint8_t* mem;
    uint16_t width;
    uint16_t height;
} screen_buffer;

screen_buffer* init_screen_buffer(uint16_t width, uint16_t height);
void free_screen_buffer(screen_buffer* buff);

#endif // __SCREEN_BUFFER_H__