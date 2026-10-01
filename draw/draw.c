#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#include "draw.h"

#define PI 3.1415926535

vec2_i _denormalize_point_coords(vec2_f p, uint16_t max_val_x, uint16_t max_val_y) {
    vec2_i ret_point = {0};

    ret_point.x = ((p.x + 1.0) / 2.0)  * max_val_x;
    ret_point.y = ((p.y - 1.0) / -2.0) * max_val_y;

    return ret_point;
}

list_f* _linear_interpolation(int16_t i0, int16_t d0, int16_t i1, int16_t d1) {

    list_f* values = init_list();

    if (i0 == i1) {
        append_el(values, (float_t)d0);
        return values;
    }

    float_t a = (float_t)(d1 - d0) / (float_t)(i1 - i0);
    float_t d = (float_t)d0;

    for (int16_t i = i0; i <= i1; i++) {
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

void draw_line_p(vec2_f p0, vec2_f p1, pixel_data p_data, screen_buffer* buffer) {

    vec2_i p0n = _denormalize_point_coords(p0, buffer->width, buffer->height);
    vec2_i p1n = _denormalize_point_coords(p1, buffer->width, buffer->height);

    draw_line(p0n.x, p0n.y, p1n.x, p1n.y, p_data, buffer);
}

void draw_triangle_wireframe(vec2_f p0, vec2_f p1, vec2_f p2, pixel_data p_data, screen_buffer* buffer) {

    draw_line_p(p0, p1, p_data, buffer);
    draw_line_p(p1, p2, p_data, buffer);
    draw_line_p(p0, p2, p_data, buffer);

}

void draw_triangle_fill(vec2_f p0, vec2_f p1, vec2_f p2, pixel_data p_data, screen_buffer* buffer) {

    vec2_i p0n = _denormalize_point_coords(p0, buffer->width, buffer->height);
    vec2_i p1n = _denormalize_point_coords(p1, buffer->width, buffer->height);
    vec2_i p2n = _denormalize_point_coords(p2, buffer->width, buffer->height);

    if (p1n.y < p0n.y) { _swap_points(&p1n, &p0n); }
    if (p2n.y < p0n.y) { _swap_points(&p2n, &p0n); }
    if (p2n.y < p1n.y) { _swap_points(&p2n, &p1n); }

    list_f* x01 = _linear_interpolation(p0n.y, p0n.x, p1n.y, p1n.x);
    list_f* x12 = _linear_interpolation(p1n.y, p1n.x, p2n.y, p2n.x);
    list_f* x02 = _linear_interpolation(p0n.y, p0n.x, p2n.y, p2n.x);

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

        for (int16_t y = p0n.y; y < p2n.y; y++) {
            for (int16_t x = (int16_t)x02->data[y - p0n.y]; x < x012->data[y - p0n.y]; x++) {
                put_pixel(x, y, buffer, p_data);
            }
        }

    }
    else {

        for (int16_t y = p0n.y; y < p2n.y; y++) {
            for (int16_t x = (int16_t)x012->data[y - p0n.y]; x < x02->data[y - p0n.y]; x++) {
                put_pixel(x, y, buffer, p_data);
            }
        }
    }

    free_list(x01);
    free_list(x12);
    free_list(x02);
    free_list(x012);

}

void clear_buffer(pixel_data data, screen_buffer* buffer) {
    for (uint16_t y = 0; y < buffer->height; y++) {
        for (uint16_t x = 0; x < buffer->width; x++) {
            put_pixel(x, y, buffer, data);
        }
    }
}

void draw_cube(screen_buffer* buffer, double_t dt, vec3_f trans, mat4x4_f proj, vec3_f camera_pos, vec3_f* look_dir, float_t f_yaw, float_t f_pitch) {
    
    static float_t angle = 0.0;
    triangle_d tri[12];

    // Front
    tri[0].p[0] = (vec3_f){-1.0,  1.0,  1.0};
    tri[0].p[1] = (vec3_f){ 1.0,  1.0,  1.0};
    tri[0].p[2] = (vec3_f){ 1.0, -1.0,  1.0};
    tri[0].color = BLUE;

    tri[1].p[0] = (vec3_f){-1.0,  1.0,  1.0};
    tri[1].p[1] = (vec3_f){ 1.0, -1.0,  1.0};
    tri[1].p[2] = (vec3_f){-1.0, -1.0,  1.0};
    tri[1].color = BLUE;

    /* Back (-Z) */
    tri[2].p[0] = (vec3_f){ 1.0,  1.0, -1.0};
    tri[2].p[1] = (vec3_f){-1.0,  1.0, -1.0};
    tri[2].p[2] = (vec3_f){-1.0, -1.0, -1.0};
    tri[2].color = RED;

    tri[3].p[0] = (vec3_f){ 1.0,  1.0, -1.0};
    tri[3].p[1] = (vec3_f){-1.0, -1.0, -1.0};
    tri[3].p[2] = (vec3_f){ 1.0, -1.0, -1.0};
    tri[3].color = RED;

    /* Top (+Y) */
    tri[4].p[0] = (vec3_f){-1.0,  1.0, -1.0};
    tri[4].p[1] = (vec3_f){ 1.0,  1.0, -1.0};
    tri[4].p[2] = (vec3_f){ 1.0,  1.0,  1.0};
    tri[4].color = PURPLE;

    tri[5].p[0] = (vec3_f){-1.0,  1.0, -1.0};
    tri[5].p[1] = (vec3_f){ 1.0,  1.0,  1.0};
    tri[5].p[2] = (vec3_f){-1.0,  1.0,  1.0};
    tri[5].color = PURPLE;

    /* Bottom (-Y) */
    tri[6].p[0] = (vec3_f){-1.0, -1.0, -1.0};
    tri[6].p[1] = (vec3_f){ 1.0, -1.0,  1.0};
    tri[6].p[2] = (vec3_f){ 1.0, -1.0, -1.0};
    tri[6].color = WHITE;

    tri[7].p[0] = (vec3_f){-1.0, -1.0, -1.0};
    tri[7].p[1] = (vec3_f){-1.0, -1.0,  1.0};
    tri[7].p[2] = (vec3_f){ 1.0, -1.0,  1.0};
    tri[7].color = WHITE;

    /* Right (+X) */
    tri[8].p[0] = (vec3_f){ 1.0,  1.0,  1.0};
    tri[8].p[1] = (vec3_f){ 1.0,  1.0, -1.0};
    tri[8].p[2] = (vec3_f){ 1.0, -1.0, -1.0};
    tri[8].color = GOLD;

    tri[9].p[0] = (vec3_f){ 1.0,  1.0,  1.0};
    tri[9].p[1] = (vec3_f){ 1.0, -1.0, -1.0};
    tri[9].p[2] = (vec3_f){ 1.0, -1.0,  1.0};
    tri[9].color = GOLD;

    /* Left (-X) */
    tri[10].p[0] = (vec3_f){-1.0,  1.0, -1.0};
    tri[10].p[1] = (vec3_f){-1.0,  1.0,  1.0};
    tri[10].p[2] = (vec3_f){-1.0, -1.0,  1.0};
    tri[10].color = GREEN;

    tri[11].p[0] = (vec3_f){-1.0,  1.0, -1.0};
    tri[11].p[1] = (vec3_f){-1.0, -1.0,  1.0};
    tri[11].p[2] = (vec3_f){-1.0, -1.0, -1.0};
    tri[11].color = GREEN;

    vec3_f up = {0.0f, 1.0f, 0.0f};
    vec3_f target_vec = {0.0f, 0.0f, 1.0f};

    mat4x4_f mat_camera_rot_y = matrix_rotation_y(f_yaw);
    mat4x4_f mat_camera_rot_x = matrix_rotation_x(f_pitch);
    mat4x4_f mat_camera = multiply_mat4_mat4(mat_camera_rot_y, mat_camera_rot_x);

    *look_dir = to_cartesian_coords(multiply_vec4_mat4
                        ((vec4_f){target_vec.x, target_vec.y, target_vec.z, 1.0f}, mat_camera));

    target_vec = add_vectors(camera_pos, *look_dir);

    mat4x4_f view_mat = look_at(camera_pos, target_vec, up);

    mat4x4_f trans_mat = matrix_translation(trans.x, trans.y, trans.z);
    mat4x4_f rot_mat_y = matrix_rotation_y(convert_to_radians(angle));
    mat4x4_f world_mat = multiply_mat4_mat4(rot_mat_y, trans_mat);

    for (int i = 0; i < 12; i++) {
        vec3_f a = tri[i].p[0];
        vec3_f b = tri[i].p[1];
        vec3_f c = tri[i].p[2];

        vec3_f trans_point_a = to_cartesian_coords(multiply_vec4_mat4
                                        ((vec4_f){a.x, a.y, a.z, 1.0f}, world_mat));

        vec3_f trans_point_b = to_cartesian_coords(multiply_vec4_mat4
                                        ((vec4_f){b.x, b.y, b.z, 1.0f}, world_mat));
                                        
        vec3_f trans_point_c = to_cartesian_coords(multiply_vec4_mat4
                                        ((vec4_f){c.x, c.y, c.z, 1.0f}, world_mat));

        vec3_f normal = calculate_normal(trans_point_a, trans_point_b, trans_point_c);

        vec3_f cam_vec = sub_vectors(trans_point_a, camera_pos);
        cam_vec = normalize(cam_vec);

        float_t dot = calculate_dot_product(cam_vec, normal);

        vec3_f view_point_a = to_cartesian_coords(multiply_vec4_mat4
                                        ((vec4_f){trans_point_a.x, trans_point_a.y, trans_point_a.z, 1.0f}, view_mat));

        vec3_f view_point_b = to_cartesian_coords(multiply_vec4_mat4
                                        ((vec4_f){trans_point_b.x, trans_point_b.y, trans_point_b.z, 1.0f}, view_mat));
                                        
        vec3_f view_point_c = to_cartesian_coords(multiply_vec4_mat4
                                        ((vec4_f){trans_point_c.x, trans_point_c.y, trans_point_c.z, 1.0f}, view_mat));

        if (dot > 0.0f) {

            vec3_f proj_point_a = to_cartesian_coords(multiply_vec4_mat4
                                            ((vec4_f){view_point_a.x, view_point_a.y, view_point_a.z, 1.0f}, proj));
            vec3_f proj_point_b = to_cartesian_coords(multiply_vec4_mat4
                                            ((vec4_f){view_point_b.x, view_point_b.y, view_point_b.z, 1.0f}, proj));
            vec3_f proj_point_c = to_cartesian_coords(multiply_vec4_mat4
                                            ((vec4_f){view_point_c.x, view_point_c.y, view_point_c.z, 1.0f}, proj));

            pixel_data new_col = {0};
            new_col.r = (float_t)tri[i].color.r * dot;
            new_col.g = (float_t)tri[i].color.g * dot;
            new_col.b = (float_t)tri[i].color.b * dot;

            draw_triangle_fill((vec2_f){proj_point_a.x, proj_point_a.y}, 
                               (vec2_f){proj_point_b.x, proj_point_b.y}, 
                               (vec2_f){proj_point_c.x, proj_point_c.y}, 
                               new_col, buffer);
        }

    }

    angle += dt * 10.0;

}