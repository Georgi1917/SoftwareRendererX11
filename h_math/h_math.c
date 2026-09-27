#include "h_math.h"

float_t convert_to_radians(float_t degrees) {
    return degrees * PI / 180.0f;
}

vec3_f calculate_normal(vec3_f a, vec3_f b, vec3_f c) {
    vec3_f normal;
    vec3_f line1, line2;

    line1.x = b.x - a.x;
    line1.y = b.y - a.y;
    line1.z = b.z - a.z;

    line2.x = c.x - a.x;
    line2.y = c.y - a.y;
    line2.z = c.z - a.z;

    normal.x = line1.y * line2.z - line1.z * line2.y;
    normal.y = line1.z * line2.x - line1.x * line2.z;
    normal.z = line1.x * line2.y - line1.y * line2.x;

    float_t len = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    normal.x /= len;
    normal.y /= len;
    normal.z /= len;

    return normal;
}

float_t calculate_dot_product(vec3_f a, vec3_f b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

vec3_f add_vectors(vec3_f a, vec3_f b) {
    vec3_f ret = {0};

    ret.x = a.x + b.x;
    ret.y = a.y + b.y;
    ret.z = a.z + b.z;

    return ret;
}

vec3_f sub_vectors(vec3_f a, vec3_f b) {
    vec3_f ret = {0};

    ret.x = a.x - b.x;
    ret.y = a.y - b.y;
    ret.z = a.z - b.z;

    return ret;
}

vec2_f project_point(vec3_f p) {
    vec2_f ret = {0};

    ret.x = p.x / p.z;
    ret.y = p.y / p.z;

    return ret;
}

vec3_f normalize(vec3_f vec) {
    float_t len = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    vec.x /= len;
    vec.y /= len;
    vec.z /= len;

    return vec;
}

mat4x4_f perspective(float_t fov_rad, float_t aspect, float_t z_near, float_t z_far) {

    mat4x4_f ret = {0};

    float_t _fov = 1.0f / tanf(fov_rad * 0.5f);

    ret.m[0][0] = aspect * _fov;
    ret.m[0][1] = 0.0f; 
    ret.m[0][2] = 0.0f; 
    ret.m[0][3] = 0.0f;

    ret.m[1][0] = 0.0f;
    ret.m[1][1] = _fov;
    ret.m[1][2] = 0.0f;
    ret.m[1][3] = 0.0f;

    ret.m[2][0] = 0.0f;
    ret.m[2][1] = 0.0f;
    ret.m[2][2] = z_far / (z_far - z_near);
    ret.m[2][3] = 1.0f;

    ret.m[3][0] = 0.0f;
    ret.m[3][1] = 0.0f;
    ret.m[3][2] = (-z_far * z_near) / (z_far - z_near);
    ret.m[3][3] = 0.0f;

    return ret;

}

vec4_f multiply_vec4_mat4(vec4_f vec, mat4x4_f mat) {
    vec4_f ret = {0};

    ret.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + vec.w * mat.m[3][0];
    ret.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + vec.w * mat.m[3][1];
    ret.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + vec.w * mat.m[3][2];
    ret.w = vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + vec.w * mat.m[3][3];

    return ret;
}

vec3_f to_cartesian_coords(vec4_f vec) {
    if (vec.w != 0.0f) {
        vec.x /= vec.w;
        vec.y /= vec.w;
        vec.z /= vec.w;
    }
    return (vec3_f) { vec.x, vec.y, vec.z };
}