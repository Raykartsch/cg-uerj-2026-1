/*
Tarefa 2: Pontos com o Mouse

• Criar uma tela branca.

• Detectar cliques com o botão esquerdo e desenhar um ponto preto no local que foi clicado de tamanho 50 na tela.

• Ao pressionar o botão direito, limpa a tela.

*/

// comandos para executar via terminal do Windows
// 1. g++ ex2.cpp -o ex1 -lfreeglut -lglu32 -lopengl32
// 2. .\main

#include <GL/glut.h>


void init(void);
void display(void);


void clean_screen() {
	// Limpa a tela
	glClear(GL_COLOR_BUFFER_BIT);
}

void init(void)
{
  // define a cor de background da janela
  glClearColor(1.0, 1.0, 1.0, 1.0);

  // define o sistema de visualização - tipo de projeção
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //O glOrtho é uma janela de projeção que renderiza objetos que estão dentro de um determinado limite
  //Parametros: glOrtho(X (left), X (right), Y (bottom), Y (top), Z (near), Z (far))
  glOrtho(0.0, 500.0, 500.0, 0.0, 0.0, 1.0); // Tendo bugs de render, mexer nessa função aqui

  clean_screen();

}

void display() {
	// Libera o buffer de comando de desenho para fazer o desenho acontecer o mais rápido possível.
	glFlush();
}


void create_dots(GLint x, GLint y){

	glPointSize(50.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
		glVertex3i(x, y, 0);
	glEnd();

}

void mouse_callback(int button, int state, int x, int y) {

	switch(button)
	{
		case GLUT_LEFT_BUTTON:
			create_dots(x, y);
			break;
		case GLUT_RIGHT_BUTTON:
			clean_screen();
			break;
		default:
			break;
	}
	display();
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
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (500, 500);

  // Cria uma janela e define seu título
  glutCreateWindow("Tarefa 2: Pontos com o Mouse");

  //Nesta função é definido o estado inicial do OpenGL. Ajustes podem ser feitos para o usuário nessa função.
  init();

  // Define display() como a função de desenho (display callback) para a janela corrente.
  // Quando GLUT determina que esta janela deve ser redesenhada, a função de desenho é chamada.
  glutDisplayFunc(display);

  // Chama a função de acionamento de evento no mouse
  glutMouseFunc(mouse_callback);
  //Inicia o loop de processamento de desenhos com GLUT.
  // Esta rotina deve ser chamada pelo menos uma vez em um programa que utilize a biblioteca GLUT.
  glutMainLoop();

  return 0;

}
