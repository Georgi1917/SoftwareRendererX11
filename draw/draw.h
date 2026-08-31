#ifndef __DRAW_H__
#define __DRAW_H__

#include <stdint.h>
#include <stdbool.h>

#include "screen_buffer.h"

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} pixel_data;

typedef struct {
    int16_t x;
    int16_t y;
} point_i;

typedef struct {
    float_t x;
    float_t y;
} point_f;

typedef struct {
    double_t x;
    double_t y;
} point_d;

bool put_pixel(uint32_t x, uint32_t y, screen_buffer* buffer, pixel_data data);

void draw_line_p(point_i p0, point_i p1, pixel_data p_data, screen_buffer* buffer);
void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, pixel_data p_data, screen_buffer* buffer);
void draw_triangle_wireframe(point_i p0, point_i p1, point_i p2, pixel_data p_data, screen_buffer* buffer);
void draw_triangle_fill(point_d p0, point_d p1, point_d p2, pixel_data p_data, screen_buffer* buffer);
void draw_triangle_transform(point_d p0, point_d p1, point_d p2, pixel_data p_data, screen_buffer* buffer);
void test_denormalization(point_i p0, point_i p1, point_i p2);

#endif //__DRAW_H__