#pragma once

// ---------------------------------------------------------------------------
// Coelho 3D: o personagem principal e uma ESFERA que se move sobre o
// quadrilatero do campo (plano XZ). O corpo do coelho sera modelado depois;
// a esfera e o volume de colisao/posicao que ele vai herdar.
// ---------------------------------------------------------------------------

// Posicao do CENTRO da esfera no mundo.
// (no jogo 2D: characterPos -> coelhoX, jump_height -> coelhoY; coelhoZ e novo)
extern float coelhoX;
extern float coelhoY;
extern float coelhoZ;

extern float anguloCoelho;  // Direcao que o coelho olha (yaw, em graus). 0 = +X. Substitui direcaoCoelho (+-1) do 2D.

extern const float RAIO_ESFERA;

// Velocidade (mesmos valores do jogo 2D, em unidades de mundo por frame)
extern float characterSpeed;
extern const float VELOCIDADE_NORMAL;

// Estado das setas do teclado (mesmos nomes do jogo 2D)
extern bool rightArrowPressed;
extern bool leftArrowPressed;
extern bool upArrowPressed;
extern bool downArrowPressed;

// Buff da cenoura: turbo de velocidade
extern const float VELOCIDADE_TURBO;
extern const int DURACAO_TURBO_EM_FRAMES;
extern int framesDeTurboRestantes;

// Pulo (mesma logica do 2D; no 3D e disparado pela barra de espaco)
extern float jump_maximum_height;
extern float speed_jump;
extern bool isJumping;
extern bool goingUp;

// Buff do rabanete: pulo reforcado
extern const float PULO_NORMAL;
extern const float PULO_REFORCADO;
extern int framesDePuloReforcadoRestantes;
extern const int DURACAO_PULO_REFORCADO_EM_FRAMES;

// Vidas (a alface concede vida extra)
extern int rabbitLives;
extern const int MAX_VIDAS;

// Ainda nao ha toca no 3D: fica sempre false. Ja existe porque a logica do
// jogo 2D congela o cenario (e ignora colisoes) enquanto o coelho esta escondido.
extern bool coelhoEscondido;

void moverCoelho();            // Atualiza a posicao a cada frame a partir das setas
void iniciarPulo();            // Dispara o pulo (se ja nao estiver pulando)
void atualizarPulo();          // Avanca a animacao do pulo
void atualizarBonusAtivos();   // Conta o tempo dos buffs (turbo e pulo reforcado)
void drawRabbit();             // Desenha a esfera (origem = centro da esfera)
void drawSombraCoelho();       // Sombra circular no chao, sob o personagem
