#pragma once
#include <vector>

// ---------------------------------------------------------------------------
// Vegetais 3D: itens de bonificacao (power-ups).
//   CENOURA  -> turbo de velocidade
//   ALFACE   -> vida extra
//   RABANETE -> pulo reforcado
//
// No plateau fixo:
//   - Aparecem aleatoriamente pela area do quadrilatero.
//   - Permanecem ativos por 5 segundos antes de desaparecerem caso nao sejam coletados.
//   - O intervalo para o proximo surgimento varia de 3 a 15 segundos apos o
//     ultimo aparecimento de um vegetal.
// ---------------------------------------------------------------------------

enum TipoVegetal { CENOURA, ALFACE, RABANETE };

struct Vegetal {
    TipoVegetal tipo;
    float x;
    float y;
    float z;
    bool ativo;
    int tempoRestante; // Tempo restante de permanencia no campo (em frames)
};

extern std::vector<Vegetal> vegetais;   // Pool de vegetais no mundo
extern const int MAX_VEGETAIS;          // Limite superior de itens
extern int framesAteProximoVegetal;     // Timer (em frames) ate o proximo spawn

// Duracao de vida de cada vegetal no plateau (5 segundos = ~208 frames a 24ms por frame)
const int TEMPO_VIDA_VEGETAL_FRAMES = 5000 / 24;

extern float vegetalGiro;               // Angulo (graus) do giro dos vegetais em torno do eixo Y

// Modelos 3D
void drawCarrot();
void drawLettuce();
void drawRadish();
void drawVegetable(TipoVegetal tipo);

void drawVegetais();          // Desenha todos os vegetais ativos (com alerta piscante nos ultimos segundos)
void drawSombrasVegetais();   // Sombra de cada vegetal ativo no solo

void spawnVegetable();
void moverVegetais();         // Atualiza rotacao e contagem de tempo dos vegetais no plateau
void controlarSurgimentoDeVegetais(); // Controla intervalo de 3 a 15s para o proximo vegetal
void aplicarBonusDoVegetal(TipoVegetal tipo);
