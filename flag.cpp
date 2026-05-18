#include "flag.h"
#include "sun.h"
#include "utils.h"

#include <GL/glut.h>
float sunRotation = 0.0f;
void display() {
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
    glBegin(GL_QUADS);
        glVertex2f(0.0f, fH);
        glVertex2f(fW,   fH);
        glVertex2f(fW,   2.0f * stripe);
        glVertex2f(0.0f, 2.0f * stripe);
    glEnd();

    COL_WHITE();
    glBegin(GL_QUADS);
        glVertex2f(0.0f, 2.0f * stripe);
        glVertex2f(fW,   2.0f * stripe);
        glVertex2f(fW,   stripe);
        glVertex2f(0.0f, stripe);
    glEnd();

    COL_BLUE();
    glBegin(GL_QUADS);
        glVertex2f(0.0f, stripe);
        glVertex2f(fW,   stripe);
        glVertex2f(fW,   0.0f);
        glVertex2f(0.0f, 0.0f);
    glEnd();

   
    const float CORE_RADIUS     = 44.0f;
    const float RAY_START       = 43.0f;
    const float LONGEST_RAY_LEN = 50.0f;

    const float MAX_SUN_RADIUS = RAY_START + LONGEST_RAY_LEN;

    float whiteStripeHeight = stripe;
    float margin = whiteStripeHeight * 0.06f;
    float allowedRadius = (whiteStripeHeight * 0.5f) - margin;

    float scale = allowedRadius / MAX_SUN_RADIUS;
    if (scale > 1.0f) scale = 1.0f;

    float sun_cx = fW * 0.5f;
    float sun_cy = stripe * 1.5f;

    glPushMatrix();
glTranslatef(sun_cx, sun_cy, 0.0f);
glRotatef(sunRotation, 0.0f, 0.0f, 1.0f);
glScalef(scale, scale, 1.0f);
draw_sun(0.0f, 0.0f, 1.0f);
glPopMatrix();
glutSwapBuffers();
    sunRotation += 0.03f;

if (sunRotation >= 360.0f)
    sunRotation -= 360.0f;

glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);
    glutPostRedisplay();
}
