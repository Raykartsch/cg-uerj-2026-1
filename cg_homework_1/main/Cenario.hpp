#ifndef CENARIO_HPP
#define CENARIO_HPP

#include <vector>
#include "Coelho.hpp"

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

extern float bgSpeed;
extern float bgWidth;
extern float bgPos;
extern bool coelhoEscondido;
extern int tocaOndeEstaEscondido;
extern std::vector<Vegetal> vegetais;
extern std::vector<Toca> tocas;
extern const float RAIO_TOCA;

void drawSun();
void drawCloud();
void animarNuvensECeu();
void drawFence();
void drawBackgroundContent();
void drawBackground1();
void desenharTocas();
void desenharVegetais();
void atualizarTocas();
void atualizarEsconderijoDoCoelho();
void moverVegetais();
void verificarColisaoComVegetais();
void controlarSurgimentoDeVegetais();
void drawText(float x, float y, const char *texto);

#endif
