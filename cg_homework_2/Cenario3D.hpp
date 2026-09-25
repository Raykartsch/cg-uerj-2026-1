#pragma once

// ---------------------------------------------------------------------------
// Cenario 3D: o "plateau" (quadrilatero fixo no plano XZ centrado na origem).
//
// Convencao de eixos adotada em todo o projeto 3D:
//   X -> largura do plateau (-8 a +8)
//   Y -> altura (chao do plateau em Y = 0; pulo e ave em +Y)
//   Z -> profundidade (-6 a +6; +Z vem em direcao a camera)
// ---------------------------------------------------------------------------

const float PI_F = 3.14159265f;

// Limites do quadrilatero do plateau no plano XZ
extern const float CAMPO_X_MIN;
extern const float CAMPO_X_MAX;
extern const float CAMPO_Z_MIN;
extern const float CAMPO_Z_MAX;

extern const float TAMANHO_LADRILHO;  // Lado de cada ladrilho do gramado

// Ceu
extern float skyR, skyG, skyB;

// Buracos ficticios no chao do plateau (obstaculos temporarios no solo)
// Regras:
//   - Maximo 2 buracos ativos simultaneamente
//   - Cada buraco permanece ativo por ate 6 segundos
//   - Apos fechar, um novo buraco pode surgir em um periodo de 3 a 15 segundos
struct Buraco {
    float x;
    float z;
    float raioBase;
    bool ativo;
    int tempoRestante; // Tempo restante ativo em frames (ate 6s = 250 frames)
    int timerSpawn;    // Contagem regressiva ate surgir (entre 3s e 15s)
};

const int MAX_BURACOS = 2;
extern Buraco buracos[MAX_BURACOS];

void inicializarBuracos();         // Inicializa timers e estados dos buracos
void controlarBuracos();           // Gerencia tempo de vida (6s) e novos spawns (3 a 15s)
void drawCampo();                  // Desenha o plateau 3D (gramado quadriculado, borda e laterais)
void drawBuracos();                // Desenha os buracos ficticios ativos no chao do plateau
void verificarColisaoComBuracos(); // Verifica se o coelho pisou num buraco
void rolarCenario();               // Funcao mantida por compatibilidade (sem rolagem)

// Disco escuro semitransparente colado ao chao (usado nas sombras)
void drawDiscoNoChao(float cx, float cz, float raio, float alpha);
