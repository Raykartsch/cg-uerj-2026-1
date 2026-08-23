// Run these commands in terminal to execute this code in Linux:
// 1. g++ -o draw_leg draw_leg.cpp -lglut -lGLU -lGL -lm
// 2. ./draw_leg

// Run these commands in terminal to execute this code in Windows:
// 1. g++ draw_leg.cpp -o draw_leg -lfreeglut -lglu32 -lopengl32
// 2. .\draw_leg


//////////////////////////////////////////////////////////////////////////////////////////////
// ANOTAÇÕES:
// - A perna começa reta, apontando pra baixo (posição em pé normal).
// - Segurando a seta DIREITA: o quadril gira a coxa pra CIMA E PRA DIREITA, e o joelho
//   dobra a canela continuando na mesma direção -> a perna "levanta e flexiona" pro
//   lado direito, parecido com o braço fazendo o gesto de bíceps (-\), só que na perna.
// - Segurando a seta ESQUERDA: a perna relaxa de volta até a posição original (reta).
// - Soltar qualquer uma das duas teclas simplesmente CONGELA a perna no ângulo atual
//   (sem retorno automático) -> o próprio usuário controla os dois sentidos do movimento.
// - A lógica é idêntica à usada no drawArm original: hierarquia de rotação
//   (quadril -> joelho), só que aplicada à perna.
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
// Formas básicas
void drawSquare()
{
    glBegin(GL_POLYGON);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(-1, 1, 0);
    glEnd();
}

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
// Variaveis usadas para animar a perna (mesmo padrão do drawArm original)

float hipAngle = 0.0f;   // gira a COXA em torno do quadril
float kneeAngle = 0.0f;  // gira a CANELA em torno do joelho

float hipTarget = 90.0f;   // coxa vai até ficar na horizontal (levantada pra frente/direita)
float kneeTarget = -45.0f; // canela dobra pra baixo a partir do joelho (sentido OPOSTO ao quadril)

float animSpeed = 3.0f; // velocidade da flexão/relaxamento (graus por frame)


// Desenha a perna que levanta/flexiona com o input do usuário
void drawLeg(){

    glPushMatrix();
        glTranslatef(0, 2, 0);            // posição do quadril na cena
        glRotatef(hipAngle, 0, 0, 1);     // gira a COXA inteira em torno do quadril (sem sinal negativo = levanta pra DIREITA)

        // Junta do quadril
        glColor3f(0, 0, 0);
        drawDisk(0.4);

        // Coxa, pendurada a partir do quadril
        glColor3f(0.3f, 0.3f, 0.6f);
        glPushMatrix();
            glTranslatef(0, -2.0f, 0);
            glScalef(0.5f, 2.0f, 1);
            drawSquare();
        glPopMatrix();

        // Anda até a ponta da coxa = posição do joelho
        glTranslatef(0, -4.0f, 0);
        glRotatef(kneeAngle, 0, 0, 1);   // gira a CANELA em torno do joelho (dobra por trás)

        // Junta do joelho
        glColor3f(0, 0, 0);
        drawDisk(0.35);

        // Canela, pendurada a partir do joelho
        glColor3f(0.3f, 0.3f, 0.6f);
        glPushMatrix();
            glTranslatef(0, -1.5f, 0);
            glScalef(0.5f, 1.5f, 1);
            drawSquare();
        glPopMatrix();

        glColor3f(0.0f, 0.0f, 0.0f);
        glPushMatrix();
			glTranslatef(0.5, -3.0f, 0);
			glScalef(1.0f, 0.5f, 1);
			drawSquare();
        glPopMatrix();
    glPopMatrix();
}


//////////////////////////////////////////////////////////////////////////////////////////////
// booleanos que controlam se a setinha do teclado está pressionada
bool rightArrowPressed = false;
bool leftArrowPressed = false;

void arrowKeysDown(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) { rightArrowPressed = true; }
    if (key == GLUT_KEY_LEFT)  { leftArrowPressed = true; }
}

void arrowKeysUp(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) { rightArrowPressed = false; }
    if (key == GLUT_KEY_LEFT)  { leftArrowPressed = false; }
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Função que controla a animação
void anim(int valor) {

    if (rightArrowPressed) {
        // segurando direita -> flexiona (levanta a perna) até o limite
        if (hipAngle < hipTarget) {
            hipAngle += animSpeed;
        }
        // kneeTarget é negativo -> o joelho vai DIMINUINDO (sentido oposto ao quadril)
        if (kneeAngle > kneeTarget) {
            kneeAngle -= animSpeed;
        }
    }

    if (leftArrowPressed) {
        // segurando esquerda -> relaxa (abaixa a perna) até a posição original
        if (hipAngle > 0) {
            hipAngle -= animSpeed;
        }
        // devolve o joelho de volta pra 0 (subindo, já que ele está negativo)
        if (kneeAngle < 0) {
            kneeAngle += animSpeed;
        }
    }
    // se nenhuma tecla estiver pressionada, os ângulos ficam parados onde estão
    // -> a perna congela na posição atual, sem retorno automático.

    //=========================================
    // Comandos padrão da função anim
    FrameNumber++;
    glutPostRedisplay();
    glutTimerFunc(msecs, anim, valor);
}


void display() {

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    //////////////////////////////////////////////////////////////////////////////////////////
    // Alterar a display daqui pra baixo

    glPushMatrix();
        drawLeg();
    glPopMatrix();

    // Alterar a display daqui pra cima
    //////////////////////////////////////////////////////////////////////////////////////////

    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Perna Levantando");

    init();

    glutDisplayFunc(display);

    // Controla a animação
    glutTimerFunc(msecs, anim, 0);

    // Controla o input por setinha do teclado
    glutSpecialFunc(arrowKeysDown);
    glutSpecialUpFunc(arrowKeysUp);

    glutMainLoop();

    return 0;
}
