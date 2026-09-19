#pragma once

// ---------------------------------------------------------------------------
// Cenario 3D: o "campo" (quadrilatero no plano XZ) e o gramado que rola.
//
// Convencao de eixos adotada em todo o projeto 3D:
//   X -> esquerda / direita   (era o X do jogo 2D)
//   Y -> altura               (era o Y do jogo 2D; usado pelo pulo e pelos vegetais)
//   Z -> profundidade         (eixo NOVO: +Z vem em direcao a camera)
// O chao fica no plano Y = 0.
// ---------------------------------------------------------------------------

const float PI_F = 3.14159265f; // (const em header: cada .cpp enxerga a sua copia, sem conflito)

// Limites do quadrilatero (retangulo) do campo, no plano XZ.
// Em X mantemos os mesmos +-8 do glOrtho do jogo 2D.
extern const float CAMPO_X_MIN;
extern const float CAMPO_X_MAX;
extern const float CAMPO_Z_MIN;
extern const float CAMPO_Z_MAX;

extern const float TAMANHO_LADRILHO;  // lado de cada ladrilho do gramado
extern const float GRAMADO_X_EXT;     // o gramado se estende de -EXT a +EXT em X (bem alem da cerca)

// Rolagem do cenario (igual ao jogo 2D): o mundo desliza para -X, dando a
// impressao de que o coelho corre para +X.
extern float bgSpeed;   // deslocamento por frame
extern float bgWidth;   // periodo do deslocamento (multiplo de 2 ladrilhos, para o padrao nao "pular")
extern float bgPos;     // deslocamento acumulado

// Ceu (reaproveita a cor de "dia" do ciclo dia/noite original)
extern float skyR, skyG, skyB;

void drawCampo();       // gramado (ladrilhos rolando) + cerca (borda) do quadrilatero
void rolarCenario();    // avanca bgPos (chamada a cada frame)

// Disco escuro semitransparente colado ao chao (usado nas sombras)
void drawDiscoNoChao(float cx, float cz, float raio, float alpha);
