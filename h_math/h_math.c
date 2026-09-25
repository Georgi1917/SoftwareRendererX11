#include "h_math.h"

double_t convert_to_radians(double_t degrees) {
    return degrees * PI / 180.0;
}

vec3_d calculate_normal(vec3_d a, vec3_d b, vec3_d c) {
    vec3_d normal;
    vec3_d line1, line2;

    line1.x = b.x - a.x;
    line1.y = b.y - a.y;
    line1.z = b.z - a.z;

    line2.x = c.x - a.x;
    line2.y = c.y - a.y;
    line2.z = c.z - a.z;

    normal.x = line1.y * line2.z - line1.z * line2.y;
    normal.y = line1.z * line2.x - line1.x * line2.z;
    normal.z = line1.x * line2.y - line1.y * line2.x;

    double_t len = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    normal.x /= len;
    normal.y /= len;
    normal.z /= len;

    return normal;
}

double_t calculate_dot_product(vec3_d a, vec3_d b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

vec3_d add_vectors(vec3_d a, vec3_d b) {
    vec3_d ret = {0};

    ret.x = a.x + b.x;
    ret.y = a.y + b.y;
    ret.z = a.z + b.z;

    return ret;
}

vec2_d project_point(vec3_d p) {
    vec2_d ret = {0};

    ret.x = p.x / p.z;
    ret.y = p.y / p.z;

    return ret;
}

vec3_d normalize(vec3_d vec) {
    double_t len = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    vec.x /= len;
    vec.y /= len;
    vec.z /= len;

    return vec;
}