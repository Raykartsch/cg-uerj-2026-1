// Run these commands in terminal to execute this code in Linux:
// 1. g++ -o code code.cpp -lglut -lGLU -lGL -lm
// 2. ./code


// Run these commands in terminal to execute this code in Windows:
// 1. g++ code.cpp -o code -lfreeglut -lglu32 -lopengl32
// 2. .\code


// 1. Fazer um fundo que se move.
// 2. Fazer um objeto se movendo independente do quadrado que tenho aqui.
// 3. Criar um modulo pro objeto não sai da tela no eixo x. (OK)


//////////////////////////////////////////////////////////////////////////////////////////////
// Importacao de bibliotecas
#include <GL/glut.h>
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
// Forma basica
void drawSquare()
{
      glBegin(GL_POLYGON);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(-1, 1, 0);
      glEnd();
}

//////////////////////////////////////////////////////////////////////////////////////////////
void drawTriangle() {
    //glBegin(GL_LINE_LOOP);
	glBegin(GL_POLYGON);
      glVertex3f(-1, 0, 0);
      glVertex3f(0, 1, 0);
      glVertex3f(1, 0, 0);
    glEnd();
}


//////////////////////////////////////////////////////////////////////////////////////////////

void drawDisk(double radius){
    int d;
    glBegin(GL_POLYGON);
    for (d = 0; d < 32; d++){
        double angle = (2*PI/32) * d;
        glVertex3f(radius*cos(angle), radius*sin(angle), 0);
    }
    glEnd();
}


//////////////////////////////////////////////////////////////////////////////////////////////

// Desenha um pneu
void drawWheel(){
    int i;
    glColor3f(0, 0, 0);
    drawDisk(1);
    glColor3f(0.75f, 0.75f, 0.75f);
    drawDisk(0.8);
    glColor3f(0, 0, 0);
    drawDisk(0.2);
    //Controla a rotacao do carro
    //glRotatef(wheel_default_angle, 0, 0, 1);
    //glRotatef(float(-FrameNumber*speed), 0, 0, 1);
    glBegin(GL_LINES);
    for (i=0; i < 15; i++) {
        glVertex2f(0, 0);
        glVertex2d(cos(i*2*PI/15), sin(i*2*PI/15));

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
// Desenha nuvem

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



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variaveis usadas para animar o braco
float shoulderAngle = 0.0f;
float elbowAngle = 0.0f;

float shoulderTarget = 90.0f;
float elbowTarget = 90.0f;

float animSpeed = 3.0f;

// Desenha o braco mecanico que roda
void drawArm(){

    glPushMatrix();
        glTranslatef(-2, 2, 0);            // posição do ombro na cena
        glRotatef(shoulderAngle, 0, 0, 1); // gira o BRAÇO inteiro em torno do ombro

        // Junta do ombro
        glColor3f(0, 0, 0);
        drawDisk(0.4);

        // Braço (parte de cima), pendurado a partir do ombro
        glColor3f(0.55f, 0.55f, 0.55f);
        glPushMatrix();
            glTranslatef(0, -2.0f, 0);
            glScalef(0.5f, 2.0f, 1);
            drawSquare();
        glPopMatrix();

        // Anda até a ponta do braço = posição do cotovelo
        glTranslatef(0, -4.0f, 0);
        glRotatef(elbowAngle, 0, 0, 1);    // gira o ANTEBRAÇO em torno do cotovelo

        // Junta do cotovelo
        glColor3f(0, 0, 0);
        drawDisk(0.35);

        // Antebraço, pendurado a partir do cotovelo
        glColor3f(0.55f, 0.55f, 0.55f);
        glPushMatrix();
            glTranslatef(0, -1.5f, 0);
            glScalef(0.5f, 1.5f, 1);
            drawSquare();
        glPopMatrix();

        glColor3f(0, 0, 0);
        glPushMatrix();

			glTranslatef(0, -3.0f, 0);
			glRotatef(180, 0, 0, 1);
			glScalef(0.8f, 2, 1);
			drawTriangle();
		glPopMatrix();

    glPopMatrix();
}




//////////////////////////////////////////////////////////////////////////////////////////////
// Quadrado controlado pelo usuario

// Controla o pulo do quadrado andante
float jump_maximum_height = 3.0f;
float speed_jump = 0.2f;
float jump_height = 0.0f;
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




//////////////////////////////////////////////////////////////////////////////////////////////

bool r_key_pressed = false;
bool e_key_pressed = false;

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
	  // segurando esquerda -> desflexiona (até 0)
	  if (shoulderAngle > 0) {
		  shoulderAngle -= animSpeed;
	  }

	  if (elbowAngle > 0) {
		  elbowAngle -= animSpeed;
	  }



	  ///////////////////////////////////
	  // Permite o quadrado a andar pra esquerda pela cena
	  if (squarePos < -8.0f) { // esse comando não deixa o quadrado sair da tela, pra mudar o limite, olhar o glOrtho, está definido para 8 agora!
		  squarePos -= 0;
	  } else {
		  squarePos -= squareSpeed;
	  }

	  ///////////////////////////////////
	  // Rotaciona o quadrado qdo anda pra esquerda
	  squareAngle += squareAngleSpeed;
	}


	// inputs inseridos para rodar com a setinha direita do teclado
    if (rightArrowPressed) {

			//////////////////////////////////////////////////////////////////////////////////////
			// Comandos utilizados para animar o braco
          // segurando direita -> flexiona (até o limite)
          if (shoulderAngle < shoulderTarget) {
        	  shoulderAngle += animSpeed;
          }
          if (elbowAngle < elbowTarget) {
        	  elbowAngle += animSpeed;
          }


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
          squareAngle -= squareAngleSpeed;

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
              if (jump_height <= 0.0f) {
                  jump_height = 0.0f; // trava no chão certinho
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

	//=========================================
	// Comandos padrao da funcao anim
	FrameNumber++;
	glutPostRedisplay();
	glutTimerFunc(msecs, anim, valor);
}

void drawBackground1(){


	// Desenhar o ceu
	glColor3f(0.53, 0.81, 0.98);
	glPushMatrix();
		glScalef(50, 50, 1);
		drawSquare();
	glPopMatrix();

	// Desenhar a grama
	glColor3f(0.486f, 0.988f, 0.0f);
	glPushMatrix();
		glTranslatef(0.0f, -4.0f, 1.0f);
		glScalef(50.0f, 4.0f, 1);
		drawSquare();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(7.0f, 7.0f, 1);
		drawSun();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(0, 4.0, 1);
		drawCloud();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(5, 6.0, 1);
		drawCloud();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-6, 5.0, 1);
		drawCloud();
	glPopMatrix();

}

void display() {

	//glClearColor(1.0, 1.0, 0.0, 1.0);
	// Limpa a janela, colocando na tela a cor definida pela função glClearColor
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Alterar a display daqui pra baixo



	//Funcao pra criar um bkg personalizado
	drawBackground1();


	// Criando quadrado com input do usuario na tela
	glColor3f(0, 0, 0);
	glPushMatrix();
		glTranslatef(squarePos, jump_height, 1);
		glRotatef(float(squareAngle), 0, 0, 1);
		drawSquare();
	glPopMatrix();


	//drawArm();

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

