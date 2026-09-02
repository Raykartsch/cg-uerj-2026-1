#pragma once

// Variáveis Globais do Coelho
extern float characterPos;
extern float characterSpeed;

extern bool rightArrowPressed;
extern bool leftArrowPressed;
extern bool upArrowPressed;
extern bool downArrowPressed;

extern const float VELOCIDADE_NORMAL;
extern const float VELOCIDADE_TURBO;
extern int framesDeTurboRestantes;
extern const int DURACAO_TURBO_EM_FRAMES;

extern const float VELOCIDADE_CORRENDO;
extern const float FOLEGO_MAXIMO;
extern float folegoAtual;
extern const float GASTO_FOLEGO_POR_FRAME;
extern const float RECUPERACAO_FOLEGO_POR_FRAME;
extern const float RECUPERACAO_FOLEGO_ESCONDIDO;
extern bool coelhoCorrendo;

extern float jump_maximum_height;
extern float speed_jump;
extern float jump_height;
extern bool isJumping;
extern bool goingUp;

extern const float PULO_NORMAL;
extern const float PULO_REFORCADO;
extern int framesDePuloReforcadoRestantes;
extern const int DURACAO_PULO_REFORCADO_EM_FRAMES;

extern float walkPhase;
extern float walkPhaseSpeed;
extern float legLiftAmount;
extern float earSwingAmount;
extern float direcaoCoelho;

extern int rabbitLives;
extern const int MAX_VIDAS;

extern bool coelhoEscondido;
extern int tocaOndeEstaEscondido;

// Funções do Coelho
void drawRabbit();
void atualizarBonusAtivos();
void atualizarCorrida();
void atualizarEsconderijoDoCoelho();
