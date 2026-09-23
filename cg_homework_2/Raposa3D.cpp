#include "Raposa3D.hpp"
#include "Cenario3D.hpp"
#include "Coelho3D.hpp"
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ---- Variaveis de Animacao do Trote da Raposa (patas e orelhas) ----
float foxWalkPhase = 0.0f;        // Fase atual do ciclo do trote
float foxWalkPhaseSpeed = 0.22f;  // Velocidade com que o ciclo avanca a cada frame
float foxLegLiftAmount = 0.15f;   // Altura maxima que as patas sobem no trote
float foxEarSwingAmount = 8.0f;   // Angulo maximo de balanco das orelhas (em graus)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ---- Variaveis da Cauda Articulada ----
// A cauda tem duas fases independentes: uma para a base e outra para a ponta.
// Isso gera um balanco suave, ondulante e organico, identico ao Trabalho 1.
float foxTailPhase = 0.0f;
float foxTailPhaseSpeed = 0.10f;
float foxTailSwingAmount = 8.0f;

float foxTailTipPhase = 0.0f;
float foxTailTipPhaseSpeed = 0.17f;
float foxTailTipSwingAmount = 14.0f;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ---- Estado e Posicao da Raposa no Mundo 3D ----
bool foxActive = false;      // True enquanto a raposa estiver atravessando o campo
float foxX = -13.0f;         // Posicao no eixo X (surge fora do campo a esquerda)
float foxY = 0.52f;          // Altura fixa (apoiada no chao)
float foxZ = 0.0f;           // Profundidade Z (faixa do campo onde ela passa)
float foxDirecao = 0.0f;     // Angulo de orientacao (0 graus = correndo para +X)

const float VELOCIDADE_RAPOSA = 0.12f;  // Velocidade de deslocamento por frame
const float RAIO_COLISAO_RAPOSA = 1.0f; // Alcance para atingir o coelho

bool foxJaTirouVidaNestaPassagem = false; // Evita tirar multiplas vidas em uma unica passagem

const int FOX_FPS = 1000 / 24;           // Base de tempo dos timers (24ms)
int framesAteProximaRaposa = 180;        // Primeira raposa surge apos alguns segundos (~4s)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funcao auxiliar didatica: desenha um elipsoide 3D utilizando glutSolidSphere + glScalef
static void drawElipsoideRaposa(float rx, float ry, float rz, int fatias = 18, int pilhas = 18) {
    glPushMatrix();
        glScalef(rx, ry, rz);
        glutSolidSphere(1.0, fatias, pilhas);
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha um olho da raposa (globo amendoado preto + reflexo branco)
static void desenharOlhoRaposa(bool olhoEsquerdo) {
    float sinalZ = olhoEsquerdo ? 1.0f : -1.0f;

    glPushMatrix();
        // Posiciona o olho na lateral superior da cabeca
        glTranslatef(0.12f, 0.08f, sinalZ * 0.14f);

        // Globo ocular preto
        glColor3f(0.05f, 0.05f, 0.05f);
        drawElipsoideRaposa(0.045f, 0.040f, 0.035f, 12, 12);

        // Ponto de brilho branco
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
            glTranslatef(0.02f, 0.015f, sinalZ * 0.02f);
            glutSolidSphere(0.014, 8, 8);
        glPopMatrix();
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha uma orelha pontuda de raposa:
// Face externa laranja, cavidade interna branca pura e ponta escura
static void desenharOrelhaRaposa(float swingAngle, bool orelhaEsquerda) {
    float sinalZ = orelhaEsquerda ? 1.0f : -1.0f;

    glPushMatrix();
        // Articulacao no topo da cabeca
        glTranslatef(-0.02f, 0.18f, sinalZ * 0.09f);

        // Inclinacao base: abre para fora no eixo Z e recua no eixo X
        glRotatef(sinalZ * 16.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(-15.0f, 0.0f, 0.0f, 1.0f);

        // Balanco dinamico da corrida (pitch para frente/tras)
        glRotatef(swingAngle, 0.0f, 0.0f, 1.0f);

        // 1. Parte externa da orelha (laranja pontudo)
        glColor3f(0.95f, 0.45f, 0.10f);
        glPushMatrix();
            glTranslatef(0.0f, 0.12f, 0.0f);
            drawElipsoideRaposa(0.05f, 0.16f, 0.075f, 14, 14);
        glPopMatrix();

        // 2. Interior da orelha (pelagem branca na cavidade frontal)
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
            glTranslatef(0.018f, 0.11f, 0.0f);
            drawElipsoideRaposa(0.035f, 0.12f, 0.055f, 12, 12);
        glPopMatrix();

        // 3. Pontinha escura no topo da orelha (caracteristica marcante da raposa)
        glColor3f(0.10f, 0.10f, 0.10f);
        glPushMatrix();
            glTranslatef(0.0f, 0.24f, 0.0f);
            drawElipsoideRaposa(0.03f, 0.05f, 0.04f, 10, 10);
        glPopMatrix();
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha uma perna da raposa com articulacao e "meias pretas" (extremidade escura)
static void desenharPataRaposa(float posX, float posY, float posZ, float liftY) {
    glPushMatrix();
        glTranslatef(posX, posY + liftY, posZ);

        // Parte superior da perna (laranja)
        glColor3f(0.95f, 0.45f, 0.10f);
        glPushMatrix();
            glTranslatef(0.0f, -0.12f, 0.0f);
            drawElipsoideRaposa(0.075f, 0.16f, 0.075f, 12, 12);
        glPopMatrix();

        // Extremidade inferior / pe com a "meia preta" classica de raposa
        glColor3f(0.12f, 0.10f, 0.08f);
        glPushMatrix();
            glTranslatef(0.02f, -0.32f, 0.0f);
            drawElipsoideRaposa(0.09f, 0.07f, 0.07f, 12, 12);
        glPopMatrix();
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha a cauda exuberante e volumosa da raposa (2 segmentos articulados com ponta branca)
static void desenharCaudaRaposa(float tailSwing, float tailTipSwing) {
    glPushMatrix();
        // Ponto de insercao da cauda na traseira do corpo (-X)
        glTranslatef(-0.48f, 0.08f, 0.0f);

        // Inclinacao base da cauda para tras e para cima
        glRotatef(20.0f, 0.0f, 0.0f, 1.0f);

        // Balanco do 1º segmento (horizontal com tailSwing e leve ondulacao vertical)
        glRotatef(tailSwing, 0.0f, 1.0f, 0.0f);
        glRotatef(tailSwing * 0.35f, 0.0f, 0.0f, 1.0f);

        // --- 1º Segmento: Base felpuda volumosa da cauda (Laranja) ---
        glColor3f(0.95f, 0.45f, 0.10f);
        glPushMatrix();
            glTranslatef(-0.25f, 0.06f, 0.0f);
            drawElipsoideRaposa(0.28f, 0.16f, 0.15f, 16, 16);
        glPopMatrix();

        // --- 2º Segmento: Articulacao intermediaria da ponta da cauda ---
        glPushMatrix();
            glTranslatef(-0.48f, 0.12f, 0.0f);

            // Balanco independente do 2º segmento (tailTipSwing)
            glRotatef(tailTipSwing, 0.0f, 1.0f, 0.0f);
            glRotatef(tailTipSwing * 0.40f, 0.0f, 0.0f, 1.0f);

            // Meio da cauda (Laranja)
            glColor3f(0.95f, 0.45f, 0.10f);
            glPushMatrix();
                glTranslatef(-0.14f, 0.03f, 0.0f);
                drawElipsoideRaposa(0.18f, 0.14f, 0.13f, 14, 14);
            glPopMatrix();

            // Ponta da cauda com mancha BRANCA pura (marca registrada da raposa!)
            glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
                glTranslatef(-0.30f, 0.05f, 0.0f);
                drawElipsoideRaposa(0.16f, 0.11f, 0.11f, 14, 14);
            glPopMatrix();
        glPopMatrix(); // Fim do 2º segmento

    glPopMatrix(); // Fim do 1º segmento
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha a cabeca completa da raposa (cranio, bochechas brancas, focinho canino afilado e orelhas)
static void desenharCabecaRaposa(float earLeftSwing, float earRightSwing) {
    glPushMatrix();
        // Posicionada a frente (+X) e acima (+Y) do tronco
        glTranslatef(0.52f, 0.20f, 0.0f);

        // 1. Cranio da Raposa (elipsoide laranja)
        glColor3f(0.95f, 0.45f, 0.10f);
        drawElipsoideRaposa(0.24f, 0.20f, 0.19f, 20, 20);

        // 2. Bochechas brancas fofas nas laterais da mandibula
        glColor3f(1.0f, 1.0f, 1.0f);
        // Bochecha esquerda (+Z)
        glPushMatrix();
            glTranslatef(0.06f, -0.06f, 0.10f);
            drawElipsoideRaposa(0.13f, 0.09f, 0.08f, 14, 14);
        glPopMatrix();
        // Bochecha direita (-Z)
        glPushMatrix();
            glTranslatef(0.06f, -0.06f, -0.10f);
            drawElipsoideRaposa(0.13f, 0.09f, 0.08f, 14, 14);
        glPopMatrix();

        // 3. Focinho Canino Afilado (Laranja na parte superior)
        glColor3f(0.95f, 0.45f, 0.10f);
        glPushMatrix();
            glTranslatef(0.20f, -0.04f, 0.0f);
            drawElipsoideRaposa(0.22f, 0.11f, 0.11f, 16, 16);
        glPopMatrix();

        // 4. Queixo e parte inferior do focinho (Branco puro)
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
            glTranslatef(0.16f, -0.10f, 0.0f);
            drawElipsoideRaposa(0.16f, 0.06f, 0.09f, 14, 14);
        glPopMatrix();

        // 5. Trufa preta do focinho (nariz)
        glColor3f(0.05f, 0.05f, 0.05f);
        glPushMatrix();
            glTranslatef(0.38f, -0.02f, 0.0f);
            glutSolidSphere(0.042, 12, 12);
        glPopMatrix();

        // 6. Olhos amendoados
        desenharOlhoRaposa(true);   // Olho esquerdo (+Z)
        desenharOlhoRaposa(false);  // Olho direito (-Z)

        // 7. Orelhas pontudas com balanco sincronizado a marcha
        desenharOrelhaRaposa(earLeftSwing, true);   // Orelha esquerda (+Z)
        desenharOrelhaRaposa(earRightSwing, false); // Orelha direita (-Z)

    glPopMatrix(); // Fim da Cabeca
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
================================================================================
 FUNCAO drawFox(): MODELAGEM 3D COMPLETA DA RAPOSA
================================================================================
 Reproduz com fidelidade todos os elementos do Trabalho 1 2D:
  - Paleta de cores oficial:
      * Pelagem principal: Laranja vibrante [0.95, 0.45, 0.10]
      * Laranja escuro (sombreamento coxas): [0.80, 0.35, 0.08]
      * Pelagem branca (peito, barriga, bochechas, cauda): [1.0, 1.0, 1.0]
      * Focinho/Patas escuras: [0.12, 0.10, 0.08]
  - Hierarquia de Transformacoes (Scene Graph):
      Corpo (Tronco principal - no raiz)
       ├── Peito e Ventre Branco
       ├── Cauda Articulada (Base + Ponta Branca com fases independentes)
       ├── Coxas Musculosas (traseiras e ombros dianteiros)
       ├── 4 Patas trotando em pares diagonais (Grupo A e Grupo B)
       ├── Pescoco inclinado
       └── Cabeca Canina
            ├── Bochechas brancas
            ├── Focinho pontudo + queixo branco + trufa preta
            ├── Olhos amendoados vivos
            └── Orelhas triangulares com interior branco
================================================================================
*/
void drawFox() {
    // Calculo do ciclo de trote das patas e balanco das orelhas (mesmas formulas do 2D)
    float foxPhaseSin = std::sin(foxWalkPhase);

    // Orelhas: balancam suavemente para frente/tras
    float earRightSwing = (foxPhaseSin > 0.0f) ?  foxPhaseSin * foxEarSwingAmount : 0.0f;
    float earLeftSwing  = (foxPhaseSin < 0.0f) ? -foxPhaseSin * foxEarSwingAmount : 0.0f;

    // Patas: revezamento dos pares diagonais (trote de canideo real)
    float legGroupALift = (foxPhaseSin > 0.0f) ?  foxPhaseSin * foxLegLiftAmount : 0.0f;
    float legGroupBLift = (foxPhaseSin < 0.0f) ? -foxPhaseSin * foxLegLiftAmount : 0.0f;

    // Cauda: duas oscilacoes senoidais com frequencias diferentes
    float tailSwing    = std::sin(foxTailPhase)    * foxTailSwingAmount;
    float tailTipSwing = std::sin(foxTailTipPhase) * foxTailTipSwingAmount;

    // Brilho especular sutil para realcar o volume 3D da musculatura
    GLfloat especularPelo[] = { 0.20f, 0.20f, 0.20f, 1.0f };
    GLfloat semBrilho[]     = { 0.0f,  0.0f,  0.0f,  1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especularPelo);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0f);

    // =========================================================================
    // 1. CORPO PRINCIPAL (TRONCO - NO RAIZ DA HIERARQUIA)
    // Elipsoide esguio e aerodinamico de predador.
    // =========================================================================
    glColor3f(0.95f, 0.45f, 0.10f);
    drawElipsoideRaposa(0.55f, 0.26f, 0.23f, 22, 22);

    // Peitoral e ventre branco da raposa
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
        glTranslatef(0.12f, -0.09f, 0.0f);
        drawElipsoideRaposa(0.40f, 0.16f, 0.19f, 16, 16);
    glPopMatrix();

    // Pescoco inclinado conectando o tronco a cabeca
    glColor3f(0.95f, 0.45f, 0.10f);
    glPushMatrix();
        glTranslatef(0.38f, 0.11f, 0.0f);
        glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
        drawElipsoideRaposa(0.16f, 0.22f, 0.18f, 16, 16);
    glPopMatrix();

    // =========================================================================
    // 2. CAUDA ARTICULADA (BASE + PONTA BRANCA)
    // =========================================================================
    desenharCaudaRaposa(tailSwing, tailTipSwing);

    // =========================================================================
    // 3. COXAS E OMBROS (VOLUME ANATOMICO)
    // =========================================================================
    // Coxas traseiras musculosas (tom levemente mais escuro)
    glColor3f(0.80f, 0.35f, 0.08f);
    // Coxa traseira esquerda (+Z)
    glPushMatrix();
        glTranslatef(-0.25f, -0.05f, 0.17f);
        drawElipsoideRaposa(0.17f, 0.18f, 0.09f, 14, 14);
    glPopMatrix();
    // Coxa traseira direita (-Z)
    glPushMatrix();
        glTranslatef(-0.25f, -0.05f, -0.17f);
        drawElipsoideRaposa(0.17f, 0.18f, 0.09f, 14, 14);
    glPopMatrix();

    // Ombros dianteiros
    glColor3f(0.95f, 0.45f, 0.10f);
    // Ombro dianteiro esquerdo (+Z)
    glPushMatrix();
        glTranslatef(0.28f, -0.04f, 0.15f);
        drawElipsoideRaposa(0.14f, 0.16f, 0.085f, 14, 14);
    glPopMatrix();
    // Ombro dianteiro direito (-Z)
    glPushMatrix();
        glTranslatef(0.28f, -0.04f, -0.15f);
        drawElipsoideRaposa(0.14f, 0.16f, 0.085f, 14, 14);
    glPopMatrix();

    // =========================================================================
    // 4. 4 PATAS COM TROTE DIAGONAL E MEIAS PRETAS
    // No chao (foxY = 0.52), o fundo das patas toca exatamente a grama (Y = 0.0).
    // =========================================================================
    // Dianteira Direita (-Z) e Traseira Esquerda (+Z) pertencem ao GRUPO A
    desenharPataRaposa( 0.28f, -0.20f, -0.16f, legGroupALift); // Dianteira Direita (-Z)
    desenharPataRaposa(-0.25f, -0.20f,  0.18f, legGroupALift); // Traseira Esquerda (+Z)

    // Dianteira Esquerda (+Z) e Traseira Direita (-Z) pertencem ao GRUPO B
    desenharPataRaposa( 0.28f, -0.20f,  0.16f, legGroupBLift); // Dianteira Esquerda (+Z)
    desenharPataRaposa(-0.25f, -0.20f, -0.18f, legGroupBLift); // Traseira Direita (-Z)

    // =========================================================================
    // 5. CABECA E ELEMENTOS FACIAIS
    // =========================================================================
    desenharCabecaRaposa(earLeftSwing, earRightSwing);

    // Restaura material padrao sem brilho
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, semBrilho);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sombra no solo: disco escuro projetado na grama sob a raposa
void drawSombraRaposa() {
    if (!foxActive) return;
    drawDiscoNoChao(foxX, foxZ, 0.65f, 0.35f);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Spawna a raposa na borda esquerda do campo, correndo para a direita (+X)
void spawnRaposa() {
    foxActive = true;
    foxX = -13.0f; // Comeca fora do campo a esquerda

    // Sorteia uma faixa Z aleatoria dentro dos limites do campo com folga da cerca
    float folgaZ = 1.0f;
    foxZ = CAMPO_Z_MIN + folgaZ + ((rand() % 1000) / 1000.0f) * (CAMPO_Z_MAX - CAMPO_Z_MIN - 2.0f * folgaZ);

    foxDirecao = 0.0f; // Olhando para +X
    foxJaTirouVidaNestaPassagem = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Controla o intervalo de tempo entre as passagens da raposa
void controlarSurgimentoDaRaposa() {
    if (foxActive) return;

    framesAteProximaRaposa--;
    if (framesAteProximaRaposa <= 0) {
        spawnRaposa();
        // Sorteia o tempo para a proxima passagem (entre 10 e 20 segundos)
        framesAteProximaRaposa = (10 + (rand() % 11)) * FOX_FPS;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Movimenta a raposa em linha reta para +X e avanca suas animacoes
void moverRaposa() {
    if (!foxActive) return;

    // Deslocamento para a frente no eixo X
    foxX += VELOCIDADE_RAPOSA;

    // Avanca as fases de animacao
    foxWalkPhase += foxWalkPhaseSpeed;
    if (foxWalkPhase > 2.0f * PI_F) foxWalkPhase -= 2.0f * PI_F;

    foxTailPhase += foxTailPhaseSpeed;
    if (foxTailPhase > 2.0f * PI_F) foxTailPhase -= 2.0f * PI_F;

    foxTailTipPhase += foxTailTipPhaseSpeed;
    if (foxTailTipPhase > 2.0f * PI_F) foxTailTipPhase -= 2.0f * PI_F;

    // Saiu pela borda direita do campo (fora da area visivel)
    if (foxX > 14.0f) {
        foxActive = false;
        framesAteProximaRaposa = (10 + (rand() % 11)) * FOX_FPS;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Verifica colisao 3D entre a raposa e o coelho
void verificarColisaoComRaposa() {
    if (!foxActive || foxJaTirouVidaNestaPassagem || coelhoEscondido) return;

    float dx = foxX - coelhoX;
    float dy = foxY - coelhoY;
    float dz = foxZ - coelhoZ;
    float distancia = std::sqrt(dx * dx + dy * dy + dz * dz);

    // Se o coelho pulou alto por cima da raposa, ele se esquiva com sucesso!
    if (coelhoY > 1.35f && std::abs(dz) < 0.8f) {
        return; // Esquivou do ataque
    }

    if (distancia < RAIO_COLISAO_RAPOSA) {
        if (rabbitLives > 0) {
            rabbitLives--;
        }
        foxJaTirouVidaNestaPassagem = true; // Uma vida por passagem
    }
}
