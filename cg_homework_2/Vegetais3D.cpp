#include "Vegetais3D.hpp"
#include "Cenario3D.hpp"
#include "Coelho3D.hpp"
#include <GL/glut.h>
#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<Vegetal> vegetais;
const int MAX_VEGETAIS = 15;

// Sorteia o intervalo de tempo ate o proximo vegetal (entre 3 e 15 segundos)
// Com base em 24ms por frame: 3s = 125 frames, 15s = 625 frames
static int sortearTempoProximoSpawn() {
    float segundos = 3.0f + (float)(rand() % 1201) / 100.0f; // Sorteia entre 3.00s e 15.00s
    return (int)(segundos * (1000.0f / 24.0f));
}

// O primeiro vegetal surge apos ~3 segundos do inicio
int framesAteProximoVegetal = (int)(3.0f * (1000.0f / 24.0f));

float vegetalGiro = 0.0f;
const float VEGETAL_GIRO_POR_FRAME = 2.0f; // Giro continuo sobre o proprio eixo

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cenoura: raiz laranja (cone com a ponta para baixo) + folhas verdes (cones finos). Inclinada -45 graus, como no 2D.
void drawCarrot() {
    glPushMatrix();
        glRotatef(-45.0f, 0, 0, 1); // Inclina toda a cenoura

        // Folhas: 3 folhas abertas em leque em volta do eixo vertical...
        glColor3f(0.243f, 0.556f, 0.180f);
        for (int i = 0; i < 3; i++) {
            glPushMatrix();
                glRotatef(i * 120.0f, 0, 1, 0);   // espalha as folhas ao redor do eixo Y
                glRotatef(-70.0f, 1, 0, 0);       // -90: o cone (que aponta para +Z) passa a apontar para cima; +20: abre para fora
                glutSolidCone(0.07, 0.55 + 0.05 * i, 10, 2);
            glPopMatrix();
        }
        // ...e uma folha central, vertical e mais alta
        glPushMatrix();
            glRotatef(-90.0f, 1, 0, 0);
            glutSolidCone(0.06, 0.75, 10, 2);
        glPopMatrix();

        // Raiz: cone com a ponta para baixo (rotacao de +90 em X leva +Z para -Y)
        glColor3f(0.95f, 0.52f, 0.13f);
        glPushMatrix();
            glRotatef(90.0f, 1, 0, 0);
            glutSolidCone(0.16, 1.1, 24, 6);

            // Sulcos da cenoura: aneis finos, mais escuros, cujo raio acompanha o afilamento do cone
            glColor3f(0.80f, 0.40f, 0.08f);
            for (int i = 0; i < 3; i++) {
                float profundidade = 0.25f + 0.25f * i;              // distancia da base do cone
                float raio = 0.16f * (1.0f - profundidade / 1.1f);   // raio do cone naquele ponto
                glPushMatrix();
                    glTranslatef(0.0f, 0.0f, profundidade);
                    glutSolidTorus(0.014, raio, 8, 24);
                glPopMatrix();
            }
        glPopMatrix();
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Elipsoide = esfera unitaria esticada. E a "folha" da alface e do rabanete.
static void drawElipsoide(float rx, float ry, float rz) {
    glPushMatrix();
        glScalef(rx, ry, rz);
        glutSolidSphere(1.0, 16, 12);
    glPopMatrix();
}

/*Alface: uma roseta de folhas em camadas, como o 2D (discos escuros por fora,
intermediarios, e um miolo claro), agora em volume: 6 folhas externas
escuras, 5 folhas intermediarias mais claras e o miolo. As folhas sao
elipsoides achatados, inclinados para cima nas pontas (formato de tigela).*/
void drawLettuce() {
    glPushMatrix();
        glTranslatef(0.0f, -0.10f, 0.0f); // centraliza a cabeca de alface na origem (como o centro do 2D)

        // Camada externa (escura)
        for (int i = 0; i < 6; i++) {
            if (i % 2 == 0) glColor3f(0.25f, 0.55f, 0.15f);
            else            glColor3f(0.30f, 0.60f, 0.18f);
            glPushMatrix();
                glRotatef(i * 60.0f, 0, 1, 0);
                glTranslatef(0.18f, 0.06f, 0.0f);
                glRotatef(22.0f, 0, 0, 1);       // ponta externa sobe
                drawElipsoide(0.30f, 0.07f, 0.22f);
            glPopMatrix();
        }

        // Camada intermediaria
        glColor3f(0.40f, 0.70f, 0.20f);
        for (int i = 0; i < 5; i++) {
            glPushMatrix();
                glRotatef(30.0f + i * 72.0f, 0, 1, 0);
                glTranslatef(0.10f, 0.16f, 0.0f);
                glRotatef(45.0f, 0, 0, 1);
                drawElipsoide(0.22f, 0.06f, 0.16f);
            glPopMatrix();
        }

        // Miolo claro
        glColor3f(0.60f, 0.85f, 0.25f);
        glPushMatrix();
            glTranslatef(0.0f, 0.15f, 0.0f);
            glutSolidSphere(0.18, 16, 16);
        glPopMatrix();
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Rabanete: bulbo vermelho (esfera), ponta branca (cone com a ponta para baixo) e folhas verdes no topo.
void drawRadish() {
    // Folhas: 3 abertas em leque + 1 central
    glColor3f(0.243f, 0.556f, 0.180f);
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
            glTranslatef(0.0f, 0.18f, 0.0f);
            glRotatef(i * 120.0f, 0, 1, 0);
            glRotatef(-65.0f, 1, 0, 0);  // aponta para cima, aberta 25 graus para fora
            glutSolidCone(0.07, 0.40, 10, 2);
        glPopMatrix();
    }
    glPushMatrix();
        glTranslatef(0.0f, 0.18f, 0.0f);
        glRotatef(-90.0f, 1, 0, 0);
        glutSolidCone(0.05, 0.45, 10, 2);
    glPopMatrix();

    // Bulbo vermelho
    glColor3f(0.86f, 0.24f, 0.35f);
    drawElipsoide(0.22f, 0.24f, 0.22f);

    // Ponta branca (raiz fina)
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
        glTranslatef(0.0f, -0.19f, 0.0f);
        glRotatef(90.0f, 1, 0, 0);
        glutSolidCone(0.09, 0.22, 16, 2);
    glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Centraliza a escolha de qual modelo desenhar de acordo com o tipo do vegetal
void drawVegetable(TipoVegetal tipo) {
    switch (tipo) {
        case CENOURA:  drawCarrot();  break;
        case ALFACE:   drawLettuce(); break;
        case RABANETE: drawRadish();  break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Desenha cada vegetal ativo na sua posicao, girando no proprio eixo Y
void drawVegetais() {
    for (const Vegetal &veg : vegetais) {
        if (!veg.ativo) continue;

        // Efeito de alerta: nos ultimos 1.5 segundos (~60 frames), pisca suavemente
        // avisando ao jogador que o item esta prestes a expirar e desaparecer
        if (veg.tempoRestante < 60 && (veg.tempoRestante / 6) % 2 == 0) {
            continue;
        }

        glPushMatrix();
            glTranslatef(veg.x, veg.y, veg.z);
            glRotatef(vegetalGiro, 0.0f, 1.0f, 0.0f);
            drawVegetable(veg.tipo);
        glPopMatrix();
    }
}

// Sombra no chao sob cada vegetal: mostra em que ponto do campo (X, Z) ele esta, e encolhe com a altura
void drawSombrasVegetais() {
    for (const Vegetal &veg : vegetais) {
        if (!veg.ativo) continue;

        // Se estiver piscando perto de expirar, a sombra acompanha o efeito
        if (veg.tempoRestante < 60 && (veg.tempoRestante / 6) % 2 == 0) {
            continue;
        }

        float raio = 0.32f - 0.04f * veg.y;
        if (raio < 0.12f) raio = 0.12f;
        drawDiscoNoChao(veg.x, veg.z, raio, 0.25f);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Spawn de um novo vegetal no plateau fixo:
   1) Sorteia o tipo (CENOURA, ALFACE ou RABANETE).
   2) Sorteia a altura: 70% no solo (Y = 0.35, alcancavel andando) e 30% aereo (Y = 2.0, exigindo pulo).
   3) Sorteia coordenadas (X, Z) aleatorias contidas dentro dos limites do plateau com folga da cerca.
   4) Define o tempo de vida como 5 segundos (TEMPO_VIDA_VEGETAL_FRAMES).
   5) Ativa no pool de vegetais. */
void spawnVegetable() {
    TipoVegetal tipoSorteado = static_cast<TipoVegetal>(rand() % 3);

    float y = 0.35f;
    if (rand() % 100 < 30) {
        y = 2.0f; // Vegetal aereo: exige pulo do coelho para alcancar
    }

    // Folga de 1.0 unidade para nao nascer colado na borda ou cerca do plateau
    float folgaX = 1.0f;
    float folgaZ = 1.0f;
    float minX = CAMPO_X_MIN + folgaX;
    float maxX = CAMPO_X_MAX - folgaX;
    float minZ = CAMPO_Z_MIN + folgaZ;
    float maxZ = CAMPO_Z_MAX - folgaZ;

    float x = minX + ((rand() % 1000) / 1000.0f) * (maxX - minX);
    float z = minZ + ((rand() % 1000) / 1000.0f) * (maxZ - minZ);

    // Procura um slot desativado no pool para reciclar
    for (Vegetal &veg : vegetais) {
        if (!veg.ativo) {
            veg.tipo = tipoSorteado;
            veg.x = x;
            veg.y = y;
            veg.z = z;
            veg.ativo = true;
            veg.tempoRestante = TEMPO_VIDA_VEGETAL_FRAMES; // 5 segundos
            return;
        }
    }

    // Se nao achou slot livre e ainda ha espaco no limite, cria um novo
    if ((int)vegetais.size() < MAX_VEGETAIS) {
        Vegetal novoVegetal = { tipoSorteado, x, y, z, true, TEMPO_VIDA_VEGETAL_FRAMES };
        vegetais.push_back(novoVegetal);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Atualizacao dos vegetais no plateau:
   - Gira os vegetais sobre o proprio eixo Y para dar volume tridimensional.
   - Decrementa o tempo de vida de cada vegetal no campo.
   - Apos 5 segundos sem coleta, o vegetal expira e desaparece do plateau. */
void moverVegetais() {
    vegetalGiro += VEGETAL_GIRO_POR_FRAME;
    if (vegetalGiro >= 360.0f) {
        vegetalGiro -= 360.0f;
    }

    for (Vegetal &veg : vegetais) {
        if (!veg.ativo) continue;

        veg.tempoRestante--;
        if (veg.tempoRestante <= 0) {
            veg.ativo = false; // Expirou os 5 segundos sem ser coletado
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Controla o surgimento dos vegetais:
   O tempo entre cada item de bonificacao varia de 3 a 15 segundos apos o
   ultimo aparecimento de um vegetal. */
void controlarSurgimentoDeVegetais() {
    framesAteProximoVegetal--;
    if (framesAteProximoVegetal <= 0) {
        spawnVegetable();
        // Sorteia novo intervalo de 3 a 15 segundos a partir deste instante
        framesAteProximoVegetal = sortearTempoProximoSpawn();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Chamada quando o coelho captura um vegetal; concede o efeito do tipo:
- CENOURA: turbo de velocidade por DURACAO_TURBO_EM_FRAMES frames.
- RABANETE: pulo reforcado por DURACAO_PULO_REFORCADO_EM_FRAMES frames.
- ALFACE: vida extra, respeitando MAX_VIDAS.
(Pegar de novo um bonus ja ativo reinicia o tempo dele, como no 2D.)*/
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
