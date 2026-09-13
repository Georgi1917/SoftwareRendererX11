#ifndef __H_MATH__
#define __H_MATH__

#include <math.h>
#include <stdint.h>

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

#endif