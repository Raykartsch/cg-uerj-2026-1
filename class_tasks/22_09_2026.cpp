// How to execute in Linux
// 1. g++ -o 22_09_2026 22_09_2026.cpp -lglut -lGLU -lGL -lm
// 2. ./22_09_2026

// Run these commands in terminal to run this code in Windows:
// 1. g++ 22_09_2026.cpp -o 22_09_2026 -lfreeglut -lglu32 -lopengl32
// 2. .\22_09_2026
//
// IMPORTANTE: coloque a pasta "textures" (com madeira.bmp, metal.bmp,
// pano.bmp, tapete.bmp e papel_de_parede.bmp) na MESMA pasta de onde
// o .exe sera executado, ou seja, ao lado de 22_09_2026.cpp /
// 22_09_2026.exe. O programa procura os arquivos em
// "texturas/<nome>.bmp" (caminho relativo).

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Variáveis para controle da câmera e interação
GLfloat angle, fAspect, rotX, rotY;
GLdouble obsX, obsY, obsZ;

int BRASS = 0;
int BRONZE = 1;
int CHROME = 2;
int COPPER = 3;
int GOLD = 4;
int PEWTER = 5;
int SILVER = 6;
int JADE = 7;
int OBSIDIAN = 8;
int PEARL = 9;
int RUBY = 10;
int TURQUOISE = 11;
int BLACK_PLASTIC = 12;
int BLACK_RUBBER = 13;
int LAST_MATERIAL = 14;
int WOOD = 15;
int VELVET = 16;

int material = -1;

float SPECULAR_EXPONENTS[] =
{
    27.897400, // BRASS
    25.600000, // BRONZE
    76.800003, // CHROME
    12.800000, // COPPER
    51.200001, // GOLD
    09.846150, // PEWTER
    51.200001, // SILVER
    76.800003, // EMERALD
    12.800000, // JADE
    38.400002, // OBSIDIAN
    11.264000, // PEARL
    76.800003, // RUBY
    12.800000, // TURQUOISE
    32.000000, // BLACK_PLASTIC
    10.000000, // BLACK_RUBBER
    40.0,
    20.0
};

float MATERIAL_COLORS[][3][4] =
{
    // BRASS 0
    {
        {0.329412, 0.223529, 0.027451, 1.000000},
        {0.780392, 0.568627, 0.113725, 1.000000},
        {0.992157, 0.941176, 0.807843, 1.000000}
    },
    // BRONZE 1
    {
        {0.212500, 0.127500, 0.054000, 1.000000},
        {0.714000, 0.428400, 0.181440, 1.000000},
        {0.393548, 0.271906, 0.166721, 1.000000}
    },
    // CHROME 2
    {
        {0.250000, 0.250000, 0.250000, 1.000000},
        {0.400000, 0.400000, 0.400000, 1.000000},
        {0.774597, 0.774597, 0.774597, 1.000000}
    },
    // COPPER 3
    {
        {0.191250, 0.073500, 0.022500, 1.000000},
        {0.703800, 0.270480, 0.082800, 1.000000},
        {0.256777, 0.137622, 0.086014, 1.000000}
    },
    // GOLD 4
    {
        {0.247250, 0.199500, 0.074500, 1.000000},
        {0.751640, 0.606480, 0.226480, 1.000000},
        {0.628281, 0.555802, 0.366065, 1.000000}
    },
    // PEWTER 5
    {
        {0.105882, 0.058824, 0.113725, 1.000000},
        {0.427451, 0.470588, 0.541176, 1.000000},
        {0.333333, 0.333333, 0.521569, 1.000000}
    },
    // SILVER 6
    {
        {0.192250, 0.192250, 0.192250, 1.000000},
        {0.507540, 0.507540, 0.507540, 1.000000},
        {0.508273, 0.508273, 0.508273, 1.000000}
    },
    // EMERALD 7
    {
        {0.021500, 0.174500, 0.021500, 0.550000},
        {0.075680, 0.614240, 0.075680, 0.550000},
        {0.633000, 0.727811, 0.633000, 0.550000}
    },
    // JADE 8
    {
        {0.135000, 0.222500, 0.157500, 0.950000},
        {0.540000, 0.890000, 0.630000, 0.950000},
        {0.316228, 0.316228, 0.316228, 0.950000}
    },
    // OBSIDIAN 9
    {
        {0.053750, 0.050000, 0.066250, 0.820000},
        {0.182750, 0.170000, 0.225250, 0.820000},
        {0.332741, 0.328634, 0.346435, 0.820000}
    },
    // PEARL 10
    {
        {0.250000, 0.207250, 0.207250, 0.922000},
        {1.000000, 0.829000, 0.829000, 0.922000},
        {0.296648, 0.296648, 0.296648, 0.922000}
    },
    // RUBY 11
    {
        {0.174500, 0.011750, 0.011750, 0.550000},
        {0.614240, 0.041360, 0.041360, 0.550000},
        {0.727811, 0.626959, 0.626959, 0.550000}
    },
    // TURQUOISE 12
    {
        {0.100000, 0.187250, 0.174500, 0.800000},
        {0.396000, 0.741510, 0.691020, 0.800000},
        {0.297254, 0.308290, 0.306678, 0.800000}
    },
    // BLACK_PLASTIC 13
    {
        {0.000000, 0.000000, 0.000000, 1.000000},
        {0.010000, 0.010000, 0.010000, 1.000000},
        {0.500000, 0.500000, 0.500000, 1.000000}
    },
    // BLACK_RUBBER 14
    {
        {0.020000, 0.020000, 0.020000, 1.000000},
        {0.010000, 0.010000, 0.010000, 1.000000},
        {0.400000, 0.400000, 0.400000, 1.000000}
    },
    // WOOD 15
    {
        {101.0/255, 67.0/255, 33.0/255, 1},
        {101.0/255, 67.0/255, 33.0/255, 1},
        {101.0/255, 67.0/255, 33.0/255, 1}
    },
    // VELVET 16
    {
        {1.0/255, 50.0/255, 32.0/255, 1},
        {1.0/255, 50.0/255, 32.0/255, 1},
        {1.0/255, 50.0/255, 32.0/255, 1}
    }
};

// ============================================================
// TEXTURAS
// ============================================================
// IDs das texturas OpenGL, geradas em init() a partir dos arquivos
// BMP na pasta "texturas".
GLuint texMadeira = 0;
GLuint texMetal   = 0;
GLuint texPano    = 0;
GLuint texTapete  = 0; // chão
GLuint texParede  = 0; // paredes

float TEX_NEUTRAL_AMBIENT[]  = {0.55f, 0.55f, 0.55f, 1.0f};
float TEX_NEUTRAL_DIFFUSE[]  = {0.85f, 0.85f, 0.85f, 1.0f};
float TEX_NEUTRAL_SPECULAR[] = {0.15f, 0.15f, 0.15f, 1.0f};
float TEX_NEUTRAL_SHININESS  = 12.0f;

float TEX_METAL_AMBIENT[]  = {0.55f, 0.55f, 0.55f, 1.0f};
float TEX_METAL_DIFFUSE[]  = {0.85f, 0.85f, 0.85f, 1.0f};
float TEX_METAL_SPECULAR[] = {0.95f, 0.95f, 0.95f, 1.0f};
float TEX_METAL_SHININESS  = 90.0f;

// Aplica um conjunto de propriedades de material na face frontal.
void setMaterial(float* ambient, float* diffuse, float* specular, float shininess) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

// Carrega uma imagem BMP de 24 bits (sem compressão) e cria uma
// textura OpenGL a partir dela. Loader simples e sem dependências
// externas (baseado no formato clássico de BMP do Windows).
bool loadBMP(const char *imagepath, GLuint &textureID) {
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    unsigned char *data;

    FILE *file = fopen(imagepath, "rb");
    if (!file) {
        printf("AVISO: nao foi possivel abrir a textura \"%s\".\n", imagepath);
        printf("       Verifique se a pasta \"texturas\" esta ao lado do executavel.\n");
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

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    delete[] data;

    printf("Textura carregada: %s (%ux%u)\n", imagepath, width, height);
    return true;
}

void carregaTexturas() {
    loadBMP("textures/madeira.bmp",         texMadeira);
    loadBMP("textures/metal.bmp",           texMetal);
    loadBMP("textures/pano.bmp",            texPano);
    loadBMP("textures/tapete.bmp",          texTapete);
    loadBMP("textures/papel_de_parede.bmp", texParede);
}

// ============================================================
// PRIMITIVAS COM COORDENADAS DE TEXTURA
// ============================================================
// glutSolidCube / glutSolidSphere / glutSolidTorus não geram
// coordenadas de textura, então usamos versões próprias para poder
// mapear as imagens BMP nos objetos.

// Cubo unitário (mesmo tamanho de glutSolidCube(1)), com normais e
// coordenadas de textura por face.
void drawTexturedCube(float size = 1.0f) {
    float s = size / 2.0f;

    glBegin(GL_QUADS);
        // Frente (+Z)
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0); glVertex3f(-s, -s,  s);
        glTexCoord2f(1, 0); glVertex3f( s, -s,  s);
        glTexCoord2f(1, 1); glVertex3f( s,  s,  s);
        glTexCoord2f(0, 1); glVertex3f(-s,  s,  s);

        // Trás (-Z)
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

// Esfera com coordenadas de textura (mapeamento equiretangular),
// equivalente a glutSolidSphere(radius, slices, stacks).
void drawTexturedSphere(float radius, int slices, int stacks) {
    for (int i = 0; i < stacks; i++) {
        float lat0 = (float)M_PI * (-0.5f + (float)i / stacks);
        float z0 = sin(lat0), zr0 = cos(lat0);

        float lat1 = (float)M_PI * (-0.5f + (float)(i + 1) / stacks);
        float z1 = sin(lat1), zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; j++) {
            float lng = 2.0f * (float)M_PI * (float)j / slices;
            float x = cos(lng), y = sin(lng);
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


void drawTexturedTorus(float innerRadius, float outerRadius, int sides, int rings) {
    for (int i = 0; i < rings; i++) {
        float theta0 = 2.0f * (float)M_PI * i / rings;
        float theta1 = 2.0f * (float)M_PI * (i + 1) / rings;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= sides; j++) {
            float phi = 2.0f * (float)M_PI * j / sides;
            float cosPhi = cos(phi), sinPhi = sin(phi);
            float u = (float)j / sides;

            for (int k = 0; k < 2; k++) {
                float theta = (k == 0) ? theta0 : theta1;
                float cosTheta = cos(theta), sinTheta = sin(theta);

                float x = (outerRadius + innerRadius * cosPhi) * cosTheta;
                float y = (outerRadius + innerRadius * cosPhi) * sinTheta;
                float z = innerRadius * sinPhi;

                float nx = cosPhi * cosTheta;
                float ny = cosPhi * sinTheta;
                float nz = sinPhi;

                float v = (k == 0) ? (float)i / rings : (float)(i + 1) / rings;

                glNormal3f(nx, ny, nz);
                glTexCoord2f(u, v);
                glVertex3f(x, y, z);
            }
        }
        glEnd();
    }
}

void drawChair() {
    // Pés e apoio da cadeira: textura de madeira.
    setMaterial(TEX_NEUTRAL_AMBIENT, TEX_NEUTRAL_DIFFUSE, TEX_NEUTRAL_SPECULAR, TEX_NEUTRAL_SHININESS);
    glBindTexture(GL_TEXTURE_2D, texMadeira);

    // Pés da cadeira
    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(-2.0f, 0, 0);
        glScalef(0.5, 2, 1);
        drawTexturedCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(2.0f, 0, 0);
        glScalef(0.5, 2, 1);
        drawTexturedCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(-2.0f, 0, -2);
        glScalef(0.5, 2, 1);
        drawTexturedCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(2.0f, 0, -2);
        glScalef(0.5, 2, 1);
        drawTexturedCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    // Assento e encosto da cadeira: textura de tecido.
    setMaterial(TEX_NEUTRAL_AMBIENT, TEX_NEUTRAL_DIFFUSE, TEX_NEUTRAL_SPECULAR, TEX_NEUTRAL_SHININESS);
    glBindTexture(GL_TEXTURE_2D, texPano);

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(0, 1, -1);
        glScalef(4, 0.5, 4);
        drawTexturedCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    // Apoio da cadeira
    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(0, 3, -3);
        glRotatef(180, 1, 0, 1);
        glScalef(0.5, 4, 4);
        drawTexturedCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();
}

void drawTable() {
    // Pés e tampo da mesa: textura de madeira.
    setMaterial(TEX_NEUTRAL_AMBIENT, TEX_NEUTRAL_DIFFUSE, TEX_NEUTRAL_SPECULAR, TEX_NEUTRAL_SHININESS);
    glBindTexture(GL_TEXTURE_2D, texMadeira);

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(-1.4f, -1, 0.9);
        glScalef(0.1, 2, 0.5);
        drawTexturedCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(1.4f, -1, 0.9);
        glScalef(0.1, 2, 0.5);
        drawTexturedCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(-1.4f, -1, -0.9);
        glScalef(0.1, 2, 0.5);
        drawTexturedCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(1.4f, -1, -0.9);
        glScalef(0.1, 2, 0.5);
        drawTexturedCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    // Tampo da mesa
    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(0, 0, 0);
        glScalef(4, 0.1, 3);
        drawTexturedCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();
}

void drawPlate() {
    // Prato: textura de metal.
    setMaterial(TEX_METAL_AMBIENT, TEX_METAL_DIFFUSE, TEX_METAL_SPECULAR, TEX_METAL_SHININESS);
    glBindTexture(GL_TEXTURE_2D, texMetal);

    glEnable(GL_LIGHT0);
    glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glScalef(1, 1, 0.3);
        drawTexturedTorus(1, 4, 30, 30);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, -0.3, 0);
        glScalef(1, 0.1, 1);
        drawTexturedSphere(4, 30, 30);
    glPopMatrix();
    glDisable(GL_LIGHT0);
}

void drawCup() {
    // Copo: textura de metal.
    setMaterial(TEX_METAL_AMBIENT, TEX_METAL_DIFFUSE, TEX_METAL_SPECULAR, TEX_METAL_SHININESS);
    glBindTexture(GL_TEXTURE_2D, texMetal);

    glEnable(GL_LIGHT0);
    glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glScalef(1, 1.5, 5);
        drawTexturedTorus(1, 4, 30, 30);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, -3.5, 0);
        glScalef(1, 0.1, 1);
        drawTexturedSphere(4, 30, 30);
    glPopMatrix();
    glDisable(GL_LIGHT0);
}

void drawCarpet() {
    // Tapete: textura de tecido.
    setMaterial(TEX_NEUTRAL_AMBIENT, TEX_NEUTRAL_DIFFUSE, TEX_NEUTRAL_SPECULAR, TEX_NEUTRAL_SHININESS);
    glBindTexture(GL_TEXTURE_2D, texPano);

    glEnable(GL_LIGHT0);
    glPushMatrix();
        drawTexturedCube(1);
    glPopMatrix();
    glDisable(GL_LIGHT0);
}

// Chão da sala: um plano no eixo XZ (Y fixo), com a textura do
// tapete repetida algumas vezes para não esticar demais.
void drawFloor() {
    setMaterial(TEX_NEUTRAL_AMBIENT, TEX_NEUTRAL_DIFFUSE, TEX_NEUTRAL_SPECULAR, TEX_NEUTRAL_SHININESS);
    glBindTexture(GL_TEXTURE_2D, texTapete);

    float tam = 20.0f; // "raio" do chão
    float rep = 6.0f;  // repetições da textura

    glEnable(GL_LIGHT0);
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glTexCoord2f(0,   0);   glVertex3f(-tam, 0, -tam);
        glTexCoord2f(rep, 0);   glVertex3f( tam, 0, -tam);
        glTexCoord2f(rep, rep); glVertex3f( tam, 0,  tam);
        glTexCoord2f(0,   rep); glVertex3f(-tam, 0,  tam);
    glEnd();
    glDisable(GL_LIGHT0);
}

// Paredes da sala: um plano paralelo ao eixo YZ (parede lateral) e
// outro paralelo ao eixo XY (parede de fundo), formando um canto
// atrás/ao lado da mesa. Mesma textura de papel de parede nas duas.
void drawWalls() {
    setMaterial(TEX_NEUTRAL_AMBIENT, TEX_NEUTRAL_DIFFUSE, TEX_NEUTRAL_SPECULAR, TEX_NEUTRAL_SHININESS);
    glBindTexture(GL_TEXTURE_2D, texParede);

    float tam = 20.0f;
    float altura = 30.0f;
    float rep = 6.0f;

    glEnable(GL_LIGHT0);

    // Parede lateral (paralela ao plano YZ, X = -tam)
    glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);
        glTexCoord2f(0,   0);   glVertex3f(-tam, 0,      -tam);
        glTexCoord2f(rep, 0);   glVertex3f(-tam, 0,       tam);
        glTexCoord2f(rep, rep); glVertex3f(-tam, altura,  tam);
        glTexCoord2f(0,   rep); glVertex3f(-tam, altura, -tam);
    glEnd();

    // Parede de fundo (paralela ao plano XY, Z = -tam)
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f(0,   0);   glVertex3f(-tam, 0,      -tam);
        glTexCoord2f(rep, 0);   glVertex3f( tam, 0,      -tam);
        glTexCoord2f(rep, rep); glVertex3f( tam, altura, -tam);
        glTexCoord2f(0,   rep); glVertex3f(-tam, altura, -tam);
    glEnd();

    glDisable(GL_LIGHT0);
}

// Posiciona o observador com base nas teclas de interação
void PosicionaObservador(void) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Afasta a câmera pela distância obsZ e rotaciona pelos eixos X e Y
    glTranslatef(0, -5, -obsZ); // Leve ajuste no Y (-5) para centrar a mesa verticalmente
    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);
}

// Configura o volume de visualização (projeção perspectiva e câmera)
void EspecificaParametrosVisualizacao(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Projeção perspectiva usando o ângulo (zoom)
    gluPerspective(angle, fAspect, 0.5, 500);

    PosicionaObservador();
}

// Callback de renderização da cena
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Atualiza a posição da câmera a cada frame antes de desenhar
    PosicionaObservador();

    // Chão e paredes da sala
    drawFloor();
    drawWalls();

    // Mesa
    glPushMatrix();
        glTranslatef(0, 8, 2);
        glScalef(6, 4, 4);
        drawTable();
    glPopMatrix();

    // Cadeiras
    glPushMatrix();
        glTranslatef(-4, 2, -4);
        glScalef(1.5, 1.75, 1.5);
        drawChair();
    glPopMatrix();
   
    glPushMatrix();
        glTranslatef(4, 2, -4);
        glScalef(1.5, 1.75, 1.5);
        drawChair();
    glPopMatrix();

    glPushMatrix();
        glRotatef(180, 0, 1, 0);
        glTranslatef(4, 2, -8);
        glScalef(1.5, 1.75, 1.5);
        drawChair();
    glPopMatrix();

    glPushMatrix();
        glRotatef(180, 0, 1, 0);
        glTranslatef(-4, 2, -8);
        glScalef(1.5, 1.75, 1.5);
        drawChair();
    glPopMatrix();

    // Kit de pratos 1 (Fundo Direita - Cadeira em 4, 8)
    glPushMatrix();
        glTranslatef(4, 8.3, 5);
        glScalef(0.25, 0.25, 0.25);
        drawPlate();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(6, 8.8, 5);
        glScalef(0.05, 0.07, 0.05);
        drawCup();
    glPopMatrix();

    // Kit de pratos 2 (Fundo Esquerda - Cadeira em -4, 8)
    glPushMatrix();
        glTranslatef(-4, 8.3, 5);
        glScalef(0.25, 0.25, 0.25);
        drawPlate();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-2, 8.8, 5);
        glScalef(0.05, 0.07, 0.05);
        drawCup();
    glPopMatrix();

    // Kit de pratos 3 (Frente Esquerda - Cadeira em -4, -4)
    glPushMatrix();
        glTranslatef(-4, 8.3, -1);
        glScalef(0.25, 0.25, 0.25);
        drawPlate();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-2, 8.8, -1);
        glScalef(0.05, 0.07, 0.05);
        drawCup();
    glPopMatrix();

    // Kit de pratos 4 (Frente Direita - Cadeira em 4, -4)
    glPushMatrix();
        glTranslatef(4, 8.3, -1);
        glScalef(0.25, 0.25, 0.25);
        drawPlate();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(6, 8.8, -1);
        glScalef(0.05, 0.07, 0.05);
        drawCup();
    glPopMatrix();

    // Tapetes
    glPushMatrix();
        glTranslatef(4.0, 8.25, 5);
        glScalef(5.0, 0.1, 4.0);
        drawCarpet();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-4.0, 8.25, 5);
        glScalef(5.0, 0.1, 4.0);
        drawCarpet();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-4.0, 8.25, -1);
        glScalef(5.0, 0.1, 4.0);
        drawCarpet();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(4.0, 8.25, -1);
        glScalef(5.0, 0.1, 4.0);
        drawCarpet();
    glPopMatrix();

    glutSwapBuffers();
}

// Callback de redimensionamento da janela
void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;
    EspecificaParametrosVisualizacao();
}

// Callback para gerenciar teclas especiais (setas e Home/End)
void TeclasEspeciais(int tecla, int x, int y) {
    switch (tecla) {
        case GLUT_KEY_LEFT:  rotY--; break;
        case GLUT_KEY_RIGHT: rotY++; break;
        case GLUT_KEY_UP:    rotX++; break;
        case GLUT_KEY_DOWN:  rotX--; break;
        case GLUT_KEY_HOME:  obsZ++; break;
        case GLUT_KEY_END:   obsZ--; break;
    }
    PosicionaObservador();
    glutPostRedisplay();
}

// Callback para gerenciar cliques do mouse (Zoom-in e Zoom-out)
void GerenciaMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Zoom-in
        if (angle >= 10)
            angle -= 5;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // Zoom-out
        if (angle <= 130)
            angle += 5;
    }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

void init() {
    GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat cyan[] = { 0.0, 1.0, 1.0, 1.0 };
    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat direction1[] = { 0.0, 0.0, 10.0, 1.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cyan);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 60);

    glLightfv(GL_LIGHT1, GL_AMBIENT, black);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightfv(GL_LIGHT1, GL_POSITION, direction1);

    glLightfv(GL_LIGHT2, GL_AMBIENT, black);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, yellow);
    glLightfv(GL_LIGHT2, GL_SPECULAR, white);
    glLightfv(GL_LIGHT2, GL_POSITION, direction1);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Habilita o uso de texturas 2D e define o modo de combinação:
    // GL_MODULATE multiplica a cor do material/iluminação pela cor
    // do texel, preservando o efeito da iluminação sobre a textura.
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    carregaTexturas();
}

void Inicializa(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    angle = 50;
    rotX = 20;
    rotY = 0;
    obsZ = 30; // Distância ajustada à escala do cenário da mesa
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cena 3D - Mesa e Cadeiras");

    // Registra callbacks de interação e renderização
    glutDisplayFunc(display);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutSpecialFunc(TeclasEspeciais);
    glutMouseFunc(GerenciaMouse);

    init();
    Inicializa();

    glutMainLoop();
    return 0;
}
