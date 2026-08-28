#include "screen_buffer.h"

#include <stdio.h>
#include <malloc.h>

screen_buffer* init_screen_buffer(uint16_t width, uint16_t height) {
    screen_buffer* buff = calloc(1, sizeof(screen_buffer));

    if (!buff) {
        perror("Error while allocating screen buffer.\n");
        return NULL;
    }

    buff->width = width;
    buff->height = height;
    buff->mem = calloc((buff->width * buff->height * 4), sizeof(uint8_t));

    if (!buff->mem) {
        perror("Error while allocating internal buffer.\n");
        free(buff);
        return NULL;
    }

    return buff;

}

void free_screen_buffer(screen_buffer* buff) {

    if (!buff) {
        perror("Error while freeing screen buffer.\n");
        return;
    }

    if (!buff->mem) {
        perror("Error while freeing internal screen buffer.\n");
        free(buff);
        return;
    }

    free(buff->mem);
    free(buff);
}