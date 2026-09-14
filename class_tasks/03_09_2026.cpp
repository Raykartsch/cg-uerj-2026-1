// How to execute
// 1. g++ -o aula_09_03 aula_09_03.cpp -lglut -lGLU -lGL -lm
// 2. ./aula_09_03


// Run these commands in terminal to run this code in Windows:

// 1. g++ aula_09_03.cpp -o aula_09_03 -lfreeglut -lglu32 -lopengl32
// 2. .\aula_09_03


// Salvo
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


float w = 800;
float h = 600;


void drawChair(){

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


        // Assento da cadeira
        glPushMatrix();
            glEnable(GL_LIGHT2);
            glTranslatef(0, 1, -1);
            glScalef(4, 0.5, 4);
            //glRotatef(45, 1, 1, 0);
            glutSolidCube(1);
            glDisable(GL_LIGHT2);
        glPopMatrix();

        //Apoio da cadeira
        glPushMatrix();
            glEnable(GL_LIGHT2);
            glTranslatef(0, 3, -3);
            glRotatef(180, 1, 0, 1);
            glScalef(0.5, 4, 4);
            glutSolidCube(1);
            glDisable(GL_LIGHT2);
        glPopMatrix();
}

void drawTable(){
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


}

void drawPlate(){

    glEnable(GL_LIGHT0);

    glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glScalef(1, 1, 0.3);
        glutSolidTorus(1, 4, 30, 30);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, -0.3, 0);
        glScalef(1, 0.1, 1);
        glutSolidSphere(4, 30, 30);
    glPopMatrix();
    glDisable(GL_LIGHT0);

}


void drawCup(){
    
    glEnable(GL_LIGHT0);

    glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glScalef(1, 1.5, 5);
        glutSolidTorus(1, 4, 30, 30);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, -3.5, 0);
        glScalef(1, 0.1, 1);
        glutSolidSphere(4, 30, 30);
    glPopMatrix();
    glDisable(GL_LIGHT0);



}
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

    //Camera padrão
    gluLookAt(10, 20, -20, //eye
            0, -15, 4, //look
            0, 1, 0); // up



    /*gluLookAt(0, 10, 10, //eye
        0, 0, 0, //look
        0, 1, 0); // up
        */

    // Desenha a mesa
    glPushMatrix();
        glTranslatef(0, 8, 0);
        glScalef(6, 4, 4);
        drawTable();
    glPopMatrix();


    glPushMatrix();
        glTranslatef(-4, 2, -4);
        glScalef(1.5, 1.75, 1.5);
        drawChair();
    glPopMatrix();
   
    glPushMatrix();
        glTranslatef(4, 2, -4);
        glScalef(1.5, 1.75, 1.5);
        drawChair();
    glPopMatrix();


    glPushMatrix();
        glRotatef(180, 0, 1, 0);
        glTranslatef(4, 2, -8);
        glScalef(1.5, 1.75, 1.5);
        drawChair();
    glPopMatrix();


    glPushMatrix();
        glRotatef(180, 0, 1, 0);
        glTranslatef(-4, 2, -8);
        glScalef(1.5, 1.75, 1.5);
        drawChair();
    glPopMatrix();


    // Kit de pratos 1
    glPushMatrix();
        glTranslatef(6, 10, -5);
        //glRotatef(180, 0, 1, 0);
        glScalef(0.25, 0.25, 0.25);
        drawPlate();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(3, 10, -5);
        //glRotatef(180, 0, 1, 0);
        glScalef(0.05, 0.07, 0.05);
        drawCup();
    glPopMatrix();

 

    // Kit de pratos 2
    glPushMatrix();
        glTranslatef(-3, 10, -5);
        //glRotatef(180, 0, 1, 0);
        glScalef(0.25, 0.25, 0.25);
        drawPlate();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-6, 10, -5);
        //glRotatef(180, 0, 1, 0);
        glScalef(0.05, 0.07, 0.05);
        drawCup();
    glPopMatrix();


        // Kit de pratos 3
    glPushMatrix();
        glTranslatef(-1, 10, 0);
        //glRotatef(180, 0, 1, 0);
        glScalef(0.25, 0.25, 0.25);
        drawPlate();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-4, 10, 0);
        //glRotatef(180, 0, 1, 0);
        glScalef(0.05, 0.07, 0.05);
        drawCup();
    glPopMatrix();

    // Kit de pratos 4
    glPushMatrix();
        glTranslatef(7, 10, 0);
        //glRotatef(180, 0, 1, 0);
        glScalef(0.25, 0.25, 0.25);
        drawPlate();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(4, 10, 0);
        //glRotatef(180, 0, 1, 0);
        glScalef(0.05, 0.07, 0.05);
        drawCup();
    glPopMatrix();


    glPushMatrix();
        glTranslatef(6, 9.5, 1);
        glScalef(5.0, 0.1, 4.0);
        glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
        glTranslatef(-2, 9.5, 1);
        glScalef(5.0, 0.1, 4.0);
        glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
        glTranslatef(6, 9.5, -5);
        glScalef(5.0, 0.1, 4.0);
        glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
        glTranslatef(-2, 9.5, -5);
        glScalef(5.0, 0.1, 4.0);
        glutSolidCube(1);
    glPopMatrix();



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
  glEnable(GL_NORMALIZE);

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