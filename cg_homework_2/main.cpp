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
#include "Raposa3D.hpp"
#include "AveRapina3D.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Unity build (igual ao main.cpp do jogo 2D): os arquivos de implementacao sao
incluidos diretamente aqui, formando uma unica unidade de compilacao. Por
isso basta compilar o main.cpp; NAO passe os outros .cpp ao g++, senao
havera erro de "multiple definition".*/
#include "Cenario3D.cpp"
#include "Coelho3D.cpp"
#include "Vegetais3D.cpp"
#include "SistemaColisao3D.cpp"
#include "Raposa3D.cpp"
#include "AveRapina3D.cpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int FrameNumber = 0;
int msecs = 24; // Intervalo (ms) entre cada chamada de anim(), o mesmo do jogo 2D

int janelaLargura = 1000;
int janelaAltura = 700;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CONFIGURACOES DE CAMERA E VISUALIZACAO (ITENS 4 E 5 DO TRABALHO)
//
// O enunciado exige a alternancia entre dois modos de visualizacao via tecla 'C' (Item 4):
//   1) VISAO GERAL DA CENA: posicionada no ponto (0, 10, 10), olhando para a origem (0, 0, 0),
//      permitindo uma visao panoramica elevada de todo o campo de jogo.
//   2) VISAO EM PRIMEIRA PESSOA: posicionada exatamente nos olhos do coelho,
//      orientada na mesma direcao (angulo yaw) para onde o personagem esta olhando.
//
// Alem disso, o Item 5 exige a rotacao da camera na visao geral:
//   "Quando a visao geral estiver acionada, permita que, ao pressionar a tecla 'r', a posicao
//    da camera seja rotacionada em relacao ao eixo Y, possibilitando observar a cena a partir
//    de diferentes angulos."
//
// Parametros geometricos da orbita da camera geral em torno do eixo Y:
// A posicao original (0, 10, 10) possui raio R = sqrt(0^2 + 10^2) = 10.0 no plano XZ e altura Y = 10.0.
const float RAIO_CAMERA_GERAL   = 10.0f; // Distancia do eixo Y no plano XZ
const float ALTURA_CAMERA_GERAL = 10.0f; // Altura constante Y da camera geral
const float PASSO_ROTACAO_CAM   = 5.0f;  // Incremento angular por pressao da tecla 'r' (em graus)
const double CAMERA_FOV         = 55.0;  // Campo de visao vertical (FOV)

// Angulo de rotacao da camera geral em torno do eixo Y (em graus).
// Em 0 graus: posicao = (0, 10, 10), exatamente como estabelecido no Item 4.
float anguloCameraGeral = 0.0f;

// Estado do modo de camera:
// false = Visao Geral da Cena (panoramica orbital)
// true  = Visao em Primeira Pessoa (olhos do coelho)
bool cameraPrimeiraPessoa = false;

// ITEM 8 (BONUS): INCLINACAO VERTICAL DA CAMERA EM 1ª PESSOA (PITCH)
// Permite que o jogador olhe para cima (e para baixo) enquanto esta na visao em primeira pessoa.
//  0 graus = olhar horizontalmente nivelado;
// >0 graus = olhar para cima em direcao ao ceu (onde voa a ave de rapina);
// <0 graus = olhar para baixo em direcao ao solo.
float pitchOlharPrimeiraPessoa = 0.0f;
const float PITCH_PASSO         = 5.0f;   // Variacao angular por toque de tecla (em graus)
const float PITCH_MIN           = -30.0f; // Limite inferior (olhar para o chao)
const float PITCH_MAX           = 85.0f;  // Limite superior (olhar para o ceu, quase zenite)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Chamada quando uma tecla de seta e pressionada
void arrowKeysDown(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT)     rightArrowPressed = true;
    if (key == GLUT_KEY_LEFT)      leftArrowPressed = true;
    if (key == GLUT_KEY_UP)        upArrowPressed = true;
    if (key == GLUT_KEY_DOWN)      downArrowPressed = true;
    if (key == GLUT_KEY_PAGE_UP) {
        // Item 8: Olhar para cima na visao em 1ª pessoa
        if (cameraPrimeiraPessoa) {
            pitchOlharPrimeiraPessoa += PITCH_PASSO;
            if (pitchOlharPrimeiraPessoa > PITCH_MAX) pitchOlharPrimeiraPessoa = PITCH_MAX;
        }
    }
    if (key == GLUT_KEY_PAGE_DOWN) {
        // Item 8: Olhar para baixo na visao em 1ª pessoa
        if (cameraPrimeiraPessoa) {
            pitchOlharPrimeiraPessoa -= PITCH_PASSO;
            if (pitchOlharPrimeiraPessoa < PITCH_MIN) pitchOlharPrimeiraPessoa = PITCH_MIN;
        }
    }
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
        rabbitLives--;        // TESTE: tira vida para testar a alface
    }
    if (key == 'f' || key == 'F') {
        spawnRaposa();        // TESTE: dispara a passagem da raposa imediatamente
    }
    if (key == 'a' || key == 'A') {
        spawnAve(coelhoX, coelhoZ); // TESTE: dispara o mergulho da ave mirando no coelho
    }
    if (key == 'c' || key == 'C') {
        // Alterna entre a Visao Geral da Cena e a Visao em Primeira Pessoa
        cameraPrimeiraPessoa = !cameraPrimeiraPessoa;
    }
    if (key == 'r' || key == 'R') {
        // Item 5: Quando a visao geral estiver acionada, pressionar 'r' rotaciona a
        // posicao da camera em relacao ao eixo Y, permitindo observar a cena de novos angulos
        if (!cameraPrimeiraPessoa) {
            anguloCameraGeral += PASSO_ROTACAO_CAM;
            if (anguloCameraGeral >= 360.0f) {
                anguloCameraGeral -= 360.0f;
            }
        }
    }
    if (key == 'w' || key == 'W') {
        // Item 8: Olhar para cima na visao em 1ª pessoa
        if (cameraPrimeiraPessoa) {
            pitchOlharPrimeiraPessoa += PITCH_PASSO;
            if (pitchOlharPrimeiraPessoa > PITCH_MAX) pitchOlharPrimeiraPessoa = PITCH_MAX;
        }
    }
    if (key == 's' || key == 'S') {
        // Item 8: Olhar para baixo na visao em 1ª pessoa
        if (cameraPrimeiraPessoa) {
            pitchOlharPrimeiraPessoa -= PITCH_PASSO;
            if (pitchOlharPrimeiraPessoa < PITCH_MIN) pitchOlharPrimeiraPessoa = PITCH_MIN;
        }
    }
    if (key == 'x' || key == 'X') {
        // Retorna o olhar vertical para a linha horizontal (0 graus)
        if (cameraPrimeiraPessoa) {
            pitchOlharPrimeiraPessoa = 0.0f;
        }
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

        // Raposa (predador terrestre): surgimento aleatorio, corrida no campo e colisao
        controlarSurgimentoDaRaposa();
        moverRaposa();
        verificarColisaoComRaposa();

        // Ave de Rapina (predador aereo): surgimento aleatorio, mergulho parabolico e colisao
        controlarSurgimentoDaAve(coelhoX, coelhoZ);
        moverAve();
        verificarColisaoComAve();
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

    // Indicador do modo atual de camera no topo direito da tela
    char textoCamera[80];
    if (cameraPrimeiraPessoa) {
        snprintf(textoCamera, sizeof(textoCamera), "Camera: 1a Pessoa (Olhar: %+.0f deg) [C] [W/S: olhar]", pitchOlharPrimeiraPessoa);
        drawText(janelaLargura - 450, janelaAltura - 30, textoCamera);
    } else {
        snprintf(textoCamera, sizeof(textoCamera), "Camera: Geral (%.0f deg) [C] [R: girar]", anguloCameraGeral);
        drawText(janelaLargura - 330, janelaAltura - 30, textoCamera);
    }

    if (cameraPrimeiraPessoa) {
        drawText(20, 20, "Setas: mover/girar | W/S ou PgUp/PgDn: olhar cima/baixo | X: nivelar | Espaco: pular | C: alternar camera");
    } else {
        drawText(20, 20, "Setas: mover | Espaco: pular | C: alternar camera | R: girar visao geral | F: raposa | A: ave | ESC: sair");
    }

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

    // -------------------------------------------------------------------------
    // SISTEMA DE CAMERA DINAMICA (gluLookAt) - ITENS 4, 5 E 8 DO ENUNCIADO
    // -------------------------------------------------------------------------
    if (!cameraPrimeiraPessoa) {
        // MODO 1: VISAO GERAL DA CENA (ITENS 4 E 5)
        // Posicionada em orbita circular ao redor do eixo Y, com altura constante Y = 10.0
        // e raio R = 10.0 no plano XZ (partindo exatamente de (0, 10, 10) para angulo = 0 graus).
        // Conforme a tecla 'r' e pressionada, a camera rotaciona em torno de Y:
        //   X = R * sin(angulo)
        //   Z = R * cos(angulo)
        // O ponto de mira e sempre o centro do campo (0, 0, 0) com vetor para cima (0, 1, 0).
        float radGeral = anguloCameraGeral * PI_F / 180.0f;
        float camGeralX = RAIO_CAMERA_GERAL * std::sin(radGeral);
        float camGeralY = ALTURA_CAMERA_GERAL;
        float camGeralZ = RAIO_CAMERA_GERAL * std::cos(radGeral);

        gluLookAt(camGeralX, camGeralY, camGeralZ, // posicao orbital da camera em torno de Y
                  0.0, 0.0, 0.0,                  // ponto de foco (origem/centro da cena)
                  0.0, 1.0, 0.0);                 // vetor "para cima" da camera
    } else {
        // MODO 2: VISAO EM PRIMEIRA PESSOA (ITENS 4 E 8 DO ENUNCIADO)
        // Posicionada na altura dos olhos do coelho e orientada no espaco 3D.
        // O angulo horizontal (yaw) e dado por anguloCoelho (0 graus = olhando para +X).
        // A inclinacao vertical (pitch) e dada por pitchOlharPrimeiraPessoa (Item 8: olhar para cima/baixo).
        float yawRad   = anguloCoelho * PI_F / 180.0f;
        float pitchRad = pitchOlharPrimeiraPessoa * PI_F / 180.0f;

        // Vetor unitario tridimensional de visada (gaze direction vector):
        //   dX = cos(pitch) * cos(yaw)
        //   dY = sin(pitch)
        //   dZ = cos(pitch) * (-sin(yaw))
        // Possui norma 1.0 para qualquer combinacao de yaw e pitch:
        //   ||d||^2 = cos^2(pitch)*cos^2(yaw) + sin^2(pitch) + cos^2(pitch)*sin^2(yaw) = 1.0
        float cosPitch  = std::cos(pitchRad);
        float dirOlharX = cosPitch * std::cos(yawRad);
        float dirOlharY = std::sin(pitchRad);
        float dirOlharZ = cosPitch * (-std::sin(yawRad));

        // Posicao dos olhos do coelho no mundo (Eye Position):
        // O corpo esta centrado em (coelhoX, coelhoY, coelhoZ). A cabeca fica a uma altura relativa
        // de +0.26 acima do centro e projetada +0.35 para a frente na orientacao horizontal do coelho:
        float dirCabecaX = std::cos(yawRad);
        float dirCabecaZ = -std::sin(yawRad);
        float camOlhoX   = coelhoX + 0.35f * dirCabecaX;
        float camOlhoY   = coelhoY + 0.26f;
        float camOlhoZ   = coelhoZ + 0.35f * dirCabecaZ;

        // Ponto de mira (Target Position): projetado 5.0 unidades a frente no espaco 3D
        float alvoOlharX = camOlhoX + dirOlharX * 5.0f;
        float alvoOlharY = camOlhoY + dirOlharY * 5.0f;
        float alvoOlharZ = camOlhoZ + dirOlharZ * 5.0f;

        gluLookAt(camOlhoX, camOlhoY, camOlhoZ,        // posicao da camera nos olhos
                  alvoOlharX, alvoOlharY, alvoOlharZ,  // ponto de foco a frente (com elevacao vertical)
                  0.0, 1.0, 0.0);                      // vetor "para cima"
    }

    // Luz direcional (w = 0) vinda de cima. Deve ser posicionada DEPOIS do gluLookAt para ficar fixa no mundo.
    GLfloat luzDirecao[] = { 0.4f, 1.0f, 0.6f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, luzDirecao);

    // Chao e sombras primeiro (as sombras sao semitransparentes e ficam sobre o gramado)
    drawCampo();
    drawSombrasVegetais();
    if (!cameraPrimeiraPessoa) {
        drawSombraCoelho();
    }
    drawSombraRaposa();
    drawSombraAve();

    // Vegetais ativos (cada um em sua posicao, girando)
    drawVegetais();

    // Raposa ativa atravessando o campo
    if (foxActive) {
        glPushMatrix();
            glTranslatef(foxX, foxY, foxZ);
            glRotatef(foxDirecao, 0.0f, 1.0f, 0.0f);
            drawFox();
        glPopMatrix();
    }

    // Ave de Rapina ativa mergulhando dos ceus
    if (aveActive) {
        glPushMatrix();
            glTranslatef(aveX, aveY, aveZ);

            // A ave voa no sentido -X (yaw de 180 graus)
            glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

            // Inclinacao do mergulho (pitch): aponta o bico para baixo na descida e para cima na subida
            float distBase = 14.0f - aveAlvoX;
            float a = (7.5f - 0.55f) / (distBase * distBase + 0.0001f);
            float dx = aveX - aveAlvoX;
            float inclinacaoGraus = std::atan(2.0f * a * dx) * 180.0f / PI_F;
            glRotatef(-inclinacaoGraus, 0.0f, 0.0f, 1.0f);

            drawBird();
        glPopMatrix();
    }

    // Coelho: desenhado apenas no modo de visao geral.
    // Em primeira pessoa, a camera esta na posicao dos proprios olhos do personagem;
    // ocultar o modelo evita que a parte interna da cabeca obstrua o campo de visao do jogador.
    if (!cameraPrimeiraPessoa) {
        glPushMatrix();
            glTranslatef(coelhoX, coelhoY, coelhoZ);
            glRotatef(anguloCoelho, 0.0f, 1.0f, 0.0f);
            drawRabbit();
        glPopMatrix();
    }

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
    glutCreateWindow("Coelho 3D - Etapa 2 (setas: mover | espaco: pular | F: raposa | A: ave | ESC: sair)");

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
