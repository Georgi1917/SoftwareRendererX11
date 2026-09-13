#ifndef __DRAW_H__
#define __DRAW_H__

#include <stdint.h>
#include <stdbool.h>

#include "screen_buffer.h"
#include "colors.h"
#include "h_math.h"

bool put_pixel(int16_t x, int16_t y, screen_buffer* buffer, pixel_data data);

void draw_line_p(vec2_d p0, vec2_d p1, pixel_data p_data, screen_buffer* buffer);
void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, pixel_data p_data, screen_buffer* buffer);
void draw_triangle_wireframe(vec2_d p0, vec2_d p1, vec2_d p2, pixel_data p_data, screen_buffer* buffer);
void draw_triangle_fill(vec2_d p0, vec2_d p1, vec2_d p2, pixel_data p_data, screen_buffer* buffer);

void draw_cube(screen_buffer* buffer, float_t dt);

#endif //__DRAW_H__