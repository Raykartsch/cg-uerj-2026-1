#include "Cenario.hpp"
#include "PrimitivasGeometricas.hpp"
#include "Borboleta.hpp" //Para acessar as borboletas
#include "Coelho.hpp" // Para acessar rabbitLives e rabbit-related globals na hora de aplicar bônus
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*O cenário se move para a esquerda continuamente (dando a impressão de que o
coelho está correndo para a direita), e é "costurado" em loop usando bgWidth.
*/


float bgSpeed = 0.1f; // Velocidade de deslocamento horizontal do cenário por frame
float bgWidth = 40.0f;  // Largura da seção de background para cálculo de repetição
float bgPos = 0.0f;  // Offset de translação do fundo contínuo

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Sistema de vegetais (itens colecionáveis/power-ups)
 */
std::vector<Vegetal> vegetais; // Pool dinâmico de vegetais ativos/inativos no mundo
const int MAX_VEGETAIS = 15; // Limite superior de alocação de itens na tela
int framesAteProximoVegetal = 60;  // Timer decrescente (em frames) para o próximo spawn


std::vector<Toca> tocas; // Armazena a posição das tocas geradas
const float RAIO_TOCA = 0.8f; // Raio de colisão/interação para o coelho entrar

// Cria as tocas iniciais do mapa com posições fixas (x, y), chamada uma única vez no começo do jogo
//Inicializa as posições iniciais das tocas no mapa
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Animacao das nuvens

/*
// As nuvens "balançam" usando uma senoide: cloudPhase avança a cada frame
// (cloudPhaseSpeed) e o seno desse ângulo gera um movimento suave de vai-e-vem,
// tanto lateral (bordas) quanto vertical (parte do meio), simulando flutuação.

*/
float cloudPhase = 0.0f;                // Fase angular para oscilação senoidal das nuvens
float cloudPhaseSpeed = 0.02f;          // Taxa de variação angular por tick
float cloudSideSwingAmount = 0.15f;     // Amplitude do movimento horizontal das bordas
float cloudMiddleBobAmount = 0.1f;      // Amplitude da flutuação vertical central

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
tempoDeDiaFase representa "onde estamos" dentro do ciclo (0 a 1, como um
relógio de 24h normalizado). A cada frame ele avança por VELOCIDADE_CICLO_DIA
e, ao passar de 1.0f, "dá a volta" (wrap-around) voltando para o início.
As cores do céu (skyR/G/B) são recalculadas a partir dessa fase.
*/

float tempoDeDiaFase = 0.0f; // Progresso do ciclo (normalizado de 0.0f a 1.0f)
// Calcula a velocidade do ciclo baseada em framerate estimado (60 FPS / ciclo de tempo)
const float VELOCIDADE_CICLO_DIA = 1.0f / (60.0f * (1000.0f / 24.0f));
float skyR = 0.68f, skyG = 0.81f, skyB = 0.98f; // Cor atual do céu (atualizada dinamicamente)

// Renderiza o Sol com raios dinâmicos girando conforme a contagem de frames
void drawSun(float alpha) {
    int i;
    glColor4f(1.0f, 0.823f, 0.298f, alpha); //glColor4f para mudar a opacidade da cor
    glLineWidth(3);
    glRotatef(float(-FrameNumber), 0, 0, 1);  // Rotação horária contínua baseada no tempo de jogo

    // Desenho procedual dos raios solares em círculo
    glBegin(GL_LINES);
    for (i = 0; i < 15; i++) {
        glVertex2f(0, 0);
        glVertex2d(cos(i*2*PI/15), sin(i*2*PI/15));
    }
    glEnd();
    drawDisk(0.65);
    glColor3f(0, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Renderiza a Lua e suas crateras usando discos hierárquicos
void drawMoon(float alpha) {
    // base da Lua
    glColor4f(0.90f, 0.90f, 0.92f, alpha); // Branco acinzentado
    drawDisk(0.65);

    // crateras
    glColor4f(0.80f, 0.80f, 0.82f, alpha); // cinza um pouco mais escuro
    glPushMatrix();
        glTranslatef(-0.2f, 0.2f, 0.1f);
        drawDisk(0.15);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.2f, -0.1f, 0.1f);
        drawDisk(0.2);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.1f, -0.25f, 0.1f);
        drawDisk(0.12);
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha a nuvem do cenario
/*
Uma nuvem é composta por 3 discos (esquerda, meio, direita). As posições
das bordas oscilam horizontalmente e a do meio oscila verticalmente,
usando o mesmo valor de seno (cloudSin) para manter o movimento sincronizado.
*/

void drawCloud() {
    float cloudSin = sin(cloudPhase);
    float sideOffsetX   = cloudSin * cloudSideSwingAmount;
    float middleOffsetY = cloudSin * cloudMiddleBobAmount;

    glColor3f(1, 1, 1);
    glPushMatrix();
        glTranslatef(-0.6f + sideOffsetX, -0.2f, 1); // Disco esquerdo, balança lateralmente
        drawDisk(0.5f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, -0.1f + middleOffsetY, 1);  // Disco do meio, "boia" verticalmente
        drawDisk(0.7f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.6f + sideOffsetX, -0.2f, 1); // Disco direito, balança lateralmente
        drawDisk(0.5f);
    glPopMatrix();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Monta uma cerca de madeira usando retângulos (drawSquare escalado): três
estacas verticais (postes) e duas tábuas horizontais que as atravessam.
*/
//  Desenho da cerca decorativa
void drawFence() {
    glColor3f(0.662f, 0.443f, 0.247f);  // Marrom escuro (postes)
    glPushMatrix();
        glTranslatef(-1.2f, 0.2f, 0.0f);
        glScalef(0.15f, 1.0f, 1.0f); // Poste fino e alto
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

    glColor3f(0.752f, 0.541f, 0.321f);  // Marrom claro (tábuas horizontais)
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);
        glScalef(1.5f, 0.15f, 1.0f); // Tábua larga e fina
        drawSquare();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, 0.8f, 0.0f);
        glScalef(1.5f, 0.15f, 1.0f);
        drawSquare();
    glPopMatrix();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha uma fruta genérica

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha uma cenoura
void drawCarrot() {
    glColor3f(0.243f, 0.556f, 0.180f);  // Verde das folhas
    glPushMatrix();
        glRotatef(-45, 0, 0, 1); // Inclina toda a cenoura
        glPushMatrix();
            glTranslatef(0.0f, 0.25f, 0.0f);
            glScalef(0.1f, 0.8f, 1.0f);
            glRotatef(180, 0, 0, 1);
            drawTriangle();  // Folha central
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.2f, 0.0f);
            glScalef(0.25f, 0.5f, 1.0f);
            glRotatef(18, 0, 0, 1);
            drawTriangle(); // Folha lateral direita
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.2f, 0.0f);
            glScalef(0.22f, 0.5f, 1.0f);
            glRotatef(-18, 0, 0, 1);
            drawTriangle(); // Folha lateral esquerda
        glPopMatrix();

        glColor3f(0.95f, 0.52f, 0.13f); // Laranja da raiz
        glPushMatrix();
            glScalef(0.15f, 1.1f, 1.0f);
            glRotatef(180, 0, 0, 1);
            drawTriangle(); // Corpo pontudo da cenoura
        glPopMatrix();
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Modelagem da alface
void drawLettuce() {
    glColor3f(0.25f, 0.55f, 0.15f);  // Camada mais externa/escura
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

    glColor3f(0.40f, 0.70f, 0.20f); // Camada intermediária
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

    glColor3f(0.30f, 0.55f, 0.15f);  // Contornos das folhas intermediárias (apenas linha)
    glPushMatrix();
        glTranslatef(-0.08f, -0.10f, 1.0f);
        drawDiskLine(0.28f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.10f, -0.05f, 1.0f);
        drawDiskLine(0.26f);
    glPopMatrix();

    glColor3f(0.60f, 0.85f, 0.25f);  // Miolo mais claro, no centro
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 1.0f);
        drawDisk(0.20f);
    glPopMatrix();

    glColor3f(0.45f, 0.75f, 0.20f); // Contorno do miolo
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 1.0f);
        drawDiskLine(0.20f);
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Modelagem do rabanete
void drawRadish() {
    glColor3f(0.243f, 0.556f, 0.180f);  // Verde da folha
    glPushMatrix();
        glTranslatef(0.0f, 0.4f, 0.0f);
        glScalef(0.15f, 0.39f, 1.0f);
        glRotatef(180, 0, 0, 1);
        drawTriangle();
    glPopMatrix();

    glColor3f(0.86f, 0.24f, 0.35f); // Corpo avermelhado do rabanete
    glPushMatrix();
        drawDisk(0.22f);
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
        glTranslatef(0.0f, -0.22f, 0.0f); // Ponta branca inferior
        drawDisk(0.1f);
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
Centraliza a escolha de qual função de desenho chamar de acordo com o tipo do vegetal (enum TipoVegetal), evitando ifs espalhados pelo código.
*/

void drawVegetable(TipoVegetal tipo) {
    switch (tipo) {
        case CENOURA: drawCarrot(); break;
        case ALFACE: drawLettuce(); break;
        case RABANETE: drawRadish(); break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*FEATURE: Spawn (criação) de um novo vegetal no cenário
1) Sorteia aleatoriamente o tipo do vegetal (0, 1 ou 2 → CENOURA/ALFACE/RABANETE).

2) Sorteia a altura (y): 70% de chance de aparecer numa faixa "baixa"
   (0.2 a 0.8) e 30% de chance numa faixa "alta" (1.8 a 3.0), dando
   variedade vertical aos itens.

3) Sorteia o x sempre à frente da tela (9.0 a 12.0), fora da área visível,
   para que o vegetal "entre" na tela conforme o cenário rola.

4) Reaproveita um slot inativo do pool (object pool) se houver; senão,
   cria um novo vegetal no vector até o limite MAX_VEGETAIS.
*/
void spawnVegetable() {
    TipoVegetal tipoSorteado = static_cast<TipoVegetal>(rand() % 3);
    float y;
    if (rand() % 100 < 70) {
        y = 0.2f + (rand() % 60) / 100.0f;
    } else {
        y = 1.8f + (rand() % 120) / 100.0f;
    }

    float x = 9.0f + (rand() % 300) / 100.0f;

    // Procura um slot desativado no pool para reciclar (evita realocar memória)
    for (Vegetal &veg : vegetais) {
        if (!veg.ativo) {
            veg.tipo = tipoSorteado;
            veg.x = x;
            veg.y = y;
            veg.ativo = true;
            return;
        }
    }

    // Se não achou slot livre e ainda há espaço no limite, cria um novo
    if ((int)vegetais.size() < MAX_VEGETAIS) {
        Vegetal novoVegetal = { tipoSorteado, x, y, true };
        vegetais.push_back(novoVegetal);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Chamada quando o coelho colide com um vegetal ativo; concede o efeito
correspondente ao tipo:
- CENOURA: ativa turbo (velocidade aumentada) por um número de frames.
- RABANETE: ativa pulo reforçado (mais alto) por um número de frames.
- ALFACE: concede vida extra, respeitando o limite máximo de vidas. */

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
Usa o GLUT bitmap font para desenhar caractere por caractere a partir da posição (x, y) informada.*/
void drawText(float x, float y, const char *texto) {
    glRasterPos3f(x, y, 1.0f);
    for (const char *c = texto; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Desenho da toca do coelho
void drawToca() {
	//Desenha a grama envolta da toca
		glColor3f(0.549f, 0.776f, 0.247f);
		glPushMatrix();
			glTranslatef(-0.1f, -0.2f, 1.0f);
			glScalef(1.25f, 0.5f, 1.0f);
			drawSquare();  // Base de grama retangular
		glPopMatrix();

		glColor3f(0.549f, 0.776f, 0.247f);
		glPushMatrix();
			glTranslatef(-0.1f, 0.2f, 1.0f);
			glScalef(1.25f, 1.35f, 1.0f);
			drawDisk(1.0f); // Monte de grama arredondado
		glPopMatrix();


	//Desenha o buraco em volta da toca
		glColor3f(0.478f, 0.290f, 0.168f);
		glPushMatrix();
			glTranslatef(0.15f, 0.30f, 1.0f);
			glScalef(0.85f, 0.9f, 1.0f);
			drawDisk(1.0f); // Borda de terra do buraco
		glPopMatrix();


		glColor3f(0.478f, 0.290f, 0.168f);
			glPushMatrix();
			glTranslatef(0.15f, 0.0f, 1.0f);
		   glScalef(0.85f, 0.5f, 1.0f);
		   drawSquare(); // Continuação da terra abaixo do buraco
		glPopMatrix();



		glColor3f(0.12f, 0.09f, 0.07f); // Quase preto: interior escuro/profundidade do buraco
		glPushMatrix();
			glTranslatef(0.25f, 0.125f, 0.1f);
			glScalef(0.6f, 0.9f, 1.0f);
			drawDisk(1.0f);
		glPopMatrix();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*

Atualização das tocas a cada frame (movimento + reciclagem)
Move cada toca para a esquerda junto com o scroll do cenário (bgSpeed).
Se o coelho estiver escondido em uma toca específica, essa toca fica
"parada" (não se move) enquanto ele estiver escondido nela — evita que
o coelho "escondido" seja arrastado para fora da tela.
Quando uma toca sai completamente da tela à esquerda, ela é reposicionada
à frente (repõe no início), criando um loop infinito de tocas.

*/
void atualizarTocas() {
    for (int i = 0; i < (int)tocas.size(); i++) {
        if (coelhoEscondido && tocaOndeEstaEscondido == i) continue;

        tocas[i].x -= bgSpeed;

        if (tocas[i].x < -10.0f) {
            tocas[i].x = 9.0f + (rand() % 600) / 100.0f;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Retorna um valor intermediário entre "inicio" e "fim" de acordo com "t"
(0.0 = início, 1.0 = fim). Usada para fazer transições suaves de cor.*/
float interpolarCor(float inicio, float fim, float t) {
    return inicio + (fim - inicio) * t;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
 Define 4 cores-chave representando os momentos do ciclo: dia, entardecer,
noite e amanhecer. Divide o ciclo (tempoDeDiaFase, de 0 a 1) em 4 quartos
iguais e, dentro de cada quarto, interpola (lerp) suavemente entre a cor
do momento atual e a do próximo, criando uma transição gradual de cores.
Ao final, avança a fase do ciclo e faz o wrap-around (volta a 0 após 1.0).*/

void atualizarCorDoCeu() {
    float diaR = 0.68f, diaG = 0.81f, diaB = 0.98f;
    float entardecerR = 0.95f, entardecerG = 0.55f, entardecerB = 0.40f;
    float noiteR = 0.07f, noiteG = 0.09f, noiteB = 0.22f;
    float amanhecerR = 0.95f, amanhecerG = 0.72f, amanhecerB = 0.58f;

    if (tempoDeDiaFase < 0.25f) {
    	// Transição: Dia -> Entardecer
        float t = tempoDeDiaFase / 0.25f;
        skyR = interpolarCor(diaR, entardecerR, t);
        skyG = interpolarCor(diaG, entardecerG, t);
        skyB = interpolarCor(diaB, entardecerB, t);
    } else if (tempoDeDiaFase < 0.5f) {
    	// Transição: Entardecer -> Noite
        float t = (tempoDeDiaFase - 0.25f) / 0.25f;
        skyR = interpolarCor(entardecerR, noiteR, t);
        skyG = interpolarCor(entardecerG, noiteG, t);
        skyB = interpolarCor(entardecerB, noiteB, t);
    } else if (tempoDeDiaFase < 0.75f) {
    	// Transição: Noite -> Amanhecer
        float t = (tempoDeDiaFase - 0.5f) / 0.25f;
        skyR = interpolarCor(noiteR, amanhecerR, t);
        skyG = interpolarCor(noiteG, amanhecerG, t);
        skyB = interpolarCor(noiteB, amanhecerB, t);
    } else {
    	// Transição: Amanhecer -> Dia
        float t = (tempoDeDiaFase - 0.75f) / 0.25f;
        skyR = interpolarCor(amanhecerR, diaR, t);
        skyG = interpolarCor(amanhecerG, diaG, t);
        skyB = interpolarCor(amanhecerB, diaB, t);
    }

    tempoDeDiaFase += VELOCIDADE_CICLO_DIA;  // Avança o relógio do ciclo dia/noite
    if (tempoDeDiaFase > 1.0f) {
        tempoDeDiaFase -= 1.0f; // Wrap-around: reinicia o ciclo
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha o conteudo estatico do cenario
void drawBackgroundContent() {

    // Cerca decorativa central
    glPushMatrix();
        glTranslatef(16.0f, 0.5f, 1.0f);
        glScalef(1.5f, 1.0f, 1.0f);
        drawFence();
    glPopMatrix();

    // Nuvens fixas na região central do mapa
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

    // Canteiro 1 (horta decorativa com frutas laranjas e folhagens)
    glPushMatrix();
        glColor3f(0.478f, 0.290f, 0.168f);  // Terra do canteiro
        glPushMatrix();
            glTranslatef(24.0f, -1.5f, 1.0f);
            glScalef(2.5f, 1.0f, 1.0f);
            drawSquare();
        glPopMatrix();
        // Frutas espalhadas dentro do canteiro, em tamanhos e posições variados
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


        // Folhagens do canteiro (triângulos verdes decorativos)
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

    // Canteiro 2 (horta decorativa com frutas avermelhadas)
    glPushMatrix();
        glColor3f(0.478f, 0.290f, 0.168f); // Terra do canteiro
        glPushMatrix();
            glTranslatef(29.0f, -1.5f, 1.0f);
            glScalef(2.0f, 1.0f, 1.0f);
            drawSquare();
        glPopMatrix();

        glPushMatrix();
			glTranslatef(29.2f, -1.5f, 1.0f);
			glScalef(1.2f, 1.2f, 1.0f);
			drawFruit(0.839f, 0.270f, 0.313f);
		glPopMatrix();
        glPushMatrix();
			glTranslatef(30.1f, -1.8f, 1.0f);
			glScalef(1.5f, 1.5f, 1.0f);
			drawFruit(0.839f, 0.270f, 0.313f);
		glPopMatrix();
        glPushMatrix();
			glTranslatef(30.8f, -1.3f, 1.0f);
			drawFruit(0.839f, 0.270f, 0.313f);
		glPopMatrix();


        glColor3f(0.243f, 0.556f, 0.180f);  // Folhagem do canteiro 2
        glPushMatrix();
            glTranslatef(29.6f, 0.5f, 1.0f);
            glScalef(0.9f, 1.5f, 1.0f);
            glRotatef(180, 0, 0, 1);
            drawTriangle();
        glPopMatrix();
    glPopMatrix();

    // Borboletas decorativas
    glPushMatrix();
		glTranslatef(23.0f, 0.9f, 1.0f);
		drawButterfly(0.0f, 0.95f, 0.55f, 0.15f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(25.6f, 1.3f, 1.0f);
		drawButterfly(2.0f, 0.75f, 0.35f, 0.85f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(29.6f, 1.0f, 1.0f);
		drawButterfly(4.0f, 0.95f, 0.85f, 0.20f);
	glPopMatrix();

	// Nuvens adicionais nas extremidades do mapa
    glPushMatrix();
		glTranslatef(4.0f, 5.0f, 1.0f);
		drawCloud();
	glPopMatrix();

    glPushMatrix();
		glTranslatef(44.0f, 5.0f, 1.0f);
		drawCloud();
	glPopMatrix();

	// Cercas adicionais nas extremidades do mapa
    glPushMatrix();
		glTranslatef(4.0f, 0.5f, 1.0f);
		drawFence();
	glPopMatrix();

    glPushMatrix();
    	glTranslatef(44.0f, 0.5f, 1.0f);
    	drawFence();
	glPopMatrix();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Desenha um grande retângulo (o céu) usando a cor dinâmica skyR/skyG/skyB
(que muda com o ciclo dia/noite) e uma faixa de grama abaixo dele.
O tamanho é calculado a partir de bgWidth (+50%) para garantir cobertura
suficiente de tela mesmo durante o scroll, evitando "buracos" nas bordas.*/


void drawBackgroundSky() {
    glColor3f(skyR, skyG, skyB);  // Cor do céu, já influenciada pelo ciclo dia/noite
    glPushMatrix();
        glScalef(bgWidth + (bgWidth / 2), 10, 1);
        drawSquare();
    glPopMatrix();

    glColor3f(0.549f, 0.776f, 0.247f); // Faixa de grama abaixo do céu
    glPushMatrix();
        glTranslatef(0.0f, -4.0f, 1.0f);
        glScalef(bgWidth + (bgWidth / 2), 4.0f, 1);
        drawSquare();
    glPopMatrix();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha a camada de grama do cenario separadamente
void drawBackgroundGrass(){
	 glColor3f(0.549f, 0.776f, 0.247f);
	    glPushMatrix();
	        glTranslatef(0.0f, -4.0f, 1.0f);
	        glScalef(bgWidth + (bgWidth / 2), 4.0f, 1);
	        drawSquare();
	    glPopMatrix();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Movimentação dos vegetais ativos a cada frame
void moverVegetais() {
    for (Vegetal &veg : vegetais) {
        if (!veg.ativo){
        	continue;
        }
        veg.x -= bgSpeed;
        if (veg.x < -10.0f) {
            veg.ativo = false;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Controla o tempo de spawn dos vegetais do cenario
void controlarSurgimentoDeVegetais() {
    framesAteProximoVegetal--;
    if (framesAteProximoVegetal <= 0) {
        spawnVegetable();
        framesAteProximoVegetal = 120 + (rand() % 80);
    }
}
