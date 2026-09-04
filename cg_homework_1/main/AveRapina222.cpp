#include "AveRapina.hpp"
#include "PrimitivasGeometricas.hpp"
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

//////////////////////////////////////////////////////////////////////////////////////////////
// AVE DE RAPINA (gaviao/falcao) -- um segundo "perigo" para o coelho, alem
// da raposa. Diferente da raposa (que corre pelo chao), a ave aparece la em
// cima e faz um mergulho em curva ate a altura do coelho, tentando pega-lo
// de surpresa vindo do ceu.

// Indica se a ave esta "no ar" fazendo o mergulho neste momento. Enquanto
// for false, ela nao existe na cena (nem e desenhada, nem colide com nada).
bool aveActive = false;

// Posicao atual da ave na tela (eixo X e Y). aveX comeca sempre no mesmo
// lugar (bem a direita, fora da tela) e vai diminuindo a cada frame; aveY e
// recalculado a cada frame para formar a curva do mergulho (veja moverAve).
float aveX = 12.0f;
float aveY = 7.0f;

// Fase usada so para animar o bater de asas da ave (nao tem relacao com a
// posicao dela na tela -- e so um "relogio" que gira sem parar, feito o
// walkPhase do coelho e o foxWalkPhase da raposa).
float avePhase = 0.0f;

// Evita que uma unica passagem da ave tire mais de uma vida do jogador,
// mesmo que a colisao dure varios frames seguidos (a mesma ideia ja usada
// no "foxJaTirouVidaNestaPassagem" da raposa).
bool aveJaTirouVida = false;

// Velocidade horizontal da ave: o quanto aveX diminui a cada frame enquanto
// ela atravessa a tela da direita para a esquerda.
const float VELOCIDADE_AVE_X = 0.18f;

// Contagem regressiva (em frames) ate a ave aparecer de novo. Comeca valendo
// 25 segundos (25 * quantos frames cabem em 1 segundo), e depois de cada
// aparicao um novo tempo aleatorio e sorteado em controlarSurgimentoDaAve.
int framesAteProximaAve = 25 * (1000 / 24);

// Posicao X onde a ave deve atingir o ponto mais baixo do mergulho -- ou
// seja, a posicao X onde o coelho estava no instante em que a ave apareceu.
// E o "alvo" que da forma a parabola do mergulho (veja moverAve).
float aveAlvoX = 0.0f;

// Desenha a ave de rapina inteira: corpo, cabeca, bico, rabo e as duas asas
// batendo. Assim como o coelho e a raposa, cada parte e um circulo/triangulo/
// quadrado simples, posicionado e escalado em volta de um "corpo" central.
void drawBird() {

    // Avanca a fase do bater de asas a cada frame que a ave e desenhada (ou
    // seja, so enquanto ela esta ativa/visivel), e mantem o valor sempre
    // dentro de uma volta completa (0 a 2*PI), sem crescer pra sempre.
    avePhase += 0.35f;
    if (avePhase > 2 * 3.1415f) {
    	avePhase -= 2 * 3.1415f;
    }

    // O seno da fase da um valor que oscila suavemente entre -1 e 1: e ele
    // que faz as asas subirem e descerem (usado no glRotatef das asas, mais
    // abaixo, multiplicado por 50 graus de amplitude).
    float wingFlap = sin(avePhase);

    glPushMatrix();

    // Asa traseira do passaro. E desenhada com DOIS triangulos sobrepostos:
    // um maior e mais escuro por baixo (funciona como um "contorno"/sombra)
    // e outro menor e mais claro por cima (o preenchimento da asa). O
    // glRotatef usa "wingFlap * 50.0f" para girar a asa para cima e para
    // baixo, entre -50 e +50 graus, seguindo o bater de asas calculado acima.
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
        // Corpo: um disco achatado (mais largo que alto), na cor marrom
        // clara, com um contorno marrom escuro em volta (drawDiskLine).
        glPushMatrix();
        	glColor3f(0.411f, 0.334f, 0.20f); // Marrom escuro
            glScalef(0.6f, 0.25f, 1.0f);
            drawDisk(1.0f);
            glColor3f(0.27f, 0.23f, 0.13f);
            drawDiskLine(1.0f);
        glPopMatrix();

        // Mancha da barriga: um disco branco/creme, menor e deslocado para
        // baixo, sobreposto ao corpo -- da o efeito de peito claro da ave.
        glPushMatrix();
        	glTranslatef(0.0f, -0.1f, 1.0f);
        	glColor3f(0.96f, 0.93f, 0.89f);// Marrom escuro
		   glScalef(0.8f, 0.25f, 1.0f);
		   drawDisk(0.5f);
	   glPopMatrix();

        // Cabeca: outro disco, bem menor, deslocado para a esquerda do
        // corpo (onde fica a "frente" da ave, do lado do bico).
		glPushMatrix();
			glColor3f(0.411f, 0.334f, 0.20f); // Marrom escuro
			glTranslatef(-0.7f, 0.1f, 1.0f);
			glScalef(0.20f, 0.16f, 1.0f);
			drawDisk(1.0f);
			glColor3f(0.27f, 0.23f, 0.13f);
			drawDiskLine(1.0f);
		glPopMatrix();

		//Detalhe cabeca: uma "mascara" escura por cima da cabeca, dando um
		// contraste de cor ao redor dos olhos (como em aves de rapina reais).
		glPushMatrix();
			glTranslatef(-0.7f, 0.125f, 1.0f);
			glScalef(0.45f, 0.16f, 1.0f);
			glColor3f(0.27f, 0.23f, 0.13f);
			drawDisk(0.4f);
		glPopMatrix();

		 // Olho: um pontinho preto, bem pequeno, sobre a cabeca.
		 glPushMatrix();
			glTranslatef(-0.7f, 0.125f, 0.0f);
			glColor3f(0.0f, 0.0f, 0.0f);
			drawDisk(0.032);
		glPopMatrix();

		//Moicano da cabeca: dois triangulos finos e compridos, rotacionados
		// em angulos diferentes (-80 e -105 graus), formando as penas
		// "espetadas" no topo da cabeca da ave.
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


		// Rabo: tres quadrados finos e compridos (as "penas" da cauda),
		// posicionados um ao lado do outro do lado direito do corpo,
		// todos com a mesma rotacao para parecerem um leque de penas.
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


        // Bico (virado para a esquerda): dois triangulos pequenos e
        // amarelos, um deles rotacionado 90 graus em relacao ao outro, para
        // formar a "ponta" pontiaguda caracteristica do bico de uma ave de
        // rapina.
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

        // Asa mais proxima da tela (a "da frente"): igual a asa traseira
        // desenhada no comeco da funcao (mesmo esquema de dois triangulos e
        // o mesmo bater de asas com "wingFlap"), so que posicionada do outro
        // lado do corpo, para dar a ideia de profundidade/3D na ave 2D.
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

// Faz a ave aparecer na tela, pronta para iniciar o mergulho. Recebe a
// posicao X onde o coelho estava naquele instante (targetX), e guarda esse
// valor em aveAlvoX -- e esse ponto que vira o "fundo" da curva do mergulho,
// calculada depois em moverAve().
void spawnAve(float targetX) {
    aveActive = true;
    aveX = 12.0f; // sempre comeca no mesmo lugar: bem a direita, fora da tela
    aveAlvoX = targetX; // Grava onde o coelho estava para fazer o vértice do mergulho
    aveJaTirouVida = false;
}

// Controla o "relogio" que decide quando a ave vai aparecer de novo,
// igualzinho ao sistema equivalente da raposa: conta os frames ate chegar a
// zero e entao chama spawnAve(), sorteando em seguida um novo tempo de
// espera (entre 25 e 45 segundos) para a proxima aparicao.
void controlarSurgimentoDaAve(float coelhoX) {
    if (aveActive) {
    	return; // ja tem uma ave voando, nao sorteia outra em cima dela
    }

    framesAteProximaAve--;
    if (framesAteProximaAve <= 0) {
        spawnAve(coelhoX);
        framesAteProximaAve = (25 + rand() % 20) * (1000 / 24); // Sorteia entre 25 e 45 segundos
    }
}

// Move a ave a cada frame, fazendo ela "mergulhar" numa curva (uma
// parabola) partindo do alto direito da tela ate passar rente ao chao, na
// posicao onde o coelho estava quando ela apareceu, e depois voltar a subir
// enquanto continua indo para a esquerda.
void moverAve() {
    if (!aveActive) return;

    // O eixo X anda sempre na mesma velocidade constante, da direita pra
    // esquerda -- quem faz a curva do mergulho e so o calculo do Y, abaixo.
    aveX -= VELOCIDADE_AVE_X;

    // Cálculo da trajetória parabólica: Y = a(X - H)^2 + K
    //
    // Isso e a equacao de uma parabola "em pe" (formato de U), igual a de um
    // grafico de funcao do 2o grau. Os termos significam:
    //   H = aveAlvoX -> a posicao X onde fica o "fundo" da curva (o ponto
    //                    mais baixo do mergulho, bem em cima de onde o
    //                    coelho estava)
    //   K = 0.5f     -> a altura Y desse ponto mais baixo (rente ao chao,
    //                    na mesma altura em que o coelho fica parado)
    //   a            -> controla o quao "aberta" ou "fechada" e a curva.
    //                    Calculamos esse valor na linha abaixo para
    //                    garantir que a ave comece exatamente na altura
    //                    inicial dela (7.0f) quando aveX ainda esta bem
    //                    longe (em 12.0f, distBase de distancia do alvo)
    float distBase = 12.0f - aveAlvoX;
    float a = (7.0f - 0.5f) / (distBase * distBase + 0.0001f); // Evita divisão por zero

    // "dx" e a distancia (no eixo X) entre onde a ave esta agora e o ponto
    // mais baixo do mergulho. Quanto mais perto de zero, mais perto do
    // fundo da curva -- e por isso mais perto do chao (aveY proximo de K).
    float dx = (aveX - aveAlvoX);
    aveY = a * (dx * dx) + 0.5f;

    // Quando a ave sai bem pra fora da tela do lado esquerdo, ela e
    // desativada -- para de ser desenhada e de colidir com o coelho, ate
    // que controlarSurgimentoDaAve() a traga de volta depois de um tempo.
    if (aveX < -12.0f) {
        aveActive = false;
    }
}
