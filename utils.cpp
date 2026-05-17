#include "utils.h"
#include <cmath>

float DEG(float d) {
    return d * PI / 180.0f;
}

void COL_BLUE(float a) {
    glColor4f(0.455f, 0.675f, 0.875f, a);
}

void COL_WHITE(float a) {
    glColor4f(1.0f, 1.0f, 1.0f, a);
}

void COL_GOLD(float a) {
    glColor4f(0.965f, 0.706f, 0.055f, a);
}

void COL_DARK_GOLD(float a) {
    glColor4f(0.784f, 0.525f, 0.039f, a);
}

void COL_DARK(float a) {
    glColor4f(0.522f, 0.204f, 0.039f, a);
}

V2::V2(float nx, float ny) : x(nx), y(ny) {}

V2 V2::operator+(const V2& b) const {
    return V2(x + b.x, y + b.y);
}

V2 V2::operator*(float f) const {
    return V2(x * f, y * f);
}

V2 V2::rotate(float angle_rad) const {
    float c = std::cos(angle_rad);
    float s = std::sin(angle_rad);
    return V2(x * c - y * s, x * s + y * c);
}

void circle_fill(float cx, float cy, float r, int seg) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);

    for (int i = 0; i <= seg; ++i) {
        float a = TWO_PI * i / seg;
        glVertex2f(cx + r * std::cos(a), cy + r * std::sin(a));
    }

    glEnd();
}

void circle_ring(float cx, float cy, float r, float w, int seg) {
    glBegin(GL_TRIANGLE_STRIP);

    for (int i = 0; i <= seg; ++i) {
        float a = TWO_PI * i / seg;
        float c = std::cos(a);
        float s = std::sin(a);

        glVertex2f(cx + (r - w) * c, cy + (r - w) * s);
        glVertex2f(cx + (r + w) * c, cy + (r + w) * s);
    }

    glEnd();
}
