#pragma once

extern bool aveActive;
extern float aveX;
extern float aveY;
extern float avePhase;
extern bool aveJaTirouVida;
extern int framesAteProximaAve;

void drawAve();
void spawnAve(float targetX);
void controlarSurgimentoDaAve(float coelhoX);
void moverAve();
