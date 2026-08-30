// Run these commands in terminal to execute this code in Linux:
// 1. g++ -o code code.cpp -lglut -lGLU -lGL -lm
// 2. ./code


// Run these commands in terminal to execute this code in Windows:
// 1. g++ code.cpp -o code -lfreeglut -lglu32 -lopengl32
// 2. .\code


// 1. Fazer um fundo que se move. (OK)
// 2. Fazer um objeto se movendo independente do quadrado que tenho aqui. (OK)
// 3. Criar um modulo pro objeto não sai da tela no eixo x. (OK)
// 4. Criar um quadrado que anda em 4 direcoes e que atira um projetil! (Esta dentro do script movimento_e_projetil)
// 5. Fazer um circulo que se move como um "8" deitado
// 6. Fazer a animacao de uma perna se movendo

//////////////////////////////////////////////////////////////////////////////////////////////
// Importacao de bibliotecas
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <numbers>


// Usado para calcular o disco
double PI = 3.14;

// Variaveis para controlar a animacao da cena
int FrameNumber = 0;
int speed = 50; //The higher this variable is, the lower is the animation
int msecs = 24;


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


//////////////////////////////////////////////////////////////////////////////////////////////
// Cria um quadrado
void drawSquare(){
	glBegin(GL_POLYGON);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(-1, 1, 0);
	glEnd();
}

void drawSquareLine(){
	glLineWidth(1.5f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(-1, 1, 0);
	glEnd();
}



//////////////////////////////////////////////////////////////////////////////////////////////
// Cria um triangulo
void drawTriangle() {
	glBegin(GL_POLYGON);
      glVertex3f(-1, 0, 0);
      glVertex3f(0, 1, 0);
      glVertex3f(1, 0, 0);
    glEnd();
}

void drawTriangleLine() {
	glLineWidth(1.5f);
	glBegin(GL_LINE_LOOP);
      glVertex3f(-1, 0, 0);
      glVertex3f(0, 1, 0);
      glVertex3f(1, 0, 0);
    glEnd();
}



//////////////////////////////////////////////////////////////////////////////////////////////
// Cria um disco
void drawDisk(double radius){
    int d;
    glBegin(GL_POLYGON);
    for (d = 0; d < 32; d++){
        double angle = (2*PI/32) * d;
        glVertex3f(radius*cos(angle), radius*sin(angle), 0);
    }
    glEnd();
}


void drawDiskLine(double radius){
    int d;
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    for (d = 0; d < 32; d++){
        double angle = (2*PI/32) * d;
        glVertex3f(radius*cos(angle), radius*sin(angle), 0);
    }
    glEnd();
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Desenha um sol
void drawSun(){
    int i;
    glColor3f(0.9,0.9,0);
    glLineWidth(3);
    glRotatef(float(-FrameNumber), 0, 0, 1);
    glBegin(GL_LINES);
    for (i=0; i<15; i++){
    	glVertex2f(0, 0);
    	glVertex2d(cos(i*2*PI/15), sin(i*2*PI/15));

    }
    glEnd();
    drawDisk(0.5);
    glColor3f(0, 0, 0);

}


//////////////////////////////////////////////////////////////////////////////////////////////
// Desenha uma nuvem

void drawCloud(){

	glColor3f(1, 1, 1);

	glPushMatrix();
		glTranslatef(-0.6f, -0.2f, 1);
		drawDisk(0.5f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, -0.1f, 1);
		drawDisk(0.7f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.6f, -0.2f, 1);
		drawDisk(0.5f);
	glPopMatrix();


}




void drawFence() {


	glColor3f(0.662f, 0.443f, 0.247f);

	glPushMatrix();
		glTranslatef(-1.4f, 0.2f, 0.0f);
		glScalef(0.15f, 1.0f, 1.0f);
		drawSquare();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(1.4f, 0.2f, 0.0f);
		glScalef(0.15f, 1.0f, 1.0f);
		drawSquare();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(0.0f, 0.2f, 0.0f);
		glScalef(0.15f, 1.0f, 1.0f);
		drawSquare();
	glPopMatrix();



	glColor3f(0.752f, 0.541f, 0.321f);

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glScalef(1.5f, 0.15f, 1.0f);
		drawSquare();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(0.0f, 1.0f, 0.0f);
		glScalef(1.5f, 0.15f, 1.0f);
		drawSquare();
	glPopMatrix();



}



void drawFruit(float red, float green, float blue){

	glColor3f(0.243f, 0.556f, 0.180f);
	glPushMatrix();
		glTranslatef(0.0f, 0.35f, 1.0f);
		glScalef(0.15f, 0.15f, 1.0f);
		glRotatef(180, 0, 0, 1);
		drawTriangle();
	glPopMatrix();


	glColor3f(red, green, blue);
	glPushMatrix();
		drawDisk(0.2);
	glPopMatrix();


}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha o coelho


// Variaveis para controlar a animacao de "correr" do coelho (patas e orelhas)
// O coelho fica sempre animado, independente de estar andando ou parado
float walkPhase = 0.0f;       // fase atual da animacao (em radianos)
float walkPhaseSpeed = 0.15f; // velocidade com que a fase avanca a cada frame
float legLiftAmount = 0.2f;  // o quanto a pata sobe no eixo Y
float earSwingAmount = 6.0f; // o quanto a orelha "balanca" (em graus)




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha o coelho
void drawRabbit(){


	// Calcula, a partir da fase atual da animacao, o quanto cada pata deve
	// subir no eixo Y e o quanto cada orelha deve balancar para frente.
	// A logica liga as duas: quando a orelha direita esta indo para frente
	// (senoide positiva), a perna ESQUERDA sobe; quando a orelha esquerda
	// esta indo para frente (senoide negativa), a perna DIREITA sobe.
	float phaseSin = sin(walkPhase);

	float earRightSwing = (phaseSin > 0.0f) ?  phaseSin * earSwingAmount : 0.0f;   // orelha direita "pra frente" quando > 0
	float earLeftSwing  = (phaseSin < 0.0f) ? -phaseSin * earSwingAmount : 0.0f;  // orelha esquerda "pra frente" quando > 0 (fase oposta)

	float legLeftLift  = (phaseSin > 0.0f)  ?  phaseSin * legLiftAmount : 0.0f; // sobe junto com a orelha direita
	float legRightLift = (phaseSin < 0.0f)  ? -phaseSin * legLiftAmount : 0.0f; // sobe junto com a orelha esquerda


	// Perna Esquerda
	glPushMatrix();
		glTranslatef(-0.4f, -1.0f + legLeftLift, 1.0f);
		glScalef(0.125f, 0.4f, 1.0f);
		glColor3f(0.90f, 0.85f, 0.79f);
		drawSquare();
		glColor3f(0.705f, 0.64f, 0.58f);
		drawSquareLine();
	glPopMatrix();


	// Perna Direita
	glPushMatrix();
		glTranslatef(0.4f, -1.0f + legRightLift, 1.0f);
		glScalef(0.125f, 0.4f, 1.0f);
		glColor3f(0.90f, 0.85f, 0.79f);
		drawSquare();
		glColor3f(0.705f, 0.64f, 0.58f);
		drawSquareLine();
	glPopMatrix();

	// Rabo
	glPushMatrix();
		glTranslatef(-0.85f, -0.4f + (legRightLift / 3), 1.0f);
		glColor3f(0.96f, 0.93f, 0.89f);
		drawDisk(0.25);
		glColor3f(0.705f, 0.64f, 0.58f);
		drawDiskLine(0.25);
	glPopMatrix();

	// Corpo
	glPushMatrix();
		glColor3f(0.96f, 0.93f, 0.89f);
		drawDisk(1);
		glColor3f(0.705f, 0.64f, 0.58f);
		drawDiskLine(1);
	glPopMatrix();

	// Cabeça
	glPushMatrix();
		glTranslatef(1.2, 0.7, 1.0f);
		glColor3f(0.96f, 0.93f, 0.89f);
		drawDisk(0.6);
		glColor3f(0.705f, 0.64f, 0.58f);
		drawDiskLine(0.6);
	glPopMatrix();


	// Olho
	glPushMatrix();
		glTranslatef(1.5f, 0.8f, 1.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		drawDisk(0.065);
	glPopMatrix();

	// Focinho
	glPushMatrix();
		glTranslatef(1.8f, 0.5f, 1.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		drawDisk(0.2);
		glColor3f(0.705f, 0.64f, 0.58f);
		drawDiskLine(0.2);
	glPopMatrix();


	// Nariz
	glPushMatrix();
		glTranslatef(1.96f, 0.5f, 1.0f);
		glScalef(0.15f, 0.07f, 1.0f);
		glRotatef(-90, 0, 0, 1);
		glColor3f(0.90f, 0.42f, 0.54f);
		drawTriangle();
	glPopMatrix();


	// Orelha Esquerda
	glPushMatrix();
		glTranslatef(1.2f, 1.2f, 1.0f);
		glRotatef(10 - earLeftSwing, 0, 0, 1);
		glScalef(0.125f, 1.2f, 1.0f);
		glColor3f(0.96f, 0.93f, 0.89f);
		drawTriangle();
		glColor3f(0.705f, 0.64f, 0.58f);
		drawTriangleLine();
	glPopMatrix();


	// Orelha Direita
	glPushMatrix();
		glTranslatef(1.6f, 1.2f, 1.0f);
		glRotatef(-10 - earRightSwing, 0, 0, 1);
		glScalef(0.125f, 1.2f, 1.0f);
		glColor3f(0.96f, 0.93f, 0.89f);
		drawTriangle();
		glColor3f(0.705f, 0.64f, 0.58f);
		drawTriangleLine();
	glPopMatrix();


}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha o background "infinito"

float bgSpeed = 0.1f;      // velocidade do fundo (independente do carro)
float bgWidth = 40.0f;     // largura de uma "tile" do fundo (2 * 50 do glScalef)
float bgPos = 0.0f;         // posição atual do fundo


void drawBackgroundContent(){

	// Fazer o background com 50 pixels de largura
	// Desenhar o ceu
		glColor3f(0.53, 0.81, 0.98);
		glPushMatrix();
			// X = 24
			glScalef(bgWidth + (bgWidth / 2), 10, 1);
			drawSquare();
		glPopMatrix();

	// Desenhar a grama
		glColor3f(0.486f, 0.988f, 0.0f);
		glPushMatrix();
			glTranslatef(0.0f, -4.0f, 1.0f);
			// X = 24
			glScalef(bgWidth + (bgWidth / 2), 4.0f, 1);
			drawSquare();
		glPopMatrix();


	// Desenha cerca
		glPushMatrix();
			glTranslatef(16.0f, 0.5f, 1.0f);
			glScalef(1.5f, 1.0f, 1.0f);
			drawFence();
		glPopMatrix();



	// Desenha as nuvens
		glPushMatrix();
			glTranslatef(14.0f, 5.0f, 1.0f);
			drawCloud();
		glPopMatrix();


		glPushMatrix();
			glTranslatef(21.0f, 4.0f, 1.0f);
			drawCloud();
		glPopMatrix();


		glPushMatrix();
			glTranslatef(29.0f, 5.5f, 1.0f);
			drawCloud();
		glPopMatrix();



	// Desenhar canteiro 1
		glPushMatrix();
			//Desenha terra
			glColor3f(0.478f, 0.290f, 0.168f);

			glPushMatrix();
				glTranslatef(24.0f, -1.5f, 1.0f);
				glScalef(2.5f, 1.0f, 1.0f);
				drawSquare();
			glPopMatrix();


			// Desenha laranjas
			//glColor3f(0.909f, 0.447f, 0.172f);
			glPushMatrix();
				glTranslatef(22.0f, -1.5f, 1.0f);
				glScalef(1.2f, 1.2f, 1.0f);
				drawFruit(0.909f, 0.447f, 0.172f);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(23.4f, -1.8f, 1.0f);
				glScalef(1.5f, 1.5f, 1.0f);
				drawFruit(0.909f, 0.447f, 0.172f);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(24.3f, -1.3f, 1.0f);
				drawFruit(0.909f, 0.447f, 0.172f);
			glPopMatrix();


			glPushMatrix();
				glTranslatef(25.7f, -2.1f, 1.0f);
				drawFruit(0.909f, 0.447f, 0.172f);
			glPopMatrix();


			// Folhagens
			glColor3f(0.243f, 0.556f, 0.180f);
			glPushMatrix();
				glTranslatef(24.9f, 0.5f, 1.0f);
				glScalef(0.8f, 1.1f, 1.0f);
				glRotatef(180, 0, 0, 1);
				drawTriangle();
			glPopMatrix();


			glColor3f(0.243f, 0.556f, 0.180f);
			glPushMatrix();
				glTranslatef(23.2f, -0.1f, 1.0f);
				glScalef(0.5f, 1.3f, 1.0f);
				glRotatef(180, 0, 0, 1);
				drawTriangle();
			glPopMatrix();

		glPopMatrix();




	// Desenhar canteiro 2
		glPushMatrix();
			//Desenha terra
			glColor3f(0.478f, 0.290f, 0.168f);

			glPushMatrix();
				glTranslatef(29.0f, -1.5f, 1.0f);
				glScalef(2.0f, 1.0f, 1.0f);
				drawSquare();
			glPopMatrix();


			// Desenha laranjas
			//glColor3f(0.909f, 0.447f, 0.172f);
			glPushMatrix();
				glTranslatef(29.2f, -1.5f, 1.0f);
				glScalef(1.2f, 1.2f, 1.0f);
				drawFruit(0.839f, 0.270f, 0.313f);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(30.1f, -1.8f, 1.0f);
				glScalef(1.5f, 1.5f, 1.0f);
				drawFruit(0.839f, 0.270f, 0.313f);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(30.8f, -1.3f, 1.0f);
				drawFruit(0.839f, 0.270f, 0.313f);
			glPopMatrix();



			// Folhagens
			glColor3f(0.243f, 0.556f, 0.180f);
			glPushMatrix();
				glTranslatef(29.6f, 0.5f, 1.0f);
				glScalef(0.9f, 1.5f, 1.0f);
				glRotatef(180, 0, 0, 1);
				drawTriangle();
			glPopMatrix();


		glPopMatrix();




	/* O mundo é 32 pixels, logo os 8 primeiros pixels e os 8 ultimos precisam ser "iguais"
	 * para dar a impressão de cenario infinito ao jogador
	*/

	// Objetos recorrentes

	glPushMatrix();
		glTranslatef(4.0f, 5.0f, 1.0f);
		drawCloud();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(44.0f, 5.0f, 1.0f);
		drawCloud();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(4.0f, 0.5f, 1.0f);
		drawFence();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(44.0f, 0.5f, 1.0f);
		drawFence();
	glPopMatrix();


}


void drawBackground1(){

	// Tile 1: normal, na posição de origem
	glPushMatrix();
		drawBackgroundContent();
	glPopMatrix();

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quadrado controlado pelo usuario

// Controla o pulo do quadrado andante
float jump_maximum_height = 3.0f;
float speed_jump = 0.2f;
float jump_height = 0.5f;
bool isJumping = false;
bool goingUp = true; // controla se está na fase de subida ou descida do pulo


// Controlam o movimento no eixo x do quadrado andante
float squarePos = 0.0f;
float squareSpeed = 0.2f;


// Controlam o angulo de rotacao do quadrado andante
int squareAngle = 0;
int squareAngleSpeed = 5.0;


// booleanos que controlam se a setinha do teclado está pressionada (neste caso, estas teclas sao especiais)
bool rightArrowPressed = false;
bool leftArrowPressed = false;
bool upArrowPressed = false;
bool downArrowPressed = false;


// Habilita a manipulacao por setinha
void arrowKeysDown(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
    	rightArrowPressed = true;
    }
    if (key == GLUT_KEY_LEFT)  {
    	leftArrowPressed = true;
    }
    if (key == GLUT_KEY_UP && !isJumping) {
    	upArrowPressed = true;
    	isJumping = true;
    	goingUp = true;

    }
    if (key == GLUT_KEY_DOWN) {
       	downArrowPressed = true;
       }
}

// Detecta quando as setinhas nao estao mais sendo pressionadas
void arrowKeysUp(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
    	rightArrowPressed = false;
    }
    if (key == GLUT_KEY_LEFT)  {
    	leftArrowPressed = false;
    }
    if (key == GLUT_KEY_UP) {
       	upArrowPressed = false;
	}
	if (key == GLUT_KEY_DOWN) {
		upArrowPressed = false;
	}
}




// Indicam para outras funcoes se as teclas abaixo estao sendo pressionadas no teclado
bool r_key_pressed = false;
bool e_key_pressed = false;

// Funcao de captura de pressionamento de teclas no teclado.
void keyboard_callback(unsigned char key, GLint x, GLint y) {

	if (key == 101){ //letra e
		e_key_pressed = true;
	}

	if (key == 114) { //letra r
		r_key_pressed = true;
	}

}


void keyboard_up_callback(unsigned char key, GLint x, GLint y) {
    if (key == 101) { //letra e
        e_key_pressed = false;
    }
    if (key == 114) { // letra r
        r_key_pressed = false;
    }
}



//////////////////////////////////////////////////////////////////////////////////////////////
// Função que controla a animacao
void anim (int valor) {


	// inputs inseridos para rodar com a setinha esquerda do teclado
	if (leftArrowPressed) {


		  ///////////////////////////////////
		  // Permite o quadrado a andar pra esquerda pela cena
		  if (squarePos < -8.0f) { // esse comando não deixa o quadrado sair da tela, pra mudar o limite, olhar o glOrtho, está definido para 8 agora!
			  squarePos -= 0;
		  } else {
			  squarePos -= squareSpeed;
		  }

		  ///////////////////////////////////
		  // Rotaciona o quadrado qdo anda pra esquerda
		  //squareAngle += squareAngleSpeed;

	}


	// inputs inseridos para rodar com a setinha direita do teclado
    if (rightArrowPressed) {

		//////////////////////////////////////////////////////////////////////////////////////
			// Comandos utilizados para animar o braco
			// segurando direita -> flexiona (até o limite)


          ///////////////////////////////////
          // Permite o quadrado a andar pra direita pela cena
			if (squarePos > 8.0f) { // esse if não deixa o quadrado sair da tela, pra mudar o limite, olhar o glOrtho, está definido para 8 agora!
				squarePos += 0;
			} else {
				squarePos += squareSpeed;
			}

          //squarePos += squareSpeed;

          ///////////////////////////////////
          // Rotaciona o quadrado qdo anda pra esquerda
          //squareAngle -= squareAngleSpeed;


     }


      //////////////////////////////////////////////////////////////////////////////////////
      // Controla a animação de pulo do quadrado
      if (isJumping) {
          if (goingUp) {
              jump_height += speed_jump;
              if (jump_height >= jump_maximum_height) {
                  jump_height = jump_maximum_height; // trava no topo
                  goingUp = false; // começa a fase de descida
              }
          } else {
              jump_height -= speed_jump;
              if (jump_height <= 0.5f) {
                  jump_height = 0.5f; // trava no chão certinho
                  isJumping = false; // pulo terminou, pode pular de novo
              }
          }
      }


     //////////////////////////////////////////////////////////////////////////////////////
     // Controla a animação de giro do quadrado ao andar
     if (r_key_pressed){
    	 squareAngle += squareAngleSpeed;
     }

     if (e_key_pressed){
		 squareAngle -= squareAngleSpeed;
     }


     //////////////////////////////////////////////////////////////////////////////////////
     // Controla a animacao de corrida do coelho (patas e orelhas)
     // Avanca sempre, independente de o coelho estar se movendo pela cena ou nao,
     // para que ele fique "sempre" animado.
     walkPhase += walkPhaseSpeed;
     if (walkPhase > 2 * PI) {
    	 walkPhase -= 2 * PI; // mantém o valor sempre dentro de uma faixa, sem crescer pra sempre
     }



     //////////////////////////////////////////////////////////////////////////////////////
     // Controla o background de fundo
     bgPos -= bgSpeed;
     bgPos = fmod(bgPos, bgWidth); // mantém o valor sempre dentro de uma faixa, sem crescer pra sempre

	//======================================================================================================================================================================================================================================================
	// Comandos padrao da funcao anim
	FrameNumber++;
	glutPostRedisplay();
	glutTimerFunc(msecs, anim, valor);
}


void display() {

	//glClearColor(1.0, 1.0, 0.0, 1.0);
	// Limpa a janela, colocando na tela a cor definida pela função glClearColor
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity();



	//////////////////////////////////////////////////////////////////////////////////////////////
	// Alterar a display daqui pra baixo

	//Exemplo de background que se move infinitamente em OpenGL
		// Exemplo de background que se move infinitamente em OpenGL[cite: 1]

		glPushMatrix();
			glTranslatef(bgPos, 0, 1);
			drawBackground1();
		glPopMatrix();



		glPushMatrix();
			glTranslatef(7.0f, 7.0f, 1);
			drawSun();
		glPopMatrix();

	// Criando quadrado com input do usuario na tela
		glColor3f(0, 0, 0);
		glPushMatrix();
			glTranslatef(squarePos, jump_height, 1.0f);
			glRotatef(float(squareAngle), 0, 0, 1);
			glScalef(0.6f, 0.6f, 1.0f);
			drawRabbit();
		glPopMatrix();





	// Instancia um carro azul que pode ser controlado pelo usuario
		/*glPushMatrix();
			glTranslatef(squarePos, -5, 1);
			//glTranslatef(-float(FrameNumber)/speed, -5, 1); // ativar isso aqui se for pro carro andar sozinho
			glScalef(0.7f, 0.7f, 1);
			drawCart(0.0f, 0.0f, 1.0f, true);
		glPopMatrix();*/


	// Instancia um braco, questao pronta da P1
		//drawArm();


	//






	// Alterar a display daqui pra cima
	//////////////////////////////////////////////////////////////////////////////////////////////

	glutSwapBuffers();

}



int main(int argc, char** argv)
{

	//Inicializa a biblioteca GLUT e negocia uma seção com o gerenciador de janelas.
	//É possível passar argumentos para a função glutInit provenientes da linha de execução, tais como informações sobre a geometria da tela
	glutInit(&argc, argv);

	//Informa à biblioteca GLUT o modo do display a ser utilizado quando a janela gráfica for criada.
	// O flag GLUT_SINGLE força o uso de uma janela com buffer simples, significando que todos os desenhos serão feitos diretamente nesta janela.
	// O flag GLUT_RGB determina que o modelo de cor utilizado será o modelo RGB.
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

	//Define o tamanho inicial da janela, 256x256 pixels, e a posição inicial do seu canto superior esquerdo na tela, (x, y)=(100, 100).
	glutInitWindowSize (800, 800);
	glutInitWindowPosition (200, 200);

	// Cria uma janela e define seu título
	glutCreateWindow("Animation 2D");

	//Nesta função é definido o estado inicial do OpenGL. Ajustes podem ser feitos para o usuário nessa função.
	init();

	// Define display() como a função de desenho (display callback) para a janela corrente.
	// Quando GLUT determina que esta janela deve ser redesenhada, a função de desenho é chamada.
	glutDisplayFunc(display);


	// Controla a animacao
	glutTimerFunc(msecs, anim, 0);

	// Controla o input de teclado pelo usuario
	glutKeyboardFunc(keyboard_callback);
	glutKeyboardUpFunc(keyboard_up_callback);

	// Controla o input por setinha do teclado feito pelo usuario
	glutSpecialFunc(arrowKeysDown);
	glutSpecialUpFunc(arrowKeysUp);

	//Inicia o loop de processamento de desenhos com GLUT.
	// Esta rotina deve ser chamada pelo menos uma vez em um programa que utilize a biblioteca GLUT.
	glutMainLoop();

	return 0;

}

