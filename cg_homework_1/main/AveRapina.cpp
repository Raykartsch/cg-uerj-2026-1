#include "AveRapina.hpp"
#include "PrimitivasGeometricas.hpp"
#include "Animacao.hpp"
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

bool aveActive = false;
float aveX = 12.0f;
float aveY = 7.0f;
float avePhase = 0.0f;
bool aveJaTirouVida = false;

const float VELOCIDADE_AVE_X = 0.18f;
int framesAteProximaAve = 25 * (1000 / 24);
float aveAlvoX = 0.0f; // Coordenada X onde será o ponto mais baixo do mergulho

void drawAve() {
    avePhase += 0.35f;
    if (avePhase > 2 * 3.1415f) avePhase -= 2 * 3.1415f;

    float wingFlap = sin(avePhase);

    glPushMatrix();
        glColor3f(0.35f, 0.20f, 0.10f); // Marrom escuro

        // Corpo
        glPushMatrix();
            glScalef(0.6f, 0.25f, 1.0f);
            drawDisk(1.0f);
        glPopMatrix();

        // Bico (virado para a esquerda)
        glColor3f(0.85f, 0.70f, 0.15f);
        glPushMatrix();
            glTranslatef(-0.6f, 0.0f, 0.0f);
            glScalef(0.2f, 0.15f, 1.0f);
            drawTriangle();
        glPopMatrix();

        // Asas batendo
        glColor3f(0.25f, 0.15f, 0.05f);
        glPushMatrix();
            glTranslatef(0.1f, 0.1f, 0.0f);
            glRotatef(wingFlap * 50.0f, 0, 0, 1);
            glScalef(0.4f, 0.8f, 1.0f);
            drawTriangle();
        glPopMatrix();
    glPopMatrix();
}

void spawnAve(float targetX) {
    aveActive = true;
    aveX = 12.0f;
    aveAlvoX = targetX; // Grava onde o coelho estava para fazer o vértice do mergulho ali
    aveJaTirouVida = false;
}

void controlarSurgimentoDaAve(float coelhoX) {
    if (aveActive) return;

    framesAteProximaAve--;
    if (framesAteProximaAve <= 0) {
        spawnAve(coelhoX);
        framesAteProximaAve = (25 + rand() % 20) * (1000 / 24); // Sorteia entre 25 e 45 segundos
    }
}

void moverAve() {
    if (!aveActive) return;

    aveX -= VELOCIDADE_AVE_X;

    // Cálculo da trajetória parabólica: Y = a(X - H)^2 + K
    // H = aveAlvoX (ponto mais baixo em X), K = 0.5f (altura de mergulho no nível do coelho)
    float distBase = 12.0f - aveAlvoX;
    float a = (7.0f - 0.5f) / (distBase * distBase + 0.0001f); // Evita divisão por zero

    float dx = (aveX - aveAlvoX);
    aveY = a * (dx * dx) + 0.5f;

    if (aveX < -12.0f) {
        aveActive = false;
    }
}
