#include "Cenario.hpp"
#include "PrimitivasGeometricas.hpp"
#include "Borboleta.hpp"
#include "Coelho.hpp" // Para acessar rabbitLives e rabbit-related globals na hora de aplicar bônus
// Dependência de Animacao.hpp removida
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>

float bgSpeed = 0.1f;
float bgWidth = 40.0f;
float bgPos = 0.0f;

std::vector<Vegetal> vegetais;
const int MAX_VEGETAIS = 15;
int framesAteProximoVegetal = 60;

std::vector<Toca> tocas; // Apenas declara o vetor vazio

void initCenario() {
    // Declara as structs e preenche os atributos explicitamente
    Toca t1;
    t1.x = 9.0f;
    t1.y = 0.35f;

    Toca t2;
    t2.x = 20.0f;
    t2.y = 0.35f;

    // Agora o compilador reconhecerá os objetos sem ambiguidade
    tocas.push_back(t1);
    tocas.push_back(t2);
}

const float RAIO_TOCA = 0.8f;

float cloudPhase = 0.0f;
float cloudPhaseSpeed = 0.02f;
float cloudSideSwingAmount = 0.15f;
float cloudMiddleBobAmount = 0.1f;

float tempoDeDiaFase = 0.0f;
const float VELOCIDADE_CICLO_DIA = 1.0f / (60.0f * (1000.0f / 24.0f));
float skyR = 0.68f, skyG = 0.81f, skyB = 0.98f;

void drawSun() {
    int i;
    glColor3f(1.0f, 0.823f, 0.298f);
    glLineWidth(3);
    glRotatef(float(-FrameNumber), 0, 0, 1);
    glBegin(GL_LINES);
    for (i = 0; i < 15; i++) {
        glVertex2f(0, 0);
        glVertex2d(cos(i*2*PI/15), sin(i*2*PI/15));
    }
    glEnd();
    drawDisk(0.65);
    glColor3f(0, 0, 0);
}

void drawCloud() {
    float cloudSin = sin(cloudPhase);
    float sideOffsetX   = cloudSin * cloudSideSwingAmount;
    float middleOffsetY = cloudSin * cloudMiddleBobAmount;

    glColor3f(1, 1, 1);
    glPushMatrix();
        glTranslatef(-0.6f + sideOffsetX, -0.2f, 1);
        drawDisk(0.5f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, -0.1f + middleOffsetY, 1);
        drawDisk(0.7f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.6f + sideOffsetX, -0.2f, 1);
        drawDisk(0.5f);
    glPopMatrix();
}

void drawFence() {
    glColor3f(0.662f, 0.443f, 0.247f);
    glPushMatrix();
        glTranslatef(-1.2f, 0.2f, 0.0f);
        glScalef(0.15f, 1.0f, 1.0f);
        drawSquare();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(1.2f, 0.2f, 0.0f);
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
        glTranslatef(0.0f, 0.8f, 0.0f);
        glScalef(1.5f, 0.15f, 1.0f);
        drawSquare();
    glPopMatrix();
}

void drawFruit(float red, float green, float blue) {
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

void drawCarrot() {
    glColor3f(0.243f, 0.556f, 0.180f);
    glPushMatrix();
        glRotatef(-45, 0, 0, 1);
        glPushMatrix();
            glTranslatef(0.0f, 0.25f, 0.0f);
            glScalef(0.1f, 0.8f, 1.0f);
            glRotatef(180, 0, 0, 1);
            drawTriangle();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.2f, 0.0f);
            glScalef(0.25f, 0.5f, 1.0f);
            glRotatef(18, 0, 0, 1);
            drawTriangle();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.2f, 0.0f);
            glScalef(0.22f, 0.5f, 1.0f);
            glRotatef(-18, 0, 0, 1);
            drawTriangle();
        glPopMatrix();

        glColor3f(0.95f, 0.52f, 0.13f);
        glPushMatrix();
            glScalef(0.15f, 1.1f, 1.0f);
            glRotatef(180, 0, 0, 1);
            drawTriangle();
        glPopMatrix();
    glPopMatrix();
}

void drawLettuce() {
    glColor3f(0.25f, 0.55f, 0.15f);
    glPushMatrix();
        glTranslatef(-0.15f, 0.15f, 1.0f);
        drawDisk(0.29f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.18f, 0.08f, 1.0f);
        drawDisk(0.26f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.02f, -0.18f, 1.0f);
        drawDisk(0.26f);
    glPopMatrix();

    glColor3f(0.40f, 0.70f, 0.20f);
    glPushMatrix();
        glTranslatef(-0.08f, -0.10f, 1.0f);
        drawDisk(0.28f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.10f, -0.05f, 1.0f);
        drawDisk(0.26f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.05f, 0.10f, 1.0f);
        drawDisk(0.25f);
    glPopMatrix();

    glColor3f(0.30f, 0.55f, 0.15f);
    glPushMatrix();
        glTranslatef(-0.08f, -0.10f, 1.0f);
        drawDiskLine(0.28f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.10f, -0.05f, 1.0f);
        drawDiskLine(0.26f);
    glPopMatrix();

    glColor3f(0.60f, 0.85f, 0.25f);
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 1.0f);
        drawDisk(0.20f);
    glPopMatrix();

    glColor3f(0.45f, 0.75f, 0.20f);
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 1.0f);
        drawDiskLine(0.20f);
    glPopMatrix();
}

void drawRadish() {
    glColor3f(0.243f, 0.556f, 0.180f);
    glPushMatrix();
        glTranslatef(0.0f, 0.4f, 0.0f);
        glScalef(0.15f, 0.39f, 1.0f);
        glRotatef(180, 0, 0, 1);
        drawTriangle();
    glPopMatrix();

    glColor3f(0.86f, 0.24f, 0.35f);
    glPushMatrix();
        drawDisk(0.22f);
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
        glTranslatef(0.0f, -0.22f, 0.0f);
        drawDisk(0.1f);
    glPopMatrix();
}

void drawVegetable(TipoVegetal tipo) {
    switch (tipo) {
        case CENOURA: drawCarrot(); break;
        case ALFACE: drawLettuce(); break;
        case RABANETE: drawRadish(); break;
    }
}

void spawnVegetable() {
    TipoVegetal tipoSorteado = static_cast<TipoVegetal>(rand() % 3);
    float y;
    if (rand() % 100 < 70) {
        y = 0.2f + (rand() % 60) / 100.0f;
    } else {
        y = 1.8f + (rand() % 120) / 100.0f;
    }

    float x = 9.0f + (rand() % 300) / 100.0f;

    for (Vegetal &veg : vegetais) {
        if (!veg.ativo) {
            veg.tipo = tipoSorteado;
            veg.x = x;
            veg.y = y;
            veg.ativo = true;
            return;
        }
    }

    if ((int)vegetais.size() < MAX_VEGETAIS) {
        Vegetal novoVegetal = { tipoSorteado, x, y, true };
        vegetais.push_back(novoVegetal);
    }
}

void aplicarBonusDoVegetal(TipoVegetal tipo) {
    switch (tipo) {
        case CENOURA:
            framesDeTurboRestantes = DURACAO_TURBO_EM_FRAMES;
            break;
        case RABANETE:
            framesDePuloReforcadoRestantes = DURACAO_PULO_REFORCADO_EM_FRAMES;
            break;
        case ALFACE:
            if (rabbitLives < MAX_VIDAS) {
                rabbitLives++;
            }
            break;
    }
}

void drawText(float x, float y, const char *texto) {
    glRasterPos3f(x, y, 1.0f);
    for (const char *c = texto; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void drawToca() {
	//Desenha a grama envolta da toca
		glColor3f(0.549f, 0.776f, 0.247f);
		glPushMatrix();
			glTranslatef(-0.1f, -0.2f, 1.0f);
			glScalef(1.25f, 0.5f, 1.0f);
			drawSquare();
		glPopMatrix();

		glColor3f(0.549f, 0.776f, 0.247f);
		glPushMatrix();
			glTranslatef(-0.1f, 0.2f, 1.0f);
			glScalef(1.25f, 1.35f, 1.0f);
			drawDisk(1.0f);
		glPopMatrix();


	//Desenha o buraco em volta da toca
		glColor3f(0.478f, 0.290f, 0.168f);
		glPushMatrix();
			glTranslatef(0.15f, 0.30f, 1.0f);
			glScalef(0.85f, 0.9f, 1.0f);
			drawDisk(1.0f);
		glPopMatrix();


		glColor3f(0.478f, 0.290f, 0.168f);
		glPushMatrix();
		glTranslatef(0.15f, 0.0f, 1.0f);
		   glScalef(0.85f, 0.5f, 1.0f);
		   drawSquare();
		glPopMatrix();



		glColor3f(0.12f, 0.09f, 0.07f);
		glPushMatrix();
			glTranslatef(0.25f, 0.125f, 0.1f);
			glScalef(0.6f, 0.9f, 1.0f);
			drawDisk(1.0f);
		glPopMatrix();

}

void atualizarTocas() {
    for (int i = 0; i < (int)tocas.size(); i++) {
        if (coelhoEscondido && tocaOndeEstaEscondido == i) continue;

        tocas[i].x -= bgSpeed;

        if (tocas[i].x < -10.0f) {
            tocas[i].x = 9.0f + (rand() % 600) / 100.0f;
        }
    }
}

float interpolarCor(float inicio, float fim, float t) {
    return inicio + (fim - inicio) * t;
}

void atualizarCorDoCeu() {
    float diaR = 0.68f, diaG = 0.81f, diaB = 0.98f;
    float entardecerR = 0.95f, entardecerG = 0.55f, entardecerB = 0.40f;
    float noiteR = 0.07f, noiteG = 0.09f, noiteB = 0.22f;
    float amanhecerR = 0.95f, amanhecerG = 0.72f, amanhecerB = 0.58f;

    if (tempoDeDiaFase < 0.25f) {
        float t = tempoDeDiaFase / 0.25f;
        skyR = interpolarCor(diaR, entardecerR, t);
        skyG = interpolarCor(diaG, entardecerG, t);
        skyB = interpolarCor(diaB, entardecerB, t);
    } else if (tempoDeDiaFase < 0.5f) {
        float t = (tempoDeDiaFase - 0.25f) / 0.25f;
        skyR = interpolarCor(entardecerR, noiteR, t);
        skyG = interpolarCor(entardecerG, noiteG, t);
        skyB = interpolarCor(entardecerB, noiteB, t);
    } else if (tempoDeDiaFase < 0.75f) {
        float t = (tempoDeDiaFase - 0.5f) / 0.25f;
        skyR = interpolarCor(noiteR, amanhecerR, t);
        skyG = interpolarCor(noiteG, amanhecerG, t);
        skyB = interpolarCor(noiteB, amanhecerB, t);
    } else {
        float t = (tempoDeDiaFase - 0.75f) / 0.25f;
        skyR = interpolarCor(amanhecerR, diaR, t);
        skyG = interpolarCor(amanhecerG, diaG, t);
        skyB = interpolarCor(amanhecerB, diaB, t);
    }

    tempoDeDiaFase += VELOCIDADE_CICLO_DIA;
    if (tempoDeDiaFase > 1.0f) {
        tempoDeDiaFase -= 1.0f;
    }
}

void drawBackgroundContent() {

    glPushMatrix();
        glTranslatef(16.0f, 0.5f, 1.0f);
        glScalef(1.5f, 1.0f, 1.0f);
        drawFence();
    glPopMatrix();

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

    // Canteiro 1
    glPushMatrix();
        glColor3f(0.478f, 0.290f, 0.168f);
        glPushMatrix();
            glTranslatef(24.0f, -1.5f, 1.0f);
            glScalef(2.5f, 1.0f, 1.0f);
            drawSquare();
        glPopMatrix();
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

        glColor3f(0.243f, 0.556f, 0.180f);
        glPushMatrix();
            glTranslatef(24.9f, 0.5f, 1.0f);
            glScalef(0.8f, 1.1f, 1.0f);
            glRotatef(180, 0, 0, 1);
            drawTriangle();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(23.2f, 0.5f, 1.0f);
            glScalef(0.5f, 1.3f, 1.0f);
            glRotatef(180, 0, 0, 1);
            drawTriangle();
        glPopMatrix();
    glPopMatrix();

    // Canteiro 2
    glPushMatrix();
        glColor3f(0.478f, 0.290f, 0.168f);
        glPushMatrix();
            glTranslatef(29.0f, -1.5f, 1.0f);
            glScalef(2.0f, 1.0f, 1.0f);
            drawSquare();
        glPopMatrix();

        glPushMatrix(); glTranslatef(29.2f, -1.5f, 1.0f); glScalef(1.2f, 1.2f, 1.0f); drawFruit(0.839f, 0.270f, 0.313f); glPopMatrix();
        glPushMatrix(); glTranslatef(30.1f, -1.8f, 1.0f); glScalef(1.5f, 1.5f, 1.0f); drawFruit(0.839f, 0.270f, 0.313f); glPopMatrix();
        glPushMatrix(); glTranslatef(30.8f, -1.3f, 1.0f); drawFruit(0.839f, 0.270f, 0.313f); glPopMatrix();

        glColor3f(0.243f, 0.556f, 0.180f);
        glPushMatrix();
            glTranslatef(29.6f, 0.5f, 1.0f);
            glScalef(0.9f, 1.5f, 1.0f);
            glRotatef(180, 0, 0, 1);
            drawTriangle();
        glPopMatrix();
    glPopMatrix();

    // Borboletas
    glPushMatrix(); glTranslatef(23.0f, 0.9f, 1.0f); drawButterfly(0.0f, 0.95f, 0.55f, 0.15f); glPopMatrix();
    glPushMatrix(); glTranslatef(25.6f, 1.3f, 1.0f); drawButterfly(2.0f, 0.75f, 0.35f, 0.85f); glPopMatrix();
    glPushMatrix(); glTranslatef(29.6f, 1.0f, 1.0f); drawButterfly(4.0f, 0.95f, 0.85f, 0.20f); glPopMatrix();

    glPushMatrix(); glTranslatef(4.0f, 5.0f, 1.0f); drawCloud(); glPopMatrix();
    glPushMatrix(); glTranslatef(44.0f, 5.0f, 1.0f); drawCloud(); glPopMatrix();
    glPushMatrix(); glTranslatef(4.0f, 0.5f, 1.0f); drawFence(); glPopMatrix();
    glPushMatrix(); glTranslatef(44.0f, 0.5f, 1.0f); drawFence(); glPopMatrix();
}


void drawBackgroundSky() {
    glColor3f(skyR, skyG, skyB);
    glPushMatrix();
        glScalef(bgWidth + (bgWidth / 2), 10, 1);
        drawSquare();
    glPopMatrix();

    glColor3f(0.549f, 0.776f, 0.247f);
    glPushMatrix();
        glTranslatef(0.0f, -4.0f, 1.0f);
        glScalef(bgWidth + (bgWidth / 2), 4.0f, 1);
        drawSquare();
    glPopMatrix();

}

void drawBackgroundGrass(){

	 glColor3f(0.549f, 0.776f, 0.247f);
	    glPushMatrix();
	        glTranslatef(0.0f, -4.0f, 1.0f);
	        glScalef(bgWidth + (bgWidth / 2), 4.0f, 1);
	        drawSquare();
	    glPopMatrix();
}

void moverVegetais() {
    for (Vegetal &veg : vegetais) {
        if (!veg.ativo) continue;
        veg.x -= bgSpeed;
        if (veg.x < -10.0f) {
            veg.ativo = false;
        }
    }
}

//Controla o tempo de spawn dos vegetais do cenario
void controlarSurgimentoDeVegetais() {
    framesAteProximoVegetal--;
    if (framesAteProximoVegetal <= 0) {
        spawnVegetable();
        framesAteProximoVegetal = 120 + (rand() % 80);
    }
}
