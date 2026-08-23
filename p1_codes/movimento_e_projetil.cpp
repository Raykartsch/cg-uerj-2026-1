// Run these commands in terminal to execute this code in Linux:
// 1. g++ -o movimento_e_projetil movimento_e_projetil.cpp -lglut -lGLU -lGL -lm
// 2. ./movimento_e_projetil

// Run these commands in terminal to execute this code in Windows:
// 1. g++ movimento_e_projetil.cpp -o movimento_e_projetil -lfreeglut -lglu32 -lopengl32
// 2. .\movimento_e_projetil


//////////////////////////////////////////////////////////////////////////////////////////////
// ANOTAÇÕES:
// - O personagem (quadrado preto) fica sempre fixo no centro da tela (origem).
// - Quem se move de verdade é o MUNDO (background e projéteis), na direção
//   OPOSTA ao movimento do personagem -> ilusão de mapa/câmera seguindo o jogador.
// - squareX/squareY guardam a posição do personagem NO MUNDO, não na tela.
// - O background usa tiling horizontal com espelhamento (normal + espelhado lado a lado)
//   para dar a ilusão de scroll infinito em X, sem repetição óbvia.
// - Y não tem tiling (ver observação no final do arquivo).
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
    glOrtho(-8, 8, -8, 8, -8, 8); // câmera enxerga de -8 a 8 em X e Y
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Formas básicas
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
// Elementos de decoração do background (sol e nuvem)

void drawSun()
{
    int i;
    glColor3f(0.9, 0.9, 0);
    glLineWidth(3);
    glRotatef(float(-FrameNumber), 0, 0, 1);
    glBegin(GL_LINES);
    for (i = 0; i < 15; i++) {
        glVertex2f(0, 0);
        glVertex2d(cos(i * 2 * PI / 15), sin(i * 2 * PI / 15));
    }
    glEnd();
    drawDisk(0.5);
    glColor3f(0, 0, 0);
}

void drawCloud()
{
    glColor3f(1, 1, 1);

    glPushMatrix();
        glTranslatef(-0.6f, -0.2f, 1);
        drawDisk(0.5f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, -0.1f, 1);
        drawDisk(0.7f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.6f, -0.2f, 1);
        drawDisk(0.5f);
    glPopMatrix();
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Background com tiling horizontal (normal + espelhado)

float bgTileWidth = 20.0f;                  // largura de UM tile
float bgPairWidth = bgTileWidth * 2.0f;     // drawBackground1() desenha um PAR (normal + espelhado)

void drawBackgroundContent()
{
    // Desenhar o céu
    glColor3f(0.53, 0.81, 0.98);
    glPushMatrix();
        glScalef(10, 10, 1);
        drawSquare();
    glPopMatrix();

    // Desenhar a grama
    glColor3f(0.486f, 0.988f, 0.0f);
    glPushMatrix();
        glTranslatef(0.0f, -4.0f, 1.0f);
        glScalef(10.0f, 4.0f, 1);
        drawSquare();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(7.0f, 7.0f, 1);
        drawSun();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, 4.0, 1);
        drawCloud();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(5, 6.0, 1);
        drawCloud();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-6, 5.0, 1);
        drawCloud();
    glPopMatrix();
}

void drawBackground1()
{
    // Tile 1: normal, na posição de origem
    glPushMatrix();
        drawBackgroundContent();
    glPopMatrix();

    // Tile 2: espelhado, colado logo ao lado do primeiro
    glPushMatrix();
        glTranslatef(bgTileWidth, 0, 0);
        glScalef(-1, 1, 1); // inverte o eixo X -> espelha o conteúdo
        drawBackgroundContent();
    glPopMatrix();
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Personagem controlado pelo usuário
// squareX / squareY representam a posição do personagem NO MUNDO (não na tela!)

float squareX = 0.0f;
float squareY = 0.0f;
float squareSpeed = 0.2f;

// Direção que o personagem está "olhando" (usada pra saber pra onde atirar)
float facingX = 1.0f; // começa olhando pra direita
float facingY = 0.0f;

// booleanos que controlam se a setinha do teclado está pressionada
bool rightArrowPressed = false;
bool leftArrowPressed = false;
bool upArrowPressed = false;
bool downArrowPressed = false;


void arrowKeysDown(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) { rightArrowPressed = true; }
    if (key == GLUT_KEY_LEFT)  { leftArrowPressed = true; }
    if (key == GLUT_KEY_UP)    { upArrowPressed = true; }
    if (key == GLUT_KEY_DOWN)  { downArrowPressed = true; }
}

void arrowKeysUp(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) { rightArrowPressed = false; }
    if (key == GLUT_KEY_LEFT)  { leftArrowPressed = false; }
    if (key == GLUT_KEY_UP)    { upArrowPressed = false; }
    if (key == GLUT_KEY_DOWN)  { downArrowPressed = false; }
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Sistema de projéteis (array de tamanho fixo)
// x, y também são posições NO MUNDO, assim como o personagem

#define MAX_PROJECTILES 20

struct Projectile {
    float x, y;
    float dx, dy;
    bool active;
};

Projectile projectiles[MAX_PROJECTILES];
float projectileSpeed = 0.3f;
float projectileRange = 12.0f; // distância máxima que o projétil percorre antes de sumir

void shootProjectile() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) {
            projectiles[i].x = squareX;
            projectiles[i].y = squareY;
            projectiles[i].dx = facingX * projectileSpeed;
            projectiles[i].dy = facingY * projectileSpeed;
            projectiles[i].active = true;
            break; // usa só o primeiro slot livre encontrado
        }
    }
}

void mouse_callback(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        shootProjectile();
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Função que controla a animação
void anim(int valor) {

    // Calcula a direção combinada de movimento (permite diagonais corretas)
    float moveX = 0.0f;
    float moveY = 0.0f;

    if (rightArrowPressed) { moveX += 1.0f; }
    if (leftArrowPressed)  { moveX -= 1.0f; }
    if (upArrowPressed)    { moveY += 1.0f; }
    if (downArrowPressed)  { moveY -= 1.0f; }

    if (moveX != 0.0f || moveY != 0.0f) {
        // normaliza o vetor (comprimento 1), senão andar na diagonal fica mais
        // rápido que andar reto (soma bruta de dx e dy sem normalizar)
        float len = sqrt(moveX * moveX + moveY * moveY);
        moveX /= len;
        moveY /= len;

        // atualiza a posição do personagem no MUNDO
        squareX += moveX * squareSpeed;
        squareY += moveY * squareSpeed;

        // atualiza a direção que o personagem está "olhando" com o vetor combinado
        // (em vez de cada seta sobrescrever facingX/facingY isoladamente)
        facingX = moveX;
        facingY = moveY;
    }
    // se nenhuma seta estiver pressionada, facingX/facingY mantêm o último valor
    // válido -> o personagem continua "olhando" pra última direção andada.

    // Não há limite de borda aqui de propósito: o mundo é "infinito",
    // quem se move na tela é a câmera/background, não o personagem.

    // Atualiza os projéteis ativos (também em coordenadas de mundo)
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            projectiles[i].x += projectiles[i].dx;
            projectiles[i].y += projectiles[i].dy;

            // desativa o projétil quando ele se afasta demais do personagem
            float dist = sqrt(
                (projectiles[i].x - squareX) * (projectiles[i].x - squareX) +
                (projectiles[i].y - squareY) * (projectiles[i].y - squareY)
            );
            if (dist > projectileRange) {
                projectiles[i].active = false;
            }
        }
    }

    //=========================================
    // Comandos padrão da função anim
    FrameNumber++;
    glutPostRedisplay();
    glutTimerFunc(msecs, anim, valor);
}


void display() {

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    //////////////////////////////////////////////////////////////////////////////////////////
    // Desenha o background deslocado na direção OPOSTA ao movimento do personagem
    // fmod aqui mantém o deslocamento sempre dentro de uma faixa, criando o loop infinito em X
    float bgOffsetX = fmod(-squareX, bgPairWidth);
    float bgOffsetY = -squareY; // Y não tem tiling (ver observação no fim do arquivo)

    // Desenha 3 cópias do "par" de tiles em X, garantindo que a tela nunca fique sem cenário
    for (int i = -1; i <= 1; i++) {
        glPushMatrix();
            glTranslatef(bgOffsetX + i * bgPairWidth, bgOffsetY, 1);
            drawBackground1();
        glPopMatrix();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Desenha o personagem SEMPRE fixo no centro da tela (origem)
    glColor3f(0, 0, 0);
    glPushMatrix();
        glTranslatef(0, 0, 1);
        drawSquare();
    glPopMatrix();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Desenha os projéteis relativos à posição do personagem (mundo -> tela)
    glColor3f(1, 0, 0);
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            glPushMatrix();
                glTranslatef(projectiles[i].x - squareX, projectiles[i].y - squareY, 1);
                glScalef(0.2f, 0.2f, 1);
                drawSquare();
            glPopMatrix();
        }
    }

    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Mapa com camera seguindo o personagem");

    init();

    glutDisplayFunc(display);

    // Controla a animação
    glutTimerFunc(msecs, anim, 0);

    // Controla o input por setinha do teclado
    glutSpecialFunc(arrowKeysDown);
    glutSpecialUpFunc(arrowKeysUp);

    // Controla o clique do mouse (botão direito = atirar)
    glutMouseFunc(mouse_callback);

    glutMainLoop();

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// OBSERVAÇÃO IMPORTANTE:
// O tiling do background só existe no eixo X (normal + espelhado lado a lado).
// Se o personagem andar muito pra cima ou pra baixo, ele vai eventualmente "sair"
// da faixa coberta pelo céu/grama e aparecer o fundo branco da janela.
// Para resolver isso, seria necessário criar um tiling vertical seguindo o mesmo
// princípio (replicando drawBackgroundContent também acima/abaixo, com ou sem
// espelhamento em Y) — a mesma lógica usada aqui pra X, só que aplicada ao eixo Y.
//////////////////////////////////////////////////////////////////////////////////////////////
