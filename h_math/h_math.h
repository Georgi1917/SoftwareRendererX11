#ifndef __H_MATH__
#define __H_MATH__

#include <math.h>
#include <stdint.h>

#define PI 3.1415926535

typedef struct {
    double_t x, y;
} vec2_d;

typedef struct {
    double_t x, y, z;
} vec3_d;

typedef struct {
    float_t x, y;
} vec2_f;

typedef struct {
    float_t x, y, z;
} vec3_f;

typedef struct {
    float_t x, y, z, w;
} vec4_f;

typedef struct {
    int16_t x, y;
} vec2_i;

typedef struct {
    int16_t x, y, z;
} vec3_i;

typedef struct {
    float_t m[2][2];
} mat2x2_f;

typedef struct {
    float_t m[3][3];
} mat3x3_f;

typedef struct {
    float_t m[4][4];
} mat4x4_f;

float_t convert_to_radians(float_t degrees);
float_t calculate_dot_product(vec3_f a, vec3_f b);
vec3_f to_cartesian_coords(vec4_f vec);
vec3_f calculate_normal(vec3_f a, vec3_f b, vec3_f c);

vec3_f normalize(vec3_f vec);
vec3_f add_vectors(vec3_f a, vec3_f b);
vec3_f sub_vectors(vec3_f a, vec3_f b);
vec2_f project_point(vec3_f p);

mat4x4_f perspective(float_t fov_rad, float_t aspect, float_t z_near, float_t z_far);
vec4_f multiply_vec4_mat4(vec4_f vec, mat4x4_f mat);

#endif