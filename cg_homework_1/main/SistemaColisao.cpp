#include "SistemaColisao.hpp"
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void verificarColisaoComVegetais(float coelhoX, float coelhoY, bool coelhoEscondido, std::vector<Vegetal>& listaVegetais) {
    if (coelhoEscondido) {
    	return;
    }

    //Centro aproximado do coelho na tela, somando-se +0.3 em X e Y
    float centroCoelhoX = coelhoX + 0.3f;
    float centroCoelhoY = coelhoY + 0.3f;
    float raioDeCaptura = 1.1f; // "alcance" do coelho para pegar um vegetal

    /*Percorre todos os vegetais que existem no jogo (ativos ou nao) e
    testa a distancia so dos que estao realmente ativos na tela*/
    for (Vegetal &veg : listaVegetais) {
        if (!veg.ativo) {
        	continue;
        }

        // Distancia entre o centro do coelho e o centro do vegetal,
        float dx = veg.x - centroCoelhoX;
        float dy = veg.y - centroCoelhoY;
        float distancia = sqrt(dx * dx + dy * dy);

        // Os dois "circulos" se tocaram: o vegetal e capturado
        if (distancia < raioDeCaptura) {
            veg.ativo = false; // o vegetal desaparece da tela
            aplicarBonusDoVegetal(veg.tipo); //O bonus correspondente eh ativado
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Verifica se a raposa encostou no coelho.
void verificarColisaoComRaposa(float coelhoX, float coelhoY, bool coelhoEscondido, int& rabbitLives,
                               float foxX, float foxY, bool foxActive, bool& foxJaTirouVidaNestaPassagem) {

	// nao ha raposa na tela agora, entao nao ha o que verificar
	if (!foxActive) {
    	return;
    }

    // Se o coelho estiver escondido na toca, ele esta a salvo da raposa
    if (coelhoEscondido) {
    	return;
    }

    //Centro aproximado da raposa na tela
    float dx = foxX - coelhoX;
    float dy = foxY - coelhoY;
    float distancia = sqrt(dx * dx + dy * dy);
    float raioColisao = 1.1f;


    /*Faz o check de colisao da raposa com o coelho.
     Se a raposa ja acertou o coelho nessa aparicao corrente, ela continua a correr pelo cenario.
     Este check é importante para tirar apenas uma vida do coelho e não varias
     devido a quantidade de frames por segundo na cena */
    if (distancia < raioColisao) {
        if (!foxJaTirouVidaNestaPassagem) {

        	// So tira vida se a trava ainda estiver "destravada" (ou seja, se
        	// essa e a primeira vez que os dois se tocam nesta passagem)
            if (rabbitLives > 0) {
            	// o jogador perde uma vida
                rabbitLives--;
            }
            foxJaTirouVidaNestaPassagem = true;
        }

    /*A raposa se afastou o suficiente: libera a trava, para poder
    tirar vida de novo caso ela volte a encostar no coelho mais pra frente*/
    } else {
        foxJaTirouVidaNestaPassagem = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Verifica se a ave encostou no coelho
void verificarColisaoComAve(float coelhoX, float coelhoY, bool coelhoEscondido, int& rabbitLives,
                            float aveX, float aveY, bool aveActive, bool& aveJaTirouVida) {

	// nao ha ave voando agora, entao nao ha o que verificar
	if (!aveActive) {
    	return;
    }

    // Se o coelho esta na toca, logo esta a salvo da ave
    if (coelhoEscondido) {
    	return;
    }

    // Distancia entre o coelho e a ave, na mesma logica de "colisao por
   // circulos" das duas funcoes anteriores
    float dx = aveX - coelhoX;
    float dy = aveY - coelhoY;
    float distancia = sqrt(dx * dx + dy * dy);
    float raioColisao = 1.0f;

    /*Faz o check de colisao da ave com o coelho.
    Se a ave ja acertou o coelho numa aparicao, a ave só continua o voo,
    isto é importante para tirar apenas uma vida do coelho e não varias
    devido a quantidade de frames por segundo na cena */
    if (distancia < raioColisao) {
        if (!aveJaTirouVida) {
            if (rabbitLives > 0) {
            	rabbitLives--; // o jogador perde uma vida
            	aveJaTirouVida = true;  // trava ate a ave se afastar de novo
            }
        }
    } else {

    	/* A ave se afastou o suficiente: libera a trava, para poder tirar
    	vida de novo caso ela volte a encostar no coelho mais pra frente*/
        aveJaTirouVida = false;
    }
}
