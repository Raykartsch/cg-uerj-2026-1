#include "PrimitivasGeometricas.hpp"
#include <GL/glut.h>
#include <cmath>

double PI = 3.14;

// Desenha a primitiva quadrado
void drawSquare() {
    glBegin(GL_POLYGON);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(-1, 1, 0);
    glEnd();
}

// Desenha o tracejado do quadrado
void drawSquareLine() {
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(-1, 1, 0);
    glEnd();
}

// Desenha a primitiva triangulo
void drawTriangle() {
    glBegin(GL_POLYGON);
      glVertex3f(-1, 0, 0);
      glVertex3f(0, 1, 0);
      glVertex3f(1, 0, 0);
    glEnd();
}

// Desenha o tracejado do triangulo
void drawTriangleLine() {
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
      glVertex3f(-1, 0, 0);
      glVertex3f(0, 1, 0);
      glVertex3f(1, 0, 0);
    glEnd();
}

// Desenha o circulo
void drawDisk(double radius) {
    int d;
    glBegin(GL_POLYGON);
    for (d = 0; d < 32; d++) {
        double angle = (2*PI/32) * d;
        glVertex3f(radius*cos(angle), radius*sin(angle), 0);
    }
    glEnd();
}

// Desenha o tracejado do circulo
void drawDiskLine(double radius) {
    int d;
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    for (d = 0; d < 32; d++) {
        double angle = (2*PI/32) * d;
        glVertex3f(radius*cos(angle), radius*sin(angle), 0);
    }
    glEnd();
}
