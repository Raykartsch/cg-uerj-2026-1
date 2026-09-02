#pragma once

// Variáveis Globais da Raposa
extern float foxWalkPhase;
extern float foxWalkPhaseSpeed;
extern float foxLegLiftAmount;
extern float foxEarSwingAmount;

extern float foxTailPhase;
extern float foxTailPhaseSpeed;
extern float foxTailSwingAmount;

extern float foxTailTipPhase;
extern float foxTailTipPhaseSpeed;
extern float foxTailTipSwingAmount;

extern bool foxActive;
extern float foxX;
extern float foxY;
extern float foxDirecao;

extern const float VELOCIDADE_RAPOSA;
extern const float RAIO_COLISAO_RAPOSA;
extern bool foxJaTirouVidaNestaPassagem;

extern const int FRAMES_POR_SEGUNDO;
extern int framesAteProximaRaposa;

// Funções da Raposa
void drawFox();
void spawnRaposa();
void controlarSurgimentoDaRaposa();
void moverRaposa();
