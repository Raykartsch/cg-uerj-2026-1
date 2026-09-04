#include "AveRapina.hpp"
#include "PrimitivasGeometricas.hpp"
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Determina o estado da ave e se ela existe na cena
bool aveActive = false;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Posicao atual da ave na tela (eixo X e Y). aveX comeca sempre no mesmo
lugar (bem a direita, fora da tela) e vai diminuindo a cada frame; aveY e
recalculado a cada frame para formar a curva do mergulho (veja moverAve).
*/

float aveX = 12.0f;
float aveY = 7.0f;
float avePhase = 0.0f;
bool aveJaTirouVida = false;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Velocidade horizontal da ave, quanto mais o aveX diminui a cada frame enquanto ela atravessa a tela da direita para a esquerda.
const float VELOCIDADE_AVE_X = 0.18f;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tempo definido para a proxima aparicao do inimigo
int framesAteProximaAve = 25 * (1000 / 24);

float aveAlvoX = 0.0f; // Coordenada X onde será o ponto mais baixo do mergulho da ave

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha o corpo da ave de rapina

void drawBird() {

	/*Avanca a fase do bater de asas a cada frame que a ave e desenhada (ou
	 seja, so enquanto ela esta ativa/visivel), e mantem o valor sempre
	 dentro de uma volta completa (0 a 2*PI), sem crescer pra sempre.
	*/
    avePhase += 0.35f;
    if (avePhase > 2 * 3.1415f) {
    	avePhase -= 2 * 3.1415f;
    }

    /*
    O seno da fase da um valor que oscila suavemente entre -1 e 1: e ele
    que faz as asas subirem e descerem (usado no glRotatef das asas, mais
    abaixo, multiplicado por 50 graus de amplitude).
     */
    float wingFlap = sin(avePhase);

    glPushMatrix();

    // Asa traseira do passaro
	   glColor3f(0.27f, 0.23f, 0.13f);
	   glPushMatrix();
		   glTranslatef(-0.2f, 0.1f, 0.0f);
		   glRotatef(wingFlap * 50.0f, 0, 0, 1);
		   glScalef(0.4f, 0.8f, 1.0f);
		   drawTriangle();

		   glColor3f(0.27f, 0.23f, 0.13f);
		   drawTriangleLine();

		   glColor3f(0.411f, 0.334f, 0.20f);
		   glScalef(0.6f, 0.5f, 1.0f);
		   drawTriangle();
	   glPopMatrix();

    	//glColor3f(0.411f, 0.334f, 0.20f); // Marrom escuro
        // Corpo
        glPushMatrix();
        	glColor3f(0.411f, 0.334f, 0.20f); // Marrom escuro
            glScalef(0.6f, 0.25f, 1.0f);
            drawDisk(1.0f);
            glColor3f(0.27f, 0.23f, 0.13f);
            drawDiskLine(1.0f);
        glPopMatrix();

        // Mancha da barriga
        glPushMatrix();
        	glTranslatef(0.0f, -0.1f, 1.0f);
        	glColor3f(0.96f, 0.93f, 0.89f);// Marrom escuro
		   glScalef(0.8f, 0.25f, 1.0f);
		   drawDisk(0.5f);
	   glPopMatrix();

        // Cabeca
		glPushMatrix();
			glColor3f(0.411f, 0.334f, 0.20f); // Marrom escuro
			glTranslatef(-0.7f, 0.1f, 1.0f);
			glScalef(0.20f, 0.16f, 1.0f);
			drawDisk(1.0f);
			glColor3f(0.27f, 0.23f, 0.13f);
			drawDiskLine(1.0f);
		glPopMatrix();

		//Detalhe cabeca
		glPushMatrix();
			glTranslatef(-0.7f, 0.125f, 1.0f);
			glScalef(0.45f, 0.16f, 1.0f);
			glColor3f(0.27f, 0.23f, 0.13f);
			drawDisk(0.4f);
		glPopMatrix();

		 glPushMatrix();
			glTranslatef(-0.7f, 0.125f, 0.0f);
			glColor3f(0.0f, 0.0f, 0.0f);
			drawDisk(0.032);
		glPopMatrix();

		//Moicano da cabeca
		glPushMatrix();
			glColor3f(0.411f, 0.334f, 0.20f); // Marrom escuro
			glTranslatef(-0.7f, 0.3f, 1.0f);
			glScalef(0.45f, 0.05f, 1.0f);
			glRotatef(-80, 0, 0, 1);
			drawTriangle();
			glColor3f(0.27f, 0.23f, 0.13f);
			drawTriangleLine();
		glPopMatrix();

		glPushMatrix();
			glColor3f(0.411f, 0.334f, 0.20f); // Marrom escuro
			glTranslatef(-0.7f, 0.35f, 1.0f);
			glScalef(0.45f, 0.05f, 1.0f);
			glRotatef(-105, 0, 0, 1);
			drawTriangle();
			glColor3f(0.27f, 0.23f, 0.13f);
			drawTriangleLine();
		glPopMatrix();


		// Rabo
		glPushMatrix();
			glColor3f(0.411f, 0.334f, 0.20f); // Marrom escuro
			glTranslatef(0.78f, 0.1f, 1.0f);
			glScalef(0.25f, 0.05f, 1.0f);
			glRotatef(-80, 0, 0, 1);
			drawSquare();
			glColor3f(0.27f, 0.23f, 0.13f);
			drawSquareLine();
		glPopMatrix();

		glPushMatrix();
			glColor3f(0.411f, 0.334f, 0.20f); // Marrom escuro
			glTranslatef(0.78f, 0.0f, 1.0f);
			glScalef(0.35f, 0.05f, 1.0f);
			glRotatef(-80, 0, 0, 1);
			drawSquare();
			glColor3f(0.27f, 0.23f, 0.13f);
			drawSquareLine();
		glPopMatrix();

		glPushMatrix();
			glColor3f(0.411f, 0.334f, 0.20f); // Marrom escuro
			glTranslatef(0.78f, -0.1f, 1.0f);
			glScalef(0.25f, 0.05f, 1.0f);
			glRotatef(-80, 0, 0, 1);
			drawSquare();
			glColor3f(0.27f, 0.23f, 0.13f);
			drawSquareLine();
		glPopMatrix();


        // Bico (virado para a esquerda)
		glColor3f(1.0f, 0.77f, 0.10f);
        glPushMatrix();
            glTranslatef(-0.92f, 0.0f, 0.0f);
            glScalef(0.12f, 0.10f, 1.0f);
            drawTriangle();
        glPopMatrix();

        glColor3f(1.0f, 0.77f, 0.10f);
		glPushMatrix();
			glTranslatef(-0.93f, 0.0f, 0.0f);
			glRotatef(90, 0, 0, 1);
			glScalef(0.12f, 0.10f, 1.0f);
			drawTriangle();
		glPopMatrix();

        // Asa mais proxima da tela
        glColor3f(0.27f, 0.23f, 0.13f);
        glPushMatrix();
            glTranslatef(0.1f, 0.3f, 0.0f);
            glRotatef(wingFlap * 50.0f, 0, 0, 1);
            glScalef(0.4f, 0.8f, 1.0f);
            drawTriangle();

            glColor3f(0.27f, 0.23f, 0.13f);
            drawTriangleLine();

            glColor3f(0.411f, 0.334f, 0.20f);
            glScalef(0.6f, 0.5f, 1.0f);
            drawTriangle();
        glPopMatrix();


    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Faz a ave aparecer na tela, pronta para iniciar o mergulho. Recebe a
 posicao X onde o coelho estava naquele instante (targetX), e guarda esse
 valor em aveAlvoX -- e esse ponto que vira o "fundo" da curva do mergulho,
 calculada depois em moverAve().*/

void spawnAve(float targetX) {
    aveActive = true;
    aveX = 12.0f;
    aveAlvoX = targetX; // Grava onde o coelho estava para fazer o vértice do mergulho
    aveJaTirouVida = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Controlar o tempo de surgimento da ave para atacar o coelho
void controlarSurgimentoDaAve(float coelhoX) {
    if (aveActive) {
    	return;
    }

    framesAteProximaAve--;
    if (framesAteProximaAve <= 0) {
        spawnAve(coelhoX);
        framesAteProximaAve = (25 + rand() % 20) * (1000 / 24); // Sorteia entre 25 e 45 segundos
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Move a ave pelo cenário
void moverAve() {
    if (!aveActive) return;


    aveX -= VELOCIDADE_AVE_X;

    // Cálculo da trajetória parabólica: Y = a(X - H)^2 + K
    // H = aveAlvoX (ponto mais baixo em X), K = 0.5f (altura de mergulho no nível do coelho)
    float distBase = 12.0f - aveAlvoX;


    float a = (7.0f - 0.5f) / (distBase * distBase + 0.0001f); // Evita divisão por zero

    /*"dx" e a distancia (no eixo X) entre onde a ave esta agora e o ponto
    mais baixo do mergulho. Quanto mais perto de zero, mais perto do
    fundo da curva -- e por isso mais perto do chao (aveY proximo de K).*/
    float dx = (aveX - aveAlvoX);
    aveY = a * (dx * dx) + 0.5f;

    //Quando a ave sai bem pra fora da tela do lado esquerdo, ela e
   // desativada -- para de ser desenhada e de colidir com o coelho, ate
   // que controlarSurgimentoDaAve() a traga de volta depois de um tempo.
    if (aveX < -12.0f) {
        aveActive = false;
    }
}
