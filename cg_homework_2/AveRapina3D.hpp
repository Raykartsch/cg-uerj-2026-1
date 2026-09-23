#pragma once

// ---------------------------------------------------------------------------
// AveRapina 3D: predador aereo do jogo (gaviao / aguia de rapina).
//
// Adaptada do Trabalho 1 2D para o ambiente tridimensional:
//  - Executa um mergulho parabolico rasante a partir do ceu em direcao
//    a posicao onde o coelho estava quando ela iniciou o ataque.
//  - Modelagem hierarquica com solidos GLUT:
//      * Corpo aerodinamico e peito/ventre creme claro
//      * Cabeca com crista/moicano de penas escuras
//      * Bico curvo de predador dourado/amarelo
//      * Olhos amendoados ferozes com iris amarela e pupila preta
//      * Asas tridimensionais articuladas com envergadura no eixo Z e
//        batimento sincronizado via onda senoidal
//      * Cauda em leque de penas na traseira
//      * Garras amarelas de predador
//      * Sombra dinamica projetada no solo indicando a proximidade do mergulho
// ---------------------------------------------------------------------------

// Estado e posicao da ave no espaco 3D
extern bool aveActive;
extern float aveX;
extern float aveY;
extern float aveZ;
extern float avePhase;      // Fase do bater de asas (em radianos)
extern bool aveJaTirouVida; // Trava para tirar no maximo uma vida por ataque

// Parametros do mergulho parabolico
extern float aveAlvoX;      // Posicao X do vertice mais baixo do mergulho
extern float aveAlvoZ;      // Posicao Z onde o coelho estava no spawn
extern float aveVelocidadeX;
extern int framesAteProximaAve;

// Funcoes da Ave de Rapina 3D
void drawBird();                        // Desenha o modelo 3D completo da ave
void drawSombraAve();                   // Sombra dinamica no chao (varia com a altura)
void spawnAve(float targetX, float targetZ); // Inicia o mergulho aereo mirando no alvo
void controlarSurgimentoDaAve(float coelhoX, float coelhoZ); // Temporizador de ataque
void moverAve();                        // Atualiza a trajetoria parabolica e o bater de asas
void verificarColisaoComAve();          // Colisao esferica 3D com o coelho
