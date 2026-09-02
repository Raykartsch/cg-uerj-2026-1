// Run these commands in terminal to execute this code in Linux:
// 1. g++ -o code code.cpp -lglut -lGLU -lGL -lm
// 2. ./code


// Run these commands in terminal to execute this code in Windows:
// 1. g++ code.cpp -o code -lfreeglut -lglu32 -lopengl32
// 2. .\code



//////////////////////////////////////////////////////////////////////////////////////////////
// Importacao de bibliotecas
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <cstdlib>   // Para sortear posicoes e tipos de vegetais aleatoriamente (rand)
#include <ctime>     // Para "semear" o sorteio com o horario atual (srand)
#include <cstdio>    // Para montar o texto do HUD (snprintf)
#include <vector>    // Para guardar a lista de vegetais que estao na tela


// Usado para calcular o disco
double PI = 3.14;

// Variaveis para controlar a animacao da cena
int FrameNumber = 0;
int speed = 50; //The higher this variable is, the lower is the animation
int msecs = 24;


void init(void);
void display(void);


void init(void)
{
  // define a cor de background da janela
  glClearColor(1.0, 1.0, 1.0, 1.0);

  // define o sistema de visualização - tipo de projeção
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  glOrtho (-8, 8, -8, 8, -8, 8);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Cria um quadrado
void drawSquare(){
	glBegin(GL_POLYGON);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(-1, 1, 0);
	glEnd();
}

void drawSquareLine(){
	glLineWidth(1.5f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(-1, 1, 0);
	glEnd();
}



//////////////////////////////////////////////////////////////////////////////////////////////
// Cria um triangulo
void drawTriangle() {
	glBegin(GL_POLYGON);
      glVertex3f(-1, 0, 0);
      glVertex3f(0, 1, 0);
      glVertex3f(1, 0, 0);
    glEnd();
}

void drawTriangleLine() {
	glLineWidth(1.5f);
	glBegin(GL_LINE_LOOP);
      glVertex3f(-1, 0, 0);
      glVertex3f(0, 1, 0);
      glVertex3f(1, 0, 0);
    glEnd();
}



//////////////////////////////////////////////////////////////////////////////////////////////
// Cria um disco
void drawDisk(double radius){
    int d;
    glBegin(GL_POLYGON);
    for (d = 0; d < 32; d++){
        double angle = (2*PI/32) * d;
        glVertex3f(radius*cos(angle), radius*sin(angle), 0);
    }
    glEnd();
}


void drawDiskLine(double radius){
    int d;
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    for (d = 0; d < 32; d++){
        double angle = (2*PI/32) * d;
        glVertex3f(radius*cos(angle), radius*sin(angle), 0);
    }
    glEnd();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Coelho controlado pelo usuario


// Controlam o movimento no eixo x do quadrado andante
float characterPos = 0.0f;
float characterSpeed = 0.05f;


// booleanos que controlam se a setinha do teclado está pressionada (neste caso, estas teclas sao especiais)
bool rightArrowPressed = false;
bool leftArrowPressed = false;
bool upArrowPressed = false;
bool downArrowPressed = false;


// Controlam a velocidade e a posicao do cenario de fundo, que "rola" pela
// tela pra dar a sensacao de que o coelho esta sempre correndo. Varios
// outros sistemas (vegetais, tocas) tambem usam bgSpeed para se mover na
// mesma velocidade do cenario.
float bgSpeed = 0.1f;      // velocidade do fundo (independente do coelho)
float bgWidth = 40.0f;     // largura de uma "tile" do fundo (2 * 50 do glScalef)
float bgPos = 0.0f;        // posição atual do fundo


// Velocidade normal do coelho e velocidade durante o "turbo" dado pela cenoura
const float VELOCIDADE_NORMAL = 0.05f;
const float VELOCIDADE_TURBO = 0.20f;
int framesDeTurboRestantes = 0;              // enquanto > 0, o turbo esta ativo
const int DURACAO_TURBO_EM_FRAMES = 150;     // ~3,6 segundos de turbo


//////////////////////////////////////////////////////////////////////////////////////////////
// SISTEMA DE CORRIDA (SPRINT) COM FOLEGO
//
// Enquanto o jogador segura a tecla "R", o coelho corre mais rapido do que
// o normal. Só que correr cansa: a cada frame correndo, um pouco do
// "folego" do coelho e gasto. Quando o folego acaba, o coelho e obrigado a
// voltar pra velocidade normal ate recuperar folego suficiente (o que
// acontece automaticamente, aos poucos, sempre que ele NAO estiver correndo).

const float VELOCIDADE_CORRENDO = 0.09f; // mais rapida que o normal, mas mais lenta que o turbo da cenoura

const float FOLEGO_MAXIMO = 100.0f;       // folego maximo que o coelho pode ter
float folegoAtual = FOLEGO_MAXIMO;        // folego atual (comeca cheio)

const float GASTO_FOLEGO_POR_FRAME = 0.6f;       // quanto o folego diminui a cada frame correndo
const float RECUPERACAO_FOLEGO_POR_FRAME = 0.3f; // quanto o folego se recupera a cada frame sem correr

bool coelhoCorrendo = false; // true enquanto o coelho estiver de fato correndo neste frame

// Controla o pulo do coelho
float jump_maximum_height = 2.5f;
float speed_jump = 0.1f;
float jump_height = 0.5f;
bool isJumping = false;
bool goingUp = true; // controla se está na fase de subida ou descida do pulo


// Altura de pulo normal e altura durante o bonus dado pelo rabanete
const float PULO_NORMAL = 2.5f;
const float PULO_REFORCADO = 3.5f;
int framesDePuloReforcadoRestantes = 0;          // enquanto > 0, o pulo alto esta ativo
const int DURACAO_PULO_REFORCADO_EM_FRAMES = 300; // ~7,2 segundos de pulo reforcado




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha o coelho


// Variaveis para controlar a animacao de "correr" do coelho (patas e orelhas)
// O coelho fica sempre animado, independente de estar andando ou parado
float walkPhase = 0.0f;       // fase atual da animacao (em radianos)
float walkPhaseSpeed = 0.15f; // velocidade com que a fase avanca a cada frame
float legLiftAmount = 0.2f;  // o quanto a pata sobe no eixo Y
float earSwingAmount = 6.0f; // o quanto a orelha "balanca" (em graus)
float direcaoCoelho = 1.0f;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha o coelho
void drawRabbit(){

	// Calcula, a partir da fase atual da animacao, o quanto cada pata deve
	// subir no eixo Y e o quanto cada orelha deve balancar para frente.
	// A logica liga as duas: quando a orelha direita esta indo para frente
	// (senoide positiva), a perna ESQUERDA sobe; quando a orelha esquerda
	// esta indo para frente (senoide negativa), a perna DIREITA sobe.
	float phaseSin = sin(walkPhase);

	float earRightSwing = (phaseSin > 0.0f) ?  phaseSin * earSwingAmount : 0.0f;   // orelha direita "pra frente" quando > 0
	float earLeftSwing  = (phaseSin < 0.0f) ? -phaseSin * earSwingAmount : 0.0f;  // orelha esquerda "pra frente" quando > 0 (fase oposta)

	float legLeftLift  = (phaseSin > 0.0f)  ?  phaseSin * legLiftAmount : 0.0f; // sobe junto com a orelha direita
	float legRightLift = (phaseSin < 0.0f)  ? -phaseSin * legLiftAmount : 0.0f; // sobe junto com a orelha esquerda


	//////////////////////////////////////////////////////////////////////////////////////////////
	// CORPO -> nó PAI (raiz) da hierarquia do coelho.
	// Ele é modelado em seu próprio sistema de coordenadas, centrado na origem
	// (um disco de raio 1 desenhado em (0,0)). Todas as outras partes do coelho
	// (patas, rabo, orelhas e cabeça) são desenhadas DENTRO deste glPushMatrix/
	// glPopMatrix, ou seja, são "filhas" do corpo: qualquer transformação
	// aplicada aqui (translação, rotação, escala) seria automaticamente
	// herdada por todas elas.
	glPushMatrix(); // abre o sistema de coordenadas do CORPO (nó pai)

		glColor3f(0.96f, 0.93f, 0.89f);
		drawDisk(1);
		glColor3f(0.705f, 0.64f, 0.58f);
		drawDiskLine(1);


		// -------- Pata esquerda (filha do corpo) --------
		// Modelada em seu proprio sistema de coordenadas (um quadrado
		// centrado na origem) e depois posicionada/escalada em relacao ao corpo.
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
		// A cabeça abre seu próprio glPushMatrix e, dentro dele, desenha suas
		// partes (olho, focinho, nariz) usando coordenadas relativas ao
		// CENTRO DA PRÓPRIA CABEÇA (e não mais coordenadas absolutas da tela),
		// formando um segundo nível da hierarquia: corpo -> cabeça -> olho/focinho/nariz.
		glPushMatrix();
			glTranslatef(1.2f, 0.7f, 1.0f);

			glColor3f(0.96f, 0.93f, 0.89f);
			drawDisk(0.6);
			glColor3f(0.705f, 0.64f, 0.58f);
			drawDiskLine(0.6);


			// Olho (filho da cabeça)
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
		// Modelada como um triângulo centrado na origem, depois rotacionada
		// (efeito do "swing" da corrida) e posicionada no topo do corpo.
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

	glPopMatrix(); // fecha o sistema de coordenadas do CORPO (nó pai)


}

//////////////////////////////////////////////////////////////////////////////////////////////
// Desenha um sol
void drawSun(){
    int i;
    glColor3f(1.0f, 0.823f, 0.298f);
    glLineWidth(3);
    glRotatef(float(-FrameNumber), 0, 0, 1);
    glBegin(GL_LINES);
    for (i=0; i<15; i++){
    	glVertex2f(0, 0);
    	glVertex2d(cos(i*2*PI/15), sin(i*2*PI/15));

    }
    glEnd();
    drawDisk(0.65);
    glColor3f(0, 0, 0);

}


//////////////////////////////////////////////////////////////////////////////////////////////
// Desenha uma nuvem

// Variaveis para controlar a animacao da nuvem (ela fica sempre se mexendo
// bem devagar, dando a sensacao de estar "flutuando" no ceu)
float cloudPhase = 0.0f;        // fase atual da animacao (em radianos)
float cloudPhaseSpeed = 0.02f;  // velocidade com que a fase avanca a cada frame (bem lenta)
float cloudSideSwingAmount = 0.15f; // o quanto as esferas das pontas se movem no eixo X
float cloudMiddleBobAmount = 0.1f;  // o quanto a esfera do meio se move no eixo Y

void drawCloud(){

	// Usamos uma unica onda senoidal para gerar os dois movimentos: as
	// esferas das pontas deslizam um pouco para os lados (eixo X), enquanto
	// a esfera do meio sobe e desce (eixo Y), tudo no mesmo ritmo.
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

	// Desenha os pes da cerca
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


	// Desenha os pedacos que ficam "deitados" da cerca
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



void drawFruit(float red, float green, float blue){

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENREDO DO JOGO: o coelho esta fugindo de uma raposa e precisa atravessar a
// fazenda o mais rapido possivel. Pelo caminho, ele encontra vegetais que o
// ajudam na fuga:
//   - Cenoura  -> da um "turbo" de velocidade, para se distanciar da raposa
//   - Rabanete -> fortalece as pernas do coelho, permitindo pulos mais altos
//                 para desviar de obstaculos maiores
//   - Alface   -> e tao nutritiva que da uma vida extra ao coelho
//
// Sistema de vegetais de bonificacao
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Os 3 tipos de vegetais que podem aparecer no jogo
enum TipoVegetal { CENOURA, ALFACE, RABANETE };

// Cada vegetal na tela e representado por esta estrutura: o tipo dele,
// onde ele esta (x, y) e se ele ainda esta "vivo" (ativo) na cena.
// Quando o coelho pega um vegetal, ou quando ele sai da tela, ativo vira false.
struct Vegetal {
	TipoVegetal tipo;
	float x;
	float y;
	bool ativo;
};

// Lista com todos os vegetais que ja foram criados no jogo. Vegetais inativos
// (ja capturados ou que sairam da tela) sao reaproveitados para o proximo
// vegetal que precisar aparecer, em vez de ficarmos criando itens novos pra sempre.
std::vector<Vegetal> vegetais;
const int MAX_VEGETAIS = 15; // quantidade maxima de vegetais guardados na lista


//////////////////////////////////////////////////////////////////////////////////////////////
// Desenha uma cenoura: raiz laranja apontando pra baixo + folhas verdes no topo
void drawCarrot(){



	// Folhas da cenoura
		glColor3f(0.243f, 0.556f, 0.180f);

		glPushMatrix();

		glRotatef(-45, 0, 0, 1);
		//Folhas da cenoura
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


		//Base da cenoura
			glColor3f(0.95f, 0.52f, 0.13f);
			glPushMatrix();
				glScalef(0.15f, 1.1f, 1.0f);
				glRotatef(180, 0, 0, 1);
				drawTriangle();
			glPopMatrix();


		glPopMatrix();

}


//////////////////////////////////////////////////////////////////////////////////////////////
// Desenha uma alface: algumas "bolhas" verdes sobrepostas, como uma nuvem verde
void drawLettuce() {
    // 1. Camada Base (Folhas externas, maiores e mais escuras)
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

    // 2. Camada Intermediária (Verde médio, dando volume)
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

    // Adiciona algumas linhas para simular as nervuras/textura crespa
    glColor3f(0.30f, 0.55f, 0.15f);
    glPushMatrix();
        glTranslatef(-0.08f, -0.10f, 1.0f);
        drawDiskLine(0.28f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.10f, -0.05f, 1.0f);
        drawDiskLine(0.26f);
    glPopMatrix();

    // 3. Miolo da Alface (Mais claro, menor e centralizado)
    glColor3f(0.60f, 0.85f, 0.25f);

    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 1.0f);
        drawDisk(0.20f);
    glPopMatrix();

    // Contorno final do miolo
    glColor3f(0.45f, 0.75f, 0.20f);
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 1.0f);
        drawDiskLine(0.20f);
    glPopMatrix();
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Desenha um rabanete: corpo rosa/vermelho com pontinha branca e folhas no topo
void drawRadish(){

	// Folha do rabanete
		glColor3f(0.243f, 0.556f, 0.180f);
		glPushMatrix();
			glTranslatef(0.0f, 0.4f, 0.0f);
			glScalef(0.15f, 0.39f, 1.0f);
			glRotatef(180, 0, 0, 1);
			drawTriangle();
		glPopMatrix();

	// Corpo do rabanete
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


// Escolhe qual funcao de desenho usar de acordo com o tipo do vegetal
void drawVegetable(TipoVegetal tipo){
	switch (tipo) {
		case CENOURA:
			drawCarrot();
			break;
		case ALFACE:
			drawLettuce();
			break;
		case RABANETE:
			drawRadish();
			break;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Controla quando um novo vegetal deve aparecer na tela
int framesAteProximoVegetal = 60; // contagem regressiva (em frames) ate o proximo vegetal surgir


// Cria um vegetal novo (ou reaproveita um que ja saiu da tela), num tipo e
// posicao aleatorios, sempre entrando pela borda direita da tela, um pouco
// a frente de onde o coelho consegue ver.
void spawnVegetable(){

	TipoVegetal tipoSorteado = static_cast<TipoVegetal>(rand() % 3);

	// 70% de chance do vegetal aparecer rente ao chao (facil de pegar correndo)
	// 30% de chance de aparecer mais alto no ar (o coelho precisa pular para pegar)
	float y;
	if (rand() % 100 < 70) {
		y = 0.2f + (rand() % 60) / 100.0f;   // entre 0.2 e 0.8 (perto do chao)
	} else {
		y = 1.8f + (rand() % 120) / 100.0f;  // entre 1.8 e 3.0 (precisa pular)
	}

	// Comeca um pouco fora da tela, a direita, para "entrar" suavemente na cena
	float x = 9.0f + (rand() % 300) / 100.0f; // entre 9.0 e 12.0

	// Primeiro tenta reaproveitar um vegetal que ja esteja inativo
	for (Vegetal &veg : vegetais) {
		if (!veg.ativo) {
			veg.tipo = tipoSorteado;
			veg.x = x;
			veg.y = y;
			veg.ativo = true;
			return;
		}
	}

	// Se nao sobrou nenhum vegetal livre, cria um novo (respeitando o limite maximo)
	if ((int)vegetais.size() < MAX_VEGETAIS) {
		Vegetal novoVegetal = { tipoSorteado, x, y, true };
		vegetais.push_back(novoVegetal);
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////
// Vidas e bonus que o coelho pode ganhar ao capturar vegetais

int rabbitLives = 3; // vidas iniciais do coelho, aumentam ao comer alface
const int MAX_VIDAS = 3;


// Aplica o efeito correspondente ao vegetal que o coelho acabou de capturar
void aplicarBonusDoVegetal(TipoVegetal tipo){
	switch (tipo) {

		case CENOURA:
			// Da um impulso extra de velocidade, para o coelho se distanciar da raposa
			framesDeTurboRestantes = DURACAO_TURBO_EM_FRAMES;
			break;

		case RABANETE:
			// Permite pular mais alto por um tempo, para desviar de obstaculos maiores
			framesDePuloReforcadoRestantes = DURACAO_PULO_REFORCADO_EM_FRAMES;
			break;

		case ALFACE:
			// Concede uma vida extra ao coelho
			if (rabbitLives < MAX_VIDAS) {
				rabbitLives++;
			}
			break;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Pequena funcao auxiliar para desenhar texto na tela (usada no HUD de vidas/bonus)
void drawText(float x, float y, const char *texto){
	glRasterPos3f(x, y, 1.0f);
	for (const char *c = texto; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TOCA (esconderijo do coelho)
//
// De tempos em tempos, uma toca (um buraco no chao) passa pela tela, andando
// junto com o cenario (igual aos vegetais). Se o jogador segurar a seta PARA
// BAIXO enquanto o coelho estiver pertinho de uma toca, o coelho entra nela:
// fica escondido, a salvo da raposa, ate que o jogador solte a seta pra
// baixo ou tente se mover/pular (o que faz o coelho sair de novo).

// Cada toca e representada apenas pela sua posicao no eixo X (ela sempre
// fica no chao, numa altura fixa).
struct Toca {
	float x;
	float y;
};

// Comecamos com duas tocas espalhadas pela cena. Quando uma sai da tela
// (pela esquerda), ela reaparece do outro lado, um pouco mais afastada, e
// assim vai se repetindo ao longo do jogo -- igual ao truque usado no
// cenario de fundo para parecer "infinito".
std::vector<Toca> tocas = { {9.0f, 0.35f}, {20.0f, 0.35f} };

const float RAIO_TOCA = 0.8f; // distancia maxima do coelho ate a toca para poder entrar/sair dela

bool coelhoEscondido = false;   // true enquanto o coelho estiver escondido dentro de uma toca
int tocaOndeEstaEscondido = -1; // indice, dentro de "tocas", de onde o coelho esta escondido (-1 = nenhuma)


// Desenha uma toca: um monte de terra ao redor e um buraco escuro no meio
void drawToca(){

	// Monte de terra ao redor do buraco
	glColor3f(0.478f, 0.290f, 0.168f);
	glPushMatrix();
		glScalef(0.9f, 0.5f, 1.0f);
		drawDisk(1.0f);
	glPopMatrix();

	// O buraco em si (bem escuro, para parecer um vao profundo)
	glColor3f(0.12f, 0.09f, 0.07f);
	glPushMatrix();
		glTranslatef(0.0f, 0.05f, 0.1f);
		glScalef(0.6f, 0.32f, 1.0f);
		drawDisk(1.0f);
	glPopMatrix();

}


// Move as tocas junto com o cenario (igual aos vegetais). A toca onde o
// coelho estiver escondido no momento NAO se move, pra nao "arrastar" o
// coelho escondido dentro dela.
void atualizarTocas(){
	for (int i = 0; i < (int)tocas.size(); i++) {

		if (coelhoEscondido && tocaOndeEstaEscondido == i) continue;

		tocas[i].x -= bgSpeed;

		if (tocas[i].x < -10.0f) { // saiu da tela pela esquerda
			tocas[i].x = 9.0f + (rand() % 600) / 100.0f; // reaparece do outro lado (entre 9 e 15)
		}
	}
}


// Controla o coelho entrando e saindo de uma toca.
void atualizarEsconderijoDoCoelho(){

	// Se ja esta escondido, verifica se chegou a hora de sair: solta a
	// seta pra baixo, ou tenta andar/pular (nesses casos, sai da toca)
	if (coelhoEscondido) {
		if (!downArrowPressed || leftArrowPressed || rightArrowPressed || upArrowPressed) {
			coelhoEscondido = false;
			tocaOndeEstaEscondido = -1;
		}
		return;
	}

	// Ainda nao esta escondido: so tenta entrar numa toca se a seta pra
	// baixo estiver sendo segurada, e se o coelho nao estiver no meio de um pulo
	if (!downArrowPressed || isJumping) return;

	for (int i = 0; i < (int)tocas.size(); i++) {
		float distancia = fabs(tocas[i].x - characterPos);
		if (distancia < RAIO_TOCA) {
			coelhoEscondido = true;
			tocaOndeEstaEscondido = i;
			break;
		}
	}
}



float foxWalkPhase = 0.0f;
float foxWalkPhaseSpeed = 0.22f;  // um pouco mais rápida que a do coelho
float foxLegLiftAmount = 0.15f;
float foxEarSwingAmount = 8.0f;

float foxTailPhase = 0.0f;
float foxTailPhaseSpeed = 0.10f;     // balanço lento da base
float foxTailSwingAmount = 6.0f;

float foxTailTipPhase = 0.0f;
float foxTailTipPhaseSpeed = 0.17f;  // a ponta se move num ritmo diferente
float foxTailTipSwingAmount = 12.0f;


//////////////////////////////////////////////////////////////////////////////////////////////
// SISTEMA DE PERSEGUICAO DA RAPOSA
//
// A raposa fica a maior parte do tempo escondida (fora da cena). De tempos
// em tempos, ela entra correndo por uma das bordas da tela, pelo lado de
// TRAS do coelho, e persegue ele ate: (a) alcança-lo, fazendo o jogador
// perder uma vida, ou (b) o coelho conseguir escapar (correndo, pulando por
// cima dela, ou usando o turbo da cenoura), momento em que ela desiste e
// some ate a proxima vez que for sorteada para aparecer.

bool foxActive = false;   // true enquanto a raposa esta perseguindo o coelho na tela
float foxX = -12.0f;      // posicao da raposa no eixo X
float foxY = 0.5f;        // a raposa sempre corre rente ao chao (mesma altura do coelho parado)
float foxDirecao = 1.0f;  // pra qual lado a raposa esta olhando (1 = direita, -1 = esquerda)

const float VELOCIDADE_RAPOSA = 0.11f; // um pouco mais rapida que o coelho no ritmo normal, pra criar pressao
const float RAIO_COLISAO_RAPOSA = 1.1f; // "alcance" da raposa para pegar o coelho

// Como a raposa agora atravessa o coelho em vez de sumir ao colidir, ela
// pode ficar varios frames seguidos "dentro" do raio de colisao enquanto
// passa por cima dele. Essa variavel garante que cada passagem da raposa
// tire APENAS UMA vida, mesmo que a colisao dure vários frames.
bool foxJaTirouVidaNestaPassagem = false;

// Quantos frames se passam, aproximadamente, em 1 segundo (usado so para
// transformar "15 a 45 segundos" em uma contagem de frames)
const int FRAMES_POR_SEGUNDO = 1000 / msecs;

// Contagem regressiva (em frames) ate a raposa aparecer pela primeira vez.
// Usamos um valor fixo aqui (20 segundos) porque esta variavel e inicializada
// antes do srand() rodar em main() -- ou seja, antes de sortearmos numeros de
// verdade. Da segunda aparicao em diante, o tempo passa a ser sorteado
// normalmente entre 15 e 45 segundos (veja controlarSurgimentoDaRaposa).
int framesAteProximaRaposa = 20 * FRAMES_POR_SEGUNDO;

void drawFox(){

	// A mesma ideia usada no coelho: a partir de uma unica onda senoidal,
	// decidimos quais patas sobem e quais orelhas balancam pra frente.
	float foxPhaseSin = sin(foxWalkPhase);

	// Orelhas: enquanto uma balanca pra frente, a outra fica parada (igual ao coelho)
	float earRightSwing = (foxPhaseSin > 0.0f) ?  foxPhaseSin * foxEarSwingAmount : 0.0f;
	float earLeftSwing  = (foxPhaseSin < 0.0f) ? -foxPhaseSin * foxEarSwingAmount : 0.0f;

	// Patas: os dois pares diagonais se revezam subindo, igual a um trote de raposa de verdade
	float legGroupALift = (foxPhaseSin > 0.0f)  ?  foxPhaseSin * foxLegLiftAmount : 0.0f; // traseira-esq (fundo) + dianteira-dir (frente)
	float legGroupBLift = (foxPhaseSin < 0.0f)  ? -foxPhaseSin * foxLegLiftAmount : 0.0f; // dianteira-esq (fundo) + traseira-dir (frente)

	// Cauda: a base e a ponta usam fases diferentes, entao balancam de forma independente
	float tailSwing    = sin(foxTailPhase)    * foxTailSwingAmount;
	float tailTipSwing = sin(foxTailTipPhase) * foxTailTipSwingAmount;

    // Cores base
    float orangeR = 0.95f, orangeG = 0.45f, orangeB = 0.10f;
    float orangeDarkR = 0.80f, orangeDarkG = 0.35f, orangeDarkB = 0.08f; // Tom mais escuro para as patas do fundo
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

        // Pata Dianteira Esquerda (Fundo) -> pertence ao Grupo B
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



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha o background "infinito"


//////////////////////////////////////////////////////////////////////////////////////////////
// CICLO DE COR DO CEU (amanhecer -> dia -> entardecer -> noite -> amanhecer)
//
// A ideia e simples: guardamos um "relogio" (tempoDeDiaFase) que vai de 0.0
// a 1.0 e depois volta pra 0.0, se repetindo pra sempre. Esse relogio e
// dividido em 4 pedacos iguais (dia, entardecer, noite e amanhecer) e, a
// cada frame, calculamos a cor do ceu fazendo uma interpolacao (uma mistura
// gradual) entre a cor do pedaco atual e a cor do proximo pedaco.

float tempoDeDiaFase = 0.0f; // "relogio" do dia, sempre entre 0.0 e 1.0

// O quanto o relogio avanca a cada frame, calculado para completar uma
// volta inteira (um dia inteiro) a cada ~60 segundos de jogo.
const float VELOCIDADE_CICLO_DIA = 1.0f / (60.0f * (1000.0f / msecs));

// Cor atual do ceu, recalculada a cada frame por atualizarCorDoCeu()
float skyR = 0.68f, skyG = 0.81f, skyB = 0.98f;


// Faz uma mistura gradual entre duas cores: quando "t" = 0.0, o resultado e
// "inicio"; quando "t" = 1.0, o resultado e "fim"; valores no meio misturam
// as duas cores proporcionalmente.
float interpolarCor(float inicio, float fim, float t){
	return inicio + (fim - inicio) * t;
}


// Recalcula a cor do ceu (skyR/skyG/skyB) de acordo com o momento atual do
// "relogio do dia", e depois avanca esse relogio para o proximo frame.
void atualizarCorDoCeu(){

	// Cores de referencia de cada momento do dia
	float diaR = 0.68f, diaG = 0.81f, diaB = 0.98f;                     // azul claro (ceu de dia)
	float entardecerR = 0.95f, entardecerG = 0.55f, entardecerB = 0.40f; // laranja/rosa
	float noiteR = 0.07f, noiteG = 0.09f, noiteB = 0.22f;                // azul bem escuro
	float amanhecerR = 0.95f, amanhecerG = 0.72f, amanhecerB = 0.58f;    // rosa claro

	// O relogio (0.0 a 1.0) e dividido em 4 partes iguais, uma para cada transicao
	if (tempoDeDiaFase < 0.25f) {
		// Dia -> Entardecer
		float t = tempoDeDiaFase / 0.25f;
		skyR = interpolarCor(diaR, entardecerR, t);
		skyG = interpolarCor(diaG, entardecerG, t);
		skyB = interpolarCor(diaB, entardecerB, t);
	} else if (tempoDeDiaFase < 0.5f) {
		// Entardecer -> Noite
		float t = (tempoDeDiaFase - 0.25f) / 0.25f;
		skyR = interpolarCor(entardecerR, noiteR, t);
		skyG = interpolarCor(entardecerG, noiteG, t);
		skyB = interpolarCor(entardecerB, noiteB, t);
	} else if (tempoDeDiaFase < 0.75f) {
		// Noite -> Amanhecer
		float t = (tempoDeDiaFase - 0.5f) / 0.25f;
		skyR = interpolarCor(noiteR, amanhecerR, t);
		skyG = interpolarCor(noiteG, amanhecerG, t);
		skyB = interpolarCor(noiteB, amanhecerB, t);
	} else {
		// Amanhecer -> Dia
		float t = (tempoDeDiaFase - 0.75f) / 0.25f;
		skyR = interpolarCor(amanhecerR, diaR, t);
		skyG = interpolarCor(amanhecerG, diaG, t);
		skyB = interpolarCor(amanhecerB, diaB, t);
	}

	tempoDeDiaFase += VELOCIDADE_CICLO_DIA;
	if (tempoDeDiaFase > 1.0f) {
		tempoDeDiaFase -= 1.0f; // mantém o relogio sempre entre 0.0 e 1.0
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
// BORBOLETAS decorativas, sobrevoando os canteiros

float butterflyPhase = 0.0f;      // fase atual da animacao de voo (compartilhada por todas)
float butterflyPhaseSpeed = 0.12f; // velocidade com que a fase avanca a cada frame


// Desenha uma borboleta simples: um corpo bem fino no centro e duas asas
// ovais (feitas com drawDisk "achatado") que abrem e fecham, simulando o
// bater de asas. O parametro "fasePropria" desloca o ritmo de cada
// borboleta, para que elas nao fiquem todas batendo asa exatamente juntas.
void drawButterfly(float fasePropria, float asaR, float asaG, float asaB){

	float fase = butterflyPhase + fasePropria;

	// Bater de asas: a largura da asa oscila entre quase fechada e bem aberta
	float wingFlap = 0.35f + 0.65f * (float) fabs(sin(fase * 4.0f));

	// Um pequeno "passeio" no ar: sobe/desce e balança de um lado a outro
	float voarX = sin(fase) * 0.6f;
	float voarY = sin(fase * 2.3f) * 0.25f;

	glPushMatrix();
		glTranslatef(voarX, voarY, 0.0f);

		// Corpo (bem fino, no centro das asas)
		glColor3f(0.15f, 0.15f, 0.15f);
		glPushMatrix();
			glScalef(0.025f, 0.11f, 1.0f);
			drawSquare();
		glPopMatrix();

		// Asa esquerda
		glColor3f(asaR, asaG, asaB);
		glPushMatrix();
			glTranslatef(-0.11f, 0.03f, 0.0f);
			glScalef(wingFlap, 1.0f, 1.0f);
			drawDisk(0.13f);
		glPopMatrix();

		// Asa direita
		glPushMatrix();
			glTranslatef(0.11f, 0.03f, 0.0f);
			glScalef(wingFlap, 1.0f, 1.0f);
			drawDisk(0.13f);
		glPopMatrix();

	glPopMatrix();
}


void drawBackgroundContent(){

	// Fazer o background com 50 pixels de largura
	// Desenhar o ceu (a cor muda sozinha ao longo do tempo, veja atualizarCorDoCeu)
		glColor3f(skyR, skyG, skyB);
		glPushMatrix();
			// X = 24
			glScalef(bgWidth + (bgWidth / 2), 10, 1);
			drawSquare();
		glPopMatrix();

	// Desenhar a grama
		glColor3f(0.549f, 0.776f, 0.247f);
		glPushMatrix();
			glTranslatef(0.0f, -4.0f, 1.0f);
			// X = 24
			glScalef(bgWidth + (bgWidth / 2), 4.0f, 1);
			drawSquare();
		glPopMatrix();


	// Desenha cerca
		glPushMatrix();
			glTranslatef(16.0f, 0.5f, 1.0f);
			glScalef(1.5f, 1.0f, 1.0f);
			drawFence();
		glPopMatrix();



	// Desenha as nuvens
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



	// Desenhar canteiro 1
		glPushMatrix();
			//Desenha terra
			glColor3f(0.478f, 0.290f, 0.168f);

			glPushMatrix();
				glTranslatef(24.0f, -1.5f, 1.0f);
				glScalef(2.5f, 1.0f, 1.0f);
				drawSquare();
			glPopMatrix();


			// Desenha laranjas
			//glColor3f(0.909f, 0.447f, 0.172f);
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


			// Folhagens
			glColor3f(0.243f, 0.556f, 0.180f);
			glPushMatrix();
				glTranslatef(24.9f, 0.5f, 1.0f);
				glScalef(0.8f, 1.1f, 1.0f);
				glRotatef(180, 0, 0, 1);
				drawTriangle();
			glPopMatrix();


			glColor3f(0.243f, 0.556f, 0.180f);
			glPushMatrix();
				glTranslatef(23.2f, 0.5f, 1.0f);
				glScalef(0.5f, 1.3f, 1.0f);
				glRotatef(180, 0, 0, 1);
				drawTriangle();
			glPopMatrix();

		glPopMatrix();




	// Desenhar canteiro 2
		glPushMatrix();
			//Desenha terra
			glColor3f(0.478f, 0.290f, 0.168f);

			glPushMatrix();
				glTranslatef(29.0f, -1.5f, 1.0f);
				glScalef(2.0f, 1.0f, 1.0f);
				drawSquare();
			glPopMatrix();


			// Desenha laranjas
			//glColor3f(0.909f, 0.447f, 0.172f);
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



			// Folhagens
			glColor3f(0.243f, 0.556f, 0.180f);
			glPushMatrix();
				glTranslatef(29.6f, 0.5f, 1.0f);
				glScalef(0.9f, 1.5f, 1.0f);
				glRotatef(180, 0, 0, 1);
				drawTriangle();
			glPopMatrix();


		glPopMatrix();


	// Borboletas sobrevoando os canteiros, para deixar o cenario mais vivo.
	// Cada uma recebe um deslocamento de fase diferente (0.0, 2.0, 4.0...)
	// para que elas nao batam as asas todas exatamente no mesmo instante.
		glPushMatrix();
			glTranslatef(23.0f, 0.9f, 1.0f);
			drawButterfly(0.0f, 0.95f, 0.55f, 0.15f); // laranja
		glPopMatrix();

		glPushMatrix();
			glTranslatef(25.6f, 1.3f, 1.0f);
			drawButterfly(2.0f, 0.75f, 0.35f, 0.85f); // roxa
		glPopMatrix();

		glPushMatrix();
			glTranslatef(29.6f, 1.0f, 1.0f);
			drawButterfly(4.0f, 0.95f, 0.85f, 0.20f); // amarela
		glPopMatrix();


	/* O mundo é 32 pixels, logo os 8 primeiros pixels e os 8 ultimos precisam ser "iguais"
	 * para dar a impressão de cenario infinito ao jogador
	*/

	// Objetos recorrentes

	glPushMatrix();
		glTranslatef(4.0f, 5.0f, 1.0f);
		drawCloud();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(44.0f, 5.0f, 1.0f);
		drawCloud();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(4.0f, 0.5f, 1.0f);
		drawFence();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(44.0f, 0.5f, 1.0f);
		drawFence();
	glPopMatrix();


}


void drawBackground1(){

	// Tile 1: normal, na posição de origem
	glPushMatrix();
		drawBackgroundContent();
	glPopMatrix();

}



// Habilita a manipulacao por setinha
void arrowKeysDown(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
    	rightArrowPressed = true;
    }
    if (key == GLUT_KEY_LEFT)  {
    	leftArrowPressed = true;
    }
    if (key == GLUT_KEY_UP && !isJumping) {
    	upArrowPressed = true;
    	isJumping = true;
    	goingUp = true;

    }
    if (key == GLUT_KEY_DOWN) {
       	downArrowPressed = true;
       }
}

// Detecta quando as setinhas nao estao mais sendo pressionadas
void arrowKeysUp(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
    	rightArrowPressed = false;
    }
    if (key == GLUT_KEY_LEFT)  {
    	leftArrowPressed = false;
    }
    if (key == GLUT_KEY_UP) {
       	upArrowPressed = false;
	}
	if (key == GLUT_KEY_DOWN) {
		downArrowPressed = false; // correção: antes zerava "upArrowPressed" por engano
	}
}




// Indicam para outras funcoes se as teclas abaixo estao sendo pressionadas no teclado
bool r_key_pressed = false;
bool e_key_pressed = false;

// Funcao de captura de pressionamento de teclas no teclado.
void keyboard_callback(unsigned char key, GLint x, GLint y) {

	if (key == 101){ //letra e
		e_key_pressed = true;
	}

	if (key == 114) { //letra r
		r_key_pressed = true;
	}

}


void keyboard_up_callback(unsigned char key, GLint x, GLint y) {
    if (key == 101) { //letra e
        e_key_pressed = false;
    }
    if (key == 114) { // letra r
        r_key_pressed = false;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Movimenta os vegetais junto com o cenario e remove da tela os que ja
// passaram do coelho (eles ficam "inativos" e podem ser reaproveitados depois)
void moverVegetais(){
	for (Vegetal &veg : vegetais) {
		if (!veg.ativo) continue;

		veg.x -= bgSpeed; // anda junto com o cenario, na mesma velocidade do fundo

		if (veg.x < -10.0f) { // saiu da tela pela esquerda
			veg.ativo = false;
		}
	}
}


// Verifica se o coelho encostou em algum vegetal ativo. Usamos uma checagem
// simples de distancia (como se o coelho fosse um circulo) em vez de comparar
// os formatos exatos dos desenhos, o que deixa a colisao facil de entender.
void verificarColisaoComVegetais(){

	if (coelhoEscondido) return; // escondido na toca, o coelho nao alcança nada la fora

	// Centro aproximado do coelho na tela (o corpo dele fica um pouco a
	// frente da posicao "characterPos", por causa da cabeca e das orelhas)
	float centroCoelhoX = characterPos + 0.3f;
	float centroCoelhoY = jump_height + 0.3f;
	float raioDeCaptura = 1.1f; // "alcance" do coelho para pegar um vegetal

	for (Vegetal &veg : vegetais) {
		if (!veg.ativo) continue;

		float dx = veg.x - centroCoelhoX;
		float dy = veg.y - centroCoelhoY;
		float distancia = sqrt(dx * dx + dy * dy);

		if (distancia < raioDeCaptura) {
			veg.ativo = false;                   // o vegetal desaparece da tela
			aplicarBonusDoVegetal(veg.tipo);      // e concede o bonus correspondente
		}
	}
}



// Controla o "relogio" que decide quando o proximo vegetal vai aparecer,
// para que eles surjam aos poucos, um de cada vez, e nao todos juntos.
void controlarSurgimentoDeVegetais(){

	framesAteProximoVegetal--;

	if (framesAteProximoVegetal <= 0) {
		spawnVegetable();
		// Sorteia quantos frames faltam ate o proximo vegetal (entre ~1,4s e ~3,4s)
		framesAteProximoVegetal = 60 + (rand() % 80);
	}
}


// Conta o tempo restante dos bonus de turbo (cenoura) e pulo alto (rabanete),
// e devolve o coelho ao normal assim que o tempo acaba.
void atualizarBonusAtivos(){

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


// Enquanto o coelho esta escondido na toca, ele recupera folego mais rapido
// do que o normal (como se estivesse descansando la dentro)
const float RECUPERACAO_FOLEGO_ESCONDIDO = RECUPERACAO_FOLEGO_POR_FRAME * 2.0f;


// Controla o "correr" do coelho: enquanto o jogador segura a tecla R E
// ainda sobra folego, o coelho corre mais rapido, gastando folego aos
// poucos. Quando solta a tecla (ou o folego acaba), ele volta ao normal e
// o folego comeca a se recuperar sozinho. Quando ele esta escondido na
// toca, nao tem como correr de jeito nenhum -- so descansa e recupera
// folego (mais rapido do que recuperaria parado la fora).
void atualizarCorrida(){

	if (coelhoEscondido) {

		coelhoCorrendo = false;

		folegoAtual += RECUPERACAO_FOLEGO_ESCONDIDO;
		if (folegoAtual > FOLEGO_MAXIMO) {
			folegoAtual = FOLEGO_MAXIMO;
		}

		return; // escondido nao mexe em characterSpeed, ja que nao esta se movendo
	}

	// So consegue correr se estiver segurando "R" e ainda tiver folego sobrando
	if (r_key_pressed && folegoAtual > 0.0f) {

		coelhoCorrendo = true;

		folegoAtual -= GASTO_FOLEGO_POR_FRAME;
		if (folegoAtual < 0.0f) {
			folegoAtual = 0.0f; // acabou o folego, nao deixa ficar negativo
		}

	} else {

		coelhoCorrendo = false;

		// Enquanto nao esta correndo, o folego vai voltando aos poucos
		folegoAtual += RECUPERACAO_FOLEGO_POR_FRAME;
		if (folegoAtual > FOLEGO_MAXIMO) {
			folegoAtual = FOLEGO_MAXIMO;
		}
	}

	// O turbo da cenoura e mais forte e tem prioridade: se ele estiver
	// ativo, a velocidade de corrida normal nao mexe em nada aqui (quem
	// controla characterSpeed nesse caso e atualizarBonusAtivos)
	if (framesDeTurboRestantes == 0) {
		characterSpeed = coelhoCorrendo ? VELOCIDADE_CORRENDO : VELOCIDADE_NORMAL;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Faz a raposa aparecer na tela, pronta para perseguir o coelho. Ela sempre
// entra pela borda ESQUERDA da tela, um pouco fora da area visivel.
void spawnRaposa(){

	foxActive = true;
	foxX = -11.0f; // sempre entra pela esquerda (a tela vai de -8 a 8)
	foxJaTirouVidaNestaPassagem = false; // nova passagem, pode tirar vida de novo se colidir
}


// Controla o "relogio" que decide quando a raposa vai aparecer de novo.
// Ela surge periodicamente, em um intervalo aleatorio entre 15 e 45 segundos,
// e so sorteia um novo intervalo quando ela nao esta mais ativa na tela.
void controlarSurgimentoDaRaposa(){

	if (foxActive) return; // ja esta perseguindo o coelho, entao nao sorteia outra

	framesAteProximaRaposa--;

	if (framesAteProximaRaposa <= 0) {
		spawnRaposa();
		framesAteProximaRaposa = (1 + rand() % 15) * FRAMES_POR_SEGUNDO; // entre 15 e 45 segundos
	}
}


// Move a raposa sempre em direcao a DIREITA da tela, numa linha reta,
// sem se importar com a posicao do coelho. Ao sair da area visivel, ela e
// "destruida": some da cena e libera o relogio para a proxima aparicao.
void moverRaposa(){

	if (!foxActive) return;

	foxX += VELOCIDADE_RAPOSA;
	foxDirecao = 1.0f; // sempre olhando/correndo pra direita

	if (foxX > 10.0f) { // passou da borda direita da tela (visivel vai de -8 a 8)
		foxActive = false; // "destroi" a raposa: ela deixa de existir na cena
		framesAteProximaRaposa = (15 + rand() % 31) * FRAMES_POR_SEGUNDO; // sorteia quando ela volta
	}
}


// Verifica se a raposa encostou no coelho. A checagem usa a mesma ideia da
// colisao com os vegetais (distancia entre dois "circulos"), mas aqui ela
// tambem leva em conta a altura do pulo: se o coelho estiver pulando bem
// alto na hora em que a raposa passa perto, ele passa por cima dela e escapa!
//
// Ao colidir, a raposa NAO desaparece: ela continua correndo em frente
// normalmente, so o coelho perde uma vida (apenas uma vez por passagem,
// mesmo que a colisao dure varios frames seguidos).
void verificarColisaoComRaposa(){

	if (!foxActive) return;

	// O coelho esta escondido na toca: a raposa passa reto, sem alcança-lo
	if (coelhoEscondido) return;

	float dx = foxX - characterPos;
	float dy = foxY - jump_height;
	float distancia = sqrt(dx * dx + dy * dy);

	if (distancia < RAIO_COLISAO_RAPOSA) {

		if (!foxJaTirouVidaNestaPassagem) {
			if (rabbitLives > 0) {
				rabbitLives--; // o jogador perde uma vida
			}
			foxJaTirouVidaNestaPassagem = true; // trava ate a raposa se afastar de novo
		}

	} else {
		// A raposa se afastou o suficiente: libera para tirar vida de novo
		// caso ela volte a encostar no coelho mais pra frente
		foxJaTirouVidaNestaPassagem = false;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Função que controla a animacao
void anim (int valor) {


	// Verifica se o coelho esta entrando ou saindo de uma toca. Isso e feito
	// antes de tudo, para que o restante da funcao ja saiba se o coelho
	// esta escondido (e, portanto, nao deve se mover nem pular) neste frame.
	atualizarEsconderijoDoCoelho();


	// Calcula a velocidade deste frame (normal, "correndo" ou "descansando
	// na toca") antes de usa-la nos blocos das setas, logo abaixo. Chamamos
	// sempre, mesmo com o coelho escondido, pois e ela quem recupera o
	// folego dele enquanto esta na toca.
	atualizarCorrida();


	// Enquanto o coelho estiver escondido na toca, ele fica parado: nao
	// processa as setas de movimento nem a animacao de pulo.
	if (!coelhoEscondido) {

		// inputs inseridos para rodar com a setinha esquerda do teclado
		if (leftArrowPressed) {

			  direcaoCoelho = -1.0f; // vira pra esquerda
			  ///////////////////////////////////
			  // Permite o quadrado a andar pra esquerda pela cena
			  if (characterPos < -8.0f) { // esse comando não deixa o quadrado sair da tela, pra mudar o limite, olhar o glOrtho, está definido para 8 agora!
				  characterPos -= 0;
			  } else {
				  characterPos -= characterSpeed;
			  }

			  ///////////////////////////////////
			  // Rotaciona o quadrado qdo anda pra esquerda
			  //squareAngle += squareAngleSpeed;

		}


		// inputs inseridos para rodar com a setinha direita do teclado
		if (rightArrowPressed) {

			  direcaoCoelho = 1.0f; // vira pra direita
			  ///////////////////////////////////
			  // Permite o quadrado a andar pra direita pela cena
				if (characterPos > 8.0f) { // esse if não deixa o quadrado sair da tela, pra mudar o limite, olhar o glOrtho, está definido para 8 agora!
					characterPos += 0;
				} else {
					characterPos += characterSpeed;
				}

			  ///////////////////////////////////

		 }


		  //////////////////////////////////////////////////////////////////////////////////////
		  // Controla a animação de pulo do quadrado
		  if (isJumping) {
			  if (goingUp) {
				  jump_height += speed_jump;
				  if (jump_height >= jump_maximum_height) {
					  jump_height = jump_maximum_height; // trava no topo
					  goingUp = false; // começa a fase de descida
				  }
			  } else {
				  jump_height -= speed_jump;
				  if (jump_height <= 0.5f) {
					  jump_height = 0.5f; // trava no chão certinho
					  isJumping = false; // pulo terminou, pode pular de novo
				  }
			  }
		  }

	}


     //////////////////////////////////////////////////////////////////////////////////////
     // Controla a animacao de corrida do coelho (patas e orelhas)
     // Avanca sempre, independente de o coelho estar se movendo pela cena ou nao,
     // para que ele fique "sempre" animado.
     walkPhase += walkPhaseSpeed;
     if (walkPhase > 2 * PI) {
    	 walkPhase -= 2 * PI; // mantém o valor sempre dentro de uma faixa, sem crescer pra sempre
     }


     //////////////////////////////////////////////////////////////////////////////////////
     // A RAPOSA NAO CONGELA: mesmo com o coelho escondido na toca, ela
     // continua correndo da esquerda pra direita normalmente (patas, orelhas,
     // cauda, surgimento e movimento). E assim que o coelho consegue "esperar
     // ela passar" escondido, sem ficar preso enquanto ela atravessa a tela.
     // (a colisao em si e ignorada dentro de verificarColisaoComRaposa quando
     // o coelho esta escondido, entao ele nao perde vida mesmo se ela passar por cima)
     foxWalkPhase += foxWalkPhaseSpeed;
     if (foxWalkPhase > 2 * PI) {
    	 foxWalkPhase -= 2 * PI;
     }

     foxTailPhase += foxTailPhaseSpeed;
     if (foxTailPhase > 2 * PI) {
    	 foxTailPhase -= 2 * PI;
     }

     foxTailTipPhase += foxTailTipPhaseSpeed;
     if (foxTailTipPhase > 2 * PI) {
    	 foxTailTipPhase -= 2 * PI;
     }

     controlarSurgimentoDaRaposa();
     moverRaposa();
     verificarColisaoComRaposa();


     //////////////////////////////////////////////////////////////////////////////////////
     // AS BORBOLETAS TAMBEM NAO CONGELAM: sao só um detalhe decorativo
     // sobrevoando os canteiros, entao nao faz sentido elas pararem no ar
     // junto com o resto do cenario.
     butterflyPhase += butterflyPhaseSpeed;
     if (butterflyPhase > 2 * PI) {
    	 butterflyPhase -= 2 * PI;
     }


     //////////////////////////////////////////////////////////////////////////////////////
     // Nuvens (o balanço/flutuação delas) -- NAO congela: continuam
     // flutuando no ceu normalmente, mesmo com o coelho escondido.
     cloudPhase += cloudPhaseSpeed;
     if (cloudPhase > 2 * PI) {
    	 cloudPhase -= 2 * PI;
     }


     //////////////////////////////////////////////////////////////////////////////////////
     // Cor do ceu (ciclo de amanhecer/dia/entardecer/noite) -- NAO congela:
     // o tempo continua passando mesmo com o coelho escondido.
     atualizarCorDoCeu();


     //////////////////////////////////////////////////////////////////////////////////////
     // DAQUI PRA BAIXO E O QUE REALMENTE "CONGELA" enquanto o coelho esta
     // escondido: a rolagem do CENARIO de fundo (chao, cerca, canteiros), as
     // TOCAS e os VEGETAIS (surgimento, movimento, colisao e bonus). Só essas
     // coisas ficam paradas -- tudo o mais (raposa, borboletas, nuvens, ceu)
     // continua se movendo/animando normalmente.
     if (!coelhoEscondido) {

	     // Rolagem do fundo (chao, cerca, canteiros)
	     bgPos -= bgSpeed;
	     bgPos = fmod(bgPos, bgWidth);

	     // Tocas
	     atualizarTocas();

	     // Vegetais de bonificacao: surgimento, movimento, colisao e os
	     // bonus (turbo/pulo alto) que estiverem ativos
	     controlarSurgimentoDeVegetais();
	     moverVegetais();
	     verificarColisaoComVegetais();
	     atualizarBonusAtivos();

     }




	//======================================================================================================================================================================================================================================================
	// Comandos padrao da funcao anim
	FrameNumber++;
	glutPostRedisplay();
	glutTimerFunc(msecs, anim, valor);
}


void display() {

	//glClearColor(1.0, 1.0, 0.0, 1.0);
	// Limpa a janela, colocando na tela a cor definida pela função glClearColor
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity();



	//////////////////////////////////////////////////////////////////////////////////////////////
	// Alterar a display daqui pra baixo

	//Exemplo de background que se move infinitamente em OpenGL
		// Exemplo de background que se move infinitamente em OpenGL[cite: 1]

		glPushMatrix();
			glTranslatef(bgPos, 0, 1);
			drawBackground1();
		glPopMatrix();



		glPushMatrix();
			glTranslatef(6.5f, 6.8f, 1);
			drawSun();
		glPopMatrix();


	// Desenha cada toca (buraco) que estiver na tela
		for (const Toca &toca : tocas) {
			glPushMatrix();
				glTranslatef(toca.x, toca.y, 1.0f);
				drawToca();
			glPopMatrix();
		}


	// Desenha a raposa somente enquanto ela estiver perseguindo o coelho
		if (foxActive) {
			glPushMatrix();
				glTranslatef(foxX, foxY, 1.0f);
				glScalef(0.9f * foxDirecao, 0.8f, 1.0f); // mesmo tamanho do coelho, virada pro lado que esta correndo
				drawFox();
			glPopMatrix();
		}



	// Desenha cada vegetal de bonificacao que estiver ativo na cena
		for (const Vegetal &veg : vegetais) {
			if (!veg.ativo) continue;

			glPushMatrix();
				glTranslatef(veg.x, veg.y, 1.0f);
				drawVegetable(veg.tipo);
			glPopMatrix();
		}

	// Criando coelho com input do usuario na tela
		glColor3f(0, 0, 0);

		if (coelhoEscondido) {

			// Escondido: so mostramos as orelhas espiando pra fora do
			// buraco, pra deixar claro que o coelho ainda esta ali,
			// só que a salvo. As orelhas ficam mais curtas (metade pra
			// fora) do que as orelhas normais do coelho.
			const Toca &tocaAtual = tocas[tocaOndeEstaEscondido];

			glPushMatrix();
				glTranslatef(tocaAtual.x, tocaAtual.y + 0.18f, 1.0f);
				glScalef(0.4f * direcaoCoelho, 0.4f, 1.0f);
				glColor3f(0.96f, 0.93f, 0.89f);

				glPushMatrix();
					glRotatef(10.0f, 0, 0, 1);
					glScalef(0.125f, 0.6f, 1.0f);
					drawTriangle();
				glPopMatrix();

				glPushMatrix();
					glTranslatef(0.5f, 0.0f, 0.0f);
					glRotatef(-10.0f, 0, 0, 1);
					glScalef(0.125f, 0.6f, 1.0f);
					drawTriangle();
				glPopMatrix();
			glPopMatrix();

		} else {

			glPushMatrix();
				glTranslatef(characterPos, jump_height, 1.0f);
				//glRotatef(float(squareAngle), 0, 0, 1);
				glScalef(0.4f * direcaoCoelho, 0.4f, 1.0f);
				drawRabbit();
			glPopMatrix();

		}


	// HUD: mostra as vidas do coelho e, quando ativos, os bonus de turbo e pulo alto
		glColor3f(0.0f, 0.0f, 0.0f);
		char textoVidas[32];
		snprintf(textoVidas, sizeof(textoVidas), "Vidas: %d", rabbitLives);
		drawText(-7.7f, 7.2f, textoVidas);

		if (framesDeTurboRestantes > 0) {
			drawText(-7.7f, 6.6f, "Turbo de velocidade!");
		}
		if (framesDePuloReforcadoRestantes > 0) {
			drawText(-7.7f, 6.0f, "Pulo reforcado!");
		}
		if (foxActive) {
			drawText(-7.7f, 5.4f, "Cuidado, a raposa esta te perseguindo!");
		}

		if (coelhoEscondido) {
			// O coelho esta escondido: avisa que ele esta a salvo por enquanto
			drawText(-7.7f, 4.8f, "Escondido na toca - a salvo da raposa!");
		} else {
			// Nao esta escondido: se estiver perto o suficiente de uma toca,
			// mostra uma dica de como se esconder nela
			for (const Toca &toca : tocas) {
				if (fabs(toca.x - characterPos) < RAIO_TOCA) {
					drawText(-7.7f, 4.8f, "Aperte a seta para baixo para se esconder!");
					break;
				}
			}
		}


	// Barra de folego: um retangulo cinza (o "fundo", sempre do tamanho
	// maximo) com outro retangulo verde por cima, que encolhe conforme o
	// folego vai acabando. E facil de entender: quanto mais verde, mais
	// folego o coelho ainda tem pra correr.
		float folegoPorcentagem = folegoAtual / FOLEGO_MAXIMO; // de 0.0 (vazio) a 1.0 (cheio)
		float larguraBarra = 1.6f;
		float alturaBarra = 0.12f;

		glPushMatrix();
			glTranslatef(-7.7f, 4.15f, 1.0f);

			// Fundo da barra (cinza escuro)
			glColor3f(0.35f, 0.35f, 0.35f);
			glPushMatrix();
				glTranslatef(larguraBarra / 2.0f, 0.0f, 0.0f);
				glScalef(larguraBarra / 2.0f, alturaBarra, 1.0f);
				drawSquare();
			glPopMatrix();

			// Parte preenchida (verde), proporcional ao folego atual
			glColor3f(0.30f, 0.75f, 0.35f);
			glPushMatrix();
				glTranslatef((larguraBarra * folegoPorcentagem) / 2.0f, 0.0f, 0.0f);
				glScalef((larguraBarra * folegoPorcentagem) / 2.0f, alturaBarra, 1.0f);
				drawSquare();
			glPopMatrix();

		glPopMatrix();

		glColor3f(0.0f, 0.0f, 0.0f);
		drawText(-7.7f, 3.75f, "Folego (segure R para correr)");


	// Alterar a display daqui pra cima
	//////////////////////////////////////////////////////////////////////////////////////////////

	glutSwapBuffers();

}



int main(int argc, char** argv)
{

	// "Semeia" o gerador de numeros aleatorios com o horario atual, para que
	// os vegetais sorteados (tipo e posicao) sejam diferentes a cada execucao
	srand(static_cast<unsigned int>(time(NULL)));

	//Inicializa a biblioteca GLUT e negocia uma seção com o gerenciador de janelas.
	//É possível passar argumentos para a função glutInit provenientes da linha de execução, tais como informações sobre a geometria da tela
	glutInit(&argc, argv);

	//Informa à biblioteca GLUT o modo do display a ser utilizado quando a janela gráfica for criada.
	// O flag GLUT_SINGLE força o uso de uma janela com buffer simples, significando que todos os desenhos serão feitos diretamente nesta janela.
	// O flag GLUT_RGB determina que o modelo de cor utilizado será o modelo RGB.
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

	//Define o tamanho inicial da janela, 256x256 pixels, e a posição inicial do seu canto superior esquerdo na tela, (x, y)=(100, 100).
	glutInitWindowSize (800, 800);
	glutInitWindowPosition (200, 200);

	// Cria uma janela e define seu título
	glutCreateWindow("Trabalho 1 - Computacao Grafica");

	//Nesta função é definido o estado inicial do OpenGL. Ajustes podem ser feitos para o usuário nessa função.
	init();

	// Define display() como a função de desenho (display callback) para a janela corrente.
	// Quando GLUT determina que esta janela deve ser redesenhada, a função de desenho é chamada.
	glutDisplayFunc(display);


	// Controla a animacao
	glutTimerFunc(msecs, anim, 0);

	// Controla o input de teclado pelo usuario
	glutKeyboardFunc(keyboard_callback);
	glutKeyboardUpFunc(keyboard_up_callback);

	// Controla o input por setinha do teclado feito pelo usuario
	glutSpecialFunc(arrowKeysDown);
	glutSpecialUpFunc(arrowKeysUp);

	//Inicia o loop de processamento de desenhos com GLUT.
	// Esta rotina deve ser chamada pelo menos uma vez em um programa que utilize a biblioteca GLUT.
	glutMainLoop();

	return 0;

}
