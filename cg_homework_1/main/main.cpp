// Run these commands in terminal to execute this code in Linux:
// 1. g++ -o code code.cpp -lglut -lGLU -lGL -lm
// 2. ./code


// Run these commands in terminal to execute this code in Windows:
// 1. g++ main.cpp -o main -lfreeglut -lglu32 -lopengl32
// 2. .\main


#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>


int FrameNumber = 0;
int speed = 50;
int msecs = 24;

bool r_key_pressed = false;
bool e_key_pressed = false;

#include "AveRapina.cpp"
#include "PrimitivasGeometricas.cpp"
#include "Borboleta.cpp"
#include "Cenario.cpp"
#include "Coelho.cpp"
#include "Raposa.cpp"
#include "SistemaColisao.cpp"


void arrowKeysDown(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
    	rightArrowPressed = true;
    }
    if (key == GLUT_KEY_LEFT) {
    	leftArrowPressed = true;
    }
    if (key == GLUT_KEY_UP && !isJumping) {
        upArrowPressed = true;
        isJumping = true;
        goingUp = true;
    }
    if (key == GLUT_KEY_DOWN) {
    	downArrowPressed = true;
    }
}

void arrowKeysUp(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
    	rightArrowPressed = false;
    }
    if (key == GLUT_KEY_LEFT)  {
		leftArrowPressed = false;
    }
    if (key == GLUT_KEY_UP)    {
		upArrowPressed = false;
    }
    if (key == GLUT_KEY_DOWN)  {
		downArrowPressed = false;
    }
}

void keyboard_callback(unsigned char key, int x, int y) {
    if (key == 101) {
    	e_key_pressed = true; // letra e
    }
    if (key == 114) {
    	r_key_pressed = true; // letra r
    }
}

void keyboard_up_callback(unsigned char key, int x, int y) {
    if (key == 101) {
    	e_key_pressed = false;
    }
    if (key == 114) {
    	r_key_pressed = false;
    }
}

void anim(int valor) {
	/*Verifica se o coelho esta entrando ou saindo de uma toca. Isso e feito
	antes de tudo, para que o restante da funcao ja saiba se o coelho
	esta escondido (e, portanto, nao deve se mover nem pular) neste frame.*/
    atualizarEsconderijoDoCoelho();

    /* Calcula a velocidade deste frame (normal, "correndo" ou "descansando
     na toca") antes de usa-la nos blocos das setas, logo abaixo. Chamamos
     sempre, mesmo com o coelho escondido, pois e ela quem recupera o
     folego dele enquanto esta na toca.*/
    atualizarCorrida();

    /*Enquanto o coelho estiver escondido na toca, ele fica parado: nao
    processa as setas de movimento nem a animacao de pulo.*/

    if (!coelhoEscondido) {

    	// inputs inseridos para rodar com a setinha esquerda do teclado
        if (leftArrowPressed) {

            direcaoCoelho = -1.0f; // vira o personagem pra esquerda

            // Permite o personagem andar pra esquerda pela cena
            if (characterPos < -8.0f) { // esse comando não deixa o quadrado sair da tela, pra mudar o limite, olhar o glOrtho, está definido para 8 agora!
            	characterPos -= 0;
            }
            else {
            	characterPos -= characterSpeed;
            }
        }

        // inputs inseridos para rodar com a setinha direita do teclado
        if (rightArrowPressed) {
            direcaoCoelho = 1.0f; // vira o personagem pra direita

            // Permite o personagem andar pra direita pela cena
            if (characterPos > 8.0f) {  // esse if não deixa o quadrado sair da tela, pra mudar o limite, olhar o glOrtho, está definido para 8 agora!
            	characterPos += 0;
            }
            else {
            	characterPos += characterSpeed;
            }
        }


        //////////////////////////////////////////////////////////////////////////////////////
        // Controla a animação de pulo do quadrado
        if (isJumping) {
            if (goingUp) {
                jump_height += speed_jump;
                if (jump_height >= jump_maximum_height) {
                    jump_height = jump_maximum_height;  // trava no topo
                    goingUp = false; // começa a fase de descida
                }
            } else {
                jump_height -= speed_jump;
                if (jump_height <= 0.5f) {
                    jump_height = 0.5f; // trava no chão certinho
                    isJumping = false; // pulo terminou, pode pular de novo
                }
            }
        }
    }


    //////////////////////////////////////////////////////////////////////////////////////
   	/*Controla a animacao de corrida do coelho (patas e orelhas)
   	Avanca sempre, independente de o coelho estar se movendo pela cena ou nao,
   	para que ele fique "sempre" animado.*/

    walkPhase += walkPhaseSpeed;
    if (walkPhase > 2 * PI) {
    	walkPhase -= 2 * PI; // mantém o valor sempre dentro de uma faixa, sem crescer pra sempre
    }



    //////////////////////////////////////////////////////////////////////////////////////
  	/*Mesmo com o coelho escondido na toca, ela
    continua correndo da esquerda pra direita normalmente (patas, orelhas,
    cauda, surgimento e movimento). E assim que o coelho consegue "esperar
    ela passar" escondido, sem ficar preso enquanto ela atravessa a tela.
    (a colisao em si e ignorada dentro de verificarColisaoComRaposa quando
    o coelho esta escondido, entao ele nao perde vida mesmo se ela passar por cima)*/
    foxWalkPhase += foxWalkPhaseSpeed;
    if (foxWalkPhase > 2 * PI) {
    	foxWalkPhase -= 2 * PI;
    }


    /* Determina a fase da cauda da raposa*/
    foxTailPhase += foxTailPhaseSpeed;
    if (foxTailPhase > 2 * PI) {
    	foxTailPhase -= 2 * PI;
    }

    foxTailTipPhase += foxTailTipPhaseSpeed;
    if (foxTailTipPhase > 2 * PI) {
    	foxTailTipPhase -= 2 * PI;
    }


    /////////////////////////////////////////////////////////////////////////////
    //Controla o surgimento, movimento e colisao da ave
    controlarSurgimentoDaRaposa();
    moverRaposa();
    verificarColisaoComRaposa(characterPos, jump_height, coelhoEscondido, rabbitLives,
                              foxX, foxY, foxActive, foxJaTirouVidaNestaPassagem);

    /////////////////////////////////////////////////////////////////////////////
    //Controlar o surgimento, movimento e colisao da ave
    controlarSurgimentoDaAve(characterPos);
	moverAve();
	verificarColisaoComAve(characterPos, jump_height, coelhoEscondido, rabbitLives,
						   aveX, aveY, aveActive, aveJaTirouVida);



	//////////////////////////////////////////////////////////////////////////////////////
	/*As borboletas sao um detalhe decorativo
	 sobrevoando os canteiros, entao nao faz sentido elas pararem no ar
	 junto com o resto do cenario.**/
    butterflyPhase += butterflyPhaseSpeed;
    if (butterflyPhase > 2 * PI) {
    	butterflyPhase -= 2 * PI;
    }

    //////////////////////////////////////////////////////////////////////////////////////
    //Configura as nuvens flutuando no ceu
    cloudPhase += cloudPhaseSpeed;
    if (cloudPhase > 2 * PI) {
    	cloudPhase -= 2 * PI;
    }


    //////////////////////////////////////////////////////////////////////////////////////
    //Chamada do ciclo de passagem do tempo: amanhecer/dia/entardecer/noite
    atualizarCorDoCeu();


    //////////////////////////////////////////////////////////////////////////////////////
	/*Esse condicional abaixo congela o coelho e outras funcionalidades enquanto
	 * ele esta escondido como tocas, surgimento de vegetais, animacao do cenarios, passagem do background
	 * no fundo, etc.*/
    if (!coelhoEscondido) {

    	 // Rolagem do fundo (chao, cerca, canteiros)
        bgPos -= bgSpeed;
        bgPos = fmod(bgPos, bgWidth);


        // Tocas
        atualizarTocas();

        /* Vegetais de bonificacao: surgimento, movimento, colisao e os
     	 bonus (turbo/pulo alto) que estiverem ativos*/
        controlarSurgimentoDeVegetais();
        moverVegetais();
        verificarColisaoComVegetais(characterPos, jump_height, coelhoEscondido, vegetais);
        atualizarBonusAtivos();
    }


    //======================================================================================================================================================================================================================================================
    // Comandos padrao da funcao anim
    FrameNumber++;
    glutPostRedisplay();
    glutTimerFunc(msecs, anim, valor);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Desenha o efeito do ceu no background
    glPushMatrix();
    	drawBackgroundSky();
    glPopMatrix();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Calcula a transicao entre o sol e a lua do cenario
    // Desenha o sol no background
    float alphaSol = 1.0f;
    float alphaLua = 0.0f;

	if (tempoDeDiaFase >= 0.25f && tempoDeDiaFase <= 0.35f) {
		// Entardecer: Sol desaparece, Lua aparece
		float t = (tempoDeDiaFase - 0.25f) / 0.10f; // t vai de 0.0 a 1.0
		alphaSol = 1.0f - t;
		alphaLua = t;
	} else if (tempoDeDiaFase > 0.35f && tempoDeDiaFase < 0.75f) {
		// Noite Pura
		alphaSol = 0.0f;
		alphaLua = 1.0f;
	} else if (tempoDeDiaFase >= 0.75f && tempoDeDiaFase <= 0.85f) {
		// Amanhecer: Lua desaparece, Sol aparece
		float t = (tempoDeDiaFase - 0.75f) / 0.10f; // t vai de 0.0 a 1.0
		alphaSol = t;
		alphaLua = 1.0f - t;
	}

	// Renderiza o Sol se ele estiver visível
	if (alphaSol > 0.0f) {
		glPushMatrix();
			glTranslatef(6.5f, 6.8f, 1);
			drawSun(alphaSol);
		glPopMatrix();
	}

	// Renderiza a Lua se ela estiver visível
	if (alphaLua > 0.0f) {
		glPushMatrix();
			glTranslatef(6.5f, 6.8f, 1);
			drawMoon(alphaLua);
		glPopMatrix();
	}

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Desenha cada toca (buraco) que estiver na tela
    for (const Toca &toca : tocas) {
        glPushMatrix();
            glTranslatef(toca.x, toca.y, 1.0f);
            drawToca();
        glPopMatrix();
    }


	glPushMatrix();
		  drawBackgroundGrass();
		  glTranslatef(bgPos, 0, 1);
		  drawBackgroundContent();
	glPopMatrix();

	// Desenha a raposa somente enquanto ela estiver perseguindo o coelho
    if (foxActive) {
        glPushMatrix();
            glTranslatef(foxX, foxY, 1.0f);
            glScalef(0.9f * foxDirecao, 0.8f, 1.0f);
            drawFox();
        glPopMatrix();
    }

    // Verifica se a ave esta ativa no cenario
	if (aveActive) {
		glPushMatrix();
			glTranslatef(aveX, aveY, 1.0f);
			drawBird();
		glPopMatrix();
	}


	// Desenha cada vegetal de bonificacao que estiver ativo na cena
    for (const Vegetal &veg : vegetais) {
        if (!veg.ativo) continue;
        glPushMatrix();
            glTranslatef(veg.x, veg.y, 1.0f);
            drawVegetable(veg.tipo);
        glPopMatrix();
    }

    glColor3f(0, 0, 0);

    // Esta e a parte que faz o coelho ficar "escondido" ou não no cenario
    if (coelhoEscondido) {
        const Toca &tocaAtual = tocas[tocaOndeEstaEscondido];
        glPushMatrix();
            glTranslatef(tocaAtual.x, tocaAtual.y + 0.18f, 1.0f);
            glScalef(0.4f * direcaoCoelho, 0.4f, 1.0f);
            glColor3f(0.96f, 0.93f, 0.89f);

            glPushMatrix();
                glRotatef(10.0f, 0, 0, 1);
                glScalef(0.125f, 0.6f, 1.0f);
                drawTriangle();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(0.5f, 0.0f, 0.0f);
                glRotatef(-10.0f, 0, 0, 1);
                glScalef(0.125f, 0.6f, 1.0f);
                drawTriangle();
            glPopMatrix();
        glPopMatrix();
    } else {
        glPushMatrix();
            glTranslatef(characterPos, jump_height, 1.0f);
            glScalef(0.4f * direcaoCoelho, 0.4f, 1.0f);
            drawRabbit();
        glPopMatrix();
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    char textoVidas[32];
    snprintf(textoVidas, sizeof(textoVidas), "Vidas: %d", rabbitLives);
    drawText(-7.7f, 7.2f, textoVidas);

    if (framesDeTurboRestantes > 0) {
    	drawText(-7.7f, 6.6f, "Turbo de velocidade!");
    }
    if (framesDePuloReforcadoRestantes > 0) {
    	drawText(-7.7f, 6.0f, "Pulo reforcado!");
    }
    if (foxActive) {
    	drawText(-7.7f, 5.4f, "Cuidado, a raposa esta te perseguindo!");
    }
    if (aveActive) {
    	drawText(-7.7f, 4.0f, "Gaviao atacando do alto!");
    }


    if (coelhoEscondido) {
        drawText(-7.7f, 4.8f, "Escondido na toca!");
    } else {
        for (const Toca &toca : tocas) {
            if (fabs(toca.x - characterPos) < RAIO_TOCA) {
                drawText(-7.7f, 4.8f, "Aperte a seta para baixo pra se esconder!");
                break;
            }
        }
    }


    // Gerencia a barra de estamina da HUD
    float folegoPorcentagem = folegoAtual / FOLEGO_MAXIMO;
    float larguraBarra = 1.6f;
    float alturaBarra = 0.12f;

    glPushMatrix();
        glTranslatef(-7.7f, 4.15f, 1.0f);

        // Parte cinza da barra de folego
        glColor3f(0.35f, 0.35f, 0.35f);
        glPushMatrix();
            glTranslatef(larguraBarra / 2.0f, 0.0f, 0.0f);
            glScalef(larguraBarra / 2.0f, alturaBarra + 0.05, 1.0f);
            drawSquare();
        glPopMatrix();


        // Parte verde da barra de folego
        glColor3f(0.30f, 0.75f, 0.35f);
        glPushMatrix();
            glTranslatef((larguraBarra * folegoPorcentagem) / 2.0f, 0.0f, 0.0f);
            glScalef((larguraBarra * folegoPorcentagem) / 2.0f, alturaBarra, 1.0f);
            drawSquare();
        glPopMatrix();
    glPopMatrix();

    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(-7.7f, 3.75f, "Folego (segure R para correr)");

    glutSwapBuffers();
}

// =========================================================

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-8, 8, -8, 8, -8, 8);

    // Habilita transparencia do openGL
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    initCenario();
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(NULL)));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Trabalho 1 - Computacao Grafica");

    init();

    glutDisplayFunc(display);
    glutTimerFunc(msecs, anim, 0);

    glutKeyboardFunc(keyboard_callback);
    glutKeyboardUpFunc(keyboard_up_callback);
    glutSpecialFunc(arrowKeysDown);
    glutSpecialUpFunc(arrowKeysUp);

    glutMainLoop();

    return 0;
}
