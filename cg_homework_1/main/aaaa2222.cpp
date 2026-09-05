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

// ============================================================================
// FEATURE: Estado global do jogo
// FrameNumber conta quantos frames já se passaram desde o início (usado, por
// exemplo, para girar os raios do Sol). speed/msecs controlam o "relógio" do
// jogo: msecs é o intervalo (em milissegundos) entre cada chamada de anim(),
// definindo o framerate lógico do jogo.
// ============================================================================
int FrameNumber = 0;
int speed = 50;
int msecs = 24;

// Flags de estado das teclas 'R' (correr) e 'E' (provavelmente esconder/entrar na toca)
bool r_key_pressed = false;
bool e_key_pressed = false;

// ============================================================================
// FEATURE: Organização do projeto via includes de .cpp
// Em vez de compilar cada .cpp separadamente e linkar, este projeto inclui
// os arquivos de implementação diretamente no main.cpp (unity build /
// single translation unit). Isso faz com que todas as variáveis e funções
// globais desses arquivos (ex: rightArrowPressed, drawFox, tocas, etc.)
// fiquem visíveis aqui, o que é o que permite este arquivo usá-las livremente
// mais abaixo sem precisar de headers próprios para cada uma.
// ============================================================================
#include "AveRapina.cpp"          // Ave de rapina (gavião): surgimento, movimento e colisão
#include "PrimitivasGeometricas.cpp" // Funções básicas de desenho (quadrado, disco, triângulo, etc.)
#include "Borboleta.cpp"          // Borboletas decorativas
#include "Cenario.cpp"            // Cenário, céu, ciclo dia/noite, tocas e vegetais
#include "Coelho.cpp"             // Personagem principal (coelho): desenho, vidas, corrida
#include "Raposa.cpp"             // Raposa: inimigo que persegue o coelho
#include "SistemaColisao.cpp"     // Funções de detecção de colisão entre entidades

// ============================================================================
// FEATURE: Entrada do teclado — teclas de seta (movimento e pulo)
// GLUT trata teclas especiais (setas, F1-F12, etc.) separadamente das teclas
// normais, por isso usam callbacks próprios (glutSpecialFunc/glutSpecialUpFunc).
// Aqui apenas se atualizam flags booleanas; o movimento de fato acontece
// dentro de anim(), lendo essas flags a cada frame.
// ============================================================================

// Chamada quando uma tecla de seta é pressionada
void arrowKeysDown(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
    	rightArrowPressed = true;
    }
    if (key == GLUT_KEY_LEFT) {
    	leftArrowPressed = true;
    }
    if (key == GLUT_KEY_UP && !isJumping) {
        // Só inicia um novo pulo se o coelho não estiver já pulando
        // (evita "pulo duplo" ao segurar/apertar a seta várias vezes no ar)
        upArrowPressed = true;
        isJumping = true;
        goingUp = true; // Começa subindo
    }
    if (key == GLUT_KEY_DOWN) {
    	downArrowPressed = true; // Usada, por exemplo, para entrar na toca
    }
}

// Chamada quando uma tecla de seta é solta
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

// ============================================================================
// FEATURE: Entrada do teclado — teclas normais (R e E)
// Usa os códigos ASCII diretamente (101 = 'e', 114 = 'r') em vez de
// comparar com caracteres literais como 'e'/'r'. Funcionalmente é a mesma
// coisa, só menos legível — poderia ser escrito como key == 'e'.
// ============================================================================

// Chamada quando uma tecla normal é pressionada
void keyboard_callback(unsigned char key, int x, int y) {
    if (key == 101) {
    	e_key_pressed = true; // letra e
    }
    if (key == 114) {
    	r_key_pressed = true; // letra r (provavelmente "correr" / sprint)
    }
}

// Chamada quando uma tecla normal é solta
void keyboard_up_callback(unsigned char key, int x, int y) {
    if (key == 101) {
    	e_key_pressed = false;
    }
    if (key == 114) {
    	r_key_pressed = false;
    }
}

// ============================================================================
// FEATURE: Loop de atualização lógica do jogo (game loop / update)
// anim() é chamada repetidamente pelo GLUT a cada "msecs" milissegundos
// (agendada no final da própria função via glutTimerFunc, criando um
// loop contínuo). É aqui que TODA a lógica do jogo é atualizada a cada
// frame: input, física do pulo, animações, IA dos inimigos, colisões,
// spawn de itens e o avanço do tempo (ciclo dia/noite).
// A ordem das chamadas importa bastante — comentado passo a passo abaixo.
// ============================================================================
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

    // FEATURE: Bloco de movimento e pulo — só roda se o coelho NÃO está escondido
    if (!coelhoEscondido) {

    	// inputs inseridos para rodar com a setinha esquerda do teclado
        if (leftArrowPressed) {

            direcaoCoelho = -1.0f; // vira o personagem pra esquerda

            // Permite o personagem andar pra esquerda pela cena
            if (characterPos < -8.0f) { // esse comando não deixa o quadrado sair da tela, pra mudar o limite, olhar o glOrtho, está definido para 8 agora!
            	characterPos -= 0; // Já no limite esquerdo: não avança mais (trava a posição)
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
            	characterPos += 0; // Já no limite direito: trava a posição
            }
            else {
            	characterPos += characterSpeed;
            }
        }


        //////////////////////////////////////////////////////////////////////////////////////
        // FEATURE: Física do pulo (máquina de estados simples: subindo/descendo)
        // Controla a animação de pulo do quadrado
        if (isJumping) {
            if (goingUp) {
                // Fase de subida: incrementa a altura até o limite máximo
                jump_height += speed_jump;
                if (jump_height >= jump_maximum_height) {
                    jump_height = jump_maximum_height;  // trava no topo
                    goingUp = false; // começa a fase de descida
                }
            } else {
                // Fase de descida: decrementa a altura até voltar ao chão
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
    // FEATURE: Fase de animação de caminhada do coelho (wrap-around clássico de ângulo)
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
    // FEATURE: Fases de animação da raposa (patas + cauda), sempre avançando
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
    // FEATURE: IA/comportamento da raposa (inimigo terrestre)
    // Cada frame: verifica se deve surgir uma nova raposa, move a que estiver
    // ativa, e checa se colidiu com o coelho (a própria função de colisão
    // trata internamente o caso do coelho estar escondido na toca).
    //Controla o surgimento, movimento e colisao da ave
    controlarSurgimentoDaRaposa();
    moverRaposa();
    verificarColisaoComRaposa(characterPos, jump_height, coelhoEscondido, rabbitLives,
                              foxX, foxY, foxActive, foxJaTirouVidaNestaPassagem);

    /////////////////////////////////////////////////////////////////////////////
    // FEATURE: IA/comportamento da ave de rapina (inimigo aéreo)
    // Mesma lógica da raposa, mas para o gavião: surgimento, movimento e colisão.
    //Controlar o surgimento, movimento e colisao da ave
    controlarSurgimentoDaAve(characterPos);
	moverAve();
	verificarColisaoComAve(characterPos, jump_height, coelhoEscondido, rabbitLives,
						   aveX, aveY, aveActive, aveJaTirouVida);



	//////////////////////////////////////////////////////////////////////////////////////
	/*As borboletas sao um detalhe decorativo
	 sobrevoando os canteiros, entao nao faz sentido elas pararem no ar
	 junto com o resto do cenario.**/
    // FEATURE: Animação das borboletas — roda sempre, mesmo com o coelho escondido
    butterflyPhase += butterflyPhaseSpeed;
    if (butterflyPhase > 2 * PI) {
    	butterflyPhase -= 2 * PI;
    }

    //////////////////////////////////////////////////////////////////////////////////////
    //Configura as nuvens flutuando no ceu
    // FEATURE: Animação das nuvens — também roda sempre (efeito puramente estético)
    cloudPhase += cloudPhaseSpeed;
    if (cloudPhase > 2 * PI) {
    	cloudPhase -= 2 * PI;
    }


    //////////////////////////////////////////////////////////////////////////////////////
    //Chamada do ciclo de passagem do tempo: amanhecer/dia/entardecer/noite
    // FEATURE: Avança o ciclo dia/noite — independe do coelho estar escondido,
    // ou seja, o tempo do "mundo" continua passando mesmo com o jogador parado.
    atualizarCorDoCeu();


    //////////////////////////////////////////////////////////////////////////////////////
	/*Esse condicional abaixo congela o coelho e outras funcionalidades enquanto
	 * ele esta escondido como tocas, surgimento de vegetais, animacao do cenarios, passagem do background
	 * no fundo, etc.*/
    // FEATURE: Tudo que depende do "mundo se movendo" pausa quando o coelho se esconde
    // Isso cria a mecânica de "refúgio seguro": ao entrar na toca, o cenário para
    // de rolar e novos vegetais param de surgir, dando um respiro ao jogador
    // (mas raposa/ave/borboletas/nuvens/ciclo do dia continuam, como comentado acima).
    if (!coelhoEscondido) {

    	 // Rolagem do fundo (chao, cerca, canteiros)
        bgPos -= bgSpeed;
        bgPos = fmod(bgPos, bgWidth); // Mantém bgPos sempre dentro de [0, bgWidth), permitindo o loop do cenário

        // Tocas
        atualizarTocas();

        /* Vegetais de bonificacao: surgimento, movimento, colisao e os
     	 bonus (turbo/pulo alto) que estiverem ativos*/
        controlarSurgimentoDeVegetais();
        moverVegetais();
        verificarColisaoComVegetais(characterPos, jump_height, coelhoEscondido, vegetais);
        atualizarBonusAtivos(); // Decrementa os timers de turbo/pulo reforçado ativos
    }


    //======================================================================================================================================================================================================================================================
    // Comandos padrao da funcao anim
    // FEATURE: Fechamento do game loop
    // Incrementa o contador global de frames, solicita o redesenho da tela
    // (display() será chamada) e reagenda a si mesma para daqui a "msecs"
    // milissegundos — é isso que faz anim() rodar continuamente.
    FrameNumber++;
    glutPostRedisplay();
    glutTimerFunc(msecs, anim, valor);
}

// ============================================================================
// FEATURE: Função de renderização (desenha tudo na tela)
// Chamada pelo GLUT sempre que a tela precisa ser redesenhada (disparada por
// glutPostRedisplay() dentro de anim()). Diferente de anim(), aqui NADA de
// estado de jogo é alterado — só se lê o estado atual e se desenha na ordem
// correta (de trás para frente): céu -> sol/lua -> tocas -> chão/cenário ->
// inimigos -> vegetais -> coelho -> HUD (textos e barra de fôlego).
// ============================================================================
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
    // FEATURE: Transição suave (fade) entre Sol e Lua conforme o horário
    // alphaSol/alphaLua controlam a opacidade de cada astro. Fora das faixas
    // de transição definidas abaixo, um fica 100% visível e o outro 100%
    // invisível; dentro das faixas de entardecer/amanhecer, os dois se
    // misturam gradualmente (um sumindo enquanto o outro aparece).
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
	// Só desenha (evita custo de desenho desnecessário) quando alpha > 0
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

	// FEATURE: Desenho de todas as tocas presentes no vector "tocas"
	// Desenha cada toca (buraco) que estiver na tela
    for (const Toca &toca : tocas) {
        glPushMatrix();
            glTranslatef(toca.x, toca.y, 1.0f);
            drawToca();
        glPopMatrix();
    }


    // FEATURE: Desenho do chão/grama e do conteúdo do cenário com scroll
    // drawBackgroundGrass() é fixo (não rola), enquanto drawBackgroundContent()
    // é deslocado por bgPos, criando o efeito de parallax/rolagem do cenário.
	glPushMatrix();
		  drawBackgroundGrass();
		  glTranslatef(bgPos, 0, 1);
		  drawBackgroundContent();
	glPopMatrix();

	// FEATURE: Desenho da raposa (só aparece quando ativa/perseguindo)
	// Desenha a raposa somente enquanto ela estiver perseguindo o coelho
    if (foxActive) {
        glPushMatrix();
            glTranslatef(foxX, foxY, 1.0f);
            glScalef(0.9f * foxDirecao, 0.8f, 1.0f); // foxDirecao inverte o desenho (espelha) conforme o sentido do movimento
            drawFox();
        glPopMatrix();
    }

    // FEATURE: Desenho da ave de rapina (só aparece quando ativa)
    // Verifica se a ave esta ativa no cenario
	if (aveActive) {
		glPushMatrix();
			glTranslatef(aveX, aveY, 1.0f);
			drawBird();
		glPopMatrix();
	}


	// FEATURE: Desenho de todos os vegetais ativos (ignora os inativos do pool)
	// Desenha cada vegetal de bonificacao que estiver ativo na cena
    for (const Vegetal &veg : vegetais) {
        if (!veg.ativo) continue;
        glPushMatrix();
            glTranslatef(veg.x, veg.y, 1.0f);
            drawVegetable(veg.tipo);
        glPopMatrix();
    }

    glColor3f(0, 0, 0);

    // FEATURE: Alterna entre desenhar o coelho "escondido" (só as orelhas
    // aparecendo para fora da toca, representadas por dois triângulos) ou
    // o coelho completo, normal, andando pelo cenário.
    // Esta e a parte que faz o coelho ficar "escondido" ou não no cenario
    if (coelhoEscondido) {
        const Toca &tocaAtual = tocas[tocaOndeEstaEscondido];
        glPushMatrix();
            glTranslatef(tocaAtual.x, tocaAtual.y + 0.18f, 1.0f);
            glScalef(0.4f * direcaoCoelho, 0.4f, 1.0f);
            glColor3f(0.96f, 0.93f, 0.89f);

            // Orelha esquerda (visível saindo do buraco)
            glPushMatrix();
                glRotatef(10.0f, 0, 0, 1);
                glScalef(0.125f, 0.6f, 1.0f);
                drawTriangle();
            glPopMatrix();

            // Orelha direita (visível saindo do buraco)
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
            glScalef(0.4f * direcaoCoelho, 0.4f, 1.0f); // direcaoCoelho espelha o desenho conforme o lado que ele está olhando
            drawRabbit();
        glPopMatrix();
    }

    // ============================================================================
    // FEATURE: HUD (interface) — vidas, status de bônus e avisos ao jogador
    // ============================================================================
    glColor3f(0.0f, 0.0f, 0.0f);
    char textoVidas[32];
    snprintf(textoVidas, sizeof(textoVidas), "Vidas: %d", rabbitLives);
    drawText(-7.7f, 7.2f, textoVidas);

    // Avisos de bônus ativos (turbo/pulo reforçado), só aparecem enquanto os timers estiverem > 0
    if (framesDeTurboRestantes > 0) {
    	drawText(-7.7f, 6.6f, "Turbo de velocidade!");
    }
    if (framesDePuloReforcadoRestantes > 0) {
    	drawText(-7.7f, 6.0f, "Pulo reforcado!");
    }
    // Avisos de perigo (inimigos ativos na tela)
    if (foxActive) {
    	drawText(-7.7f, 5.4f, "Cuidado, a raposa esta te perseguindo!");
    }
    if (aveActive) {
    	drawText(-7.7f, 4.0f, "Gaviao atacando do alto!");
    }

    // FEATURE: Dica contextual de esconder-se na toca
    // Se o coelho já está escondido, avisa isso. Caso contrário, verifica se
    // ele está perto o suficiente (dentro do RAIO_TOCA) de alguma toca para
    // sugerir que ele pode se esconder ali.
    if (coelhoEscondido) {
        drawText(-7.7f, 4.8f, "Escondido na toca!");
    } else {
        for (const Toca &toca : tocas) {
            if (fabs(toca.x - characterPos) < RAIO_TOCA) {
                drawText(-7.7f, 4.8f, "Aperte a seta para baixo pra se esconder!");
                break; // Basta encontrar uma toca próxima; não precisa checar as demais
            }
        }
    }


    // ============================================================================
    // FEATURE: Barra de fôlego (estamina) do coelho
    // Desenha duas camadas sobrepostas: uma cinza de fundo (representando a
    // barra "vazia"/máxima) e uma verde por cima, cuja largura é proporcional
    // a folegoAtual/FOLEGO_MAXIMO — a barra "esvazia" conforme o coelho corre.
    // ============================================================================
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


        // Parte verde da barra de folego (proporcional ao fôlego restante)
        glColor3f(0.30f, 0.75f, 0.35f);
        glPushMatrix();
            glTranslatef((larguraBarra * folegoPorcentagem) / 2.0f, 0.0f, 0.0f);
            glScalef((larguraBarra * folegoPorcentagem) / 2.0f, alturaBarra, 1.0f);
            drawSquare();
        glPopMatrix();
    glPopMatrix();

    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(-7.7f, 3.75f, "Folego (segure R para correr)");

    // FEATURE: Double buffering — troca o buffer que estava sendo desenhado
    // (invisível) pelo que é exibido na tela, evitando "flickering"
    glutSwapBuffers();
}

// =========================================================

// ============================================================================
// FEATURE: Inicialização do OpenGL e do jogo (chamada uma única vez)
// Configura a cor de fundo, o sistema de projeção (câmera ortográfica 2D,
// de -8 a 8 em X e Y), habilita transparência (necessária para o alpha do
// sol/lua e outros elementos translúcidos) e inicializa o estado do cenário.
// ============================================================================
void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Cor de fundo usada por glClear (branco)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-8, 8, -8, 8, -8, 8); // Define a "câmera" 2D: janela de mundo visível de -8 a 8 em X, Y e Z

    // Habilita transparencia do openGL
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Fórmula clássica de blending por transparência (alpha)


    initCenario(); // Cria as tocas iniciais do mapa
}

// ============================================================================
// FEATURE: Ponto de entrada do programa
// Inicializa a semente do gerador de números aleatórios (baseada no horário
// atual, para que cada execução tenha spawns diferentes), configura a
// janela GLUT, registra todas as funções de callback (desenho, timer,
// teclado) e entra no loop principal do GLUT, que mantém o programa rodando
// até a janela ser fechada.
// ============================================================================
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(NULL))); // Semente aleatória baseada no tempo atual

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Double buffer + modo de cor RGB
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Trabalho 1 - Computacao Grafica");

    init();

    glutDisplayFunc(display);       // Registra a função de desenho
    glutTimerFunc(msecs, anim, 0);  // Agenda a primeira chamada do game loop (anim)

    glutKeyboardFunc(keyboard_callback);     // Tecla normal pressionada
    glutKeyboardUpFunc(keyboard_up_callback); // Tecla normal solta
    glutSpecialFunc(arrowKeysDown);           // Tecla especial (seta) pressionada
    glutSpecialUpFunc(arrowKeysUp);           // Tecla especial (seta) solta

    glutMainLoop(); // Loop principal do GLUT (bloqueante até o programa ser encerrado)

    return 0;
}
