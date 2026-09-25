#include "Cenario3D.hpp"
#include "Coelho3D.hpp"
#include <GL/glut.h>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quadrilatero (plateau fixo) do campo, contido no plano XZ centrado na origem (Y = 0).
// X varia de -8.0 a +8.0; Z varia de -6.0 a +6.0.
const float CAMPO_X_MIN = -8.0f;
const float CAMPO_X_MAX =  8.0f;
const float CAMPO_Z_MIN = -6.0f;
const float CAMPO_Z_MAX =  6.0f;

const float TAMANHO_LADRILHO = 1.0f;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cor do ceu (azul suave de dia)
float skyR = 0.68f, skyG = 0.81f, skyB = 0.98f;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Como o jogo agora se passa em um plateau fixo (nao e um runner com rolagem infinita),
// esta funcao nao precisa deslocar o mundo, mantendo-se apenas para compatibilidade.
void rolarCenario() {
    // Mundo fixo: sem rolagem
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Desenha o plateau fixo no plano XZ:
   1. Superficie superior de grama: ladrilhos quadriculados que alternam dois tons
      de verde, oferecendo excelente nocao de profundidade e escala espacial.
   2. Laterais do plateau: paredes verticais descendo de Y = 0.0 ate Y = -0.5 em tom
      de terra/rocha, dando a sensacao de uma plataforma suspensa no cenario.
   3. Borda perimetral: moldura de madeira marrom delimitando a area jogavel. */
void drawCampo() {
    int qtdX = (int)((CAMPO_X_MAX - CAMPO_X_MIN) / TAMANHO_LADRILHO);
    int qtdZ = (int)((CAMPO_Z_MAX - CAMPO_Z_MIN) / TAMANHO_LADRILHO);

    // -------------------------------------------------------------------------
    // 1. Superficie superior do plateau (Y = 0.0)
    // -------------------------------------------------------------------------
    glNormal3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    for (int i = 0; i < qtdX; i++) {
        float x0 = CAMPO_X_MIN + i * TAMANHO_LADRILHO;
        float x1 = x0 + TAMANHO_LADRILHO;

        for (int j = 0; j < qtdZ; j++) {
            float z0 = CAMPO_Z_MIN + j * TAMANHO_LADRILHO;
            float z1 = z0 + TAMANHO_LADRILHO;

            if ((i + j) % 2 == 0) {
                glColor3f(0.549f, 0.776f, 0.247f); // Verde claro da grama
            } else {
                glColor3f(0.490f, 0.700f, 0.210f); // Tom de verde ligeiramente mais escuro
            }

            // Ordem anti-horaria vista de cima (+Y)
            glVertex3f(x0, 0.0f, z0);
            glVertex3f(x0, 0.0f, z1);
            glVertex3f(x1, 0.0f, z1);
            glVertex3f(x1, 0.0f, z0);
        }
    }
    glEnd();

    // -------------------------------------------------------------------------
    // 2. Laterais do plateau (paredes de terra de Y = 0.0 ate Y = -0.5)
    // -------------------------------------------------------------------------
    float alturaBase = -0.5f;
    glColor3f(0.38f, 0.25f, 0.15f); // Tom de terra/rocha

    glBegin(GL_QUADS);
        // Face frontal (+Z)
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(CAMPO_X_MIN,  0.0f,       CAMPO_Z_MAX);
        glVertex3f(CAMPO_X_MIN,  alturaBase, CAMPO_Z_MAX);
        glVertex3f(CAMPO_X_MAX,  alturaBase, CAMPO_Z_MAX);
        glVertex3f(CAMPO_X_MAX,  0.0f,       CAMPO_Z_MAX);

        // Face traseira (-Z)
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(CAMPO_X_MAX,  0.0f,       CAMPO_Z_MIN);
        glVertex3f(CAMPO_X_MAX,  alturaBase, CAMPO_Z_MIN);
        glVertex3f(CAMPO_X_MIN,  alturaBase, CAMPO_Z_MIN);
        glVertex3f(CAMPO_X_MIN,  0.0f,       CAMPO_Z_MIN);

        // Face lateral esquerda (-X)
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(CAMPO_X_MIN,  0.0f,       CAMPO_Z_MIN);
        glVertex3f(CAMPO_X_MIN,  alturaBase, CAMPO_Z_MIN);
        glVertex3f(CAMPO_X_MIN,  alturaBase, CAMPO_Z_MAX);
        glVertex3f(CAMPO_X_MIN,  0.0f,       CAMPO_Z_MAX);

        // Face lateral direita (+X)
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(CAMPO_X_MAX,  0.0f,       CAMPO_Z_MAX);
        glVertex3f(CAMPO_X_MAX,  alturaBase, CAMPO_Z_MAX);
        glVertex3f(CAMPO_X_MAX,  alturaBase, CAMPO_Z_MIN);
        glVertex3f(CAMPO_X_MAX,  0.0f,       CAMPO_Z_MIN);
    glEnd();

    // -------------------------------------------------------------------------
    // 3. Moldura de madeira (cerca do quadrilatero)
    // -------------------------------------------------------------------------
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
/* Disco escuro e semitransparente no plano XZ, ligeiramente acima do chao
   (Y = 0.02) para nao conflitar no Z-buffer. Serve de sombra para o
   personagem e para os objetos do jogo. */
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gerenciamento dinamico de buracos no plateau:
//  - No maximo 2 buracos ativos simultaneamente (MAX_BURACOS = 2)
//  - Cada buraco permanece ativo por ate 6 segundos (TEMPO_VIDA_BURACO_FRAMES)
//  - Quando se fecha, outro buraco pode surgir em um periodo de 3 a 15 segundos
const int TEMPO_VIDA_BURACO_FRAMES = (int)(6.0f * (1000.0f / 24.0f)); // 250 frames = 6s

Buraco buracos[MAX_BURACOS] = {
    { 0.0f, 0.0f, 0.85f, false, 0, (int)(3.0f * (1000.0f / 24.0f)) },
    { 0.0f, 0.0f, 0.85f, false, 0, (int)(8.0f * (1000.0f / 24.0f)) }
};

// Flag para evitar que o coelho perca multiplas vidas em frames seguidos ao pisar no mesmo buraco
static bool coelhoNoBuraco = false;

// Sorteia o intervalo de tempo ate o proximo buraco surgir (entre 3 e 15 segundos)
static int sortearIntervaloBuraco() {
    float segundos = 3.0f + (float)(rand() % 1201) / 100.0f; // 3.00s a 15.00s
    return (int)(segundos * (1000.0f / 24.0f));
}

// Inicializa os timers iniciais de spawn
void inicializarBuracos() {
    buracos[0].ativo = false;
    buracos[0].tempoRestante = 0;
    buracos[0].raioBase = 0.85f;
    buracos[0].timerSpawn = (int)(3.0f * (1000.0f / 24.0f)); // Primeiro buraco em ~3s

    buracos[1].ativo = false;
    buracos[1].tempoRestante = 0;
    buracos[1].raioBase = 0.85f;
    buracos[1].timerSpawn = (int)(8.0f * (1000.0f / 24.0f)); // Segundo buraco em ~8s
}

// Spawna um buraco em uma posicao aleatoria no meio do plateau
static void spawnBuraco(Buraco &b) {
    // Folga para nao colar na cerca de madeira
    float minX = CAMPO_X_MIN + 2.0f;
    float maxX = CAMPO_X_MAX - 2.0f;
    float minZ = CAMPO_Z_MIN + 1.8f;
    float maxZ = CAMPO_Z_MAX - 1.8f;

    float sorteadoX = 0.0f;
    float sorteadoZ = 0.0f;

    for (int tentativa = 0; tentativa < 15; tentativa++) {
        sorteadoX = minX + ((rand() % 1000) / 1000.0f) * (maxX - minX);
        sorteadoZ = minZ + ((rand() % 1000) / 1000.0f) * (maxZ - minZ);

        // Nao spawna em cima do coelho
        float dcx = sorteadoX - coelhoX;
        float dcz = sorteadoZ - coelhoZ;
        if (dcx * dcx + dcz * dcz < 2.2f * 2.2f) {
            continue;
        }

        // Nao sobrepoe outro buraco ativo
        bool colidiuOutro = false;
        for (int j = 0; j < MAX_BURACOS; j++) {
            if (&buracos[j] != &b && buracos[j].ativo) {
                float dbx = sorteadoX - buracos[j].x;
                float dbz = sorteadoZ - buracos[j].z;
                if (dbx * dbx + dbz * dbz < 3.0f * 3.0f) {
                    colidiuOutro = true;
                    break;
                }
            }
        }
        if (!colidiuOutro) {
            break;
        }
    }

    b.x = sorteadoX;
    b.z = sorteadoZ;
    b.raioBase = 0.85f;
    b.tempoRestante = TEMPO_VIDA_BURACO_FRAMES; // 6 segundos ativo
    b.ativo = true;
}

// Calcula o raio com animacao suave de abertura (0.5s) e fechamento (0.6s)
static float obterRaioAtual(const Buraco &b) {
    if (!b.ativo) return 0.0f;

    const int FRAMES_ABERTURA = 20;   // ~0.5s para se abrir
    const int FRAMES_FECHAMENTO = 25; // ~0.6s para se fechar

    // Fase de fechamento (ultimos frames de vida)
    if (b.tempoRestante < FRAMES_FECHAMENTO) {
        float f = (float)b.tempoRestante / (float)FRAMES_FECHAMENTO;
        return b.raioBase * f;
    }

    // Fase de abertura (primeiros frames de vida)
    int decorrido = TEMPO_VIDA_BURACO_FRAMES - b.tempoRestante;
    if (decorrido < FRAMES_ABERTURA) {
        float f = (float)decorrido / (float)FRAMES_ABERTURA;
        return b.raioBase * f;
    }

    return b.raioBase;
}

// Controla o ciclo de vida dos buracos: decrementa duracao (6s) e conta o tempo para novo spawn (3 a 15s)
void controlarBuracos() {
    for (int i = 0; i < MAX_BURACOS; i++) {
        if (buracos[i].ativo) {
            buracos[i].tempoRestante--;
            if (buracos[i].tempoRestante <= 0) {
                // O buraco se fecha apos 6 segundos
                buracos[i].ativo = false;
                // Quando se fecha, agenda o proximo para surgir em um periodo de 3 a 15 segundos
                buracos[i].timerSpawn = sortearIntervaloBuraco();
            }
        } else {
            buracos[i].timerSpawn--;
            if (buracos[i].timerSpawn <= 0) {
                spawnBuraco(buracos[i]);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Desenha um buraco ficticio no solo:
   - Uma borda externa com tom de terra escura/revirada.
   - Um circulo interno preto profundo, simulando o abismo do buraco.
   Desenhado ligeiramente acima do gramado para evitar z-fighting. */
static void drawBuraco(float cx, float cz, float raio) {
    if (raio <= 0.05f) return;

    glDisable(GL_LIGHTING);

    const int SEGMENTOS = 28;
    float raioInterno = raio * 0.75f;

    // 1. Borda de terra ao redor do buraco (anel externo)
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= SEGMENTOS; i++) {
        float ang = (2.0f * PI_F / SEGMENTOS) * i;
        float cosA = std::cos(ang);
        float sinA = std::sin(ang);

        // Borda externa: terra mais clara
        glColor3f(0.28f, 0.18f, 0.10f);
        glVertex3f(cx + raio * cosA, 0.008f, cz + raio * sinA);

        // Borda interna: terra bem escura
        glColor3f(0.08f, 0.05f, 0.02f);
        glVertex3f(cx + raioInterno * cosA, 0.009f, cz + raioInterno * sinA);
    }
    glEnd();

    // 2. Interior escuro (fundo simulando o vazio/abismo)
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.01f, 0.01f, 0.01f);
        glVertex3f(cx, 0.010f, cz);
        for (int i = 0; i <= SEGMENTOS; i++) {
            float ang = (2.0f * PI_F / SEGMENTOS) * i;
            glVertex3f(cx + raioInterno * std::cos(ang), 0.010f, cz + raioInterno * std::sin(ang));
        }
    glEnd();

    glEnable(GL_LIGHTING);
}

void drawBuracos() {
    for (int i = 0; i < MAX_BURACOS; i++) {
        if (!buracos[i].ativo) continue;
        float r = obterRaioAtual(buracos[i]);
        drawBuraco(buracos[i].x, buracos[i].z, r);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Logica de colisao do coelho com os buracos ficticios:
   - Se o coelho passar por cima andando (no nivel do solo), ele cai no buraco e perde 1 vida.
   - Se estiver pulando alto, consegue saltar sobre o buraco sem se machucar.
   - Utiliza a flag coelhoNoBuraco para tirar apenas 1 vida por pisada. */
void verificarColisaoComBuracos() {
    if (coelhoEscondido) return;

    bool sobreBuraco = false;

    for (int i = 0; i < MAX_BURACOS; i++) {
        if (!buracos[i].ativo) continue;

        float raioAtual = obterRaioAtual(buracos[i]);
        if (raioAtual < 0.35f) continue; // Ainda se abrindo ou quase fechado

        float dx = coelhoX - buracos[i].x;
        float dz = coelhoZ - buracos[i].z;
        float distancia = std::sqrt(dx * dx + dz * dz);

        // Verifica se o coelho pisou na area do buraco
        if (distancia < raioAtual) {
            // Se estiver no chao (nao pulou por cima)
            if (coelhoY < RAIO_ESFERA + 0.35f) {
                sobreBuraco = true;

                // Tira uma vida se acabou de pisar no buraco
                if (!coelhoNoBuraco) {
                    if (rabbitLives > 0) {
                        rabbitLives--;
                    }
                    coelhoNoBuraco = true;
                }
                break;
            }
        }
    }

    // Ao sair completamente de cima do buraco, reseta a flag
    if (!sobreBuraco) {
        coelhoNoBuraco = false;
    }
}
