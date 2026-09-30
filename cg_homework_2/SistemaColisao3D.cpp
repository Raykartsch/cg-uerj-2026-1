#include "SistemaColisao3D.hpp"
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Colisao do coelho com os vegetais. Mesma ideia do 2D ("colisao por
circulos"), agora por ESFERAS: o vegetal e capturado quando a distancia 3D
entre o centro do coelho e o do vegetal e menor que o alcance de captura.

Diferencas em relacao ao 2D:
 - a distancia inclui o eixo Z (dz);
 - nao ha mais o deslocamento de +0.3 no centro do coelho: la ele compensava
   o desenho do coelho, que nao era centrado na origem; a esfera ja e.
 - o alcance continua 1.1, o mesmo do 2D.*/
void verificarColisaoComVegetais(float coelhoX, float coelhoY, float coelhoZ, bool coelhoEscondido,
                                 std::vector<Vegetal>& listaVegetais) {
    if (coelhoEscondido) {
        return;
    }

    float raioDeCaptura = 1.1f; // "alcance" do coelho para pegar um vegetal

    for (Vegetal &veg : listaVegetais) {
        if (!veg.ativo) {
            continue;
        }

        float dx = veg.x - coelhoX;
        float dy = veg.y - coelhoY;
        float dz = veg.z - coelhoZ;
        float distancia = std::sqrt(dx * dx + dy * dy + dz * dz);

        if (distancia < raioDeCaptura) {
            veg.ativo = false;               // o vegetal desaparece do campo
            aplicarBonusDoVegetal(veg.tipo); // o bonus correspondente e ativado
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Verifica colisao 3D entre a raposa (predador terrestre) e o coelho
void verificarColisaoComRaposa() {
    if (!foxActive || foxJaTirouVidaNestaPassagem || coelhoEscondido) return;

    float dx = foxX - coelhoX;
    float dy = foxY - coelhoY;
    float dz = foxZ - coelhoZ;
    float distancia = std::sqrt(dx * dx + dy * dy + dz * dz);

    // Se o coelho pulou alto por cima da raposa, ele se esquiva com sucesso
    if (coelhoY > 1.35f && std::abs(dz) < 0.8f) {
        return; // Esquivou do ataque
    }

    if (distancia < RAIO_COLISAO_RAPOSA) {
        if (rabbitLives > 0) {
            rabbitLives--;
        }
        foxJaTirouVidaNestaPassagem = true; // Uma vida por passagem
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Verifica colisao 3D por distancia esferica euclidiana entre a ave de rapina / falcao e o coelho
void verificarColisaoComAve() {
    if (!aveActive || aveJaTirouVida || coelhoEscondido) return;

    float dx = aveX - coelhoX;
    float dy = aveY - coelhoY;
    float dz = aveZ - coelhoZ;
    float distancia = std::sqrt(dx * dx + dy * dy + dz * dz);

    float raioColisao = 0.95f;

    if (distancia < raioColisao) {
        if (rabbitLives > 0) {
            rabbitLives--;
        }
        aveJaTirouVida = true; // Trava para tirar no maximo uma vida por mergulho
    }
}

