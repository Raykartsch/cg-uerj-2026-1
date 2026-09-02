#include "Coelho.hpp"
#include "Primitivas.hpp"
#include "Animacao.hpp"
#include "Cenario.hpp"

float characterPos = 0.0f;
float characterSpeed = 0.05f;

const float VELOCIDADE_NORMAL = 0.05f;
const float VELOCIDADE_TURBO = 0.20f;
int framesDeTurboRestantes = 0;
const int DURACAO_TURBO_EM_FRAMES = 150;

const float VELOCIDADE_CORRENDO = 0.09f;
const float FOLEGO_MAXIMO = 100.0f;
float folegoAtual = FOLEGO_MAXIMO;
const float GASTO_FOLEGO_POR_FRAME = 0.6f;
const float RECUPERACAO_FOLEGO_POR_FRAME = 0.3f;
bool coelhoCorrendo = false;

float jump_maximum_height = 2.5f;
float speed_jump = 0.1f;
float jump_height = 0.5f;
bool isJumping = false;
bool goingUp = true;

const float PULO_NORMAL = 2.5f;
const float PULO_REFORCADO = 3.5f;
int framesDePuloReforcadoRestantes = 0;
const int DURACAO_PULO_REFORCADO_EM_FRAMES = 300;

float walkPhase = 0.0f;
float walkPhaseSpeed = 0.15f;
float legLiftAmount = 0.2f;
float earSwingAmount = 6.0f;
float direcaoCoelho = 1.0f;

int rabbitLives = 3;
const int MAX_VIDAS = 3;

void drawRabbit(){
	float phaseSin = sin(walkPhase);
	float earRightSwing = (phaseSin > 0.0f) ?  phaseSin * earSwingAmount : 0.0f;
	float earLeftSwing  = (phaseSin < 0.0f) ? -phaseSin * earSwingAmount : 0.0f;
	float legLeftLift  = (phaseSin > 0.0f)  ?  phaseSin * legLiftAmount : 0.0f;
	float legRightLift = (phaseSin < 0.0f)  ? -phaseSin * legLiftAmount : 0.0f;

	glPushMatrix();
		glColor3f(0.96f, 0.93f, 0.89f); drawDisk(1);
		glColor3f(0.705f, 0.64f, 0.58f); drawDiskLine(1);

		glPushMatrix();
			glTranslatef(-0.4f, -1.0f + legLeftLift, 1.0f); glScalef(0.125f, 0.4f, 1.0f);
			glColor3f(0.90f, 0.85f, 0.79f); drawSquare();
			glColor3f(0.705f, 0.64f, 0.58f); drawSquareLine();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.4f, -1.0f + legRightLift, 1.0f); glScalef(0.125f, 0.4f, 1.0f);
			glColor3f(0.90f, 0.85f, 0.79f); drawSquare();
			glColor3f(0.705f, 0.64f, 0.58f); drawSquareLine();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.85f, -0.4f + (legRightLift / 3), 1.0f);
			glColor3f(0.96f, 0.93f, 0.89f); drawDisk(0.25);
			glColor3f(0.705f, 0.64f, 0.58f); drawDiskLine(0.25);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(1.2f, 0.7f, 1.0f);
			glColor3f(0.96f, 0.93f, 0.89f); drawDisk(0.6);
			glColor3f(0.705f, 0.64f, 0.58f); drawDiskLine(0.6);

			glPushMatrix();
				glTranslatef(0.3f, 0.1f, 0.0f);
				glColor3f(0.0f, 0.0f, 0.0f); drawDisk(0.065);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(0.6f, -0.2f, 0.0f);
				glColor3f(1.0f, 1.0f, 1.0f); drawDisk(0.2);
				glColor3f(0.705f, 0.64f, 0.58f); drawDiskLine(0.2);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(0.76f, -0.2f, 0.0f); glScalef(0.15f, 0.07f, 1.0f); glRotatef(-90, 0, 0, 1);
				glColor3f(0.90f, 0.42f, 0.54f); drawTriangle();
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(1.2f, 1.2f, 1.0f); glRotatef(10 - earLeftSwing, 0, 0, 1); glScalef(0.125f, 1.2f, 1.0f);
			glColor3f(0.96f, 0.93f, 0.89f); drawTriangle();
			glColor3f(0.705f, 0.64f, 0.58f); drawTriangleLine();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(1.6f, 1.2f, 1.0f); glRotatef(-10 - earRightSwing, 0, 0, 1); glScalef(0.125f, 1.2f, 1.0f);
			glColor3f(0.96f, 0.93f, 0.89f); drawTriangle();
			glColor3f(0.705f, 0.64f, 0.58f); drawTriangleLine();
		glPopMatrix();
	glPopMatrix();
}

void moverCoelho() {
	if (leftArrowPressed) {
		direcaoCoelho = -1.0f;
		if (characterPos < -8.0f) characterPos -= 0;
		else characterPos -= characterSpeed;
	}
	if (rightArrowPressed) {
		direcaoCoelho = 1.0f;
		if (characterPos > 8.0f) characterPos += 0;
		else characterPos += characterSpeed;
	}
	if (isJumping) {
		if (goingUp) {
			jump_height += speed_jump;
			if (jump_height >= jump_maximum_height) {
				jump_height = jump_maximum_height;
				goingUp = false;
			}
		} else {
			jump_height -= speed_jump;
			if (jump_height <= 0.5f) {
				jump_height = 0.5f;
				isJumping = false;
			}
		}
	}
}

void animarCoelho() {
    walkPhase += walkPhaseSpeed;
    if (walkPhase > 2 * PI) walkPhase -= 2 * PI;
}

void aplicarBonusDoVegetal(TipoVegetal tipo){
	switch (tipo) {
		case CENOURA: framesDeTurboRestantes = DURACAO_TURBO_EM_FRAMES; break;
		case RABANETE: framesDePuloReforcadoRestantes = DURACAO_PULO_REFORCADO_EM_FRAMES; break;
		case ALFACE: if (rabbitLives < MAX_VIDAS) rabbitLives++; break;
	}
}

void atualizarBonusAtivos(){
	if (framesDeTurboRestantes > 0) {
		framesDeTurboRestantes--;
		characterSpeed = VELOCIDADE_TURBO;
		if (framesDeTurboRestantes == 0) characterSpeed = VELOCIDADE_NORMAL;
	}
	if (framesDePuloReforcadoRestantes > 0) {
		framesDePuloReforcadoRestantes--;
		jump_maximum_height = PULO_REFORCADO;
		if (framesDePuloReforcadoRestantes == 0) jump_maximum_height = PULO_NORMAL;
	}
}

void atualizarCorrida(){
	if (coelhoEscondido) {
		coelhoCorrendo = false;
		folegoAtual += RECUPERACAO_FOLEGO_POR_FRAME * 2.0f;
		if (folegoAtual > FOLEGO_MAXIMO) folegoAtual = FOLEGO_MAXIMO;
		return;
	}
	if (r_key_pressed && folegoAtual > 0.0f) {
		coelhoCorrendo = true;
		folegoAtual -= GASTO_FOLEGO_POR_FRAME;
		if (folegoAtual < 0.0f) folegoAtual = 0.0f;
	} else {
		coelhoCorrendo = false;
		folegoAtual += RECUPERACAO_FOLEGO_POR_FRAME;
		if (folegoAtual > FOLEGO_MAXIMO) folegoAtual = FOLEGO_MAXIMO;
	}
	if (framesDeTurboRestantes == 0) {
		characterSpeed = coelhoCorrendo ? VELOCIDADE_CORRENDO : VELOCIDADE_NORMAL;
	}
}
