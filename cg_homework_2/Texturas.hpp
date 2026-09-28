#pragma once
#include <GL/glut.h>

// ---------------------------------------------------------------------------
// Gerenciamento de Texturas OpenGL (baseado na logica de class_tasks/22_09_2026.cpp)
// ---------------------------------------------------------------------------

// IDs das texturas carregadas
extern GLuint texChao;     // Textura aplicada ao chao do plateau
extern GLuint texCoelho;   // Textura aplicada a pelagem do coelho
extern GLuint texRaposa;   // Textura aplicada a raposa
extern GLuint texAve;      // Textura aplicada as penas da ave de rapina
extern GLuint texMadeira;  // Textura de madeira para a moldura/cerca
extern GLuint texCenoura;  // Textura aplicada a raiz da cenoura (bonificacao)
extern GLuint texAlface;   // Textura aplicada as folhas da alface (bonificacao)
extern GLuint texRabanete; // Textura aplicada ao bulbo do rabanete (bonificacao)

// Loader de imagens BMP 24-bits sem compressao
bool loadBMP(const char *imagepath, GLuint &textureID);

// Carregamento de todas as texturas do projeto
void carregarTodasTexturas();

// Primitiva de esfera com coordenadas de textura UV (substitui glutSolidSphere para objetos texturizados)
void drawTexturedSphere(float radius, int slices, int stacks);

// Primitiva de cone com coordenadas de textura UV ao longo do corpo
void drawTexturedCone(float radius, float height, int slices, int stacks);

// Primitiva de cubo com coordenadas de textura UV em cada face
void drawTexturedCube(float size);

