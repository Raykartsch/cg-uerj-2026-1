// Run these commands in terminal to execute this code in Linux:
// 1. g++ -o tabuleiro_xadrez tabuleiro_xadrez.cpp -lglut -lGLU -lGL -lm
// 2. ./tabuleiro_xadrez

// Run these commands in terminal to execute this code in Windows:
// 1. g++ tabuleiro_xadrez.cpp -o tabuleiro_xadrez -lfreeglut -lglu32 -lopengl32
// 2. .\tabuleiro_xadrez


//////////////////////////////////////////////////////////////////////////////////////////////
// ANOTAÇÕES:
// - Tabuleiro 8x8 desenhado com drawSquare, cores alternando conforme (linha+coluna) par/ímpar.
// - O peão é representado como uma "pessoa" simples: cabeça (drawDisk) + corpo (drawSquare).
// - Diferente dos jogos anteriores (movimento livre/contínuo), aqui o movimento é
//   POR CASA: cada aperto de seta anda exatamente 1 casa e já para -> não precisa
//   de booleano + anim() como nos outros códigos, o próprio evento de tecla já
//   move o peão diretamente (movimento discreto, tipo jogo de tabuleiro).
// - pawnCol/pawnRow guardam a posição do peão em "coordenadas de tabuleiro" (0 a 7),
//   e são convertidas pra coordenadas de tela só na hora de desenhar.
//////////////////////////////////////////////////////////////////////////////////////////////


#include <GL/glut.h>
#include <cmath>

double PI = 3.14;

int FrameNumber = 0;
int msecs = 24; // intervalo do timer da animação (em milissegundos)


void init(void);
void display(void);


void init(void)
{
    // define a cor de background da janela
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // define o sistema de visualização - tipo de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-8, 8, -8, 8, -8, 8);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Formas básicas (mesmo padrão usado nos códigos anteriores)
void drawSquare()
{
    glBegin(GL_POLYGON);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(-1, 1, 0);
    glEnd();
}

void drawDisk(double radius)
{
    int d;
    glBegin(GL_POLYGON);
    for (d = 0; d < 32; d++) {
        double angle = (2 * PI / 32) * d;
        glVertex3f(radius * cos(angle), radius * sin(angle), 0);
    }
    glEnd();
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Tabuleiro de xadrez 8x8

const int BOARD_SIZE = 8;
const float SQUARE_SIZE = 1.0f; // cada casa ocupa 1 unidade (tabuleiro fica de -4 a 4)

// Converte coordenada de tabuleiro (0 a 7) pra coordenada de tela (centro da casa)
float boardToScreen(int coord) {
    // -4 é a borda esquerda/inferior do tabuleiro; +0.5 centraliza dentro da casa
    return -4.0f + coord * SQUARE_SIZE + (SQUARE_SIZE / 2.0f);
}

void drawBoard()
{
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {

            // Alterna a cor da casa conforme a soma linha+coluna ser par ou ímpar
            if ((row + col) % 2 == 0) {
                glColor3f(0.93f, 0.85f, 0.68f); // casa clara (bege)
            } else {
                glColor3f(0.45f, 0.29f, 0.15f); // casa escura (marrom)
            }

            glPushMatrix();
                glTranslatef(boardToScreen(col), boardToScreen(row), 0);
                glScalef(SQUARE_SIZE / 2.0f, SQUARE_SIZE / 2.0f, 1); // drawSquare vai de -1 a 1 -> escala pra caber na casa
                drawSquare();
            glPopMatrix();
        }
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Peão controlado pelo usuário (representado como uma pessoa simples)

int pawnCol = 4; // posição inicial (coluna "e", se fosse notação de xadrez)
int pawnRow = 1; // posição inicial (linha 2, típica posição inicial de peão)

void drawPawn()
{
    // Corpo (tronco)
    glColor3f(0.1f, 0.1f, 0.8f); // azul
    glPushMatrix();
        glTranslatef(0, -0.15f, 1);
        glScalef(0.25f, 0.35f, 1);
        drawSquare();
    glPopMatrix();

    // Cabeça
    glColor3f(0.94f, 0.78f, 0.63f); // tom de pele
    glPushMatrix();
        glTranslatef(0, 0.35f, 1);
        drawDisk(0.18);
    glPopMatrix();
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Movimento do peão: DISCRETO e SÓ NA DIAGONAL.
//
// O GLUT entrega uma tecla por evento -> pra conseguir diagonal, rastreamos se cada
// seta está pressionada (booleanos) e, quando DUAS setas adjacentes estão seguradas
// ao mesmo tempo (ex: CIMA + DIREITA), movemos o peão na diagonal correspondente.
//
// diagonalMoveLock evita que o peão ande várias casas de uma vez só (o sistema
// operacional dispara o evento de "tecla pressionada" repetidamente enquanto você
// segura) -> a trava só libera de novo quando alguma das duas teclas é solta.

bool upPressed = false;
bool downPressed = false;
bool leftPressed = false;
bool rightPressed = false;
bool diagonalMoveLock = false;

void tryMoveDiagonal() {
    if (diagonalMoveLock) return; // já moveu nessa combinação, espera soltar uma tecla

    if (upPressed && rightPressed) {
        if (pawnCol < BOARD_SIZE - 1) pawnCol++;
        if (pawnRow < BOARD_SIZE - 1) pawnRow++;
        diagonalMoveLock = true;
    }
    else if (upPressed && leftPressed) {
        if (pawnCol > 0) pawnCol--;
        if (pawnRow < BOARD_SIZE - 1) pawnRow++;
        diagonalMoveLock = true;
    }
    else if (downPressed && rightPressed) {
        if (pawnCol < BOARD_SIZE - 1) pawnCol++;
        if (pawnRow > 0) pawnRow--;
        diagonalMoveLock = true;
    }
    else if (downPressed && leftPressed) {
        if (pawnCol > 0) pawnCol--;
        if (pawnRow > 0) pawnRow--;
        diagonalMoveLock = true;
    }
    // se só uma seta estiver pressionada (movimento reto), nada acontece de propósito
}

void arrowKeysDown(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) { rightPressed = true; }
    if (key == GLUT_KEY_LEFT)  { leftPressed = true; }
    if (key == GLUT_KEY_UP)    { upPressed = true; }
    if (key == GLUT_KEY_DOWN)  { downPressed = true; }

    tryMoveDiagonal();
}

void arrowKeysUp(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) { rightPressed = false; }
    if (key == GLUT_KEY_LEFT)  { leftPressed = false; }
    if (key == GLUT_KEY_UP)    { upPressed = false; }
    if (key == GLUT_KEY_DOWN)  { downPressed = false; }

    diagonalMoveLock = false; // libera a trava pra permitir o próximo movimento
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Função que controla a animação (só precisa girar o "relógio" da cena, sem lógica de movimento)
void anim(int valor)
{
    FrameNumber++;
    glutPostRedisplay();
    glutTimerFunc(msecs, anim, valor);
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    //////////////////////////////////////////////////////////////////////////////////////////
    // Desenha o tabuleiro
    drawBoard();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Desenha o peão na casa correspondente à posição atual
    glPushMatrix();
        glTranslatef(boardToScreen(pawnCol), boardToScreen(pawnRow), 1);
        drawPawn();
    glPopMatrix();

    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Tabuleiro de Xadrez com Peao");

    init();

    glutDisplayFunc(display);

    // Controla a animação (mantém o timer rodando, mesmo sem lógica contínua de movimento)
    glutTimerFunc(msecs, anim, 0);

    // Controla o input por setinha do teclado (movimento em diagonal, precisa de 2 setas)
    glutSpecialFunc(arrowKeysDown);
    glutSpecialUpFunc(arrowKeysUp);

    glutMainLoop();

    return 0;
}
