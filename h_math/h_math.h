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
    int16_t x, y;
} vec2_i;

typedef struct {
    int16_t x, y, z;
} vec3_i;

typedef struct {
    double_t m[2][2];
} mat2x2_d;

typedef struct {
    double_t m[3][3];
} mat3x3_d;

typedef struct {
    double_t m[4][4];
} mat4x4_d;

double_t convert_to_radians(double_t degrees);
vec3_d calculate_normal(vec3_d a, vec3_d b, vec3_d c);
double_t calculate_dot_product(vec3_d a, vec3_d b);

vec3_d normalize(vec3_d vec);
vec3_d add_vectors(vec3_d a, vec3_d b);
vec2_d project_point(vec3_d p);

#endif