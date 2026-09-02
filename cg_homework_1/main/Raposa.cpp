#include "Raposa.hpp"
#include "Primitivas.hpp"
#include "Coelho.hpp"
#include "Cenario.hpp"
#include "Animacao.hpp"
#include <cstdlib>

float foxWalkPhase = 0.0f;
float foxWalkPhaseSpeed = 0.22f;
float foxLegLiftAmount = 0.15f;
float foxEarSwingAmount = 8.0f;
float foxTailPhase = 0.0f;
float foxTailPhaseSpeed = 0.10f;
float foxTailSwingAmount = 6.0f;
float foxTailTipPhase = 0.0f;
float foxTailTipPhaseSpeed = 0.17f;
float foxTailTipSwingAmount = 12.0f;

bool foxActive = false;
float foxX = -12.0f;
float foxY = 0.5f;
float foxDirecao = 1.0f;
const float VELOCIDADE_RAPOSA = 0.11f;
const float RAIO_COLISAO_RAPOSA = 1.1f;
bool foxJaTirouVidaNestaPassagem = false;

#define FRAMES_POR_SEGUNDO (1000 / msecs)
int framesAteProximaRaposa = 20 * (1000 / 24); // Inicial base

void drawFox(){
	float foxPhaseSin = sin(foxWalkPhase);
	float earRightSwing = (foxPhaseSin > 0.0f) ?  foxPhaseSin * foxEarSwingAmount : 0.0f;
	float earLeftSwing  = (foxPhaseSin < 0.0f) ? -foxPhaseSin * foxEarSwingAmount : 0.0f;
	float legGroupALift = (foxPhaseSin > 0.0f)  ?  foxPhaseSin * foxLegLiftAmount : 0.0f;
	float legGroupBLift = (foxPhaseSin < 0.0f)  ? -foxPhaseSin * foxLegLiftAmount : 0.0f;
	float tailSwing    = sin(foxTailPhase)    * foxTailSwingAmount;
	float tailTipSwing = sin(foxTailTipPhase) * foxTailTipSwingAmount;

    float orangeR = 0.95f, orangeG = 0.45f, orangeB = 0.10f;
    float orangeDarkR = 0.80f, orangeDarkG = 0.35f, orangeDarkB = 0.08f;
    float whiteR  = 1.00f, whiteG  = 1.00f, whiteB  = 1.00f;
    float darkLineR = 0.65f, darkLineG = 0.25f, darkLineB = 0.05f;

    glPushMatrix();
        glPushMatrix();
            glTranslatef(-0.8f, -0.6f + legGroupALift, -0.1f); glScalef(0.12f, 0.4f, 1.0f);
            glColor3f(orangeDarkR, orangeDarkG, orangeDarkB); drawSquare();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.6f, -0.6f + legGroupBLift, -0.1f); glScalef(0.12f, 0.4f, 1.0f);
            glColor3f(orangeDarkR, orangeDarkG, orangeDarkB); drawSquare();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-1.1f, 0.0f, 0.5f); glRotatef(30.0f + tailSwing, 0, 0, 1);
            glPushMatrix();
                glTranslatef(-0.20f, -0.09f, 0.0f); glScalef(0.65f, 0.25f, 1.0f);
                glColor3f(orangeR, orangeG, orangeB); drawSquare();
                glColor3f(darkLineR, darkLineG, darkLineB); drawSquareLine();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(-0.80f, -0.05f, 0.0f); glRotatef(80.0f + tailTipSwing, 0, 0, 1); glScalef(0.28f, 1.05f, 1.0f);
                glColor3f(orangeR, orangeG, orangeB); drawTriangle();
                glColor3f(darkLineR, darkLineG, darkLineB); drawTriangleLine();
                glColor3f(whiteR, whiteG, whiteB);
                glPushMatrix();
                    glTranslatef(0.0f, 0.5f, 0.0f); glScalef(0.5f, 0.5f, 1.0f); drawTriangle();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();
            glScalef(1.0f, 0.40f, 1.0f);
            glColor3f(orangeR, orangeG, orangeB); drawSquare();
            glColor3f(darkLineR, darkLineG, darkLineB); drawSquareLine();
        glPopMatrix();

        glColor3f(whiteR, whiteG, whiteB);
        glPushMatrix(); glScalef(1.0f, 0.1f, 1.0f); glTranslatef(0.0f, -3.0f, 1.0f); drawSquare(); glPopMatrix();

        glPushMatrix();
            glTranslatef(1.0f, 0.4f, 1.0f); glScalef(0.15f, 0.6f, 1.0f); glRotatef(-50, 0, 0, 1);
            glColor3f(orangeR, orangeG, orangeB); drawSquare();
            glColor3f(darkLineR, darkLineG, darkLineB); drawSquareLine();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-0.6f, -0.65f + legGroupBLift, 1.0f); glScalef(0.13f, 0.42f, 1.0f);
            glColor3f(orangeR, orangeG, orangeB); drawSquare();
            glColor3f(darkLineR, darkLineG, darkLineB); drawSquareLine();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.8f, -0.65f + legGroupALift, 1.0f); glScalef(0.13f, 0.42f, 1.0f);
            glColor3f(orangeR, orangeG, orangeB); drawSquare();
            glColor3f(darkLineR, darkLineG, darkLineB); drawSquareLine();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(1.35f, 0.7f, 1.0f);
            glColor3f(orangeR, orangeG, orangeB); drawDisk(0.45);
            glColor3f(darkLineR, darkLineG, darkLineB); drawDiskLine(0.45);

            glPushMatrix();
                glTranslatef(0.25f, -0.15f, 0.0f); glRotatef(-115.0f, 0, 0, 1); glScalef(0.35f, 0.7f, 1.0f);
                glColor3f(orangeR, orangeG, orangeB); drawTriangle();
                glColor3f(whiteR, whiteG, whiteB);
                glPushMatrix();
                    glTranslatef(0.3f, 0.2f, 0.0f); glRotatef(40.0f, 0, 0, 1); glScalef(0.4f, 0.8f, 1.0f); drawSquare();
                glPopMatrix();
                glColor3f(0.05f, 0.05f, 0.05f);
                glPushMatrix(); glTranslatef(0.0f, 0.95f, 0.0f); drawDisk(0.18); glPopMatrix();
            glPopMatrix();

            glColor3f(0.0f, 0.0f, 0.0f);
            glPushMatrix(); glTranslatef(0.25f, 0.09f, 0.1f); drawDisk(0.06); glPopMatrix();

            glPushMatrix();
                glTranslatef(-0.2f, 0.35f, 0.0f); glRotatef(15.0f - earLeftSwing, 0, 0, 1); glScalef(0.2f, 0.6f, 1.0f);
                glColor3f(orangeR, orangeG, orangeB); drawTriangle();
                glColor3f(darkLineR, darkLineG, darkLineB); drawTriangleLine();
                glColor3f(whiteR, whiteG, whiteB); glScalef(0.5f, 0.6f, 1.0f); drawTriangle();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(0.35f, 0.3f, 0.0f); glRotatef(-35.0f - earRightSwing, 0, 0, 1); glScalef(0.18f, 0.5f, 1.0f);
                glColor3f(orangeR, orangeG, orangeB); drawTriangle();
                glColor3f(darkLineR, darkLineG, darkLineB); drawTriangleLine();
                glColor3f(whiteR, whiteG, whiteB); glScalef(0.5f, 0.6f, 1.0f); drawTriangle();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void animarRaposa() {
    foxWalkPhase += foxWalkPhaseSpeed;
    if (foxWalkPhase > 2 * PI) foxWalkPhase -= 2 * PI;
    foxTailPhase += foxTailPhaseSpeed;
    if (foxTailPhase > 2 * PI) foxTailPhase -= 2 * PI;
    foxTailTipPhase += foxTailTipPhaseSpeed;
    if (foxTailTipPhase > 2 * PI) foxTailTipPhase -= 2 * PI;
}

void spawnRaposa(){
	foxActive = true;
	foxX = -11.0f;
	foxJaTirouVidaNestaPassagem = false;
}

void controlarSurgimentoDaRaposa(){
	if (foxActive) return;
	framesAteProximaRaposa--;
	if (framesAteProximaRaposa <= 0) {
		spawnRaposa();
		framesAteProximaRaposa = (1 + rand() % 15) * FRAMES_POR_SEGUNDO;
	}
}

void moverRaposa(){
	if (!foxActive) return;
	foxX += VELOCIDADE_RAPOSA;
	foxDirecao = 1.0f;
	if (foxX > 10.0f) {
		foxActive = false;
		framesAteProximaRaposa = (15 + rand() % 31) * FRAMES_POR_SEGUNDO;
	}
}

void verificarColisaoComRaposa(){
	if (!foxActive || coelhoEscondido) return;

	float dx = foxX - characterPos;
	float dy = foxY - jump_height;
	float distancia = sqrt(dx * dx + dy * dy);

	if (distancia < RAIO_COLISAO_RAPOSA) {
		if (!foxJaTirouVidaNestaPassagem) {
			if (rabbitLives > 0) rabbitLives--;
			foxJaTirouVidaNestaPassagem = true;
		}
	} else {
		foxJaTirouVidaNestaPassagem = false;
	}
}
