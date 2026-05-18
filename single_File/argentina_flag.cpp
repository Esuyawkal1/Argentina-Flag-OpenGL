/*
 * Argentina Flag – OpenGL (GLUT/freeglut) - Enhanced Authentic Version
 * Matches the official intricate facial linework and precise ray curves.
 *
 * Compile:
 * You can create openGL project, copy this code and run
 */

#if defined(APPLE)
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <cmath>
#include <cstdlib>
#include <vector>

constexpr float PI = 3.14159265358979323846f;
constexpr float TWO_PI = 2.0f * PI;
static GLfloat sunRotation = 0.0f;
static GLfloat waveOffset = 0.0f;
inline float DEG(float d) {
    return d * PI / 180.0f;
}

inline void COL_BLUE(float a = 1.0f)      { glColor4f(0.455f, 0.675f, 0.875f, a); }
inline void COL_WHITE(float a = 1.0f)     { glColor4f(1.0f, 1.0f, 1.0f, a); }
inline void COL_GOLD(float a = 1.0f)      { glColor4f(0.965f, 0.706f, 0.055f, a); }
inline void COL_DARK_GOLD(float a = 1.0f) { glColor4f(0.784f, 0.525f, 0.039f, a); }
inline void COL_DARK(float a = 1.0f)      { glColor4f(0.522f, 0.204f, 0.039f, a); }

struct V2 {
    float x = 0.0f;
    float y = 0.0f;

    V2() = default;
    V2(float nx, float ny) : x(nx), y(ny) {}

    V2 operator+(const V2& b) const { return V2(x + b.x, y + b.y); }
    V2 operator*(float f) const { return V2(x * f, y * f); }

    V2 rotate(float angle_rad) const {
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        return V2(x * c - y * s, x * s + y * c);
    }
};

static void circle_fill(float cx, float cy, float r, int seg) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= seg; ++i) {
        float a = TWO_PI * static_cast<float>(i) / static_cast<float>(seg);
        glVertex2f(cx + r * std::cos(a), cy + r * std::sin(a));
    }
    glEnd();
}

static void circle_ring(float cx, float cy, float r, float w, int seg) {
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= seg; ++i) {
        float a = TWO_PI * static_cast<float>(i) / static_cast<float>(seg);
        float c = std::cos(a);
        float s = std::sin(a);
        glVertex2f(cx + (r - w) * c, cy + (r - w) * s);
        glVertex2f(cx + (r + w) * c, cy + (r + w) * s);
    }
    glEnd();
}

static V2 bezier3(const V2& p0, const V2& p1, const V2& p2, const V2& p3, float t) {
    float u = 1.0f - t;
    float b0 = u * u * u;
    float b1 = 3.0f * u * u * t;
    float b2 = 3.0f * u * t * t;
    float b3 = t * t * t;
    return V2(b0 * p0.x + b1 * p1.x + b2 * p2.x + b3 * p3.x,
              b0 * p0.y + b1 * p1.y + b2 * p2.y + b3 * p3.y);
}

static void ray_bezier(const V2& p0, const V2& p1, const V2& p2, const V2& p3,
                       float hw_base, float hw_tip,
                       float cx, float cy, float angle_rad,
                       int segs)
{
    std::vector<V2> left(segs + 1);
    std::vector<V2> right(segs + 1);

    for (int i = 0; i <= segs; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(segs);
        V2 spine = bezier3(p0, p1, p2, p3, t);

        float dt = 0.01f;
        float tc = (t < 1.0f) ? t + dt : t - dt;
        float ts = (t < 1.0f) ? t : t - dt;
        V2 ahead = bezier3(p0, p1, p2, p3, tc);
        V2 back = bezier3(p0, p1, p2, p3, ts);
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
        left[i] = origin + (spine + offset_L).rotate(angle_rad);
        right[i] = origin + (spine + offset_R).rotate(angle_rad);
    }

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segs; ++i) {
        glVertex2f(left[i].x, left[i].y);
        glVertex2f(right[i].x, right[i].y);
    }
    glEnd();
}

static void draw_sun(float cx, float cy, float scale) {
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
            hw_tip = 0.0f;
        } else {
            float len = 49.0f * scale;
            float wave = 7.5f * scale;
            p0 = V2(0.0f, r_start);
            p1 = V2(-wave * 1.1f, r_start + len * 0.30f);
            p2 = V2(wave * 0.9f,  r_start + len * 0.65f);
            p3 = V2(-wave * 0.4f, r_start + len);
            hw_base = 3.4f * scale;
            hw_tip = 0.2f * scale;
        }

        COL_DARK();
        ray_bezier(p0, p1, p2, p3, hw_base + 1.0f * scale, hw_tip + 0.2f * scale, cx, cy, angle, 32);

        COL_GOLD();
        ray_bezier(p0, p1, p2, p3, hw_base, hw_tip, cx, cy, angle, 32);
    }

    float r = 44.0f * scale;
    COL_DARK();
    circle_ring(cx, cy, r, 1.2f * scale, 128);
    COL_GOLD();
    circle_fill(cx, cy, r - 0.4f * scale, 128);

    float s = scale;

    {
        glLineWidth(2.2f * s);
        COL_DARK();
        int seg = 30;

        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= seg; ++j) {
            float t = static_cast<float>(j) / static_cast<float>(seg);
            float u = 1.0f - t;
            float bx = u * u * (-20.5f * s) + 2.0f * u * t * (-12.5f * s) + t * t * (-2.2f * s);
            float by = u * u * (10.0f * s) + 2.0f * u * t * (17.5f * s) + t * t * (11.0f * s);
            glVertex2f(cx + bx, cy + by);
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= seg; ++j) {
            float t = static_cast<float>(j) / static_cast<float>(seg);
            float u = 1.0f - t;
            float bx = u * u * (2.2f * s) + 2.0f * u * t * (12.5f * s) + t * t * (20.5f * s);
            float by = u * u * (11.0f * s) + 2.0f * u * t * (17.5f * s) + t * t * (10.0f * s);
            glVertex2f(cx + bx, cy + by);
        }
        glEnd();
    }

    {
        int seg = 40;

        float lx = cx - 11.5f * s, ley = cy + 4.0f * s;
        float rx_radius = 5.8f * s, ry_radius = 3.4f * s;

        COL_WHITE();
        circle_fill(lx, ley, rx_radius, seg);

        COL_DARK();
        circle_fill(lx, ley, 2.4f * s, 24);

        glLineWidth(2.5f * s);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= seg / 2; ++j) {
            float a = (static_cast<float>(j) / (seg / 2)) * PI;
            glVertex2f(lx + rx_radius * std::cos(a), ley + (ry_radius * 1.1f) * std::sin(a));
        }
        glEnd();

        glLineWidth(1.4f * s);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= seg / 2; ++j) {
            float a = PI + (static_cast<float>(j) / (seg / 2)) * PI;
            glVertex2f(lx + rx_radius * std::cos(a), ley + ry_radius * std::sin(a) - 0.2f * s);
        }
        glEnd();

        float rx = cx + 11.5f * s, rey = cy + 4.0f * s;

        COL_WHITE();
        circle_fill(rx, rey, rx_radius, seg);

        COL_DARK();
        circle_fill(rx, rey, 2.4f * s, 24);

        glLineWidth(2.5f * s);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= seg / 2; ++j) {
            float a = (static_cast<float>(j) / (seg / 2)) * PI;
            glVertex2f(rx + rx_radius * std::cos(a), rey + (ry_radius * 1.1f) * std::sin(a));
        }
        glEnd();

        glLineWidth(1.4f * s);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= seg / 2; ++j) {
            float a = PI + (static_cast<float>(j) / (seg / 2)) * PI;
            glVertex2f(rx + rx_radius * std::cos(a), rey + ry_radius * std::sin(a) - 0.2f * s);
        }
        glEnd();
    }

    {
        glLineWidth(2.2f * s);
        COL_DARK();

        glBegin(GL_LINE_STRIP);
        glVertex2f(cx - 2.2f * s, cy + 11.0f * s);
        glVertex2f(cx - 2.8f * s, cy + 4.0f * s);
        glVertex2f(cx - 3.2f * s, cy - 2.0f * s);
        glVertex2f(cx - 5.5f * s, cy - 4.5f * s);
        glVertex2f(cx - 4.0f * s, cy - 7.0f * s);
        glVertex2f(cx - 1.5f * s, cy - 5.5f * s);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex2f(cx + 2.2f * s, cy + 11.0f * s);
        glVertex2f(cx + 2.8f * s, cy + 4.0f * s);
        glVertex2f(cx + 3.2f * s, cy - 2.0f * s);
        glVertex2f(cx + 5.5f * s, cy - 4.5f * s);
        glVertex2f(cx + 4.0f * s, cy - 7.0f * s);
        glVertex2f(cx + 1.5f * s, cy - 5.5f * s);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex2f(cx - 2.0f * s, cy - 5.8f * s);
        glVertex2f(cx,            cy - 7.2f * s);
        glVertex2f(cx + 2.0f * s, cy - 5.8f * s);
        glEnd();
    }

    {
        int seg = 30;
        glLineWidth(2.2f * s);
        COL_DARK();

        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= seg; ++j) {
            float t = static_cast<float>(j) / static_cast<float>(seg);
            float u = 1.0f - t;
            float bx = u * u * (-11.0f * s) + 2.0f * u * t * (0.0f) + t * t * (11.0f * s);
            float by = u * u * (-14.5f * s) + 2.0f * u * t * (-16.5f * s) + t * t * (-14.5f * s);
            glVertex2f(cx + bx, cy + by);
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex2f(cx - 11.0f * s, cy - 14.5f * s);
        glVertex2f(cx - 5.5f * s,  cy - 10.5f * s);
        glVertex2f(cx,             cy - 13.0f * s);
        glVertex2f(cx + 5.5f * s,  cy - 10.5f * s);
        glVertex2f(cx + 11.0f * s, cy - 14.5f * s);
        glEnd();

        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= seg; ++j) {
            float t = static_cast<float>(j) / static_cast<float>(seg);
            float u = 1.0f - t;
            float bx = u * u * (-9.0f * s) + 2.0f * u * t * (0.0f) + t * t * (9.0f * s);
            float by = u * u * (-17.5f * s) + 2.0f * u * t * (-22.0f * s) + t * t * (-17.5f * s);
            glVertex2f(cx + bx, cy + by);
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= seg; ++j) {
            float t = static_cast<float>(j) / static_cast<float>(seg);
            float u = 1.0f - t;
            float bx = u * u * (-4.5f * s) + 2.0f * u * t * (0.0f) + t * t * (4.5f * s);
            float by = u * u * (-26.0f * s) + 2.0f * u * t * (-28.5f * s) + t * t * (-26.0f * s);
            glVertex2f(cx + bx, cy + by);
        }
        glEnd();
    }

    {
        COL_DARK_GOLD(0.15f);
        circle_fill(cx - 20.0f * s, cy - 2.0f * s, 6.5f * s, 32);
        circle_fill(cx + 20.0f * s, cy - 2.0f * s, 6.5f * s, 32);
    }
    glLineWidth(1.0f);
}

static void drawWavingStripe(float yBottom, float yTop, float width) {
    const int segments = 200;

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        float x = width * i / segments;

        float offset = 8.0f * std::sin(0.02f * x + waveOffset);

        glVertex2f(x, yBottom + offset);
        glVertex2f(x, yTop + offset);
    }
    glEnd();
}
static void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    int W = glutGet(GLUT_WINDOW_WIDTH);
    int H = glutGet(GLUT_WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, W, 0, H);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float stripe = static_cast<float>(H) / 3.0f;
    float fW = static_cast<float>(W);
    float fH = static_cast<float>(H);

    
COL_BLUE();
drawWavingStripe(2.0f * stripe, fH, fW);

COL_WHITE();
drawWavingStripe(stripe, 2.0f * stripe, fW);

COL_BLUE();
drawWavingStripe(0.0f, stripe, fW);

    const float CORE_RADIUS     = 44.0f;
    const float RAY_START       = 43.0f;
    const float LONGEST_RAY_LEN = 50.0f;

    const float MAX_SUN_RADIUS = RAY_START + LONGEST_RAY_LEN;

    float whiteStripeHeight = stripe;

    float margin = whiteStripeHeight * 0.06f;

    float allowedRadius = (whiteStripeHeight * 0.5f) - margin;

    float scale = allowedRadius / MAX_SUN_RADIUS;

    if (scale > 1.0f)
        scale = 1.0f;

    float sun_cx = fW * 0.5f;
    float sun_cy = stripe * 1.5f;

    glPushMatrix();


    glTranslatef(sun_cx, sun_cy, 0.0f);


    glRotatef(sunRotation, 0.0f, 0.0f, 1.0f);


    glScalef(scale, scale, 1.0f);

    draw_sun(0.0f, 0.0f, 1.0f);

    glPopMatrix();

    sunRotation += 0.03f;

    if (sunRotation >= 360.0f)
        sunRotation -= 360.0f;
    waveOffset += 0.05f;

    glutPostRedisplay();

    glutSwapBuffers();
}

static void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowSize(900, 600);
    glutCreateWindow("Flag of Argentina – Secured Layout Sun of May");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
