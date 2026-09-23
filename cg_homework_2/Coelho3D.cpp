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

// Animacao de caminhada/corrida do coelho (mesmas variaveis e logica do Trabalho 1)
float walkPhase = 0.0f;        // Fase atual do ciclo de passos (em radianos, 0 a 2*PI)
float walkPhaseSpeed = 0.20f;  // Velocidade com que o ciclo avanca a cada frame
float legLiftAmount = 0.12f;   // Altura maxima que cada patinha sobe ao caminhar
float earSwingAmount = 8.0f;   // Angulo maximo (em graus) de balanco das orelhas

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
        // Suaviza a fase para que as patinhas e orelhas voltem ao repouso no chao
        if (std::abs(walkPhase) > 0.05f) {
            walkPhase *= 0.8f;
        } else {
            walkPhase = 0.0f;
        }
        return;
    }

    // Avanca a fase da caminhada: quando corre no turbo, o ciclo de passos e mais rapido
    float multiplicadorVelocidade = (characterSpeed > VELOCIDADE_NORMAL) ? 1.8f : 1.0f;
    walkPhase += walkPhaseSpeed * multiplicadorVelocidade;
    if (walkPhase > 2.0f * PI_F) {
        walkPhase -= 2.0f * PI_F; // Mantem a fase dentro de [0, 2*PI], sem crescer indefinidamente
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
/*
================================================================================
 MODELAGEM TRIDIMENSIONAL DO COELHO (HIERARQUICA COM SOLIDOS GLUT)
================================================================================
 Esta modelagem traduz fielmente para o espaco 3D todos os elementos e a paleta
 de cores desenvolvidos na Etapa 1 (2D):
  - Cores originais:
      * Corpo/Cabeca/Orelhas/Rabo: Creme claro [0.96, 0.93, 0.89]
      * Bochechas/Focinho/Dentes:  Branco puro  [1.00, 1.00, 1.00]
      * Nariz e Interior Orelhas:  Rosa pastel  [0.90, 0.42, 0.54]
      * Patas e pes:               Bege macio   [0.90, 0.85, 0.79]
      * Olhos:                     Preto com ponto reflexivo de luz branca
  - Hierarquia de Transformacoes (Scene Graph):
      Corpo (Tronco principal - no raiz)
       ├── Rabo (pom-pom na traseira)
       ├── Coxas e Patas Traseiras (esquerda e direita, apoiadas no chao)
       ├── Patas Dianteiras (esquerda e direita, com animacao de passos)
       └── Cabeca (inclinada a frente e acima do tronco)
            ├── Bochechas fofas (esquerda e direita)
            ├── Nariz rosa central
            ├── Dentinhos de coelho
            ├── Olhos simetricos (com brilho especular vivo)
            └── Orelhas articuladas (com cavidade interna rosada e balanco)
================================================================================
*/

// Funcao auxiliar didatica: desenha um elipsoide 3D suave.
// Como o GLUT nao possui glutSolidEllipsoid nativo, escalamos uma esfera unitaria
// (de raio 1.0) pelos raios desejados (rx, ry, rz) usando glScalef.
// O glEnable(GL_NORMALIZE) no init() garante que a iluminacao continue perfeita!
static void drawElipsoideCoelho(float rx, float ry, float rz, int fatias = 20, int pilhas = 20) {
    glPushMatrix();
        glScalef(rx, ry, rz);
        glutSolidSphere(1.0, fatias, pilhas);
    glPopMatrix();
}

// Desenha um olho completo (globo ocular preto brilhante + pontinho reflexivo de luz branca)
static void desenharOlho(bool olhoEsquerdo) {
    float sinalZ = olhoEsquerdo ? 1.0f : -1.0f;

    glPushMatrix();
        // Posiciona o olho na lateral da cabeca (visao panoramica natural de presas)
        glTranslatef(0.12f, 0.08f, sinalZ * 0.16f);

        // Globo ocular preto
        glColor3f(0.0f, 0.0f, 0.0f);
        glutSolidSphere(0.048, 14, 14);

        // Ponto de brilho/reflexo branco na pupila (da vida e expressao ao personagem)
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
            glTranslatef(0.025f, 0.02f, sinalZ * 0.025f);
            glutSolidSphere(0.016, 8, 8);
        glPopMatrix();
    glPopMatrix();
}

// Desenha uma orelha completa: parte externa (creme) + parte interna rosada (cavidade)
static void desenharOrelha(float swingAngle, bool orelhaEsquerda) {
    float sinalZ = orelhaEsquerda ? 1.0f : -1.0f;

    glPushMatrix();
        // 1. Ponto de articulacao da orelha no topo da cabeca
        glTranslatef(0.02f, 0.18f, sinalZ * 0.08f);

        // 2. Inclinacao base: abre suavemente para fora no eixo Z e recua levemente em X
        glRotatef(sinalZ * 12.0f, 1.0f, 0.0f, 0.0f); // Abertura lateral
        glRotatef(-10.0f, 0.0f, 0.0f, 1.0f);         // Inclinacao natural para tras

        // 3. Balanco dinamico: oscila para frente e para tras ao caminhar (igual ao Trabalho 1)
        glRotatef(swingAngle, 0.0f, 0.0f, 1.0f);

        // 4. Posiciona o centro da massa da orelha acima do ponto de articulacao
        glTranslatef(0.0f, 0.16f, 0.0f);

        // --- Camada Externa (creme claro, cor do pelo) ---
        glColor3f(0.96f, 0.93f, 0.89f);
        drawElipsoideCoelho(0.065f, 0.32f, 0.095f, 16, 16);

        // --- Camada Interna (rosa suave, cavidade da orelha voltada para a frente) ---
        glColor3f(0.90f, 0.42f, 0.54f);
        glPushMatrix();
            glTranslatef(0.022f, -0.015f, 0.0f); // Levemente projetada para a face frontal (+X)
            drawElipsoideCoelho(0.045f, 0.26f, 0.065f, 14, 14);
        glPopMatrix();
    glPopMatrix();
}

// Desenha uma patinha no solo, modelada como um elipsoide achatado
static void desenharPatinha(float posX, float posY, float posZ, float raioX, float raioY, float raioZ) {
    glPushMatrix();
        glTranslatef(posX, posY, posZ);
        // Cor das patas da Etapa 1: bege macio ligeiramente mais escuro que o corpo
        glColor3f(0.90f, 0.85f, 0.79f);
        drawElipsoideCoelho(raioX, raioY, raioZ, 14, 14);
    glPopMatrix();
}

// Desenha a cabeca completa do coelho e seus elementos filhos
static void desenharCabeca(float earLeftSwing, float earRightSwing) {
    glPushMatrix();
        // Posicionamento da cabeca: a frente (+X) e acima (+Y) do corpo
        glTranslatef(0.32f, 0.18f, 0.0f);

        // 1. Cranio / Cabeca (elipsoide macio creme claro)
        glColor3f(0.96f, 0.93f, 0.89f);
        drawElipsoideCoelho(0.24f, 0.22f, 0.21f, 22, 22);

        // 2. Focinho e Bochechas brancas (duas esferas simetricas dando volume fofo a face)
        glColor3f(1.0f, 1.0f, 1.0f);
        // Bochecha esquerda (+Z)
        glPushMatrix();
            glTranslatef(0.18f, -0.06f, 0.075f);
            drawElipsoideCoelho(0.09f, 0.075f, 0.08f, 14, 14);
        glPopMatrix();
        // Bochecha direita (-Z)
        glPushMatrix();
            glTranslatef(0.18f, -0.06f, -0.075f);
            drawElipsoideCoelho(0.09f, 0.075f, 0.08f, 14, 14);
        glPopMatrix();

        // 3. Nariz rosa pastel na pontinha frontal do focinho
        glColor3f(0.90f, 0.42f, 0.54f);
        glPushMatrix();
            glTranslatef(0.25f, -0.02f, 0.0f);
            glutSolidSphere(0.042, 12, 12);
        glPopMatrix();

        // 4. Dentinhos brancos de coelho logo abaixo do nariz
        glColor3f(1.0f, 1.0f, 1.0f);
        // Dente esquerdo
        glPushMatrix();
            glTranslatef(0.21f, -0.11f, 0.025f);
            glScalef(0.025f, 0.045f, 0.022f);
            glutSolidCube(1.0);
        glPopMatrix();
        // Dente direito
        glPushMatrix();
            glTranslatef(0.21f, -0.11f, -0.025f);
            glScalef(0.025f, 0.045f, 0.022f);
            glutSolidCube(1.0);
        glPopMatrix();

        // 5. Olhos esquerdo e direito (com reflexos de luz)
        desenharOlho(true);   // Olho esquerdo (+Z)
        desenharOlho(false);  // Olho direito (-Z)

        // 6. Orelhas articuladas no topo da cabeca com balanco dinâmico
        desenharOrelha(earLeftSwing, true);   // Orelha esquerda (+Z)
        desenharOrelha(earRightSwing, false); // Orelha direita (-Z)

    glPopMatrix(); // Fecha o sistema de coordenadas da cabeca
}

// Funcao principal que desenha o coelho 3D completo
void drawRabbit() {
    /*
      Calcula o ciclo de marcha a partir de walkPhase (identico ao Trabalho 1 2D):
      - Quando a orelha direita vai para frente (seno positivo), a pata esquerda sobe.
      - Quando a orelha esquerda vai para frente (seno negativo), a pata direita sobe.
      - No pulo, as patinhas encolhem graciosamente contra o corpo.
    */
    float phaseSin = std::sin(walkPhase);

    float earRightSwing = (phaseSin > 0.0f) ?  phaseSin * earSwingAmount : 0.0f;
    float earLeftSwing  = (phaseSin < 0.0f) ? -phaseSin * earSwingAmount : 0.0f;

    float legLeftLift  = (phaseSin > 0.0f) ?  phaseSin * legLiftAmount : 0.0f;
    float legRightLift = (phaseSin < 0.0f) ? -phaseSin * legLiftAmount : 0.0f;

    // Se estiver no ar pulando, recolhe as patinhas e joga as orelhas levemente para tras
    if (isJumping) {
        legLeftLift = 0.10f;
        legRightLift = 0.10f;
        earLeftSwing += 10.0f;
        earRightSwing += 10.0f;
    }

    // Brilho especular sutil na pelagem para evidenciar o volume 3D sob a iluminacao
    GLfloat especularPelo[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat semBrilho[]     = { 0.0f,  0.0f,  0.0f,  1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especularPelo);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 25.0f);

    // =========================================================================
    // 1. CORPO (TRONCO PRINCIPAL - NO RAIZ DA HIERARQUIA)
    // Origem (0, 0, 0) no centro de gravidade do coelho.
    // =========================================================================
    glColor3f(0.96f, 0.93f, 0.89f);
    drawElipsoideCoelho(0.42f, 0.30f, 0.28f, 24, 24);

    // =========================================================================
    // 2. RABO (POM-POM NA TRASEIRA DO COELHO)
    // Fica em -X (traseira) e vibra sutilmente com o movimento da marcha.
    // =========================================================================
    glPushMatrix();
        glTranslatef(-0.43f, 0.06f + (legRightLift * 0.12f), 0.0f);
        glColor3f(0.96f, 0.93f, 0.89f);
        glutSolidSphere(0.11, 16, 16);
    glPopMatrix();

    // =========================================================================
    // 3. COXAS TRASEIRAS (ANATOMIA DO QUADRIL)
    // Elipsoides laterais que dao volume anatomico ao quadril do coelho.
    // =========================================================================
    glColor3f(0.96f, 0.93f, 0.89f);
    // Coxa traseira esquerda (+Z)
    glPushMatrix();
        glTranslatef(-0.14f, -0.08f, 0.21f);
        drawElipsoideCoelho(0.15f, 0.17f, 0.09f, 16, 16);
    glPopMatrix();
    // Coxa traseira direita (-Z)
    glPushMatrix();
        glTranslatef(-0.14f, -0.08f, -0.21f);
        drawElipsoideCoelho(0.15f, 0.17f, 0.09f, 16, 16);
    glPopMatrix();

    // =========================================================================
    // 4. PATAS (4 PATAS: 2 TRASEIRAS E 2 DIANTEIRAS APOIADAS NO SOLO)
    // Quando coelhoY = 0.5 (no chao), o fundo das patas em Y = -0.42 - 0.08 = -0.50
    // toca perfeitamente a grama em Y = 0.0 do mundo!
    // =========================================================================
    // Patas traseiras:
    desenharPatinha(-0.10f, -0.42f + legLeftLift,   0.21f, 0.16f, 0.08f, 0.095f); // Traseira esquerda (+Z)
    desenharPatinha(-0.10f, -0.42f + legRightLift, -0.21f, 0.16f, 0.08f, 0.095f); // Traseira direita (-Z)

    // Patas dianteiras:
    desenharPatinha( 0.20f, -0.42f + legRightLift,  0.13f, 0.12f, 0.075f, 0.08f); // Dianteira esquerda (+Z)
    desenharPatinha( 0.20f, -0.42f + legLeftLift,  -0.13f, 0.12f, 0.075f, 0.08f); // Dianteira direita (-Z)

    // =========================================================================
    // 5. CABECA (E SEUS COMPONENTES FILHOS: OLHOS, BOCHECHAS, NARIZ, ORELHAS)
    // =========================================================================
    desenharCabeca(earLeftSwing, earRightSwing);

    // Restaura o material padrao sem brilho para os proximos objetos da cena
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
