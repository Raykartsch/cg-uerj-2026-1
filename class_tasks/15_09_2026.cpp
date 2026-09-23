// How to execute in Linux
// 1. g++ -o aula_15_09 aula_15_09.cpp -lglut -lGLU -lGL -lm
// 2. ./aula_15_09

// Run these commands in terminal to run this code in Windows:
// 1. g++ aula_15_09.cpp -o aula_15_09 -lfreeglut -lglu32 -lopengl32
// 2. .\aula_15_09

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>

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

void drawChair() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, MATERIAL_COLORS[15][0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, MATERIAL_COLORS[15][1]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, MATERIAL_COLORS[15][2]);
    glMaterialf(GL_FRONT, GL_SHININESS, SPECULAR_EXPONENTS[15]);
    
    // Pés da cadeira
    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(-2.0f, 0, 0);
        glScalef(0.5, 2, 1);
        glutSolidCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(2.0f, 0, 0);
        glScalef(0.5, 2, 1);
        glutSolidCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(-2.0f, 0, -2);
        glScalef(0.5, 2, 1);
        glutSolidCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(2.0f, 0, -2);
        glScalef(0.5, 2, 1);
        glutSolidCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    // Assento da cadeira
    glMaterialfv(GL_FRONT, GL_AMBIENT, MATERIAL_COLORS[16][0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, MATERIAL_COLORS[16][1]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, MATERIAL_COLORS[16][2]);
    glMaterialf(GL_FRONT, GL_SHININESS, SPECULAR_EXPONENTS[16]);
    
    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(0, 1, -1);
        glScalef(4, 0.5, 4);
        glutSolidCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    // Apoio da cadeira
    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(0, 3, -3);
        glRotatef(180, 1, 0, 1);
        glScalef(0.5, 4, 4);
        glutSolidCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();
}

void drawTable() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, MATERIAL_COLORS[15][0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, MATERIAL_COLORS[15][1]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, MATERIAL_COLORS[15][2]);
    glMaterialf(GL_FRONT, GL_SHININESS, SPECULAR_EXPONENTS[15]);

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(-1.4f, -1, 0.9);
        glScalef(0.1, 2, 0.5);
        glutSolidCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(1.4f, -1, 0.9);
        glScalef(0.1, 2, 0.5);
        glutSolidCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(-1.4f, -1, -0.9);
        glScalef(0.1, 2, 0.5);
        glutSolidCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(1.4f, -1, -0.9);
        glScalef(0.1, 2, 0.5);
        glutSolidCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();

    // Tampo da mesa
    glPushMatrix();
        glEnable(GL_LIGHT2);
        glTranslatef(0, 0, 0);
        glScalef(4, 0.1, 3);
        glutSolidCube(1);
        glDisable(GL_LIGHT2);
    glPopMatrix();
}

void drawPlate() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, MATERIAL_COLORS[6][0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, MATERIAL_COLORS[6][1]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, MATERIAL_COLORS[6][2]);
    glMaterialf(GL_FRONT, GL_SHININESS, SPECULAR_EXPONENTS[6]);

    glEnable(GL_LIGHT0);
    glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glScalef(1, 1, 0.3);
        glutSolidTorus(1, 4, 30, 30);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, -0.3, 0);
        glScalef(1, 0.1, 1);
        glutSolidSphere(4, 30, 30);
    glPopMatrix();
    glDisable(GL_LIGHT0);
}

void drawCup() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, MATERIAL_COLORS[11][0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, MATERIAL_COLORS[11][1]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, MATERIAL_COLORS[11][2]);
    glMaterialf(GL_FRONT, GL_SHININESS, SPECULAR_EXPONENTS[11]);

    glEnable(GL_LIGHT0);
    glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glScalef(1, 1.5, 5);
        glutSolidTorus(1, 4, 30, 30);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, -3.5, 0);
        glScalef(1, 0.1, 1);
        glutSolidSphere(4, 30, 30);
    glPopMatrix();
    glDisable(GL_LIGHT0);
}

void drawCarpet() {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MATERIAL_COLORS[13][0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MATERIAL_COLORS[13][1]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MATERIAL_COLORS[13][2]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, SPECULAR_EXPONENTS[13]);

    glEnable(GL_LIGHT0);
    glPushMatrix();
        glutSolidCube(1);
    glPopMatrix();
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