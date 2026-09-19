#include "Cenario3D.hpp"
#include <GL/glut.h>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quadrilatero (retangulo) do campo, contido no plano XZ (Y = 0).
// X vai de -8 a +8 (igual ao limite do jogo 2D); Z vai de -6 a +6.
const float CAMPO_X_MIN = -8.0f;
const float CAMPO_X_MAX =  8.0f;
const float CAMPO_Z_MIN = -6.0f;
const float CAMPO_Z_MAX =  6.0f;

const float TAMANHO_LADRILHO = 1.0f;
const float GRAMADO_X_EXT = 20.0f;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Rolagem do cenario (mesmos valores do jogo 2D)
float bgSpeed = 0.1f;
float bgWidth = 40.0f;
float bgPos = 0.0f;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cor do ceu no "dia" (mesmos valores de diaR/diaG/diaB do atualizarCorDoCeu original)
float skyR = 0.68f, skyG = 0.81f, skyB = 0.98f;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Igual ao 2D: o fundo anda para a esquerda e da a volta (fmod) para nao crescer para sempre
void rolarCenario() {
    bgPos -= bgSpeed;
    bgPos = std::fmod(bgPos, bgWidth);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Desenha o campo: um gramado no plano XZ dividido em ladrilhos que alternam
dois tons do verde da grama original. Os ladrilhos servem de referencia
visual e, principalmente, e neles que se ve a ROLAGEM: cada ladrilho k ocupa
[k*T + bgPos, (k+1)*T + bgPos], entao o padrao inteiro desliza para -X junto
com os vegetais (sem isso, os vegetais pareceriam deslizar sobre um chao
parado). O gramado vai bem alem da cerca (de -GRAMADO_X_EXT a +GRAMADO_X_EXT)
para que os vegetais nao aparecam "flutuando no vazio" ao entrar/sair.
Depois desenha a "cerca" (borda marrom) que delimita a area onde o coelho
pode andar, na mesma cor dos postes da cerca do jogo 2D.*/
void drawCampo() {
    int kMin = (int)std::floor((-GRAMADO_X_EXT - bgPos) / TAMANHO_LADRILHO);
    int kMax = (int)std::ceil(( GRAMADO_X_EXT - bgPos) / TAMANHO_LADRILHO);
    int qtdZ = (int)((CAMPO_Z_MAX - CAMPO_Z_MIN) / TAMANHO_LADRILHO);

    // Todos os ladrilhos estao no plano XZ, portanto a normal aponta para +Y (necessaria para a iluminacao)
    glNormal3f(0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);
    for (int k = kMin; k < kMax; k++) {
        float x0 = k * TAMANHO_LADRILHO + bgPos;
        float x1 = x0 + TAMANHO_LADRILHO;

        // Recorta as pontas para o gramado terminar exatamente em +-GRAMADO_X_EXT
        if (x0 < -GRAMADO_X_EXT) x0 = -GRAMADO_X_EXT;
        if (x1 >  GRAMADO_X_EXT) x1 =  GRAMADO_X_EXT;
        if (x1 <= x0) continue;

        for (int j = 0; j < qtdZ; j++) {
            int paridade = ((k + j) % 2 + 2) % 2; // (k pode ser negativo)
            if (paridade == 0) {
                glColor3f(0.549f, 0.776f, 0.247f); // verde da grama original
            } else {
                glColor3f(0.490f, 0.700f, 0.210f); // tom um pouco mais escuro
            }

            float z0 = CAMPO_Z_MIN + j * TAMANHO_LADRILHO;
            float z1 = z0 + TAMANHO_LADRILHO;

            // Ordem anti-horaria vista de cima (+Y)
            glVertex3f(x0, 0.0f, z0);
            glVertex3f(x0, 0.0f, z1);
            glVertex3f(x1, 0.0f, z1);
            glVertex3f(x1, 0.0f, z0);
        }
    }
    glEnd();

    // Borda (cerca) do quadrilatero. Sem iluminacao, para manter a cor pura.
    glDisable(GL_LIGHTING);
    glColor3f(0.662f, 0.443f, 0.247f);
    glLineWidth(4.0f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(CAMPO_X_MIN, 0.02f, CAMPO_Z_MIN);
        glVertex3f(CAMPO_X_MIN, 0.02f, CAMPO_Z_MAX);
        glVertex3f(CAMPO_X_MAX, 0.02f, CAMPO_Z_MAX);
        glVertex3f(CAMPO_X_MAX, 0.02f, CAMPO_Z_MIN);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Disco escuro e semitransparente no plano XZ, ligeiramente acima do chao
(Y = 0.02) para nao "brigar" com ele no z-buffer. Serve de sombra para o
coelho e para os vegetais: em 3D, sem sombra, e muito dificil perceber a
posicao em Z e a altura de um objeto que esta no ar.*/
void drawDiscoNoChao(float cx, float cz, float raio, float alpha) {
    glDisable(GL_LIGHTING);
    glColor4f(0.0f, 0.0f, 0.0f, alpha);

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cx, 0.02f, cz);
        for (int i = 0; i <= 32; i++) {
            float angulo = (2.0f * PI_F / 32.0f) * i;
            glVertex3f(cx + raio * std::cos(angulo), 0.02f, cz + raio * std::sin(angulo));
        }
    glEnd();

    glEnable(GL_LIGHTING);
}
