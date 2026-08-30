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
#include <numbers>
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

// Velocidade normal do coelho e velocidade durante o "turbo" dado pela cenoura
const float VELOCIDADE_NORMAL = 0.1f;
const float VELOCIDADE_TURBO = 0.30f;
int framesDeTurboRestantes = 0;              // enquanto > 0, o turbo esta ativo
const int DURACAO_TURBO_EM_FRAMES = 150;     // ~3,6 segundos de turbo

// Altura de pulo normal e altura durante o bonus dado pelo rabanete
const float PULO_NORMAL = 3.5f;
const float PULO_REFORCADO = 5.0f;
int framesDePuloReforcadoRestantes = 0;          // enquanto > 0, o pulo alto esta ativo
const int DURACAO_PULO_REFORCADO_EM_FRAMES = 300; // ~7,2 segundos de pulo reforcado


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



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha o background "infinito"

float bgSpeed = 0.1f;      // velocidade do fundo (independente do carro)
float bgWidth = 40.0f;     // largura de uma "tile" do fundo (2 * 50 do glScalef)
float bgPos = 0.0f;         // posição atual do fundo


void drawBackgroundContent(){

	// Fazer o background com 50 pixels de largura
	// Desenhar o ceu
		glColor3f(0.68f, 0.81f, 0.98f);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quadrado controlado pelo usuario

// Controla o pulo do quadrado andante
float jump_maximum_height = 3.5f;
float speed_jump = 0.15f;
float jump_height = 0.5f;
bool isJumping = false;
bool goingUp = true; // controla se está na fase de subida ou descida do pulo


// Controlam o movimento no eixo x do quadrado andante
float squarePos = 0.0f;
float squareSpeed = 0.1f;


// booleanos que controlam se a setinha do teclado está pressionada (neste caso, estas teclas sao especiais)
bool rightArrowPressed = false;
bool leftArrowPressed = false;
bool upArrowPressed = false;
bool downArrowPressed = false;


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
		upArrowPressed = false;
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

	// Centro aproximado do coelho na tela (o corpo dele fica um pouco a
	// frente da posicao "squarePos", por causa da cabeca e das orelhas)
	float centroCoelhoX = squarePos + 0.3f;
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
		squareSpeed = VELOCIDADE_TURBO;
		if (framesDeTurboRestantes == 0) {
			squareSpeed = VELOCIDADE_NORMAL;
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


//////////////////////////////////////////////////////////////////////////////////////////////
// Função que controla a animacao
void anim (int valor) {


	// inputs inseridos para rodar com a setinha esquerda do teclado
	if (leftArrowPressed) {

		  direcaoCoelho = -1.0f; // vira pra esquerda
		  ///////////////////////////////////
		  // Permite o quadrado a andar pra esquerda pela cena
		  if (squarePos < -8.0f) { // esse comando não deixa o quadrado sair da tela, pra mudar o limite, olhar o glOrtho, está definido para 8 agora!
			  squarePos -= 0;
		  } else {
			  squarePos -= squareSpeed;
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
			if (squarePos > 8.0f) { // esse if não deixa o quadrado sair da tela, pra mudar o limite, olhar o glOrtho, está definido para 8 agora!
				squarePos += 0;
			} else {
				squarePos += squareSpeed;
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


     //////////////////////////////////////////////////////////////////////////////////////
     // Controla a animacao de corrida do coelho (patas e orelhas)
     // Avanca sempre, independente de o coelho estar se movendo pela cena ou nao,
     // para que ele fique "sempre" animado.
     walkPhase += walkPhaseSpeed;
     if (walkPhase > 2 * PI) {
    	 walkPhase -= 2 * PI; // mantém o valor sempre dentro de uma faixa, sem crescer pra sempre
     }



     //////////////////////////////////////////////////////////////////////////////////////
     // Controla a animacao de "flutuar" das nuvens
     cloudPhase += cloudPhaseSpeed;
     if (cloudPhase > 2 * PI) {
    	 cloudPhase -= 2 * PI; // mantém o valor sempre dentro de uma faixa, sem crescer pra sempre
     }



     //////////////////////////////////////////////////////////////////////////////////////
     // Controla o background de fundo
     bgPos -= bgSpeed;
     bgPos = fmod(bgPos, bgWidth); // mantém o valor sempre dentro de uma faixa, sem crescer pra sempre


     //////////////////////////////////////////////////////////////////////////////////////
     // Controla os vegetais de bonificacao: faz eles surgirem aos poucos,
     // andarem pela tela junto com o cenario, sumirem quando capturados
     // pelo coelho, e atualiza os bonus (turbo/pulo alto) que estiverem ativos
     controlarSurgimentoDeVegetais();
     moverVegetais();
     verificarColisaoComVegetais();
     atualizarBonusAtivos();




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


		glPushMatrix();
			glTranslatef(3.0f, 5.0f, 1);
			drawLettuce();
		glPopMatrix();



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
		glPushMatrix();
			glTranslatef(squarePos, jump_height, 1.0f);
			//glRotatef(float(squareAngle), 0, 0, 1);
			glScalef(0.5f * direcaoCoelho, 0.5f, 1.0f);
			drawRabbit();
		glPopMatrix();


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
