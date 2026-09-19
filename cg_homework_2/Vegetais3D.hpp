#pragma once
#include <vector>

// ---------------------------------------------------------------------------
// Vegetais 3D: itens de bonificacao (power-ups). Mesmos tres tipos do 2D:
//   CENOURA  -> turbo de velocidade
//   ALFACE   -> vida extra
//   RABANETE -> pulo reforcado
// ---------------------------------------------------------------------------

enum TipoVegetal { CENOURA, ALFACE, RABANETE };

// Igual ao 2D, com a coordenada z nova (profundidade no campo)
struct Vegetal {
    TipoVegetal tipo;
    float x;
    float y;
    float z;
    bool ativo;
};

extern std::vector<Vegetal> vegetais;   // Pool de vegetais ativos/inativos no mundo
extern const int MAX_VEGETAIS;          // Limite superior de itens
extern int framesAteProximoVegetal;     // Timer (em frames) para o proximo spawn

extern const float VEGETAL_SPAWN_X;     // X onde os vegetais surgem (fora da area visivel, a direita)
extern const float VEGETAL_SAIDA_X;     // X a partir do qual sao descartados (fora da area visivel, a esquerda)
extern float vegetalGiro;               // Angulo (graus) do giro dos vegetais em torno do eixo Y

// Modelos 3D (origem = ponto de referencia do item, o mesmo usado na colisao)
void drawCarrot();
void drawLettuce();
void drawRadish();
void drawVegetable(TipoVegetal tipo);

void drawVegetais();          // Desenha todos os vegetais ativos
void drawSombrasVegetais();   // Sombra de cada vegetal ativo, no chao

void spawnVegetable();
void moverVegetais();
void controlarSurgimentoDeVegetais();
void aplicarBonusDoVegetal(TipoVegetal tipo);
