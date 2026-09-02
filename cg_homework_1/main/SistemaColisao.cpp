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
    if (!foxActive) return;
    if (coelhoEscondido) return;

    float dx = foxX - coelhoX;
    float dy = foxY - coelhoY;
    float distancia = sqrt(dx * dx + dy * dy);
    float raioColisao = 1.1f; // Equivalente a RAIO_COLISAO_RAPOSA

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
