#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#include "draw.h"
#include "dyn_list.h"

#define PI 3.1415926535

double_t _convert_to_radians(double_t degrees) {
    return degrees * PI / 180.0;
}

point_d _normalize_point_coords(point_i p, uint16_t max_val_x, uint16_t max_val_y) {
    point_d ret_point = {0};
    ret_point.x = 2.0 * ((double_t)p.x / (double_t)max_val_x) - 1.0;
    ret_point.y = 1.0 - 2.0 * ((double_t)p.y / (double_t)max_val_y);
    return ret_point;
}

point_i _denormalize_point_coords(point_d p, uint16_t max_val_x, uint16_t max_val_y) {
    point_i ret_point = {0};
    double_t aspect_ratio = (double_t)max_val_x / (double_t)max_val_y;

    p.x /= aspect_ratio;

    ret_point.x = ((p.x + 1.0) / 2.0)  * max_val_x;
    ret_point.y = ((p.y - 1.0) / -2.0) * max_val_y;

    return ret_point;
}

void test_denormalization(point_i p0, point_i p1, point_i p2) {

    double_t rot_angle = _convert_to_radians(45.0);

    printf("Points before nomralization : \n");
    printf("P0 : %d ; %d\n", p0.x, p0.y);
    printf("P1 : %d ; %d\n", p1.x, p1.y);
    printf("P2 : %d ; %d\n", p2.x, p2.y);

    point_d p0n = _normalize_point_coords(p0, 800, 600);
    point_d p1n = _normalize_point_coords(p1, 800, 600);
    point_d p2n = _normalize_point_coords(p2, 800, 600);

    point_d temp_0 = {0};
    point_d temp_1 = {0};
    point_d temp_2 = {0};

    double_t rot_mat[4] = {0};

    rot_mat[0] = cos(rot_angle);
    rot_mat[1] = -(sin(rot_angle));
    rot_mat[2] = sin(rot_angle);
    rot_mat[3] = cos(rot_angle);
    
    temp_0.x = p0n.x * rot_mat[0] + p0n.y * rot_mat[1];
    temp_0.y = p0n.x * rot_mat[2] + p0n.y * rot_mat[3];

    temp_1.x = p1n.x * rot_mat[0] + p1n.y * rot_mat[1];
    temp_1.y = p1n.x * rot_mat[2] + p1n.y * rot_mat[3];
    
    temp_2.x = p2n.x * rot_mat[0] + p2n.y * rot_mat[1];
    temp_2.y = p2n.x * rot_mat[2] + p2n.y * rot_mat[3];

    printf("Points after normalization (and rotation) : \n");
    printf("P0 : %f ; %f\n", temp_0.x, temp_0.y);
    printf("P1 : %f ; %f\n", temp_1.x, temp_1.y);
    printf("P2 : %f ; %f\n", temp_2.x, temp_2.y);

    point_i p0d = _denormalize_point_coords(temp_0, 800, 600);
    point_i p1d = _denormalize_point_coords(temp_1, 800, 600);
    point_i p2d = _denormalize_point_coords(temp_2, 800, 600);

    printf("Points after denormalization (and rotation) : \n");
    printf("P0 : %d ; %d\n", p0d.x, p0d.y);
    printf("P1 : %d ; %d\n", p1d.x, p1d.y);
    printf("P2 : %d ; %d\n", p2d.x, p2d.y);

}

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

void _draw_line_high(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, pixel_data p_data, screen_buffer* buffer) {
    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    int8_t xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int32_t D = (2 * dx) - dy;

    int32_t x = x0;

    for (int16_t y = y0; y <= y1; y++) {

        put_pixel(x, y, buffer, p_data);

        if (D > 0) {
            x = x + xi;
            D = D + (2 * (dx - dy));
        }
        else {
            D = D + 2 * dx;
        }

    }

}

void _draw_line_low(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, pixel_data p_data, screen_buffer* buffer) {
    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    int8_t yi = 1;

    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }

    int32_t D = (2 * dy) - dx;

    int32_t y = y0;

    for (int16_t x = x0; x <= x1; x++) {

        put_pixel(x, y, buffer, p_data);

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

bool put_pixel(uint32_t x, uint32_t y, screen_buffer* buffer, pixel_data data) {

    if ((x > buffer->width) || (x < 0) || (y > buffer->height) || (y < 0))
      return false;

    buffer->mem[4 * (x + y * buffer->width) + 0] = data.b;
    buffer->mem[4 * (x + y * buffer->width) + 1] = data.g;
    buffer->mem[4 * (x + y * buffer->width) + 2] = data.r;
    buffer->mem[4 * (x + y * buffer->width) + 3] = 0;

    return true;

}

void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, pixel_data p_data, screen_buffer* buffer) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) _draw_line_low(x1, y1, x0, y0, p_data, buffer);
        else _draw_line_low(x0, y0, x1, y1, p_data, buffer);
    }
    else {
        if (y0 > y1) _draw_line_high(x1, y1, x0, y0, p_data, buffer);
        else _draw_line_high(x0, y0, x1, y1, p_data, buffer);
    }
}

void draw_line_p(point_i p0, point_i p1, pixel_data p_data, screen_buffer* buffer) {
    draw_line(p0.x, p0.y, p1.x, p1.y, p_data, buffer);
}

void draw_triangle_wireframe(point_i p0, point_i p1, point_i p2, pixel_data p_data, screen_buffer* buffer) {

    draw_line_p(p0, p1, p_data, buffer);
    draw_line_p(p1, p2, p_data, buffer);
    draw_line_p(p0, p2, p_data, buffer);

}

void draw_triangle_fill(point_d p0, point_d p1, point_d p2, pixel_data p_data, screen_buffer* buffer) {

    point_i p0n = _denormalize_point_coords(p0, buffer->width, buffer->height);
    point_i p1n = _denormalize_point_coords(p1, buffer->width, buffer->height);
    point_i p2n = _denormalize_point_coords(p2, buffer->width, buffer->height);

    if (p1n.y < p0n.y) { _swap_points(&p1n, &p0n); }
    if (p2n.y < p0n.y) { _swap_points(&p2n, &p0n); }
    if (p2n.y < p1n.y) { _swap_points(&p2n, &p1n); }

    int16_t first = 0;
    int16_t final = 0;

    list_f* x01 = _linear_interpolation(p0n.y, p0n.x, p1n.y, p1n.x);
    list_f* x12 = _linear_interpolation(p1n.y, p1n.x, p2n.y, p2n.x);
    list_f* x02 = _linear_interpolation(p0n.y, p0n.x, p2n.y, p2n.x);

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

    for (uint16_t y = p0n.y; y < p2n.y; y++) {
        for (uint16_t x = (uint16_t)x_left->data[y - p0n.y]; x < x_right->data[y - p0n.y]; x++) {
            put_pixel(x, y, buffer, p_data);
        }
    }

    free_list(x01);
    free_list(x12);
    free_list(x02);
    free_list(x012);

}

void draw_triangle_transform(point_d p0, point_d p1, point_d p2, pixel_data p_data, screen_buffer* buffer) {

    double_t rot_angle = _convert_to_radians(90.0);

    double_t rot_mat[4] = {0};

    point_d temp_0 = {0};
    point_d temp_1 = {0};
    point_d temp_2 = {0};

    rot_mat[0] = cos(rot_angle);
    rot_mat[1] = -(sin(rot_angle));
    rot_mat[2] = sin(rot_angle);
    rot_mat[3] = cos(rot_angle);
    
    temp_0.x = p0.x * rot_mat[0] + p0.y * rot_mat[1];
    temp_0.y = p0.x * rot_mat[2] + p0.y * rot_mat[3];

    temp_1.x = p1.x * rot_mat[0] + p1.y * rot_mat[1];
    temp_1.y = p1.x * rot_mat[2] + p1.y * rot_mat[3];
    
    temp_2.x = p2.x * rot_mat[0] + p2.y * rot_mat[1];
    temp_2.y = p2.x * rot_mat[2] + p2.y * rot_mat[3];

    draw_triangle_fill(temp_0, temp_1, temp_2, p_data, buffer);

}