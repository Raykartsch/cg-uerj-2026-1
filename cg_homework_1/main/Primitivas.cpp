#include "Primitivas.hpp"

double PI = 3.14;

void drawSquare(){
	glBegin(GL_POLYGON);
		glVertex3f(-1, -1, 0); glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);   glVertex3f(-1, 1, 0);
	glEnd();
}

void drawSquareLine(){
	glLineWidth(1.5f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-1, -1, 0); glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);   glVertex3f(-1, 1, 0);
	glEnd();
}

void drawTriangle() {
	glBegin(GL_POLYGON);
      glVertex3f(-1, 0, 0); glVertex3f(0, 1, 0); glVertex3f(1, 0, 0);
    glEnd();
}

void drawTriangleLine() {
	glLineWidth(1.5f);
	glBegin(GL_LINE_LOOP);
      glVertex3f(-1, 0, 0); glVertex3f(0, 1, 0); glVertex3f(1, 0, 0);
    glEnd();
}

void drawDisk(double radius){
    glBegin(GL_POLYGON);
    for (int d = 0; d < 32; d++){
        double angle = (2*PI/32) * d;
        glVertex3f(radius*cos(angle), radius*sin(angle), 0);
    }
    glEnd();
}

void drawDiskLine(double radius){
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    for (int d = 0; d < 32; d++){
        double angle = (2*PI/32) * d;
        glVertex3f(radius*cos(angle), radius*sin(angle), 0);
    }
    glEnd();
}
