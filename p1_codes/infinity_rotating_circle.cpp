// Run these commands in terminal to execute this code in Linux:
// 1. g++ -o infinity_rotating_circle infinity_rotating_circle.cpp -lglut -lGLU -lGL -lm
// 2. ./infinity_rotating_circle

// Run these commands in terminal to execute this code in Windows:
// 1. g++ infinity_rotating_circle.cpp -o infinity_rotating_circle -lfreeglut -lglu32 -lopengl32
// 2. .\infinity_rotating_circle


//////////////////////////////////////////////////////////////////////////////////////////////
// ANOTAÇÕES:
// - O círculo percorre um caminho no formato do símbolo do infinito (∞), usando
//   a fórmula da "Lemniscata de Gerono":
//       x(t) = A * cos(t)
//       y(t) = A * sin(t) * cos(t)
//   Enquanto t vai de 0 a 2*PI, esse par de equações desenha naturalmente um "8"
//   deitado -> não precisa de nenhuma lógica extra de colisão/troca de direção,
//   a própria matemática já faz o círculo cruzar o centro e trocar de laço sozinho.
// - orbitSpeed controla a velocidade com que "t" avança (e, portanto, a velocidade
//   do círculo no caminho). orbitSize controla o tamanho do "8" (a amplitude A).
// - Desenhei também o caminho do "8" como referência visual (linha cinza fina),
//   só pra ficar mais fácil de ver a trajetória - é totalmente opcional/decorativo.
//////////////////////////////////////////////////////////////////////////////////////////////


#include <GL/glut.h>
#include <cmath>

double PI = 3.14;

int FrameNumber = 0;
int msecs = 24; // intervalo do timer da animação (em milissegundos)


void init(void);
void display(void);


void init(void)
{
    // define a cor de background da janela
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // define o sistema de visualização - tipo de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-8, 8, -8, 8, -8, 8);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Cria um disco (mesmo padrão usado nos códigos anteriores)
void drawDisk(double radius)
{
    int d;
    glBegin(GL_POLYGON);
    for (d = 0; d < 32; d++) {
        double angle = (2 * PI / 32) * d;
        glVertex3f(radius * cos(angle), radius * sin(angle), 0);
    }
    glEnd();
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Variaveis que controlam a órbita em formato de infinito

float orbitT = 0.0f;        // parâmetro "t" da curva, avança a cada frame
float orbitSpeed = 0.03f;   // velocidade com que t avança (controla a velocidade do círculo)
float orbitSize = 5.0f;     // amplitude do "8" (controla o tamanho do caminho)
float circleRadius = 0.4f;  // tamanho do círculo que percorre o caminho


// Desenha o caminho do "8" como referência visual (opcional, só decorativo)
void drawInfinityPath()
{
    glColor3f(0.8f, 0.8f, 0.8f); // cinza claro, discreto
    glLineWidth(1.5f);

    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 200; i++) {
            float t = (2 * PI / 200) * i;
            float x = orbitSize * cos(t);
            float y = orbitSize * sin(t) * cos(t);
            glVertex2f(x, y);
        }
    glEnd();
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Função que controla a animação
void anim(int valor)
{
    orbitT += orbitSpeed;

    // opcional: mantém orbitT sempre numa faixa pequena, sem crescer pra sempre
    if (orbitT > 2 * PI) {
        orbitT -= 2 * PI;
    }

    FrameNumber++;
    glutPostRedisplay();
    glutTimerFunc(msecs, anim, valor);
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    //////////////////////////////////////////////////////////////////////////////////////////
    // Desenha o caminho de referência (o "8" completo, parado)
    drawInfinityPath();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Calcula a posição atual do círculo em cima da Lemniscata de Gerono
    float x = orbitSize * cos(orbitT);
    float y = orbitSize * sin(orbitT) * cos(orbitT);

    // Desenha o círculo na posição calculada
    glColor3f(0.9f, 0.1f, 0.1f);
    glPushMatrix();
        glTranslatef(x, y, 1);
        drawDisk(circleRadius);
    glPopMatrix();

    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Circulo em orbita infinita");

    init();

    glutDisplayFunc(display);

    // Controla a animação
    glutTimerFunc(msecs, anim, 0);

    glutMainLoop();

    return 0;
}
