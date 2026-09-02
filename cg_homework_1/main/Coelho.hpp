#ifndef COELHO_HPP
#define COELHO_HPP

enum TipoVegetal { CENOURA, ALFACE, RABANETE };

extern float characterPos;
extern float jump_height;
extern int rabbitLives;
extern float direcaoCoelho;
extern int framesDeTurboRestantes;
extern int framesDePuloReforcadoRestantes;
extern float folegoAtual;
extern const float FOLEGO_MAXIMO;

void drawRabbit();
void moverCoelho();
void animarCoelho();
void atualizarBonusAtivos();
void atualizarCorrida();
void aplicarBonusDoVegetal(TipoVegetal tipo);

#endif


