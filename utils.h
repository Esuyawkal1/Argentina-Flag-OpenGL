#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <GL/glut.h>

constexpr float PI = 3.14159265358979323846f;
constexpr float TWO_PI = 2.0f * PI;

float DEG(float d);

void COL_BLUE(float a = 1.0f);
void COL_WHITE(float a = 1.0f);
void COL_GOLD(float a = 1.0f);
void COL_DARK_GOLD(float a = 1.0f);
void COL_DARK(float a = 1.0f);

struct V2 {
    float x = 0.0f;
    float y = 0.0f;

    V2() = default;
    V2(float nx, float ny);

    V2 operator+(const V2& b) const;
    V2 operator*(float f) const;

    V2 rotate(float angle_rad) const;
};

void circle_fill(float cx, float cy, float r, int seg);
void circle_ring(float cx, float cy, float r, float w, int seg);

#endif
