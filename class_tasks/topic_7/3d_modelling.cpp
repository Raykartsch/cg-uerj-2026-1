// How to execute
// 1. g++ -o aula_18_08 aula_18_08.cpp -lglut -lGLU -lGL -lm
// 2. ./aula_18_08


// Run these commands in terminal to run this code in Windows:

// 1. g++ animation_2d.cpp -o animation_2d -lfreeglut -lglu32 -lopengl32
// 2. .\animation_2d


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
// The solids are placed so that they either sit or float above the x-z plane; aaaaaaaaaaaaaaa
// therefore note one of the first things that is done is to rotate the whole
// scene 20 degrees about x to turn the top of the scene toward the viewer.
// This lets the viewer see how the torus goes around the cone.
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);


  glViewport(0, 0, w, h);

    gluLookAt(5, 5, 5, //eye
            0, 0, 0, //look
            0, 1, 0); // up


   // Pes cadeira
  glPushMatrix();
    glEnable(GL_LIGHT2);
    glTranslatef(-2.0f, 0, 0);
    glScalef(0.5, 2, 1);
    //glRotatef(90, 0, 1, 0);
    glutSolidCube(1);
    glDisable(GL_LIGHT2);
  glPopMatrix();


  glPushMatrix();
    glEnable(GL_LIGHT2);
    glTranslatef(2.0f, 0, 0);
    glScalef(0.5, 2, 1);
    //glRotatef(45, 1, 1, 0);
    glutSolidCube(1);
    glDisable(GL_LIGHT2);
  glPopMatrix();


  glPushMatrix();
    glEnable(GL_LIGHT2);
    glTranslatef(-2.0f, 0, -2);
    glScalef(0.5, 2, 1);
    //glRotatef(45, 1, 1, 0);
    glutSolidCube(1);
    glDisable(GL_LIGHT2);
  glPopMatrix();


  glPushMatrix();
    glEnable(GL_LIGHT2);
    glTranslatef(2.0f, 0, -2);
    glScalef(0.5, 2, 1);
    //glRotatef(45, 1, 1, 0);
    glutSolidCube(1);
    glDisable(GL_LIGHT2);
  glPopMatrix();



    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(0, 1, -1);
        glScalef(4, 0.5, 4);
        //glRotatef(45, 1, 1, 0);
        glutSolidCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

//    glPushMatrix();
//     //gluLookAt(0,0,0, 0,0,0, 0,0,-1);

//     glEnable(GL_LIGHT2);
//     glTranslatef(-2.0f, 0, -1.0f);
//     glScalef(0.5, 2, 1);
//     //glRotatef(45, 1, 1, 0);
//     glutSolidCube(1);
//     glDisable(GL_LIGHT2);
//   glPopMatrix();

//   glPushMatrix();
//   gluLookAt(5,0,0, 0,0,0, 0,1,0);
//   glViewport(w/2,0, w/2, h/2);
//   glEnable(GL_LIGHT2);
//   glutSolidTeapot(1);
//   glDisable(GL_LIGHT2);
//   glPopMatrix();

//   glPushMatrix();
//   gluLookAt(0,-5,0, 0,0,0, 0,0,1);
//   glViewport(0,0, w/2, h/2);
//   glEnable(GL_LIGHT1);
//   glutSolidTeapot(1);
//   glDisable(GL_LIGHT1);
//   glPopMatrix();



  glFlush();
}


// void reshape(GLint w, GLint h) {
//   glMatrixMode(GL_PROJECTION);
//   GLfloat aspect = GLfloat(w) / GLfloat(h);
//   glLoadIdentity();

//  if (w <= h) {
//     // width is smaller, so stretch out the height
//     glOrtho(-2.5, 2.5, -2.5/aspect, 2.5/aspect, -10.0, 10.0);
//   } else {
//     // height is smaller, so stretch out the width
//     glOrtho(-2.5*aspect, 2.5*aspect, -2.5, 2.5, -10.0, 10.0);
//   }


//   //gluPerspective (60,aspect, 0.1, 15);

// }


void reshape(GLint w, GLint h) {
    //glViewport(0, 0, w, h);
    GLfloat aspect = GLfloat(w) / GLfloat(h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, aspect, 0.1, 40);

}

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
