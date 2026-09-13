#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#include "draw.h"
#include "dyn_list.h"

#define PI 3.1415926535

double_t _convert_to_radians(double_t degrees) {
    return degrees * PI / 180.0;
}

vec2_i _denormalize_point_coords(vec2_d p, uint16_t max_val_x, uint16_t max_val_y) {
    vec2_i ret_point = {0};

    ret_point.x = ((p.x + 1.0) / 2.0)  * max_val_x;
    ret_point.y = ((p.y - 1.0) / -2.0) * max_val_y;

    return ret_point;
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

void _draw_line_high(int16_t x0, int16_t y0, int16_t x1, int16_t y1, pixel_data p_data, screen_buffer* buffer) {
    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    int8_t xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int32_t D = (2 * dx) - dy;

    int16_t x = x0;

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

void _draw_line_low(int16_t x0, int16_t y0, int16_t x1, int16_t y1, pixel_data p_data, screen_buffer* buffer) {
    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    int8_t yi = 1;

    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }

    int32_t D = (2 * dy) - dx;

    int16_t y = y0;

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

void _swap_points(vec2_i*a, vec2_i *b) {

    vec2_i temp = *a;
    *a = *b;
    *b = temp;

}

bool put_pixel(int16_t x, int16_t y, screen_buffer* buffer, pixel_data data) {

    if (((x > buffer->width) || (x < 0)) || ((y > buffer->height) || (y < 0))) {
        return false;
    }

    buffer->mem[4 * (x + y * buffer->width) + 0] = data.b;
    buffer->mem[4 * (x + y * buffer->width) + 1] = data.g;
    buffer->mem[4 * (x + y * buffer->width) + 2] = data.r;
    buffer->mem[4 * (x + y * buffer->width) + 3] = 0;

    return true;

}

void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, pixel_data p_data, screen_buffer* buffer) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) _draw_line_low(x1, y1, x0, y0, p_data, buffer);
        else _draw_line_low(x0, y0, x1, y1, p_data, buffer);
    }
    else {
        if (y0 > y1) _draw_line_high(x1, y1, x0, y0, p_data, buffer);
        else _draw_line_high(x0, y0, x1, y1, p_data, buffer);
    }
}

void draw_line_p(vec2_d p0, vec2_d p1, pixel_data p_data, screen_buffer* buffer) {

    vec2_i p0n = _denormalize_point_coords(p0, buffer->width, buffer->height);
    vec2_i p1n = _denormalize_point_coords(p1, buffer->width, buffer->height);

    draw_line(p0n.x, p0n.y, p1n.x, p1n.y, p_data, buffer);
}

void draw_triangle_wireframe(vec2_d p0, vec2_d p1, vec2_d p2, pixel_data p_data, screen_buffer* buffer) {

    draw_line_p(p0, p1, p_data, buffer);
    draw_line_p(p1, p2, p_data, buffer);
    draw_line_p(p0, p2, p_data, buffer);

}

void draw_triangle_fill(vec2_d p0, vec2_d p1, vec2_d p2, pixel_data p_data, screen_buffer* buffer) {

    vec2_i p0n = _denormalize_point_coords(p0, buffer->width, buffer->height);
    vec2_i p1n = _denormalize_point_coords(p1, buffer->width, buffer->height);
    vec2_i p2n = _denormalize_point_coords(p2, buffer->width, buffer->height);

    if (p1n.y < p0n.y) { _swap_points(&p1n, &p0n); }
    if (p2n.y < p0n.y) { _swap_points(&p2n, &p0n); }
    if (p2n.y < p1n.y) { _swap_points(&p2n, &p1n); }

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

vec2_d _project_point(vec3_d p) {
    vec2_d ret_point;

    ret_point.x = p.x / p.z;
    ret_point.y = p.y / p.z;

    return ret_point;

}

vec3_d _add_vectors(vec3_d v, vec3_d t) {
    vec3_d ret_point = {0};

    ret_point.x = v.x + t.x;
    ret_point.y = v.y + t.y;
    ret_point.z = v.z + t.z;

    return ret_point;

}

vec3_d _rotate_vector_y(vec3_d v, double_t angle) {

    double_t rad_angle = _convert_to_radians(angle);
    vec3_d ret_point = {0};

    // double_t rot_matrix[9] = {0};
    // rot_matrix[0] = cos(rad_angle);
    // rot_matrix[1] = 0.0;
    // rot_matrix[2] = sin(rad_angle);
    // rot_matrix[3] = 0.0;
    // rot_matrix[4] = 1.0;
    // rot_matrix[5] = 0.0;
    // rot_matrix[6] = -sin(rad_angle);
    // rot_matrix[7] = 0.0;
    // rot_matrix[8] = cos(rad_angle);

    mat3x3_d rot_matrix = {0};
    rot_matrix.m[0][0] = cos(rad_angle);
    rot_matrix.m[0][1] = 0.0;
    rot_matrix.m[0][2] = sin(rad_angle);
    rot_matrix.m[1][0] = 0.0;
    rot_matrix.m[1][1] = 1.0;
    rot_matrix.m[1][2] = 0.0;
    rot_matrix.m[2][0] = -sin(rad_angle);
    rot_matrix.m[2][1] = 0.0;
    rot_matrix.m[2][2] = cos(rad_angle);

    ret_point.x = v.x * rot_matrix.m[0][0] + v.y * rot_matrix.m[0][1] + v.z * rot_matrix.m[0][2];
    ret_point.y = v.x * rot_matrix.m[1][0] + v.y * rot_matrix.m[1][1] + v.z * rot_matrix.m[1][2];
    ret_point.z = v.x * rot_matrix.m[2][0] + v.y * rot_matrix.m[2][1] + v.z * rot_matrix.m[2][2];

    return ret_point;

}

void draw_cube(screen_buffer* buffer, float_t dt) {
    
    static double_t angle = 0.0;

    vec3_d front_p0 = {-1.0, 1.0, 1.0};
    vec3_d front_p1 = {1.0, 1.0, 1.0};
    vec3_d front_p2 = {1.0, -1.0, 1.0};
    vec3_d front_p3 = {-1.0, -1.0, 1.0};

    vec3_d back_p0 = {-1.0, 1.0, -1.0};
    vec3_d back_p1 = {1.0, 1.0, -1.0};
    vec3_d back_p2 = {1.0, -1.0, -1.0};
    vec3_d back_p3 = {-1.0, -1.0, -1.0};

    vec3_d trans = {1.5, -1.5, 5.0};

    vec2_d proj_point_front0 = _project_point(_add_vectors(_rotate_vector_y(front_p0, angle), trans));
    vec2_d proj_point_front1 = _project_point(_add_vectors(_rotate_vector_y(front_p1, angle), trans));
    vec2_d proj_point_front2 = _project_point(_add_vectors(_rotate_vector_y(front_p2, angle), trans));
    vec2_d proj_point_front3 = _project_point(_add_vectors(_rotate_vector_y(front_p3, angle), trans));

    vec2_d proj_point_back0 = _project_point(_add_vectors(_rotate_vector_y(back_p0, angle), trans));
    vec2_d proj_point_back1 = _project_point(_add_vectors(_rotate_vector_y(back_p1, angle), trans));
    vec2_d proj_point_back2 = _project_point(_add_vectors(_rotate_vector_y(back_p2, angle), trans));
    vec2_d proj_point_back3 = _project_point(_add_vectors(_rotate_vector_y(back_p3, angle), trans));

    angle += dt * 5.0;

    draw_triangle_wireframe(proj_point_back0, proj_point_back1, proj_point_back2, BLUE, buffer);
    draw_triangle_wireframe(proj_point_back3, proj_point_back0, proj_point_back2, BLUE, buffer);
    draw_triangle_wireframe(proj_point_front0, proj_point_front1, proj_point_front2, RED, buffer);
    draw_triangle_wireframe(proj_point_front3, proj_point_front0, proj_point_front2, RED, buffer);

    draw_line_p(proj_point_front0, proj_point_back0, BLUE, buffer);
    draw_line_p(proj_point_front1, proj_point_back1, BLUE, buffer);
    draw_line_p(proj_point_front2, proj_point_back2, BLUE, buffer);
    draw_line_p(proj_point_front3, proj_point_back3, BLUE, buffer);

}