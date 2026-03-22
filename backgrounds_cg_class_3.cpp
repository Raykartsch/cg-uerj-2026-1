
/*
Exercicio Slide: Desenvolver codigos que gerem determinadas imagens com OpenGL
(as imagens solicitadas estao nos slides da aula 3)
*/

// comandos para executar o codigo via terminal do Windows
// 1. g++ backgrounds_cg_class_3.cpp -o backgrounds_cg_class_3 -lfreeglut -lglu32 -lopengl32
// 2. .\backgrounds_cg_class_3
#include <GL/glut.h>

void init(void);
void display(void);
void clean_screen(void);
void bg_1(void);

void init(void)
{
  // define a cor de background da janela
  glClearColor(1.0, 1.0, 1.0, 1.0);

  // define o sistema de visualização - tipo de projeção
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //O glOrtho é uma janela de projeção que renderiza objetos que estão dentro de um determinado limite
  //Parametros: glOrtho(X (left), X (right), Y (bottom), Y (top), Z (near), Z (far))
  glOrtho(0.0, 500.0, 0.0, 500.0, -1.0, 1.0);
  //glOrtho (-100,100.0,-100.0,100.0,-1.0,1.0);

  // Limpar tela
  clean_screen();

  //Instancia o primeiro background por default
  bg_1();
}

void clean_screen() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void display() {
	// Libera o buffer de comando de desenho para fazer o desenho acontecer o mais rápido possível.
	glFlush();
}

void bg_1(){
	glClearColor(0.0, 1.0, 0.0, 1.0);
	clean_screen();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Losango azul
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
		glVertex3f(150.0, 150.0, 0.0); //Topo
		glVertex3f(200.0, 100.0, 0.0); //Direita
		glVertex3f(150.0, 50.0, 0.0); //Baixo
		glVertex3f(100.0, 100.0, 0.0); //Esquerda
	glEnd();

	//Losango Amarelo
	glColor3f(255.0, 255.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex3f(75.0, 225.0, 0.0); //Topo
		glVertex3f(125.0, 175.0, 0.0); //Direita
		glVertex3f(75.0, 125.0, 0.0); //Baixo
		glVertex3f(25.0, 175.0, 0.0); //Esquerda
	glEnd();

	//Losango Vermelho
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex3f(225.0, 225.0, 0.0); //Topo
		glVertex3f(275.0, 175.0, 0.0); //Direita
		glVertex3f(225.0, 125.0, 0.0); //Baixo
		glVertex3f(175.0, 175.0, 0.0); //Esquerda
	glEnd();

	//Losango Preto
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex3f(150.0, 300.0, 0.0); //Topo
		glVertex3f(200.0, 250.0, 0.0); //Direita
		glVertex3f(150.0, 200.0, 0.0); //Baixo
		glVertex3f(100.0, 250.0, 0.0); //Esquerda
	glEnd();


}

void bg_2() {

	glClearColor(1.0, 1.0, 0.0, 1.0);
	clean_screen();

	// Define a cor do desenho para vermelho
	glColor3f(1.0, 0.0, 0.0);
	//Define a forma como o polígono será criado (preencher o polígono na parte de frente e atrás)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Determina a espessura da linha
	glLineWidth(10.0);
	//glRecti(155, 150, 355, 350); //Outra forma de gerar um quadrado com OpenGL

	// Gera o quadrado vermelho
	glBegin(GL_LINE_LOOP);
		glVertex3f(155.0, 150.0, 0.0);
		glVertex3f(155.0, 350.0, 0.0);
		glVertex3f(355.0, 350.0, 0.0);
		glVertex3f(355.0, 150.0, 0.0);
	glEnd();

	//Gera as linhas de mira do desenho
	glBegin(GL_LINES);
		// Linha superior
		glVertex3f(255.0, 0.0, 0.0);
		glVertex3f(255.0, 240.0, 0.0);

		// Linha inferior
		glVertex3f(255.0, 260.0, 0.0);
		glVertex3f(255.0, 500.0, 0.0);

		// Linha da esquerda
		glVertex3f(0.0, 250.0, 0.0);
		glVertex3f(245.0, 250.0, 0.0);

		// Linha da direita
		glVertex3f(264.0, 250.0, 0.0);
		glVertex3f(500.0, 250.0, 0.0);
	glEnd();

}

void bg_3() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	clean_screen();
	// Define a espessura da linha
	glLineWidth(2.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_LINE_LOOP);
		// Topo
		glColor3f(0.0, 1.0, 0.0); //Define a cor verde do vertice
		glVertex3f(150.0, 300.0, 0.0);

		// Meio Esquerdo
		glColor3f(1.0, 0.0, 0.0); // Define a cor vermelho do vertice
		glVertex3f(115.0, 210.0, 0.0);

		// Base Esquerda
		glColor3f(1.0, 0.0, 0.0); // Define a cor vermelho do vertice (essa funcao nao é necessaria, coloquei mas pra me guiar mesmo)
		glVertex3f(70.0, 140.0, 0.0);

		// Base centro
		glColor3f(0.0, 0.0, 1.0); // Define a cor azul do vertice
		glVertex3f(150.0, 165.0, 0.0);

		// Base direita
		glColor3f(0.0, 0.0, 1.0); // Define a cor azul do vertice (essa funcao nao é necessaria, coloquei mas pra me guiar mesmo)
		glVertex3f(230.0, 140.0, 0.0);

		// Meio direito
		glColor3f(0.0, 1.0, 0.0); // Define a cor verde do vertice
		glVertex3f(185.0, 210.0, 0.0);
	glEnd();
}

void keyboard_callback(unsigned char key, int x, int y){
	switch (key) {
	case '1':
		bg_1();
		break;
	case '2':
		bg_2();
		break;
	case '3':
		bg_3();
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
  glutCreateWindow("Backgrounds no OpenGL");

  //Nesta função é definido o estado inicial do OpenGL. Ajustes podem ser feitos para o usuário nessa função.
  init();

  // Define display() como a função de desenho (display callback) para a janela corrente.
  // Quando GLUT determina que esta janela deve ser redesenhada, a função de desenho é chamada.
  glutDisplayFunc(display);

  //Captura eventos no teclado
  glutKeyboardFunc(keyboard_callback);
  //Inicia o loop de processamento de desenhos com GLUT.
  // Esta rotina deve ser chamada pelo menos uma vez em um programa que utilize a biblioteca GLUT.
  glutMainLoop();

  return 0;

}
