#include "Texturas.hpp"
#include <cstdio>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// IDs globais das texturas
GLuint texChao     = 0;
GLuint texCoelho   = 0;
GLuint texRaposa   = 0;
GLuint texAve      = 0;
GLuint texMadeira  = 0;
GLuint texCenoura  = 0;
GLuint texAlface   = 0;
GLuint texRabanete = 0;
// Carrega uma imagem BMP de 24 bits (sem compressao) e cria uma
// textura OpenGL a partir dela. Loader simples e sem dependencias externas,
// identico ao implementado em class_tasks/22_09_2026.cpp.
bool loadBMP(const char *imagepath, GLuint &textureID) {
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    unsigned char *data;

    FILE *file = fopen(imagepath, "rb");
    if (!file) {
        printf("AVISO: nao foi possivel abrir a textura \"%s\".\n", imagepath);
        printf("       Verifique se a pasta \"textures\" esta ao lado do executavel.\n");
        return false;
    }

    if (fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M') {
        printf("AVISO: \"%s\" nao e um arquivo BMP valido de 24 bits.\n", imagepath);
        fclose(file);
        return false;
    }

    dataPos   = *(unsigned int*)&(header[0x0A]);
    imageSize = *(unsigned int*)&(header[0x22]);
    width     = *(unsigned int*)&(header[0x12]);
    height    = *(unsigned int*)&(header[0x16]);

    if (imageSize == 0) imageSize = width * height * 3;
    if (dataPos == 0)   dataPos = 54;

    data = new unsigned char[imageSize];
    fseek(file, dataPos, SEEK_SET);
    fread(data, 1, imageSize, file);
    fclose(file);

    // BMP guarda os pixels em BGR; troca para RGB.
    for (unsigned int i = 0; i + 2 < imageSize; i += 3) {
        unsigned char tmp = data[i];
        data[i]     = data[i + 2];
        data[i + 2] = tmp;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Gera os mipmaps para que a textura mantenha excelente nitidez a diferentes distancias da camera
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    delete[] data;

    printf("Textura carregada com sucesso: %s (%ux%u)\n", imagepath, width, height);
    return true;
}

void carregarTodasTexturas() {
    loadBMP("textures/grama.bmp",    texChao);
    loadBMP("textures/pelagem.bmp",  texCoelho);
    loadBMP("textures/raposa.bmp",   texRaposa);
    loadBMP("textures/penas.bmp",    texAve);
    loadBMP("textures/madeira.bmp",  texMadeira);
    loadBMP("textures/cenoura.bmp",  texCenoura);
    loadBMP("textures/alface.bmp",   texAlface);
    loadBMP("textures/rabanete.bmp", texRabanete);
}

// Esfera com coordenadas de textura equiretangulares (UV)
// Substitui glutSolidSphere para objetos que recebem textura
void drawTexturedSphere(float radius, int slices, int stacks) {
    for (int i = 0; i < stacks; i++) {
        float lat0 = (float)M_PI * (-0.5f + (float)i / stacks);
        float z0 = std::sin(lat0), zr0 = std::cos(lat0);

        float lat1 = (float)M_PI * (-0.5f + (float)(i + 1) / stacks);
        float z1 = std::sin(lat1), zr1 = std::cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; j++) {
            float lng = 2.0f * (float)M_PI * (float)j / slices;
            float x = std::cos(lng), y = std::sin(lng);
            float u = (float)j / slices;

            glNormal3f(x * zr0, y * zr0, z0);
            glTexCoord2f(u, (float)i / stacks);
            glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);

            glNormal3f(x * zr1, y * zr1, z1);
            glTexCoord2f(u, (float)(i + 1) / stacks);
            glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
        }
        glEnd();
    }
}

// Cone com coordenadas de textura UV ao longo do corpo
// Substitui glutSolidCone para objetos que recebem textura
void drawTexturedCone(float radius, float height, int slices, int stacks) {
    for (int i = 0; i < stacks; i++) {
        float t0 = (float)i / stacks;
        float t1 = (float)(i + 1) / stacks;
        float r0 = radius * (1.0f - t0);
        float r1 = radius * (1.0f - t1);
        float z0 = height * t0;
        float z1 = height * t1;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; j++) {
            float u = (float)j / slices;
            float ang = 2.0f * (float)M_PI * u;
            float x = std::cos(ang);
            float y = std::sin(ang);

            // Vetor normal a superficie conica
            float nz = radius / height;
            glNormal3f(x, y, nz);

            glTexCoord2f(u, t0);
            glVertex3f(r0 * x, r0 * y, z0);

            glTexCoord2f(u, t1);
            glVertex3f(r1 * x, r1 * y, z1);
        }
        glEnd();
    }

    // Tampa circular da base em z = 0
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        for (int j = 0; j <= slices; j++) {
            float u = (float)j / slices;
            float ang = 2.0f * (float)M_PI * u;
            float x = std::cos(ang);
            float y = std::sin(ang);
            glTexCoord2f(0.5f + 0.5f * x, 0.5f + 0.5f * y);
            glVertex3f(radius * x, radius * y, 0.0f);
        }
    glEnd();
}

// Cubo com coordenadas de textura por face
void drawTexturedCube(float size) {
    float s = size / 2.0f;

    glBegin(GL_QUADS);
        // Frente (+Z)
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0); glVertex3f(-s, -s,  s);
        glTexCoord2f(1, 0); glVertex3f( s, -s,  s);
        glTexCoord2f(1, 1); glVertex3f( s,  s,  s);
        glTexCoord2f(0, 1); glVertex3f(-s,  s,  s);

        // Tras (-Z)
        glNormal3f(0, 0, -1);
        glTexCoord2f(1, 0); glVertex3f(-s, -s, -s);
        glTexCoord2f(1, 1); glVertex3f(-s,  s, -s);
        glTexCoord2f(0, 1); glVertex3f( s,  s, -s);
        glTexCoord2f(0, 0); glVertex3f( s, -s, -s);

        // Esquerda (-X)
        glNormal3f(-1, 0, 0);
        glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
        glTexCoord2f(1, 0); glVertex3f(-s, -s,  s);
        glTexCoord2f(1, 1); glVertex3f(-s,  s,  s);
        glTexCoord2f(0, 1); glVertex3f(-s,  s, -s);

        // Direita (+X)
        glNormal3f(1, 0, 0);
        glTexCoord2f(1, 0); glVertex3f( s, -s, -s);
        glTexCoord2f(1, 1); glVertex3f( s,  s, -s);
        glTexCoord2f(0, 1); glVertex3f( s,  s,  s);
        glTexCoord2f(0, 0); glVertex3f( s, -s,  s);

        // Topo (+Y)
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 1); glVertex3f(-s,  s, -s);
        glTexCoord2f(0, 0); glVertex3f(-s,  s,  s);
        glTexCoord2f(1, 0); glVertex3f( s,  s,  s);
        glTexCoord2f(1, 1); glVertex3f( s,  s, -s);

        // Base (-Y)
        glNormal3f(0, -1, 0);
        glTexCoord2f(1, 1); glVertex3f(-s, -s, -s);
        glTexCoord2f(0, 1); glVertex3f( s, -s, -s);
        glTexCoord2f(0, 0); glVertex3f( s, -s,  s);
        glTexCoord2f(1, 0); glVertex3f(-s, -s,  s);
    glEnd();
}
