// Prototipo 3D - Etapa 2: coelho (esfera) no campo + vegetais de bonificacao (power-ups)
//
// Compila-se APENAS o main.cpp (unity build, como no jogo 2D):
//
// Linux:
//   g++ -o main main.cpp -lglut -lGLU -lGL -lm
//   ./main
//
// Windows (terminal MSYS2 UCRT64):
//   g++ main.cpp -o main -lfreeglut -lglu32 -lopengl32
//   ./main.exe
//
// Controles: setas = mover no plano XZ | ESPACO = pular | ESC = sair
//            V = (TESTE) tira uma vida, para poder testar a alface

#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "Coelho3D.hpp"
#include "Cenario3D.hpp"
#include "Vegetais3D.hpp"
#include "SistemaColisao3D.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Unity build (igual ao main.cpp do jogo 2D): os arquivos de implementacao sao
incluidos diretamente aqui, formando uma unica unidade de compilacao. Por
isso basta compilar o main.cpp; NAO passe os outros .cpp ao g++, senao
havera erro de "multiple definition".*/
#include "Cenario3D.cpp"
#include "Coelho3D.cpp"
#include "Vegetais3D.cpp"
#include "SistemaColisao3D.cpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int FrameNumber = 0;
int msecs = 24; // Intervalo (ms) entre cada chamada de anim(), o mesmo do jogo 2D

int janelaLargura = 1000;
int janelaAltura = 700;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Camera fixa, elevada e olhando para o centro do campo (enquadra o quadrilatero inteiro)
const float CAMERA_X = 0.0f;
const float CAMERA_Y = 11.0f;
const float CAMERA_Z = 12.0f;
const double CAMERA_FOV = 55.0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Chamada quando uma tecla de seta e pressionada
void arrowKeysDown(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) rightArrowPressed = true;
    if (key == GLUT_KEY_LEFT)  leftArrowPressed = true;
    if (key == GLUT_KEY_UP)    upArrowPressed = true;
    if (key == GLUT_KEY_DOWN)  downArrowPressed = true;
}

// Chamada quando uma tecla de seta e solta
void arrowKeysUp(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) rightArrowPressed = false;
    if (key == GLUT_KEY_LEFT)  leftArrowPressed = false;
    if (key == GLUT_KEY_UP)    upArrowPressed = false;
    if (key == GLUT_KEY_DOWN)  downArrowPressed = false;
}

// Teclas normais
void keyboard_callback(unsigned char key, int x, int y) {
    if (key == 27) {          // ESC
        exit(0);
    }
    if (key == ' ' && !coelhoEscondido) {
        iniciarPulo();        // Barra de espaco: pula (no 2D era a seta para cima)
    }
    if ((key == 'v' || key == 'V') && rabbitLives > 0) {
        rabbitLives--;        // TESTE: sem raposa/ave ainda nao ha como perder vida; remover quando os inimigos entrarem
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Loop de atualizacao logica (igual ao do jogo 2D): o GLUT chama anim() a
cada "msecs" ms e ela se reagenda no final com glutTimerFunc.

Como no 2D, enquanto o coelho esta escondido o cenario e os vegetais ficam
congelados. (Ainda nao existe toca no 3D, entao coelhoEscondido e sempre false.)*/
void anim(int valor) {
    if (!coelhoEscondido) {
        moverCoelho();
        atualizarPulo();

        // Rolagem do fundo, surgimento/movimento/captura dos vegetais e contagem dos bonus
        rolarCenario();
        controlarSurgimentoDeVegetais();
        moverVegetais();
        verificarColisaoComVegetais(coelhoX, coelhoY, coelhoZ, coelhoEscondido, vegetais);
        atualizarBonusAtivos();
    }

    FrameNumber++;
    glutPostRedisplay();
    glutTimerFunc(msecs, anim, valor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Usa o GLUT bitmap font para desenhar caractere por caractere a partir da posicao (x, y) em pixels
void drawText(float x, float y, const char *texto) {
    glRasterPos2f(x, y);
    for (const char *c = texto; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

/*HUD (mesmos avisos do 2D): vidas e status dos bonus. Texto e feito numa
projecao 2D (em pixels) sobreposta a cena 3D, sem iluminacao nem z-buffer.*/
void drawHUD() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, janelaLargura, 0, janelaAltura);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0.0f, 0.0f, 0.0f);
    char textoVidas[32];
    snprintf(textoVidas, sizeof(textoVidas), "Vidas: %d", rabbitLives);
    drawText(20, janelaAltura - 30, textoVidas);

    // Avisos de bonus ativos: so aparecem enquanto os timers estiverem > 0
    if (framesDeTurboRestantes > 0) {
        drawText(20, janelaAltura - 58, "Turbo de velocidade!");
    }
    if (framesDePuloReforcadoRestantes > 0) {
        drawText(20, janelaAltura - 86, "Pulo reforcado!");
    }

    drawText(20, 20, "Setas: mover | Espaco: pular | ESC: sair");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void display() {
    // Alem da cor, agora limpamos o z-buffer (necessario para o 3D)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(CAMERA_X, CAMERA_Y, CAMERA_Z,   // posicao da camera
              0.0, 0.0, 0.0,                  // ponto para onde olha (centro do campo)
              0.0, 1.0, 0.0);                 // "para cima" da camera

    // Luz direcional (w = 0) vinda de cima. Deve ser posicionada DEPOIS do gluLookAt para ficar fixa no mundo.
    GLfloat luzDirecao[] = { 0.4f, 1.0f, 0.6f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, luzDirecao);

    // Chao e sombras primeiro (as sombras sao semitransparentes e ficam sobre o gramado)
    drawCampo();
    drawSombrasVegetais();
    drawSombraCoelho();

    // Vegetais ativos (cada um em sua posicao, girando)
    drawVegetais();

    // Coelho: primeiro posiciona (translate) e depois gira (rotate) em torno do eixo Y
    glPushMatrix();
        glTranslatef(coelhoX, coelhoY, coelhoZ);
        glRotatef(anguloCoelho, 0.0f, 1.0f, 0.0f);
        drawRabbit();
    glPopMatrix();

    drawHUD();

    glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Substitui o glOrtho do 2D por uma projecao em perspectiva, ajustada a proporcao da janela
void reshape(int w, int h) {
    if (h == 0) h = 1;
    janelaLargura = w;
    janelaAltura = h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(CAMERA_FOV, (double)w / (double)h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void init(void) {
    glClearColor(skyR, skyG, skyB, 1.0f); // ceu de "dia"

    glEnable(GL_DEPTH_TEST);  // z-buffer: objetos mais proximos escondem os mais distantes
    glShadeModel(GL_SMOOTH);

    // Iluminacao: sem luz a esfera e os vegetais pareceriam chapados
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);      // necessario porque escalamos as esferas para formar as folhas
    glEnable(GL_COLOR_MATERIAL); // glColor passa a definir a cor do material (ambiente + difusa)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat luzAmbiente[]  = { 0.35f, 0.35f, 0.35f, 1.0f };
    GLfloat luzDifusa[]    = { 0.85f, 0.85f, 0.85f, 1.0f };
    GLfloat luzEspecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT,  luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);

    // Transparencia (usada nas sombras), igual ao init do jogo 2D
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(NULL))); // Semente aleatoria: cada execucao tem spawns diferentes

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // + GLUT_DEPTH: buffer de profundidade
    glutInitWindowSize(janelaLargura, janelaAltura);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Coelho 3D - Etapa 2 (setas: mover | espaco: pular | ESC: sair)");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(msecs, anim, 0);

    glutKeyboardFunc(keyboard_callback);
    glutSpecialFunc(arrowKeysDown);
    glutSpecialUpFunc(arrowKeysUp);

    glutMainLoop();
    return 0;
}
