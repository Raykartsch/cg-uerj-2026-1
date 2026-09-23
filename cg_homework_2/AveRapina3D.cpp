#include "AveRapina3D.hpp"
#include "Cenario3D.hpp"
#include "Coelho3D.hpp"
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ---- Variaveis de Estado da Ave de Rapina 3D ----
bool aveActive = false;       // True enquanto a ave estiver executando o ataque
float aveX = 14.0f;           // Posicao X (inicia fora da tela a direita)
float aveY = 7.5f;            // Altura Y (inicia no alto do ceu)
float aveZ = 0.0f;            // Profundidade Z (faixa do ataque)
float avePhase = 0.0f;        // Fase do bater de asas (em radianos)
bool aveJaTirouVida = false;  // Trava para evitar perder multiplas vidas no mesmo ataque

float aveAlvoX = 0.0f;        // Ponto X do vertice mais baixo do mergulho
float aveAlvoZ = 0.0f;        // Ponto Z registrado no momento do ataque
float aveVelocidadeX = 0.16f; // Velocidade horizontal do deslocamento

const int AVE_FPS = 1000 / 24;
int framesAteProximaAve = 300; // Primeira ave surge apos cerca de 7 segundos

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funcao auxiliar didatica: desenha um elipsoide 3D com glutSolidSphere + glScalef
static void drawElipsoideAve(float rx, float ry, float rz, int fatias = 16, int pilhas = 16) {
    glPushMatrix();
        glScalef(rx, ry, rz);
        glutSolidSphere(1.0, fatias, pilhas);
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha um olho feroz de ave de rapina (iris dourada + pupila preta + reflexo branco)
static void desenharOlhoAve(bool olhoEsquerdo) {
    float sinalZ = olhoEsquerdo ? 1.0f : -1.0f;

    glPushMatrix();
        // Posicionado na lateral superior da cabeca
        glTranslatef(0.12f, 0.07f, sinalZ * 0.12f);

        // Iris amarela/dourada tipica de predador aereo
        glColor3f(1.0f, 0.77f, 0.10f);
        drawElipsoideAve(0.040f, 0.040f, 0.030f, 10, 10);

        // Pupila preta central
        glColor3f(0.0f, 0.0f, 0.0f);
        glPushMatrix();
            glTranslatef(0.015f, 0.0f, sinalZ * 0.015f);
            glutSolidSphere(0.022, 10, 10);
        glPopMatrix();

        // Ponto de brilho branco
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
            glTranslatef(0.025f, 0.012f, sinalZ * 0.018f);
            glutSolidSphere(0.010, 8, 8);
        glPopMatrix();
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha a asa tridimensional da ave com envergadura ao longo do eixo Z e bater de asas articulado
static void desenharAsaAve(float wingAngle, bool asaEsquerda) {
    float sinalZ = asaEsquerda ? 1.0f : -1.0f;

    glPushMatrix();
        // 1. Ponto de articulacao da asa na lateral superior do dorso
        glTranslatef(0.05f, 0.08f, sinalZ * 0.14f);

        // 2. Bater de asas: rotacao em torno do eixo X (longitudinal)
        // A asa esquerda e a direita batem simetricamente em sentidos opostos
        glRotatef(sinalZ * wingAngle, 1.0f, 0.0f, 0.0f);

        // Leve inclinacao da asa para tras (aerodinamica de voo veloz)
        glRotatef(sinalZ * 12.0f, 0.0f, 1.0f, 0.0f);

        // --- Braco / Base da Asa (Marrom escuro) ---
        glColor3f(0.411f, 0.334f, 0.20f);
        glPushMatrix();
            glTranslatef(-0.04f, 0.0f, sinalZ * 0.28f);
            drawElipsoideAve(0.24f, 0.045f, 0.30f, 14, 14);
        glPopMatrix();

        // --- Penas Primarias da Ponta da Asa (Mais escuras e afiladas) ---
        glColor3f(0.27f, 0.23f, 0.13f);
        glPushMatrix();
            glTranslatef(-0.10f, -0.01f, sinalZ * 0.60f);
            glRotatef(sinalZ * -15.0f, 0.0f, 1.0f, 0.0f); // Ponta das penas abre para tras
            drawElipsoideAve(0.18f, 0.035f, 0.34f, 14, 14);
        glPopMatrix();

        // Camada secundária de penas intermediárias
        glColor3f(0.35f, 0.28f, 0.16f);
        glPushMatrix();
            glTranslatef(0.02f, 0.01f, sinalZ * 0.38f);
            drawElipsoideAve(0.16f, 0.040f, 0.20f, 12, 12);
        glPopMatrix();
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha a cauda em leque de penas na traseira da ave
static void desenharCaudaAve() {
    glPushMatrix();
        // Insercao na traseira do tronco (-X)
        glTranslatef(-0.46f, 0.02f, 0.0f);

        // Leve inclinacao para baixo
        glRotatef(-10.0f, 0.0f, 0.0f, 1.0f);

        // Penas centrais e laterais da cauda
        glColor3f(0.27f, 0.23f, 0.13f);
        // Pena central
        glPushMatrix();
            glTranslatef(-0.18f, 0.0f, 0.0f);
            drawElipsoideAve(0.20f, 0.03f, 0.11f, 10, 10);
        glPopMatrix();

        // Pena lateral esquerda (+Z)
        glPushMatrix();
            glTranslatef(-0.16f, 0.0f, 0.08f);
            glRotatef(18.0f, 0.0f, 1.0f, 0.0f);
            drawElipsoideAve(0.18f, 0.028f, 0.08f, 10, 10);
        glPopMatrix();

        // Pena lateral direita (-Z)
        glPushMatrix();
            glTranslatef(-0.16f, 0.0f, -0.08f);
            glRotatef(-18.0f, 0.0f, 1.0f, 0.0f);
            drawElipsoideAve(0.18f, 0.028f, 0.08f, 10, 10);
        glPopMatrix();
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha a cabeca completa da ave de rapina (cranio, moicano de penas, olhos e bico curvo afiado)
static void desenharCabecaAve() {
    glPushMatrix();
        // Posicionada a frente (+X) e ligeiramente acima do corpo
        glTranslatef(0.48f, 0.10f, 0.0f);

        // 1. Cranio da ave (elipsoide marrom escuro)
        glColor3f(0.411f, 0.334f, 0.20f);
        drawElipsoideAve(0.20f, 0.17f, 0.16f, 16, 16);

        // 2. Crista / Moicano de penas agressivas no topo da cabeca (Trabalho 1)
        glColor3f(0.27f, 0.23f, 0.13f);
        // Pena 1 do moicano
        glPushMatrix();
            glTranslatef(-0.06f, 0.16f, 0.0f);
            glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
            drawElipsoideAve(0.12f, 0.04f, 0.04f, 10, 10);
        glPopMatrix();
        // Pena 2 do moicano
        glPushMatrix();
            glTranslatef(-0.12f, 0.14f, 0.0f);
            glRotatef(55.0f, 0.0f, 0.0f, 1.0f);
            drawElipsoideAve(0.14f, 0.038f, 0.038f, 10, 10);
        glPopMatrix();

        // 3. Olhos ferozes
        desenharOlhoAve(true);   // Olho esquerdo (+Z)
        desenharOlhoAve(false);  // Olho direito (-Z)

        // 4. Bico Curvo de Rapina (Amarelo / Dourado brilhante)
        glColor3f(1.0f, 0.77f, 0.10f);
        glPushMatrix();
            glTranslatef(0.16f, -0.02f, 0.0f);
            glRotatef(-25.0f, 0.0f, 0.0f, 1.0f); // Bico inclinado para baixo (gancho)
            glPushMatrix();
                glScalef(0.18f, 0.09f, 0.09f);
                glutSolidCone(1.0, 1.2, 14, 4);
            glPopMatrix();
        glPopMatrix();

        // Base da narina do bico (cera)
        glColor3f(0.85f, 0.60f, 0.08f);
        glPushMatrix();
            glTranslatef(0.14f, 0.01f, 0.0f);
            drawElipsoideAve(0.06f, 0.07f, 0.08f, 10, 10);
        glPopMatrix();

    glPopMatrix(); // Fim da Cabeca
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha as garras da ave recolhidas sob o ventre
static void desenharGarrasAve() {
    glColor3f(1.0f, 0.77f, 0.10f); // Amarelo/dourado
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
            glTranslatef(0.05f, -0.16f, i * 0.09f);
            glRotatef(30.0f, 0.0f, 0.0f, 1.0f); // Inclinadas para tras em voo
            drawElipsoideAve(0.09f, 0.04f, 0.04f, 10, 10);

            // Dedo curvo
            glPushMatrix();
                glTranslatef(0.06f, -0.02f, 0.0f);
                drawElipsoideAve(0.05f, 0.03f, 0.03f, 8, 8);
            glPopMatrix();
        glPopMatrix();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
================================================================================
 FUNCAO drawBird(): MODELAGEM 3D COMPLETA DA AVE DE RAPINA
================================================================================
 Reproduz com fidelidade todos os elementos e cores da Etapa 1 2D:
  - Paleta de cores oficial:
      * Corpo / Cabeca: Marrom escuro [0.411, 0.334, 0.20]
      * Ponta das asas / Crista / Cauda: Marrom mais escuro [0.27, 0.23, 0.13]
      * Peito / Ventre: Creme claro [0.96, 0.93, 0.89] (mesma cor da barriga original)
      * Bico e Garras: Amarelo ouro [1.00, 0.77, 0.10]
      * Olhos: Iris dourada + pupila preta + reflexo branco
  - Hierarquia de Transformacoes:
      Corpo (Tronco aerodinamico - no raiz)
       ├── Peito e Ventre Creme
       ├── Garras amarelas recolhidas
       ├── Cauda em leque de penas
       ├── Asas tridimensionais (esquerda e direita, com batimento articulado)
       └── Cabeca Predatoria
            ├── Crista de penas (moicano)
            ├── Olhos amendoados vivos
            └── Bico curvo adunco
================================================================================
*/
void drawBird() {
    // Avanca a fase do bater de asas (apenas enquanto a ave e desenhada)
    avePhase += 0.35f;
    if (avePhase > 2.0f * PI_F) {
        avePhase -= 2.0f * PI_F;
    }

    // Ondulacao senoidal para as asas
    float wingAngle = std::sin(avePhase) * 38.0f;

    // Brilho especular suave na plumagem
    GLfloat especular[] = { 0.25f, 0.22f, 0.18f, 1.0f };
    GLfloat semBrilho[] = { 0.0f,  0.0f,  0.0f,  1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 25.0f);

    // =========================================================================
    // 1. CORPO (TRONCO PRINCIPAL - NO RAIZ DA HIERARQUIA)
    // Elipsoide fusiforme aerodinamico.
    // =========================================================================
    glColor3f(0.411f, 0.334f, 0.20f);
    drawElipsoideAve(0.52f, 0.22f, 0.20f, 20, 20);

    // Peito e Ventre (mancha clara original da barriga da ave 2D)
    glColor3f(0.96f, 0.93f, 0.89f);
    glPushMatrix();
        glTranslatef(0.08f, -0.07f, 0.0f);
        drawElipsoideAve(0.38f, 0.14f, 0.16f, 16, 16);
    glPopMatrix();

    // =========================================================================
    // 2. GARRAS RECOLHIDAS
    // =========================================================================
    desenharGarrasAve();

    // =========================================================================
    // 3. CAUDA EM LEQUE DE PENAS
    // =========================================================================
    desenharCaudaAve();

    // =========================================================================
    // 4. CABECA, MOICANO E BICO ADUNCO
    // =========================================================================
    desenharCabecaAve();

    // =========================================================================
    // 5. ASAS TRIDIMENSIONAIS ARTICULADAS (ESQUERDA E DIREITA)
    // =========================================================================
    desenharAsaAve(wingAngle, true);   // Asa esquerda (+Z)
    desenharAsaAve(wingAngle, false);  // Asa direita (-Z)

    // Restaura material padrao sem brilho
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, semBrilho);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sombra no solo: acompanha (aveX, aveZ) e fica mais nitida/escura a medida que a ave atinge o rasante
void drawSombraAve() {
    if (!aveActive) return;

    // Quanto mais baixa a ave, menor e mais escura a sombra (aviso visual de perigo!)
    float t = (aveY - 0.55f) / (7.5f - 0.55f);
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    float raio = 0.35f + 0.35f * t;   // Menor no rasante (0.35) e mais difusa no alto (0.70)
    float alpha = 0.40f - 0.28f * t;  // Muito mais escura e nítida no rasante (0.40)

    drawDiscoNoChao(aveX, aveZ, raio, alpha);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inicia o ataque aereo: a ave comeca fora da tela em X = 14 e desce mirando em (targetX, targetZ)
void spawnAve(float targetX, float targetZ) {
    aveActive = true;
    aveX = 14.0f;
    aveY = 7.5f;

    aveAlvoX = targetX;
    aveAlvoZ = targetZ;
    aveZ = targetZ; // A ave percorre a linha Z do ataque

    aveJaTirouVida = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Controla o temporizador para disparar os ataques aereos periodicos
void controlarSurgimentoDaAve(float coelhoX, float coelhoZ) {
    if (aveActive) return;

    framesAteProximaAve--;
    if (framesAteProximaAve <= 0) {
        spawnAve(coelhoX, coelhoZ);
        // Sorteia o tempo para o proximo ataque (entre 12 e 22 segundos)
        framesAteProximaAve = (12 + (rand() % 11)) * AVE_FPS;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Movimenta a ave ao longo da parabola tridimensional de mergulho:
// Y(X) = a * (X - aveAlvoX)^2 + K
void moverAve() {
    if (!aveActive) return;

    // Deslocamento para a esquerda (direcao -X)
    aveX -= aveVelocidadeX;

    // Calculo da trajetoria parabolica: o vertice mais baixo ocorre exatamente em (aveAlvoX, 0.55f)
    float distBase = 14.0f - aveAlvoX;
    float a = (7.5f - 0.55f) / (distBase * distBase + 0.0001f);

    float dx = aveX - aveAlvoX;
    aveY = a * (dx * dx) + 0.55f;

    // Saiu do campo pelo lado esquerdo: encerra o ataque e agenda o proximo
    if (aveX < -14.0f) {
        aveActive = false;
        framesAteProximaAve = (12 + (rand() % 11)) * AVE_FPS;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Verifica colisao 3D por distancia esferica euclidiana entre a ave e o coelho
void verificarColisaoComAve() {
    if (!aveActive || aveJaTirouVida || coelhoEscondido) return;

    float dx = aveX - coelhoX;
    float dy = aveY - coelhoY;
    float dz = aveZ - coelhoZ;
    float distancia = std::sqrt(dx * dx + dy * dy + dz * dz);

    float raioColisao = 0.95f;

    if (distancia < raioColisao) {
        if (rabbitLives > 0) {
            rabbitLives--;
        }
        aveJaTirouVida = true; // Trava para tirar no maximo uma vida por mergulho
    }
}
