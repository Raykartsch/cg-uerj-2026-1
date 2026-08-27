// Criar uma mesa com uma esfera em cima dela!
// How to execute this code in Linux
// How to execute
// 1. g++ -o task_3_table_and_sphere task_3_table_and_sphere.cpp -lglut -lGLU -lGL -lm
// 2. ./task_3_table_and_sphere



// Run these commands in terminal to run this code in Windows:
// 1. g++ task_3_table_and_sphere.cpp -o task_3_table_and_sphere -lfreeglut -lglu32 -lopengl32
// 2. .\task_3_table_and_sphere



// Salvo
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


float w = 800;
float h = 600;
// Clears the window and depth buffer and draws three solids.
//
// The solids are placed so that they either sit or float above the x-z plane;
// therefore note one of the first things that is done is to rotate the whole
// scene 20 degrees about x to turn the top of the scene toward the viewer.
// This lets the viewer see how the torus goes around the cone.


void createTable(){
     // Pes cadeira
  glPushMatrix();
    glEnable(GL_LIGHT2);
    glTranslatef(-1.4f, -1, 0.9);
    glScalef(0.1, 2, 0.5);
    glutSolidCube(1);
    glDisable(GL_LIGHT2);
  glPopMatrix();


  glPushMatrix();
    glEnable(GL_LIGHT2);
    glTranslatef(1.4f, -1, 0.9);
    glScalef(0.1, 2, 0.5);
    glutSolidCube(1);
    glDisable(GL_LIGHT2);
  glPopMatrix();


  glPushMatrix();
    glEnable(GL_LIGHT2);
    glTranslatef(-1.4f, -1, -0.9);
    glScalef(0.1, 2, 0.5);
    glutSolidCube(1);
    glDisable(GL_LIGHT2);
  glPopMatrix();


  glPushMatrix();
    glEnable(GL_LIGHT2);
    glTranslatef(1.4f, -1, -0.9);
    glScalef(0.1, 2, 0.5);
    glutSolidCube(1);
    glDisable(GL_LIGHT2);
  glPopMatrix();


    double table_thickness = 0.1;
    double table_height_pos = 4;
    double table_xpos = 0;
    double table_ypos = 0;
    double table_zpos = 0;

    // Tampo da mesa
    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(table_xpos, table_ypos, table_zpos);
        glScalef(4, 0.1, 3);
        //glRotatef(45, 1, 1, 0);
        glutSolidCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    // Esfera

    double sphere_radius = 0.5;
    glPushMatrix();
    glEnable(GL_LIGHT2);
        // k (altura da mesa) + r (raio da esfera) + h/2 (h = espessura da mesa)
        glTranslatef(0, (table_ypos + sphere_radius + (table_thickness / 2)), 0);
        glScalef(sphere_radius, sphere_radius, sphere_radius);
        glutSolidSphere(1.0, 30, 30);
        glDisable(GL_LIGHT2);
    glPopMatrix();

}


void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);

    // É o que funciona atualmente!
    gluLookAt(0.2, 0.9, 2, //eye
            0, 0.5, 0, //look
            0, 1, 0); // up



    glPushMatrix();
    // "Camera que visualiza a cena"
    gluLookAt(0.2, 0.9, 2, //eye
            0, 0.5, 0, //look
            0, 1, 0); // up
        glViewport(w/2,h/2, w/2, h/2);
        glEnable(GL_LIGHT2);
        createTable();
        glDisable(GL_LIGHT2);
    glPopMatrix();


     glPushMatrix();
    // "Camera que visualiza a cena"
    gluLookAt(1.2, 0.9, 2, //eye
            0, 0.5, 0, //look
            0, 0, -1); // up
        glViewport(0,h/2, w/2, h/2);
        glEnable(GL_LIGHT2);
        createTable();
        glDisable(GL_LIGHT2);
    glPopMatrix();


  glFlush();
}


void reshape(GLint w, GLint h) {
  glMatrixMode(GL_PROJECTION);
  GLfloat aspect = GLfloat(w) / GLfloat(h);
  glLoadIdentity();

 if (w <= h) {
    // width is smaller, so stretch out the height
    glOrtho(-2.5, 2.5, -2.5/aspect, 2.5/aspect, -10.0, 10.0);
  } else {
    // height is smaller, so stretch out the width
    glOrtho(-2.5*aspect, 2.5*aspect, -2.5, 2.5, -10.0, 10.0);
  }


  //gluPerspective (60,aspect, 0.1, 15);

}


// void reshape(GLint w, GLint h) {
//     //glViewport(0, 0, w, h);
//     GLfloat aspect = GLfloat(w) / GLfloat(h);

//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     gluPerspective(90, aspect, 0.1, 40);

// }

void init() {


  GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
  GLfloat cyan[] = { 0.0, 1.0, 1.0, 1.0 };
  GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat blue[] = { 0.0, 0.0, 1.0, 1.0 };
  GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
  GLfloat direction[] = { 0.0, -20.0, 0.0, 1.0 };
  GLfloat direction1[] = { 0.0, 00.0, 10.0, 1.0 };

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cyan);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  glMaterialf(GL_FRONT, GL_SHININESS, 60);

  glLightfv(GL_LIGHT1, GL_AMBIENT, black);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
  glLightfv(GL_LIGHT1, GL_SPECULAR, white);
  glLightfv(GL_LIGHT1, GL_POSITION, direction1);

  glLightfv(GL_LIGHT2, GL_AMBIENT, black);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, yellow);
  glLightfv(GL_LIGHT2, GL_SPECULAR, white);
  glLightfv(GL_LIGHT2, GL_POSITION, direction1);



  glEnable(GL_LIGHTING);                // so the renderer considers light
  glEnable(GL_DEPTH_TEST);              // so the renderer considers depth


}

// The usual application statup code.
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Cyan Shapes in Yellow Light");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  init();
  glutMainLoop();

}
