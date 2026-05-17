#include "sun.h"
#include "utils.h"

#include <vector>
#include <cmath>
#include <GL/glut.h>

static V2 bezier3(const V2& p0, const V2& p1, const V2& p2, const V2& p3, float t) {
    float u = 1.0f - t;
    float b0 = u * u * u;
    float b1 = 3.0f * u * u * t;
    float b2 = 3.0f * u * t * t;
    float b3 = t * t * t;

    return V2(
        b0 * p0.x + b1 * p1.x + b2 * p2.x + b3 * p3.x,
        b0 * p0.y + b1 * p1.y + b2 * p2.y + b3 * p3.y
    );
}

static void ray_bezier(
    const V2& p0, const V2& p1, const V2& p2, const V2& p3,
    float hw_base, float hw_tip,
    float cx, float cy, float angle_rad,
    int segs
) {
    std::vector<V2> left(segs + 1);
    std::vector<V2> right(segs + 1);

    for (int i = 0; i <= segs; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(segs);

        V2 spine = bezier3(p0, p1, p2, p3, t);

        float dt = 0.01f;
        float tc = (t < 1.0f) ? t + dt : t - dt;
        float ts = (t < 1.0f) ? t : t - dt;

        V2 ahead = bezier3(p0, p1, p2, p3, tc);
        V2 back  = bezier3(p0, p1, p2, p3, ts);

        float dx = ahead.x - back.x;
        float dy = ahead.y - back.y;
        float len = std::sqrt(dx * dx + dy * dy) + 1e-6f;

        float px = -dy / len;
        float py = dx / len;

        float hw = hw_base * (1.0f - t * 0.85f) + hw_tip * t;
        if (i == segs) hw = 0.0f;

        V2 offset_L(px * hw, py * hw);
        V2 offset_R(-px * hw, -py * hw);

        V2 origin(cx, cy);

        left[i]  = origin + (spine + offset_L).rotate(angle_rad);
        right[i] = origin + (spine + offset_R).rotate(angle_rad);
    }

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segs; ++i) {
        glVertex2f(left[i].x, left[i].y);
        glVertex2f(right[i].x, right[i].y);
    }
    glEnd();
}

void draw_sun(float cx, float cy, float scale) {

    float r_start = 43.0f * scale;

    for (int i = 0; i < 32; ++i) {
        float angle = DEG(-90.0f + static_cast<float>(i) * (360.0f / 32.0f));
        bool straight = (i % 2 == 0);

        V2 p0, p1, p2, p3;
        float hw_base, hw_tip;

        if (straight) {
            float len = 50.0f * scale;

            p0 = V2(0.0f, r_start);
            p1 = V2(0.0f, r_start + len * 0.33f);
            p2 = V2(0.0f, r_start + len * 0.66f);
            p3 = V2(0.0f, r_start + len);

            hw_base = 3.8f * scale;
            hw_tip  = 0.0f;
        } else {
            float len = 49.0f * scale;
            float wave = 7.5f * scale;

            p0 = V2(0.0f, r_start);
            p1 = V2(-wave * 1.1f, r_start + len * 0.30f);
            p2 = V2(wave * 0.9f,  r_start + len * 0.65f);
            p3 = V2(-wave * 0.4f, r_start + len);

            hw_base = 3.4f * scale;
            hw_tip  = 0.2f * scale;
        }


        COL_DARK();
        ray_bezier(p0, p1, p2, p3,
                   hw_base + 1.0f * scale,
                   hw_tip + 0.2f * scale,
                   cx, cy, angle, 32);

    
        COL_GOLD();
        ray_bezier(p0, p1, p2, p3,
                   hw_base,
                   hw_tip,
                   cx, cy, angle, 32);
    }

    float r = 44.0f * scale;

    COL_DARK();
    circle_ring(cx, cy, r, 1.2f * scale, 128);

    COL_GOLD();
    circle_fill(cx, cy, r - 0.4f * scale, 128);

    float s = scale;

    glLineWidth(2.2f * s);
    COL_DARK();

    int seg = 30;

    glBegin(GL_LINE_STRIP);
    for (int j = 0; j <= seg; ++j) {
        float t = (float)j / seg;
        float u = 1.0f - t;

        float bx = u*u*(-20.5f*s) + 2*u*t*(-12.5f*s) + t*t*(-2.2f*s);
        float by = u*u*(10.0f*s) + 2*u*t*(17.5f*s) + t*t*(11.0f*s);

        glVertex2f(cx + bx, cy + by);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int j = 0; j <= seg; ++j) {
        float t = (float)j / seg;
        float u = 1.0f - t;

        float bx = u*u*(2.2f*s) + 2*u*t*(12.5f*s) + t*t*(20.5f*s);
        float by = u*u*(11.0f*s) + 2*u*t*(17.5f*s) + t*t*(10.0f*s);

        glVertex2f(cx + bx, cy + by);
    }
    glEnd();

    float lx = cx - 11.5f * s;
    float rx = cx + 11.5f * s;
    float ey = cy + 4.0f * s;

    COL_WHITE();
    circle_fill(lx, ey, 5.8f * s, 40);
    circle_fill(rx, ey, 5.8f * s, 40);

    COL_DARK();
    circle_fill(lx, ey, 2.4f * s, 24);
    circle_fill(rx, ey, 2.4f * s, 24);

    glLineWidth(2.2f * s);
    COL_DARK();

    glBegin(GL_LINE_STRIP);
    glVertex2f(cx - 2.2f*s, cy + 11.0f*s);
    glVertex2f(cx - 3.2f*s, cy - 2.0f*s);
    glVertex2f(cx - 5.5f*s, cy - 4.5f*s);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(cx + 2.2f*s, cy + 11.0f*s);
    glVertex2f(cx + 3.2f*s, cy - 2.0f*s);
    glVertex2f(cx + 5.5f*s, cy - 4.5f*s);
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int j = 0; j <= seg; ++j) {
        float t = (float)j / seg;
        float u = 1.0f - t;

        float bx = u*u*(-11.0f*s) + t*t*(11.0f*s);
        float by = u*u*(-14.5f*s) + t*t*(-14.5f*s);

        glVertex2f(cx + bx, cy + by);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int j = 0; j <= seg; ++j) {
        float t = (float)j / seg;
        float u = 1.0f - t;

        float bx = u*u*(-4.5f*s) + t*t*(4.5f*s);
        float by = u*u*(-26.0f*s) + t*t*(-26.0f*s);

        glVertex2f(cx + bx, cy + by);
    }
    glEnd();

    COL_DARK_GOLD(0.15f);

    circle_fill(cx - 20.0f*s, cy - 2.0f*s, 6.5f*s, 32);
    circle_fill(cx + 20.0f*s, cy - 2.0f*s, 6.5f*s, 32);
}
