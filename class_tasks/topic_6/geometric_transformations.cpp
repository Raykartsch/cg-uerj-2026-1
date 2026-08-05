// Made By Italo Ventura for CG course (30/07/2026)
// Run these commands in terminal to run this code:

// 1. g++ geometric_transformations.cpp -o geometric_transformations -lfreeglut -lglu32 -lopengl32
// 2. .\geometric_transformations


#include <GL/glut.h>
#include <cmath>
#include <numbers>

double PI = 3.14;

void init(void);
void display(void);

void init(void)
{
  // define a cor de background da janela
  glClearColor(1.0, 1.0, 1.0, 1.0);

  // define o sistema de visualização - tipo de projeção
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  glOrtho (-8, 8, -8, 8, -8, 8);
}

void square()
{
      glBegin(GL_POLYGON);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(-1, 1, 0);
      glEnd();
}

void triangle() {
    glBegin(GL_LINE_LOOP);
      glVertex3f(-1, 0, 0);
      glVertex3f(0, 1, 0);
      glVertex3f(1, 0, 0);
    glEnd();
}

//double PI = 3.14;

void drawDisk(double radius){
    int d;
    glBegin(GL_POLYGON);
    for (d = 0; d < 32; d++){
        double angle = (2*PI/32) * d;
        glVertex3f(radius*cos(angle), radius*sin(angle), 0);
    }
    glEnd();
}

void drawSun(){
    int i;
    glColor3f(1,1,0);
    for (i=0; i<13; i++){
        glRotatef(360/13, 0, 0, 1);
        glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(0.75f, 0);
        glEnd();
    }
    drawDisk(1);
    glColor3f(0, 0, 0);

}

void drawCart(
        int red_n,
        int green_n,
        int blue_n

    ) {

    //glPushMatrix();
    // Definindo cor da lataria do carro
    //glPopMatrix();
    glColor3f(red_n, green_n, blue_n);

    glPushMatrix();
        glTranslatef(0, 0.7f, 0);
        glScalef(3, 0.8f, 1);
        square();
    glPopMatrix();


    glPushMatrix();
        glTranslatef(-0.5f, 2.0, 0);
        glScalef(1.5, 0.8, 1);
        square();
    glPopMatrix();

    // Definindo a cor para preto
    //glPopMatrix();
    glColor3f(0, 0, 0);

    //glPushMatrix();

    // Roda esquerda
    glPushMatrix();
        glTranslatef(-1.5f, -0.1f, 0);
        glScalef(0.8f, 0.8f, 1);
        drawDisk(1);
    glPopMatrix();


    // Roda direita
    glPushMatrix();
        glTranslatef(1.5f, -0.1f, 0);
        glScalef(0.8f, 0.8f, 1);
        drawDisk(1);
    glPopMatrix();

}


void drawBuilding(
        int red_n,
        int green_n,
        int blue_n) {

    glColor3f(red_n, green_n, blue_n);


    glPushMatrix();
		glTranslatef(0, 0.7f, 0);
		glScalef(0.8f, 3.0f, 1);
		square();
	glPopMatrix();


}


void display() {

    // Limpa a janela, colocando na tela a cor definida pela função glClearColor
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();


    glPushMatrix();
        glTranslatef(-6, 6, 0);
        drawSun();
    glPopMatrix();

    // Vermelho
    glPushMatrix();
        glTranslatef(-6, -6, 1);
        glScalef(0.7, 0.7, 1);
        drawCart(1, 0, 0);
    glPopMatrix();


    glPushMatrix();
        glTranslatef(6, -4, 1);
        glScalef(0.9, 0.9, 1);
        drawCart(0, 1, 0);
    glPopMatrix();

    glPushMatrix();
        drawBuilding(0.4f, 0.4f, 0.4f);
    glPopMatrix();



    // Libera o buffer de comando de desenho para fazer o desenho acontecer o mais rápido possível.
    glFlush();
}


int main(int argc, char** argv)
{

 //Inicializa a biblioteca GLUT e negocia uma seção com o gerenciador de janelas.
 //É possível passar argumentos para a função glutInit provenientes da linha de execução, tais como informações sobre a geometria da tela
  glutInit(&argc, argv);

  //Informa à biblioteca GLUT o modo do display a ser utilizado quando a janela gráfica for criada.
  // O flag GLUT_SINGLE força o uso de uma janela com buffer simples, significando que todos os desenhos serão feitos diretamente nesta janela.
  // O flag GLUT_RGB determina que o modelo de cor utilizado será o modelo RGB.
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

  //Define o tamanho inicial da janela, 256x256 pixels, e a posição inicial do seu canto superior esquerdo na tela, (x, y)=(100, 100).
  glutInitWindowSize (800, 800);
  glutInitWindowPosition (200, 200);

  // Cria uma janela e define seu título
  glutCreateWindow ("Primeiro OpenGL");

  //Nesta função é definido o estado inicial do OpenGL. Ajustes podem ser feitos para o usuário nessa função.
  init();

  // Define display() como a função de desenho (display callback) para a janela corrente.
  // Quando GLUT determina que esta janela deve ser redesenhada, a função de desenho é chamada.
  glutDisplayFunc(display);


  //Inicia o loop de processamento de desenhos com GLUT.
  // Esta rotina deve ser chamada pelo menos uma vez em um programa que utilize a biblioteca GLUT.
  glutMainLoop();

  return 0;

}
