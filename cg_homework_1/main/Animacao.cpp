#include "Animacao.hpp"
#include "Primitivas.hpp"
#include "Cenario.hpp"
#include "Coelho.hpp"
#include "Raposa.hpp"
#include "Borboleta.hpp"
#include <cstdio>
#include <GL/glut.h>
#include <math.h>

int FrameNumber = 0;
int speed = 50;
int msecs = 24;

bool rightArrowPressed = false;
bool leftArrowPressed = false;
bool upArrowPressed = false;
bool downArrowPressed = false;
bool r_key_pressed = false;
bool e_key_pressed = false;

void init(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  glOrtho (-8, 8, -8, 8, -8, 8);
}

void arrowKeysDown(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) rightArrowPressed = true;
    if (key == GLUT_KEY_LEFT)  leftArrowPressed = true;
    if (key == GLUT_KEY_UP && !isJumping) {
    	upArrowPressed = true; isJumping = true; goingUp = true;
    }
    if (key == GLUT_KEY_DOWN) downArrowPressed = true;
}

void arrowKeysUp(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) rightArrowPressed = false;
    if (key == GLUT_KEY_LEFT)  leftArrowPressed = false;
    if (key == GLUT_KEY_UP)    upArrowPressed = false;
	if (key == GLUT_KEY_DOWN)  downArrowPressed = false;
}

void keyboard_callback(unsigned char key, int x, int y) {
	if (key == 101) e_key_pressed = true;
	if (key == 114) r_key_pressed = true;
}

void keyboard_up_callback(unsigned char key, int x, int y) {
    if (key == 101) e_key_pressed = false;
    if (key == 114) r_key_pressed = false;
}

void desenharHUD() {
    glColor3f(0.0f, 0.0f, 0.0f);
    char textoVidas[32];
    snprintf(textoVidas, sizeof(textoVidas), "Vidas: %d", rabbitLives);
    drawText(-7.7f, 7.2f, textoVidas);

    if (framesDeTurboRestantes > 0) drawText(-7.7f, 6.6f, "Turbo de velocidade!");
    if (framesDePuloReforcadoRestantes > 0) drawText(-7.7f, 6.0f, "Pulo reforcado!");
    if (foxActive) drawText(-7.7f, 5.4f, "Cuidado, a raposa esta te perseguindo!");

    if (coelhoEscondido) {
        drawText(-7.7f, 4.8f, "Escondido na toca - a salvo da raposa!");
    } else {
        for (const Toca &toca : tocas) {
            if (fabs(toca.x - characterPos) < RAIO_TOCA) {
                drawText(-7.7f, 4.8f, "Aperte a seta para baixo para se esconder!");
                break;
            }
        }
    }

    float folegoPorcentagem = folegoAtual / FOLEGO_MAXIMO;
    float larguraBarra = 1.6f; float alturaBarra = 0.12f;

    glPushMatrix();
        glTranslatef(-7.7f, 4.15f, 1.0f);
        glColor3f(0.35f, 0.35f, 0.35f);
        glPushMatrix();
            glTranslatef(larguraBarra / 2.0f, 0.0f, 0.0f); glScalef(larguraBarra / 2.0f, alturaBarra, 1.0f); drawSquare();
        glPopMatrix();
        glColor3f(0.30f, 0.75f, 0.35f);
        glPushMatrix();
            glTranslatef((larguraBarra * folegoPorcentagem) / 2.0f, 0.0f, 0.0f);
            glScalef((larguraBarra * folegoPorcentagem) / 2.0f, alturaBarra, 1.0f); drawSquare();
        glPopMatrix();
    glPopMatrix();

    glColor3f(0.0f, 0.0f, 0.0f); drawText(-7.7f, 3.75f, "Folego (segure R para correr)");
}

void anim(int valor) {
	atualizarEsconderijoDoCoelho();
	atualizarCorrida();

	if (!coelhoEscondido) {
        moverCoelho();
	}

    animarCoelho();
    animarRaposa();
    controlarSurgimentoDaRaposa();
    moverRaposa();
    verificarColisaoComRaposa();
    animarBorboleta();
    animarNuvensECeu();

    if (!coelhoEscondido) {
	     bgPos -= bgSpeed;
	     bgPos = fmod(bgPos, bgWidth);
	     atualizarTocas();
	     controlarSurgimentoDeVegetais();
	     moverVegetais();
	     verificarColisaoComVegetais();
	     atualizarBonusAtivos();
    }

	FrameNumber++;
	glutPostRedisplay();
	glutTimerFunc(msecs, anim, valor);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
        glTranslatef(bgPos, 0, 1);
        drawBackground1();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(6.5f, 6.8f, 1);
        drawSun();
    glPopMatrix();

    desenharTocas();

    if (foxActive) {
        glPushMatrix();
            glTranslatef(foxX, foxY, 1.0f);
            glScalef(0.9f * foxDirecao, 0.8f, 1.0f);
            drawFox();
        glPopMatrix();
    }

    desenharVegetais();

    glColor3f(0, 0, 0);
    if (coelhoEscondido) {
        const Toca &tocaAtual = tocas[tocaOndeEstaEscondido];
        glPushMatrix();
            glTranslatef(tocaAtual.x, tocaAtual.y + 0.18f, 1.0f);
            glScalef(0.4f * direcaoCoelho, 0.4f, 1.0f);
            glColor3f(0.96f, 0.93f, 0.89f);
            glPushMatrix(); glRotatef(10.0f, 0, 0, 1); glScalef(0.125f, 0.6f, 1.0f); drawTriangle(); glPopMatrix();
            glPushMatrix(); glTranslatef(0.5f, 0.0f, 0.0f); glRotatef(-10.0f, 0, 0, 1); glScalef(0.125f, 0.6f, 1.0f); drawTriangle(); glPopMatrix();
        glPopMatrix();
    } else {
        glPushMatrix();
            glTranslatef(characterPos, jump_height, 1.0f);
            glScalef(0.4f * direcaoCoelho, 0.4f, 1.0f);
            drawRabbit();
        glPopMatrix();
    }

    desenharHUD();
    glutSwapBuffers();
}
