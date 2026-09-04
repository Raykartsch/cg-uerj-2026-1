#include "Borboleta.hpp"
#include "PrimitivasGeometricas.hpp"
#include <GL/glut.h>
#include <cmath>

float butterflyPhase = 0.0f; // Fase angular global, avança a cada frame (relógio da animação)
float butterflyPhaseSpeed = 0.12f;  // Velocidade com que a fase avança por frame (define o ritmo do voo)

/*FEATURE: Desenho de uma borboleta individual
 Parâmetros:
 - fasePropria: um deslocamento de fase único para cada borboleta, somado à
   fase global. Isso faz com que várias borboletas na tela batam as asas e
   voem de forma dessincronizada entre si, mesmo compartilhando o mesmo
   relógio de animação (butterflyPhase).
 - asaR, asaG, asaB: cor das asas (RGB), permitindo criar borboletas de
   cores diferentes reutilizando a mesma função de desenho. */

void drawButterfly(float fasePropria, float asaR, float asaG, float asaB) {
	 // Fase efetiva desta borboleta: fase global + deslocamento próprio
    float fase = butterflyPhase + fasePropria;

   /*Usa sin(fase * 4.0f) para bater as asas 4x mais rápido que o ciclo de
   voo, e fabs() (valor absoluto) para que o resultado nunca fique
   negativo, sempre oscilando entre 0 e 1. O resultado é mapeado para o
   intervalo [0.35, 1.0] (0.35 + 0.65 * seno), evitando que as asas
   "fechem" totalmente (ficando com escala 0) durante a animação.*/
    float wingFlap = 0.35f + 0.65f * (float) fabs(sin(fase * 4.0f));

	/*A posição horizontal (voarX) oscila em um ciclo simples (sin(fase)),
	enquanto a posição vertical (voarY) oscila em uma frequência diferente
	e com amplitude menor (sin(fase * 2.3f) * 0.25f). Usar frequências
	distintas para X e Y cria uma trajetória tipo "oito"/errática, mais
	parecida com o voo real de uma borboleta do que um simples vaivém linear.*/

    float voarX = sin(fase) * 0.6f; // Amplitude maior no eixo horizontal
    float voarY = sin(fase * 2.3f) * 0.25f; // Amplitude menor e mais rápida no eixo vertical

    glPushMatrix();

    	// Aplica o deslocamento de voo calculado acima a todo o desenho da borboleta
        glTranslatef(voarX, voarY, 0.0f);

        glColor3f(0.15f, 0.15f, 0.15f);
        glPushMatrix();
            glScalef(0.025f, 0.11f, 1.0f); // Bem estreito e alongado verticalmente
            drawSquare();
        glPopMatrix();

        /*Um disco colorido (cor recebida por parâmetro) posicionado à
		esquerda do corpo. A escala horizontal é multiplicada por
		wingFlap, fazendo a asa "encolher" e "abrir" no eixo X, simulando
		o movimento de bater as asas (visto de frente).
        glColor3f(asaR, asaG, asaB);*/
        glPushMatrix();
            glTranslatef(-0.11f, 0.03f, 0.0f); // Desloca para o lado esquerdo do corpo
            glScalef(wingFlap, 1.0f, 1.0f); // Escala horizontal anima a batida da asa
            drawDisk(0.13f);
        glPopMatrix();


        /*
        Espelhada em relação à esquerda (translação positiva em X), usando
        a mesma cor e o mesmo fator de batida (wingFlap), garantindo que
        as duas asas se movam de forma sincronizada entre si.
         */
        glPushMatrix();
            glTranslatef(0.11f, 0.03f, 0.0f); // Desloca para o lado direito do corpo
            glScalef(wingFlap, 1.0f, 1.0f);
            drawDisk(0.13f);
        glPopMatrix();
    glPopMatrix();
}
