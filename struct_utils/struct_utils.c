#include "struct_utils.h"

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

camera_t* camera_init(vec3_f pos, vec3_f look_dir) {
    camera_t* camera = malloc(sizeof(camera_t));

    if(!camera) {
        perror("Failed to init camera!\n");
        return NULL;
    }

    camera->camera_pos = pos;
    camera->look_dir = look_dir;
    camera->f_pitch = 0.0f;
    camera->f_yaw = 0.0f;
    camera->view_mat = look_at(pos, look_dir, (vec3_f){0.0f, 1.0f, 0.0f});

}