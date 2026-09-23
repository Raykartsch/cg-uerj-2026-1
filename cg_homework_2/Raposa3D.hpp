#pragma once

// ---------------------------------------------------------------------------
// Raposa 3D: personagem secundario / predador terrestre do jogo.
//
// Modelada a partir da versao 2D do Trabalho 1, agora com volume tridimensional
// atraves de solidos GLUT (esferas/elipsoides, cones e cubos):
//  - Corpo esguio e aerodinamico com ventre e peitoral brancos
//  - Cabeca canina com bochechas brancas, focinho afilado e trufa preta
//  - Olhos amendoados expressivos com reflexo de luz
//  - Orelhas pontudas triangulares com interior branco e pontas escuras
//  - Cauda felpuda volumosa articulada em dois segmentos com ponta branca
//  - 4 patas com animacao de trote diagonal e meias pretas
// ---------------------------------------------------------------------------

// Variaveis de animacao do trote da raposa (mesmas formulas do Trabalho 1)
extern float foxWalkPhase;
extern float foxWalkPhaseSpeed;
extern float foxLegLiftAmount;
extern float foxEarSwingAmount;

// Variaveis da cauda articulada (duas fases independentes para movimento organico)
extern float foxTailPhase;
extern float foxTailPhaseSpeed;
extern float foxTailSwingAmount;

extern float foxTailTipPhase;
extern float foxTailTipPhaseSpeed;
extern float foxTailTipSwingAmount;

// Estado e posicao da raposa no mundo 3D
extern bool foxActive;
extern float foxX;
extern float foxY;
extern float foxZ;
extern float foxDirecao; // Angulo em graus (0 = correndo para +X)

extern const float VELOCIDADE_RAPOSA;
extern const float RAIO_COLISAO_RAPOSA;
extern bool foxJaTirouVidaNestaPassagem;

extern int framesAteProximaRaposa;

// Funcoes principais da Raposa 3D
void drawFox();                         // Desenha o modelo 3D completo da raposa
void drawSombraRaposa();                // Sombra eliptica projetada no chao sob a raposa
void spawnRaposa();                     // Spawna a raposa na borda do campo
void controlarSurgimentoDaRaposa();     // Controla o intervalo aleatorio entre passagens
void moverRaposa();                     // Atualiza a posicao e as fases de animacao
void verificarColisaoComRaposa();       // Verifica colisao 3D com o coelho
