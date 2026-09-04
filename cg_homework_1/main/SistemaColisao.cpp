#include "SistemaColisao.hpp"
#include <cmath>


void verificarColisaoComVegetais(float coelhoX, float coelhoY, bool coelhoEscondido, std::vector<Vegetal>& listaVegetais) {
    if (coelhoEscondido) return;

    float centroCoelhoX = coelhoX + 0.3f;
    float centroCoelhoY = coelhoY + 0.3f;
    float raioDeCaptura = 1.1f;

    for (Vegetal &veg : listaVegetais) {
        if (!veg.ativo) continue;

        float dx = veg.x - centroCoelhoX;
        float dy = veg.y - centroCoelhoY;
        float distancia = sqrt(dx * dx + dy * dy);

        if (distancia < raioDeCaptura) {
            veg.ativo = false;
            aplicarBonusDoVegetal(veg.tipo);
        }
    }
}


void verificarColisaoComRaposa(float coelhoX, float coelhoY, bool coelhoEscondido, int& rabbitLives,
                               float foxX, float foxY, bool foxActive, bool& foxJaTirouVidaNestaPassagem) {
    if (!foxActive) {
    	return;
    }

    // Se o coelho estiver escondido na toca, ele esta a salvo da raposa
    if (coelhoEscondido) {
    	return;
    }

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
            if (rabbitLives > 0) {
                rabbitLives--;
            }
            foxJaTirouVidaNestaPassagem = true;
        }
    } else {
        foxJaTirouVidaNestaPassagem = false;
    }
}


void verificarColisaoComAve(float coelhoX, float coelhoY, bool coelhoEscondido, int& rabbitLives,
                            float aveX, float aveY, bool aveActive, bool& aveJaTirouVida) {
    if (!aveActive) {
    	return;
    }

    // Se o coelho esta na toca, logo esta a salvo da ave
    if (coelhoEscondido) {
    	return;
    }

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
            	rabbitLives--;
            	aveJaTirouVida = true;
            }
        }
    } else {
        aveJaTirouVida = false;
    }
}
