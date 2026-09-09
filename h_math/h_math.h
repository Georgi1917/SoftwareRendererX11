#ifndef __H_MATH__
#define __H_MATH__

#include <math.h>

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

#endif