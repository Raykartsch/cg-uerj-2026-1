/*
Tarefa 1: Desenhar Casa
• Desenhar uma casa simples em OpenGL

• Ao pressionar a tecla espaço do teclado, o plano de fundo escurece.

• Se o fundo estiver escuro e o usuário pressionar qualquer tecla, o plano de fundo torna-se branco.

*/

// comandos para executar o codigo via terminal do Windows
// 1. g++ ex1_class_3.cpp -o ex1_class_3 -lfreeglut -lglu32 -lopengl32
// 2. .\ex1_class_3

#include <GL/glut.h>

//Global variable declaration
int global_ctrl = 0;

void init(void);
void display(void);
void generate_house(void);

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
  glOrtho(0.0, 500.0, 0, 500.0, 0.0, 1.0); // Tendo bugs de render, mexer nessa função aqui

  //Limpar a tela colocando-a em branco
  clean_screen();

  // Gerar o desenho da casa
  generate_house();

}


void generate_house(){

	// Gerar o telhado
	glColor3f(1.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
		glVertex3f(50, 300, 0);
		glVertex3f(125, 350, 0);
		glVertex3f(200, 300, 0);
	glEnd();

	//Base da casa
	glColor3f(0.0, 0.0, 1.0);
	glRectf(50, 300, 200, 200);

	/*Alternativa ao glRectf usando glVertex
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(50, 300, 0);
		glVertex3f(150, 300, 0);
		glVertex3f(150, 200, 0);
		glVertex3f(50, 200, 0);
	glEnd();*/

	//Porta da casa
	glColor3f(1.0, 1.0, 1.0);
	glRectf(110, 270, 140, 205);

	//Janela da casa
	glColor3f(1.0, 1.0, 1.0);
	glRectf(160, 250, 190, 270);

	//Linhas da janela da casa
	glLineWidth(5.0);
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
		//Linha vertical
		glVertex3f(176.0, 250.0, 0.0);
		glVertex3f(176.0, 270.0, 0.0);

		//Linha horizontal
		glVertex3f(160.0, 260.0, 0.0);
		glVertex3f(190.0, 260.0, 0.0);
	glEnd();

}

void display() {
	// Libera o buffer de comando de desenho para fazer o desenho acontecer o mais rápido possível.
	glFlush();
}

void gen_image(int global_ctrl){

	switch(global_ctrl) {
		case 1:
			glClearColor(0.0, 0.0, 0.0, 0.0);
			clean_screen();
			generate_house();
			break;

		default:
			glClearColor(1.0, 1.0, 1.0, 1.0);
			clean_screen();
			generate_house();
			break;
	}

}

void keyboard_callback(unsigned char key, GLint x, GLint y) {

	if (key == 32 && global_ctrl == 0){
		global_ctrl = 1;
	} else {
		global_ctrl = 0;
	};
	gen_image(global_ctrl);
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
  glutCreateWindow("Tarefa 1: Desenhar Casa");

  //Nesta função é definido o estado inicial do OpenGL. Ajustes podem ser feitos para o usuário nessa função.
  init();

  // Define display() como a função de desenho (display callback) para a janela corrente.
  // Quando GLUT determina que esta janela deve ser redesenhada, a função de desenho é chamada.
  glutDisplayFunc(display);

  // Chama a função de acionamento de evento no teclado
  glutKeyboardFunc(keyboard_callback);

  //Inicia o loop de processamento de desenhos com GLUT.
  // Esta rotina deve ser chamada pelo menos uma vez em um programa que utilize a biblioteca GLUT.
  glutMainLoop();

  return 0;

}
