#include "Raposa.hpp"
#include "PrimitivasGeometricas.hpp"
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

/////////////////////////////////////////////////////////////////////////////////////
// ---- Animacao de "correr" da raposa (patas e orelhas) ----
float foxWalkPhase = 0.0f; // fase atual da animacao das patas/orelhas
float foxWalkPhaseSpeed = 0.22f;  // velocidade com que essa fase avanca a cada frame
float foxLegLiftAmount = 0.15f; // o quanto cada pata sobe no eixo Y
float foxEarSwingAmount = 8.0f; // o quanto cada orelha balanca (em graus)

/////////////////////////////////////////////////////////////////////////////////////
// ---- Animacao da cauda ----
/*A cauda tem DUAS fases independentes: uma para a base (perto do corpo) e
outra para a ponta (a extremidade com a mancha branca). Como elas avancam
em velocidades diferentes, a cauda nunca balanca de um jeito totalmente
"robotico"*/


float foxTailPhase = 0.0f;
float foxTailPhaseSpeed = 0.10f;
float foxTailSwingAmount = 6.0f;

float foxTailTipPhase = 0.0f;
float foxTailTipPhaseSpeed = 0.17f;
float foxTailTipSwingAmount = 12.0f;

/////////////////////////////////////////////////////////////////////////////////////
// ---- Estado e posicao da raposa na cena ----
bool foxActive = false; // true enquanto a raposa esta na tela correndo
float foxX = -12.0f;  // posicao atual da raposa no eixo X
float foxY = 0.5f; // altura fixa (a raposa sempre corre rente ao chao)
float foxDirecao = 1.0f;  // 1.0 = correndo para a direita (unico sentido que ela usa)

/////////////////////////////////////////////////////////////////////////////////////
// ---- Configuracoes de velocidade e colisao ----


const float VELOCIDADE_RAPOSA = 0.11f;  // o quanto foxX aumenta a cada frame
const float RAIO_COLISAO_RAPOSA = 1.1f;  // "alcance" da raposa para tocar o coelho

// Evita que uma unica passagem da raposa tire mais de uma vida do coelho
bool foxJaTirouVidaNestaPassagem = false;

const int FRAMES_POR_SEGUNDO = 1000 / 24; // 24 msecs como base original
int framesAteProximaRaposa = 20 * FRAMES_POR_SEGUNDO;

//Desenha todas as partes da raposa como um unico objeto

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawFox() {

	/*A mesma ideia usada no coelho: a partir de uma unica onda senoidal,
	decidimos quais patas sobem e quais orelhas balancam pra frente.*/
    float foxPhaseSin = sin(foxWalkPhase);

    // Orelhas: enquanto uma balanca pra frente, a outra fica parada (igual ao coelho)
    float earRightSwing = (foxPhaseSin > 0.0f) ?  foxPhaseSin * foxEarSwingAmount : 0.0f;
    float earLeftSwing  = (foxPhaseSin < 0.0f) ? -foxPhaseSin * foxEarSwingAmount : 0.0f;

    // Patas: os dois pares diagonais se revezam subindo, igual a um trote de raposa de verdade
    float legGroupALift = (foxPhaseSin > 0.0f)  ?  foxPhaseSin * foxLegLiftAmount : 0.0f;
    float legGroupBLift = (foxPhaseSin < 0.0f)  ? -foxPhaseSin * foxLegLiftAmount : 0.0f;

    // Cauda: a base e a ponta usam fases diferentes, entao balancam de forma independente
    float tailSwing    = sin(foxTailPhase)    * foxTailSwingAmount;
    float tailTipSwing = sin(foxTailTipPhase) * foxTailTipSwingAmount;

    // Cores base usadas para colorir o objeto
    float orangeR = 0.95f, orangeG = 0.45f, orangeB = 0.10f;
    float orangeDarkR = 0.80f, orangeDarkG = 0.35f, orangeDarkB = 0.08f;
    float whiteR  = 1.00f, whiteG  = 1.00f, whiteB  = 1.00f;
    float darkLineR = 0.65f, darkLineG = 0.25f, darkLineB = 0.05f;

    // Sistema de coordenadas do CORPO (nó pai)
    glPushMatrix();

    	// 1. PATAS DO FUNDO (Desenhadas antes do corpo para ficarem atrás)
        // Pata Traseira Esquerda (Fundo) -> pertence ao Grupo A
        glPushMatrix();
            glTranslatef(-0.8f, -0.6f + legGroupALift, -0.1f);
            glScalef(0.12f, 0.4f, 1.0f);
            glColor3f(orangeDarkR, orangeDarkG, orangeDarkB);
            drawSquare();
        glPopMatrix();

        // Pata Dianteira Esquerda (Fundo)
        glPushMatrix();
            glTranslatef(0.6f, -0.6f + legGroupBLift, -0.1f);
            glScalef(0.12f, 0.4f, 1.0f);
            glColor3f(orangeDarkR, orangeDarkG, orangeDarkB);
            drawSquare();
        glPopMatrix();

        // 2. RABO ALONGADO (QUADRADO NA BASE + TRIÂNGULO NA PONTA COM DETALHE BRANCO)
		// A base (glPushMatrix externo) e a ponta (glPushMatrix interno) tem
		// cada uma o seu proprio angulo de balanco (tailSwing / tailTipSwing),
		// por isso se movem de forma independente uma da outra.
        glPushMatrix();
            glTranslatef(-1.1f, 0.0f, 0.5f);
            glRotatef(30.0f + tailSwing, 0, 0, 1);

            // Base retangular/quadrada do rabo (Laranja)
            glPushMatrix();
                glTranslatef(-0.20f, -0.09f, 0.0f);
                glScalef(0.65f, 0.25f, 1.0f);
                glColor3f(orangeR, orangeG, orangeB);
                drawSquare();
                glColor3f(darkLineR, darkLineG, darkLineB);
                drawSquareLine();
            glPopMatrix();

            // Extensão / Ponta Triangular do rabo (Laranja + Ponta Branca)
            glPushMatrix();
                glTranslatef(-0.80f, -0.05f, 0.0f);
                glRotatef(80.0f + tailTipSwing, 0, 0, 1);
                glScalef(0.28f, 1.05f, 1.0f);

                // Base triangular laranja
                glColor3f(orangeR, orangeG, orangeB);
                drawTriangle();
                glColor3f(darkLineR, darkLineG, darkLineB);
                drawTriangleLine();

                // Ponta triangular branca
                glColor3f(whiteR, whiteG, whiteB);
                glPushMatrix();
                    glTranslatef(0.0f, 0.5f, 0.0f);
                    glScalef(0.5f, 0.5f, 1.0f);
                    drawTriangle();
                glPopMatrix();


            glPopMatrix();
        glPopMatrix();


        // 3. CORPO PRINCIPAL
        glPushMatrix();
            glScalef(1.0f, 0.40f, 1.0f);
            glColor3f(orangeR, orangeG, orangeB);
            drawSquare();
            glColor3f(darkLineR, darkLineG, darkLineB);
            drawSquareLine();
        glPopMatrix();

        // Peito / Barriga branca
        glColor3f(whiteR, whiteG, whiteB);
        glPushMatrix();
            glScalef(1.0f, 0.1f, 1.0f);
            glTranslatef(0.0f, -3.0f, 1.0f);
            drawSquare();
        glPopMatrix();

        // Pescoço
        glPushMatrix();
            glTranslatef(1.0f, 0.4f, 1.0f);
            glScalef(0.15f, 0.6f, 1.0f);
            glRotatef(-50, 0, 0, 1);
            glColor3f(orangeR, orangeG, orangeB);
            drawSquare();
            glColor3f(darkLineR, darkLineG, darkLineB);
            drawSquareLine();
        glPopMatrix();

        // 4. PATAS DA FRENTE (Primeiro plano)
        // Pata Traseira Direita (Frente) -> pertence ao Grupo B
        glPushMatrix();
            glTranslatef(-0.6f, -0.65f + legGroupBLift, 1.0f);
            glScalef(0.13f, 0.42f, 1.0f);
            glColor3f(orangeR, orangeG, orangeB);
            drawSquare();
            glColor3f(darkLineR, darkLineG, darkLineB);
            drawSquareLine();
        glPopMatrix();

        // Pata Dianteira Direita (Frente) -> pertence ao Grupo A
        glPushMatrix();
            glTranslatef(0.8f, -0.65f + legGroupALift, 1.0f);
            glScalef(0.13f, 0.42f, 1.0f);
            glColor3f(orangeR, orangeG, orangeB);
            drawSquare();
            glColor3f(darkLineR, darkLineG, darkLineB);
            drawSquareLine();
        glPopMatrix();

        // 5. CABEÇA (Filha do corpo e pai de olho, focinho e orelhas)
        glPushMatrix();
            glTranslatef(1.35f, 0.7f, 1.0f);

            // Base da Cabeça
            glColor3f(orangeR, orangeG, orangeB);
            drawDisk(0.45);
            glColor3f(darkLineR, darkLineG, darkLineB);
            drawDiskLine(0.45);

            // Focinho Pontudo (Laranja + Detalhe Branco + Nariz Preto)
            glPushMatrix();
                glTranslatef(0.25f, -0.15f, 0.0f);
                glRotatef(-115.0f, 0, 0, 1);
                glScalef(0.35f, 0.7f, 1.0f);

                // Base Laranja do Focinho
                glColor3f(orangeR, orangeG, orangeB);
                drawTriangle();

                // Detalhe Branco na parte inferior/bochecha do focinho
                glColor3f(whiteR, whiteG, whiteB);
                glPushMatrix();
                    glTranslatef(0.3f, 0.2f, 0.0f);
                    glRotatef(40.0f, 0, 0, 1);
                    glScalef(0.4f, 0.8f, 1.0f);
                    drawSquare();
                glPopMatrix();

                // Trufa do Nariz
                glColor3f(0.05f, 0.05f, 0.05f);
                glPushMatrix();
                    glTranslatef(0.0f, 0.95f, 0.0f);
                    drawDisk(0.18);
                glPopMatrix();
            glPopMatrix();

            // Olho
            glColor3f(0.0f, 0.0f, 0.0f);
            glPushMatrix();
                glTranslatef(0.25f, 0.09f, 0.1f);
                drawDisk(0.06);
            glPopMatrix();

            // Orelha Esquerda (Traseira) -> balanca com earLeftSwing
            glPushMatrix();
                glTranslatef(-0.2f, 0.35f, 0.0f);
                glRotatef(15.0f - earLeftSwing, 0, 0, 1);
                glScalef(0.2f, 0.6f, 1.0f);
                glColor3f(orangeR, orangeG, orangeB);
                drawTriangle();
                glColor3f(darkLineR, darkLineG, darkLineB);
                drawTriangleLine();

                // Interior Branco da Orelha
                glColor3f(whiteR, whiteG, whiteB);
                glScalef(0.5f, 0.6f, 1.0f);
                drawTriangle();
            glPopMatrix();


            // Orelha Direita (Frontal) -> balanca com earRightSwing
            glPushMatrix();
                glTranslatef(0.35f, 0.3f, 0.0f);
                glRotatef(-35.0f - earRightSwing, 0, 0, 1);
                glScalef(0.18f, 0.5f, 1.0f);
                glColor3f(orangeR, orangeG, orangeB);
                drawTriangle();
                glColor3f(darkLineR, darkLineG, darkLineB);
                drawTriangleLine();

                // Interior Branco da Orelha
                glColor3f(whiteR, whiteG, whiteB);
                glScalef(0.5f, 0.6f, 1.0f);
                drawTriangle();
            glPopMatrix();

        glPopMatrix(); // Fim da Cabeça

    glPopMatrix(); // Fim do Corpo
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Faz a raposa aparecer na tela, pronta para atravessar. Ela sempre entra
pela borda ESQUERDA da tela, um pouco fora da area visivel, e comeca a
correr para a direita (veja moverRaposa). */

void spawnRaposa() {
    foxActive = true;

    foxX = -11.0f; // Ponto onde a raposa inicia a corrida
    foxJaTirouVidaNestaPassagem = false; // nova passagem: pode tirar vida de novo se colidir
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Controla o "relogio" que decide quando a raposa vai aparecer de novo.
void controlarSurgimentoDaRaposa() {
    if (foxActive) {
    	return;
    }

    framesAteProximaRaposa--;

    if (framesAteProximaRaposa <= 0) {
        spawnRaposa();
        framesAteProximaRaposa = (1 + (rand() % 15)) * FRAMES_POR_SEGUNDO;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Move a raposa em linha reta para a direita, a uma velocidade constante ate ela sair completamente da tela do outro lado.
void moverRaposa() {

	// nao ha raposa correndo agora, nada a fazer
    if (!foxActive) {
    	return;
    }

    foxX += VELOCIDADE_RAPOSA;
    foxDirecao = 1.0f; // a raposa so corre para a direita neste jogo

    /*
    Passou da borda direita da tela: desativa a raposa (ela some da
    cena) e ja sorteia daqui a quanto tempo ela deve aparecer de novo
     */
    if (foxX > 10.0f) {
        foxActive = false;
        framesAteProximaRaposa =  (1 + (rand() % 15)) * FRAMES_POR_SEGUNDO;
    }
}
