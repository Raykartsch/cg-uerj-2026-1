// 1. g++ drawing_castle.cpp -o drawing_castle -lfreeglut -lglu32 -lopengl32
// 2. .\task1_q2


#include <GL/glut.h>


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

void display() {

  // Limpa a janela, colocando na tela a cor definida pela função glClearColor
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Define cor
  glColor3f(0, 0, 0);

  //Define a espessura da linha
  glLineWidth(3.0);

///////////////////////////////////////////////////////////////
  // O sol
    glLoadIdentity();
    glTranslated(4.5, 5, 0);
    glScaled(0.5, 0.5, 0);
    square();

    glRotated(45, 0, 0, 1);
    //glColor3f(1, 0, 0);
    square();


 ///////////////////////////////////////////////////////////////

 glColor3f(0, 0, 0); // Define a cor para preto para o restante do desenho

  // Base Castelo
    glLoadIdentity();
    glTranslated(0, 0.5, 0);
    glScaled(2, 1.5, 0);
    square();

  // Porta
    glLoadIdentity();
    glTranslated(0, -0.28, 0);
    glScaled(0.5, 0.7, 0);
    square();

///////////////////////////////////////////////////////////////

  // Torre Esquerda
    glLoadIdentity();
    glTranslated(-2.5, 1, 0);
    glScaled(0.5, 2.0, 0);
    square();

  // Telhado Torre Esquerda
    glLoadIdentity();
    glTranslated(-2.5, 3, 0);
    glScaled(0.5, 1, 0);
    triangle();


   // Janela Torre Esquerda
	glLoadIdentity();
	glTranslated(-2.5, 2, 0);
	glScaled(0.15, 0.3, 0);
	square();

///////////////////////////////////////////////////////////////

  // Torre Direita
    glLoadIdentity();
    glTranslated(2.5, 1, 0);
    glScaled(0.5, 2.0, 0);
    square();


  // Telhado Torre Direita
    glLoadIdentity();
    glTranslated(2.5, 3, 0);
    glScaled(0.5, 1, 0);
    triangle();


   // Janela Torre Direita
   	glLoadIdentity();
   	glTranslated(2.5, 2, 0);
   	glScaled(0.15, 0.3, 0);
   	square();


 ///////////////////////////////////////////////////////////////

  // Torre do meio
    glLoadIdentity();
    glTranslated(0, 3, 0);
    glScaled(0.5, 1, 0);
    square();


  // Telhado Torre Meio
    glLoadIdentity();
    glTranslated(0, 4, 0);
    glScaled(0.5, 1, 0);
    triangle();


   // Janela Torre Direita
	glLoadIdentity();
	glTranslated(0, 3, 0);
	glScaled(0.15, 0.3, 0);
	square();













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
