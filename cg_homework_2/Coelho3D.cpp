#include "Coelho3D.hpp"
#include "Cenario3D.hpp"
#include <GL/glut.h>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const float RAIO_ESFERA = 0.5f;

// O coelho comeca no centro do campo, com a esfera apoiada no chao
// (centro a uma altura igual ao raio; 0.5 tambem era o valor inicial de jump_height no 2D)
float coelhoX = 0.0f;
float coelhoY = RAIO_ESFERA;
float coelhoZ = 0.0f;

float anguloCoelho = 0.0f; // olhando para +X, como o coelho 2D (direcaoCoelho = 1)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const float VELOCIDADE_NORMAL = 0.05f;
float characterSpeed = VELOCIDADE_NORMAL;

// Buff da cenoura (turbo): mesmos valores do 2D
const float VELOCIDADE_TURBO = 0.20f;
const int DURACAO_TURBO_EM_FRAMES = 150;
int framesDeTurboRestantes = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool rightArrowPressed = false;
bool leftArrowPressed = false;
bool upArrowPressed = false;
bool downArrowPressed = false;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pulo e buff do rabanete (mesmos valores do 2D)
const float PULO_NORMAL = 2.5f;
const float PULO_REFORCADO = 3.5f;
float jump_maximum_height = PULO_NORMAL;
float speed_jump = 0.1f;
bool isJumping = false;
bool goingUp = true;
int framesDePuloReforcadoRestantes = 0;
const int DURACAO_PULO_REFORCADO_EM_FRAMES = 300;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int rabbitLives = 3;
const int MAX_VIDAS = 3;

bool coelhoEscondido = false;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mantem "valor" dentro do intervalo [minimo, maximo]
static float limitar(float valor, float minimo, float maximo) {
    if (valor < minimo) return minimo;
    if (valor > maximo) return maximo;
    return valor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Move o coelho no plano XZ. Chamada uma vez por frame (a cada 24 ms).

No 2D so existia esquerda/direita. Aqui as quatro setas controlam o plano XZ:
   seta esquerda / direita -> -X / +X
   seta cima / baixo       -> -Z / +Z  (cima = "para o fundo" da tela)
(o pulo usa a barra de espaco; a toca, que usava a seta para baixo, ganhara outra tecla)

O movimento e NORMALIZADO: em diagonal (ex.: cima + direita) o vetor
(1, 1) tem comprimento 1.41, o que faria o coelho andar 41% mais rapido
que em linha reta. Dividindo pelo comprimento, a velocidade fica sempre
igual a characterSpeed, em qualquer direcao.*/
void moverCoelho() {
    float dirX = 0.0f;
    float dirZ = 0.0f;

    if (rightArrowPressed) dirX += 1.0f;
    if (leftArrowPressed)  dirX -= 1.0f;
    if (downArrowPressed)  dirZ += 1.0f;
    if (upArrowPressed)    dirZ -= 1.0f;

    // Nenhuma seta (ou setas opostas se anulando): coelho parado
    if (dirX == 0.0f && dirZ == 0.0f) {
        return;
    }

    float comprimento = std::sqrt(dirX * dirX + dirZ * dirZ);
    dirX /= comprimento;
    dirZ /= comprimento;

    coelhoX += dirX * characterSpeed;
    coelhoZ += dirZ * characterSpeed;

    // O coelho nao pode sair do quadrilatero. Recuamos o limite em RAIO_ESFERA
    // para que a BORDA da esfera (e nao o centro) pare na cerca.
    coelhoX = limitar(coelhoX, CAMPO_X_MIN + RAIO_ESFERA, CAMPO_X_MAX - RAIO_ESFERA);
    coelhoZ = limitar(coelhoZ, CAMPO_Z_MIN + RAIO_ESFERA, CAMPO_Z_MAX - RAIO_ESFERA);

    /* Vira o coelho para onde ele anda. Uma rotacao de theta graus em torno do
    eixo Y leva o eixo +X para (cos(theta), 0, -sin(theta)); queremos que ele
    aponte para (dirX, 0, dirZ), logo theta = atan2(-dirZ, dirX).*/
    anguloCoelho = std::atan2(-dirZ, dirX) * 180.0f / PI_F;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// So inicia um novo pulo se o coelho nao estiver ja pulando (evita "pulo duplo" no ar)
void iniciarPulo() {
    if (!isJumping) {
        isJumping = true;
        goingUp = true; // Comeca subindo
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Animacao do pulo: identica ao 2D, mas agora a altura e coelhoY (o centro da esfera)
void atualizarPulo() {
    if (!isJumping) {
        return;
    }

    if (goingUp) {
        // Fase de subida: incrementa a altura ate o limite maximo
        coelhoY += speed_jump;
        if (coelhoY >= jump_maximum_height) {
            coelhoY = jump_maximum_height; // trava no topo
            goingUp = false;               // comeca a fase de descida
        }
    } else {
        // Fase de descida: decrementa a altura ate voltar ao chao
        coelhoY -= speed_jump;
        if (coelhoY <= RAIO_ESFERA) {
            coelhoY = RAIO_ESFERA; // trava no chao certinho (esfera apoiada)
            isJumping = false;     // pulo terminou, pode pular de novo
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Atualiza a questao dos buffs dados ao coelho (pulo e velocidade extra). Copia fiel do 2D.
void atualizarBonusAtivos() {
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Desenha o personagem: uma esfera na cor do corpo do coelho 2D. Para que
seja possivel ver PARA ONDE ele olha (uma esfera lisa e igual em todos os
lados), colocamos um nariz rosa e dois olhos pretos no lado +X local,
usando as mesmas cores do coelho original. Quem posiciona e gira o
personagem e o display(): aqui a origem e o centro da esfera.*/
void drawRabbit() {

    // Brilho especular so na esfera, para o volume ficar evidente
    GLfloat especular[] = { 0.35f, 0.35f, 0.35f, 1.0f };
    GLfloat semBrilho[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 40.0f);

    // Corpo
    glColor3f(0.96f, 0.93f, 0.89f);
    glutSolidSphere(RAIO_ESFERA, 32, 32);

    // Nariz
    glColor3f(0.90f, 0.42f, 0.54f);
    glPushMatrix();
        glTranslatef(RAIO_ESFERA * 0.98f, -0.02f, 0.0f);
        glutSolidSphere(0.075, 16, 16);
    glPopMatrix();

    // Olhos
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
        glTranslatef(RAIO_ESFERA * 0.80f, RAIO_ESFERA * 0.30f, RAIO_ESFERA * 0.50f);
        glutSolidSphere(0.055, 12, 12);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(RAIO_ESFERA * 0.80f, RAIO_ESFERA * 0.30f, -RAIO_ESFERA * 0.50f);
        glutSolidSphere(0.055, 12, 12);
    glPopMatrix();

    // Restaura o material para os proximos objetos nao brilharem
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, semBrilho);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Sombra do coelho. Fica sempre no chao, logo abaixo do centro da esfera:
quando o coelho pula, a esfera sobe e a sombra continua no chao, encolhendo
um pouco com a altura (ajuda a perceber a altura do pulo).*/
void drawSombraCoelho() {
    float escala = 1.0f - 0.12f * (coelhoY - RAIO_ESFERA);
    if (escala < 0.4f) {
        escala = 0.4f;
    }
    drawDiscoNoChao(coelhoX, coelhoZ, RAIO_ESFERA * escala, 0.30f);
}
