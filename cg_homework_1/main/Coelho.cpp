#include "Coelho.hpp"
#include "PrimitivasGeometricas.hpp"
#include "Cenario.hpp"
#include <GL/glut.h>
#include <cmath>

// Variaveis para gerenciar a pos do coelho
float characterPos = 0.0f;
float characterSpeed = 0.05f;

// Variaveis para detectar os inputs das arrow keys (setinhas) do teclado
bool rightArrowPressed = false;
bool leftArrowPressed = false;
bool upArrowPressed = false;
bool downArrowPressed = false;

// Variaveis que controlam a velocidade do coelho
const float VELOCIDADE_NORMAL = 0.05f;
const float VELOCIDADE_CORRENDO = 0.10f;

// Variaveis que controlam a questao do turbo do coelho (quando ele come uma cenoura)
const float VELOCIDADE_TURBO = 0.20f;
const int DURACAO_TURBO_EM_FRAMES = 150;
int framesDeTurboRestantes = 0;

// Variaveis que gerenciam o folego do coelho
const float FOLEGO_MAXIMO = 100.0f;
float folegoAtual = FOLEGO_MAXIMO;
const float GASTO_FOLEGO_POR_FRAME = 0.6f;
const float RECUPERACAO_FOLEGO_POR_FRAME = 0.3f;
const float RECUPERACAO_FOLEGO_ESCONDIDO = RECUPERACAO_FOLEGO_POR_FRAME * 2.0f;
bool coelhoCorrendo = false;

//////////////////////////////////////////////////////////
// Variaveis que gerenciam o pulo do coelho
float jump_maximum_height = 2.5f;
float speed_jump = 0.1f;
float jump_height = 0.5f;
bool isJumping = false;
bool goingUp = true;


// Variaveis que gerenciam o buff do pulo do coelho (qdo ele come o rabanete)
const float PULO_NORMAL = 2.5f;
const float PULO_REFORCADO = 3.5f;
int framesDePuloReforcadoRestantes = 0;
const int DURACAO_PULO_REFORCADO_EM_FRAMES = 300;

//////////////////////////////////////////////////////////

// Variaveis que controlam a animacao de animacao do coelho
float walkPhase = 0.0f; // fase atual da animacao (em radianos)
float walkPhaseSpeed = 0.15f;  // velocidade com que a fase avanca a cada frame
float legLiftAmount = 0.2f; // o quanto a pata sobe no eixo Y
float earSwingAmount = 6.0f;  // o quanto a orelha "balanca" (em graus)
float direcaoCoelho = 1.0f; // determina a direcao que o coelho esta olhando

// Variaveis que gerenciam a quantidade de vidas do coelho
int rabbitLives = 3;
const int MAX_VIDAS = 3;

// Gerenciam se e em qual toca o coelho esta escondido
bool coelhoEscondido = false;
int tocaOndeEstaEscondido = -1;

void drawRabbit() {
	/*

	Calcula, a partir da fase atual da animacao, o quanto cada pata deve
	subir no eixo Y e o quanto cada orelha deve balancar para frente.
	A logica liga as duas: quando a orelha direita esta indo para frente
	(senoide positiva), a perna ESQUERDA sobe; quando a orelha esquerda
	esta indo para frente (senoide negativa), a perna DIREITA sobe.

	*/
    float phaseSin = sin(walkPhase);

    float earRightSwing = (phaseSin > 0.0f) ?  phaseSin * earSwingAmount : 0.0f;  // orelha direita. "pra frente" quando > 0
    float earLeftSwing  = (phaseSin < 0.0f) ? -phaseSin * earSwingAmount : 0.0f;  // orelha esquerda "pra frente" quando > 0 (fase oposta)

    float legLeftLift  = (phaseSin > 0.0f)  ?  phaseSin * legLiftAmount : 0.0f; // sobe junto com a orelha direita
    float legRightLift = (phaseSin < 0.0f)  ? -phaseSin * legLiftAmount : 0.0f; // sobe junto com a orelha esquerda

    // CORPO -> nó PAI (raiz) da hierarquia do coelho.
    glPushMatrix(); // abre o sistema de coordenadas do CORPO (nó pai)

        glColor3f(0.96f, 0.93f, 0.89f);
        drawDisk(1);
        glColor3f(0.705f, 0.64f, 0.58f);
        drawDiskLine(1);

        // -------- Pata esquerda (filha do corpo) --------
		/* Modelada em seu proprio sistema de coordenadas (um quadrado centrado na origem)
		 e depois posicionada/escalada em relacao ao corpo.

        */
        glPushMatrix();
            glTranslatef(-0.4f, -1.0f + legLeftLift, 1.0f);
            glScalef(0.125f, 0.4f, 1.0f);
            glColor3f(0.90f, 0.85f, 0.79f);
            drawSquare();
            glColor3f(0.705f, 0.64f, 0.58f);
            drawSquareLine();
        glPopMatrix();

        // -------- Pata direita (filha do corpo) --------
        glPushMatrix();
            glTranslatef(0.4f, -1.0f + legRightLift, 1.0f);
            glScalef(0.125f, 0.4f, 1.0f);
            glColor3f(0.90f, 0.85f, 0.79f);
            drawSquare();
            glColor3f(0.705f, 0.64f, 0.58f);
            drawSquareLine();
        glPopMatrix();

		// -------- Rabo (filho do corpo) --------
		// Modelado como um disco centrado na origem, depois posicionado
		// na traseira do corpo.
        glPushMatrix();
            glTranslatef(-0.85f, -0.4f + (legRightLift / 3), 1.0f);
            glColor3f(0.96f, 0.93f, 0.89f);
            drawDisk(0.25);
            glColor3f(0.705f, 0.64f, 0.58f);
            drawDiskLine(0.25);
        glPopMatrix();


        // -------- Cabeça (filha do corpo, e também PAI de olho/focinho/nariz) --------
        glPushMatrix();
            glTranslatef(1.2f, 0.7f, 1.0f);

            glColor3f(0.96f, 0.93f, 0.89f);
            drawDisk(0.6);
            glColor3f(0.705f, 0.64f, 0.58f);
            drawDiskLine(0.6);

            //Olho
            glPushMatrix();
                glTranslatef(0.3f, 0.1f, 0.0f); // deslocamento a partir do centro da cabeça
                glColor3f(0.0f, 0.0f, 0.0f);
                drawDisk(0.065);
            glPopMatrix();


            // Focinho (filho da cabeça)
            glPushMatrix();
                glTranslatef(0.6f, -0.2f, 0.0f);
                glColor3f(1.0f, 1.0f, 1.0f);
                drawDisk(0.2);
                glColor3f(0.705f, 0.64f, 0.58f);
                drawDiskLine(0.2);
            glPopMatrix();

            // Nariz (filho da cabeça)
            glPushMatrix();
                glTranslatef(0.76f, -0.2f, 0.0f);
                glScalef(0.15f, 0.07f, 1.0f);
                glRotatef(-90, 0, 0, 1);
                glColor3f(0.90f, 0.42f, 0.54f);
                drawTriangle();
            glPopMatrix();

        glPopMatrix(); // fecha o sistema de coordenadas da CABEÇA


        // -------- Orelha esquerda (filha do corpo) --------
        glPushMatrix();
            glTranslatef(1.2f, 1.2f, 1.0f);
            glRotatef(10 - earLeftSwing, 0, 0, 1);
            glScalef(0.125f, 1.2f, 1.0f);
            glColor3f(0.96f, 0.93f, 0.89f);
            drawTriangle();
            glColor3f(0.705f, 0.64f, 0.58f);
            drawTriangleLine();
        glPopMatrix();

        // -------- Orelha direita (filha do corpo) --------
        glPushMatrix();
            glTranslatef(1.6f, 1.2f, 1.0f);
            glRotatef(-10 - earRightSwing, 0, 0, 1);
            glScalef(0.125f, 1.2f, 1.0f);
            glColor3f(0.96f, 0.93f, 0.89f);
            drawTriangle();
            glColor3f(0.705f, 0.64f, 0.58f);
            drawTriangleLine();
        glPopMatrix();

    glPopMatrix();  // fecha o sistema de coordenadas do CORPO (nó pai)
}

// Verifica se o coelho esta escondido dentro da toca do cenario
void atualizarEsconderijoDoCoelho() {
    if (coelhoEscondido) {
        if (!downArrowPressed || leftArrowPressed || rightArrowPressed || upArrowPressed) {
            coelhoEscondido = false;
            tocaOndeEstaEscondido = -1;
        }
        return;
    }

    if (!downArrowPressed || isJumping) {
    	return;
    }

    // Verifica onde o coelho esta escondido
    for (int i = 0; i < (int)tocas.size(); i++) {
        float distancia = float(fabs(tocas[i].x - characterPos));
        if (distancia < RAIO_TOCA) {
            coelhoEscondido = true;
            tocaOndeEstaEscondido = i;
            break;
        }
    }
}

// Atualiza a questao dos buffs dados ao coelho (pulo e velocidade extra)
void atualizarBonusAtivos() {
    if (framesDeTurboRestantes > 0) {
        framesDeTurboRestantes--;
        characterSpeed = VELOCIDADE_TURBO;
        if (framesDeTurboRestantes == 0) {
            characterSpeed = VELOCIDADE_NORMAL;
        }
    }

    if (framesDePuloReforcadoRestantes > 0) {
        framesDePuloReforcadoRestantes--;
        jump_maximum_height = PULO_REFORCADO;
        if (framesDePuloReforcadoRestantes == 0) {
            jump_maximum_height = PULO_NORMAL;
        }
    }
}

// Gerencia o sistema de folego/corrida do coelho
void atualizarCorrida() {
    if (coelhoEscondido) {
        coelhoCorrendo = false;
        folegoAtual += RECUPERACAO_FOLEGO_ESCONDIDO;
        if (folegoAtual > FOLEGO_MAXIMO) {
        	folegoAtual = FOLEGO_MAXIMO;
        }
        return;
    }

    if (r_key_pressed && folegoAtual > 0.0f) {
        coelhoCorrendo = true;
        folegoAtual -= GASTO_FOLEGO_POR_FRAME;
        if (folegoAtual < 0.0f) {
        	folegoAtual = 0.0f;
        }
    } else {
        coelhoCorrendo = false;
        folegoAtual += RECUPERACAO_FOLEGO_POR_FRAME;
        if (folegoAtual > FOLEGO_MAXIMO) {
        	folegoAtual = FOLEGO_MAXIMO;
        }
    }

    if (framesDeTurboRestantes == 0) {
        characterSpeed = coelhoCorrendo ? VELOCIDADE_CORRENDO : VELOCIDADE_NORMAL;
    }
}
