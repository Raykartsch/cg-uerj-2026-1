#include "Borboleta.hpp"
#include "PrimitivasGeometricas.hpp"
#include <GL/glut.h>
#include <cmath>

float butterflyPhase = 0.0f;
float butterflyPhaseSpeed = 0.12f;

void drawButterfly(float fasePropria, float asaR, float asaG, float asaB) {
    float fase = butterflyPhase + fasePropria;
    float wingFlap = 0.35f + 0.65f * (float) fabs(sin(fase * 4.0f));
    float voarX = sin(fase) * 0.6f;
    float voarY = sin(fase * 2.3f) * 0.25f;

    glPushMatrix();
        glTranslatef(voarX, voarY, 0.0f);

        glColor3f(0.15f, 0.15f, 0.15f);
        glPushMatrix();
            glScalef(0.025f, 0.11f, 1.0f);
            drawSquare();
        glPopMatrix();

        glColor3f(asaR, asaG, asaB);
        glPushMatrix();
            glTranslatef(-0.11f, 0.03f, 0.0f);
            glScalef(wingFlap, 1.0f, 1.0f);
            drawDisk(0.13f);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.11f, 0.03f, 0.0f);
            glScalef(wingFlap, 1.0f, 1.0f);
            drawDisk(0.13f);
        glPopMatrix();
    glPopMatrix();
}
