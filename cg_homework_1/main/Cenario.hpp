#pragma once
#include <vector>

// Enum e Structs do Cenario
enum TipoVegetal { CENOURA, ALFACE, RABANETE };

struct Vegetal {
    TipoVegetal tipo;
    float x;
    float y;
    bool ativo;
};

struct Toca {
    float x;
    float y;
};

// Variáveis Globais do Cenario
extern float bgSpeed;
extern float bgWidth;
extern float bgPos;

extern std::vector<Vegetal> vegetais;
extern const int MAX_VEGETAIS;
extern int framesAteProximoVegetal;

extern std::vector<Toca> tocas;
extern const float RAIO_TOCA;

extern float cloudPhase;
extern float cloudPhaseSpeed;
extern float cloudSideSwingAmount;
extern float cloudMiddleBobAmount;

extern float tempoDeDiaFase;
extern const float VELOCIDADE_CICLO_DIA;
extern float skyR, skyG, skyB;

// Funções do Cenario
void drawSun();
void drawCloud();
void drawFence();
void drawFruit(float red, float green, float blue);
void drawCarrot();
void drawLettuce();
void drawRadish();
void drawVegetable(TipoVegetal tipo);
void spawnVegetable();
void aplicarBonusDoVegetal(TipoVegetal tipo);
void drawText(float x, float y, const char *texto);
void drawToca();
void atualizarTocas();
float interpolarCor(float inicio, float fim, float t);
void atualizarCorDoCeu();
void drawBackgroundContent();
void drawBackgroundSky();   // Assinatura corrigida para o .cpp
void drawBackgroundGrass(); // Assinatura corrigida para o .cpp
void moverVegetais();
void controlarSurgimentoDeVegetais();
void initCenario();
