#ifndef FLAG_H
#define FLAG_H

extern float sunRotation;
extern float wavePhase;
void display();
void reshape(int w, int h);
void drawFlagBackground(float width, float height);
void drawSunWrapper(float width, float height);

#endif
