#include <stdlib.h>

#include "draw.h"
#include "../struct_utils/dyn_list.h"

list_f* _linear_interpolation(uint16_t i0, uint16_t d0, uint16_t i1, uint16_t d1) {

    list_f* values = init_list();

    if (i0 == i1) {
        append_el(values, (float_t)d0);
        return values;
    }

    float_t a = (float_t)(d1 - d0) / (float_t)(i1 - i0);
    float_t d = (float_t)d0;

    for (uint16_t i = i0; i <= i1; i++) {
        append_el(values, d);
        d = d + a;
    }

    return values;

}

void _draw_line_high(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, unsigned char* buffer) {
    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    int8_t xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int32_t D = (2 * dx) - dy;

    int32_t x = x0;
    pixel_data data = {255, 0, 0};

    for (int y = y0; y <= y1; y++) {

        put_pixel(x, y, WIDTH, HEIGHT, buffer, data);

        if (D > 0) {
            x = x + xi;
            D = D + (2 * (dx - dy));
        }
        else {
            D = D + 2 * dx;
        }

    }

}

void _draw_line_low(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, unsigned char* buffer) {
    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    int8_t yi = 1;

    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }

    int32_t D = (2 * dy) - dx;

    int32_t y = y0;
    pixel_data data = {255, 0, 0};

    for (int x = x0; x <= x1; x++) {

        put_pixel(x, y, WIDTH, HEIGHT, buffer, data);

        if (D > 0) {
            y = y + yi;
            D = D + (2 * (dy - dx));
        }
        else {
            D = D + 2 * dy;
        }

    }
}

void _swap_points(point_i *a, point_i *b) {

    point_i temp = *a;
    *a = *b;
    *b = temp;

}

bool put_pixel(uint32_t x, uint32_t y, 
               uint16_t width, uint16_t height, 
               unsigned char* buffer, pixel_data data) {

    if ((x > width) || (x < 0) || (y > height) || (y < 0))
      return false;

    buffer[4 * (x + y * width) + 0] = data.b;
    buffer[4 * (x + y * width) + 1] = data.g;
    buffer[4 * (x + y * width) + 2] = data.r;
    buffer[4 * (x + y * width) + 3] = 0;

    return true;

}

void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, unsigned char* buffer) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) _draw_line_low(x1, y1, x0, y0, buffer);
        else _draw_line_low(x0, y0, x1, y1, buffer);
    }
    else {
        if (y0 > y1) _draw_line_high(x1, y1, x0, y0, buffer);
        else _draw_line_high(x0, y0, x1, y1, buffer);
    }
}

void draw_line_p(point_i p0, point_i p1, unsigned char* buffer) {
    draw_line(p0.x, p0.y, p1.x, p1.y, buffer);
}

void draw_triangle_wireframe(point_i p0, point_i p1, point_i p2, unsigned char* buffer) {

    draw_line_p(p0, p1, buffer);
    draw_line_p(p1, p2, buffer);
    draw_line_p(p0, p2, buffer);

}

void draw_triangle_fill(point_i p0, point_i p1, point_i p2, pixel_data p_data, unsigned char* buffer) {

    if (p1.y < p0.y) { _swap_points(&p1, &p0); }
    if (p2.y < p0.y) { _swap_points(&p2, &p0); }
    if (p2.y < p1.y) { _swap_points(&p2, &p1); }

    int16_t first = 0;
    int16_t final = 0;

    list_f* x01 = _linear_interpolation(p0.y, p0.x, p1.y, p1.x);
    list_f* x12 = _linear_interpolation(p1.y, p1.x, p2.y, p2.x);
    list_f* x02 = _linear_interpolation(p0.y, p0.x, p2.y, p2.x);

    list_f* x_left = init_list();
    list_f* x_right = init_list();

    pop_back(x01);

    list_f* x012 = init_list();

    for (int i = 0; i < x01->count; i++) {
        append_el(x012, x01->data[i]);
    }

    for (int i = 0; i < x12->count; i++) {
        append_el(x012, x12->data[i]);
    }

    uint16_t mid = x02->count / 2;

    if (x02->data[mid] < x012->data[mid]) {
        x_left = x02;
        x_right = x012;
    }
    else {
        x_left = x012;
        x_right = x02;
    }

    pixel_data data = {255, 0, 0};

    for (uint16_t y = p0.y; y < p2.y; y++) {
        for (uint16_t x = (uint16_t)x_left->data[y - p0.y]; x < x_right->data[y - p0.y]; x++) {
            put_pixel(x, y, WIDTH, HEIGHT, buffer, data);
        }
    }

    free_list(x01);
    free_list(x12);
    free_list(x02);
    free_list(x012);

}
