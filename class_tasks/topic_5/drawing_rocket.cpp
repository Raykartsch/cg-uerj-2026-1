// Made By Italo Ventura for CG course (30/07/2026)
// Run these commands in terminal to run this code:
// 1. g++ drawing_rocket.cpp -o drawing_rocket -lfreeglut -lglu32 -lopengl32
// 2. .\drawing_rocket

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
      glBegin(GL_LINE_LOOP);
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

void Disk(double radius){
    int d;
    glBegin(GL_LINE_LOOP);
    for (d = 0; d < 32; d++){
        double angle = (2*PI/32) * d;
        glVertex3f(radius*cos(angle), radius*sin(angle), 0);
    }
    glEnd();
}

void display() {

  // Limpa a janela, colocando na tela a cor definida pela função glClearColor
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();

    glColor3f(0, 0, 0);
    glLineWidth(1.5);
    // Base foguete
        glPushMatrix();
        glScaled(2.0, 4.0, 0);
        square();
        glPopMatrix();

    // Detalhes foguete
        glScaled(0.5, 0.5, 0); // Definindo tamanho do detalhe

        glPushMatrix(); // Salvando estado da matriz
            glTranslated(-2, -6, 0); // Definindo a pos do primeiro quadrado
            square();
        glPopMatrix(); // Limpando a transformacao definida para o quadrado

        glPushMatrix(); // Salvando estado da matriz
            glTranslated(2, -6, 0);
            square();
        glPopMatrix(); // Limpando a transformacao definida para o quadrado
        //glScaled(2.0, 4.0, 0);

        glPushMatrix(); // Salvando estado da matriz
            glTranslated(0, 1, 0);
            Disk(1.5);
        glPopMatrix(); // Limpando a transformacao definida para o quadrado

        glPushMatrix(); // Salvando estado da matriz
            glTranslated(0, 5, 0);
            Disk(1.5);
        glPopMatrix(); // Limpando a transformacao definida para o quadrado

    // Propulsores
        glScaled(0.5, 2, 0);

        // Propulsor esquerdo
            glPushMatrix(); // Salvando estado da matriz
                glTranslated(-6, -5.5, 0);
                square();
            glPopMatrix(); // Limpando a transformacao definida para o quadrado

        // Propulsor meio
            glPushMatrix(); // Salvando estado da matriz
                glTranslated(-0, -5.5, 0);
                square();
            glPopMatrix(); // Limpando a transformacao definida para o quadrado

        // Propulsor direita
            glPushMatrix(); // Salvando estado da matriz
                glTranslated(6, -5.5, 0);
                square();
            glPopMatrix(); // Limpando a transformacao definida para o quadrado

    // Ponta
        glPushMatrix(); // Salvando estado da matriz
            glTranslated(0.0, 4.0, 0.0);
            glScaled(8.0, 4.0, 0);
            triangle();
        glPopMatrix(); // Limpando a transformacao definida para o quadrado



    // Asas laterais
        // Asa esquerda
            glPushMatrix(); // Salvando estado da matriz
                glTranslated(-8.0, -4.0, 0.0);
                glScaled(4.0, 4.0, 0);
                glBegin(GL_LINE_LOOP);
                    glVertex3f(-1.5, 0, 0);
                    glVertex3f(0, 1, 0);
                    glVertex3f(0, 0, 0);
                glEnd();
            glPopMatrix(); // Limpando a transformacao definida para o quadrado

        // Asa direita
            glPushMatrix(); // Salvando estado da matriz
                glTranslated(8.0, -4.0, 0.0);
                glScaled(4.0, 4.0, 0);
                glBegin(GL_LINE_LOOP);
                    glVertex3f(0, 0, 0);
                    glVertex3f(0, 1, 0);
                    glVertex3f(1.5, 0, 0);
                glEnd();
            glPopMatrix(); // Limpando a transformacao definida para o quadrado

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
