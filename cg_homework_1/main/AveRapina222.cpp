#include "SistemaColisao.hpp"
#include <cmath>

//////////////////////////////////////////////////////////////////////////////////////////////
// SISTEMA DE COLISAO
//
// Este arquivo concentra toda a deteccao de "toque" entre o coelho e os
// outros elementos do jogo (vegetais, raposa e ave). A ideia por tras de
// todas as funcoes aqui e sempre a mesma, bem simples: tratamos cada
// personagem/objeto como se fosse um circulo invisivel, e calculamos a
// distancia entre os centros desses circulos. Se essa distancia for menor
// que um certo "raio de colisao", consideramos que os dois se tocaram.
//
// Essa tecnica se chama "colisao por circulos" (ou bounding circle) e e uma
// forma bem mais simples de detectar toque do que comparar o formato exato
// de cada desenho -- funciona muito bem para jogos 2D como este.


// Verifica se o coelho encostou em algum vegetal ativo (cenoura, alface ou
// rabanete) espalhado pelo cenario. Cada vegetal capturado desaparece da
// tela e concede o bonus correspondente ao coelho (velocidade, pulo alto
// ou vida extra -- veja aplicarBonusDoVegetal).
void verificarColisaoComVegetais(float coelhoX, float coelhoY, bool coelhoEscondido, std::vector<Vegetal>& listaVegetais) {

    // Escondido na toca, o coelho nao alcança nada que esteja la fora
    if (coelhoEscondido) return;

    // Centro aproximado do coelho na tela. Somamos +0.3 em X e Y porque o
    // corpo do coelho fica um pouco a frente/acima da posicao "oficial"
    // dele (coelhoX, coelhoY), por causa da cabeca e das orelhas -- esse
    // pequeno ajuste faz o "circulo de colisao" ficar melhor centralizado
    // no desenho de verdade.
    float centroCoelhoX = coelhoX + 0.3f;
    float centroCoelhoY = coelhoY + 0.3f;
    float raioDeCaptura = 1.1f; // "alcance" do coelho para pegar um vegetal

    // Percorre todos os vegetais que existem no jogo (ativos ou nao) e
    // testa a distancia so dos que estao realmente ativos na tela
    for (Vegetal &veg : listaVegetais) {
        if (!veg.ativo) continue;

        // Distancia entre o centro do coelho e o centro do vegetal,
        // calculada com o Teorema de Pitagoras: distancia = raiz(dx² + dy²)
        float dx = veg.x - centroCoelhoX;
        float dy = veg.y - centroCoelhoY;
        float distancia = sqrt(dx * dx + dy * dy);

        // Os dois "circulos" se tocaram: o vegetal e capturado
        if (distancia < raioDeCaptura) {
            veg.ativo = false;                // o vegetal desaparece da tela
            aplicarBonusDoVegetal(veg.tipo);  // e concede o bonus correspondente
        }
    }
}


// Verifica se a raposa encostou no coelho. Diferente dos vegetais (que
// desaparecem ao serem tocados), a raposa NAO some quando colide -- ela
// continua correndo pelo cenario normalmente. O que precisa ser controlado
// aqui e o jogador perder exatamente UMA vida por passagem da raposa, e nao
// uma vida a cada frame em que ela estiver encostada nele (o que aconteceria
// muito rapido, ja que o jogo roda varios frames por segundo).
//
// Repare que "rabbitLives" e "foxJaTirouVidaNestaPassagem" sao recebidos por
// REFERENCIA (o "&" depois do tipo): isso significa que essa funcao pode
// alterar o valor original dessas variaveis, nao so uma copia -- e assim que
// a vida perdida e a trava "ja tirou vida" realmente afetam o resto do jogo.
void verificarColisaoComRaposa(float coelhoX, float coelhoY, bool coelhoEscondido, int& rabbitLives,
                               float foxX, float foxY, bool foxActive, bool& foxJaTirouVidaNestaPassagem) {
    if (!foxActive) {
    	return; // nao ha raposa na tela agora, entao nao ha o que verificar
    }

    // Se o coelho estiver escondido na toca, ele esta a salvo da raposa
    if (coelhoEscondido) {
    	return;
    }

    // Mesma ideia de "colisao por circulos" usada com os vegetais: calcula
    // a distancia entre o coelho e a raposa via Teorema de Pitagoras
    float dx = foxX - coelhoX;
    float dy = foxY - coelhoY;
    float distancia = sqrt(dx * dx + dy * dy);
    float raioColisao = 1.1f;


    /*Faz o check de colisao da raposa com o coelho.
     Se a raposa ja acertou o coelho nessa aparicao corrente, ela continua a correr pelo cenario.
     Este check é importante para tirar apenas uma vida do coelho e não varias
     devido a quantidade de frames por segundo na cena */
    if (distancia < raioColisao) {
        // So tira vida se a trava ainda estiver "destravada" (ou seja, se
        // essa e a primeira vez que os dois se tocam nesta passagem)
        if (!foxJaTirouVidaNestaPassagem) {
            if (rabbitLives > 0) {
                rabbitLives--; // o jogador perde uma vida
            }
            foxJaTirouVidaNestaPassagem = true; // trava ate a raposa se afastar de novo
        }
    } else {
        // A raposa se afastou o suficiente: libera a trava, para poder
        // tirar vida de novo caso ela volte a encostar no coelho mais pra frente
        foxJaTirouVidaNestaPassagem = false;
    }
}


// Verifica se a ave de rapina encostou no coelho durante o mergulho.
// Segue exatamente a mesma logica da colisao com a raposa (a ave tambem nao
// desaparece ao acertar o coelho, so continua o voo, e tambem so pode tirar
// UMA vida por passagem, gracas a trava "aveJaTirouVida").
void verificarColisaoComAve(float coelhoX, float coelhoY, bool coelhoEscondido, int& rabbitLives,
                            float aveX, float aveY, bool aveActive, bool& aveJaTirouVida) {
    if (!aveActive) {
    	return; // nao ha ave voando agora, entao nao ha o que verificar
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
            // ATENCAO: aqui a trava "aveJaTirouVida" so e ligada DENTRO do
            // "if (rabbitLives > 0)". Isso e uma pequena diferenca em
            // relacao a verificarColisaoComRaposa() (onde a trava e ligada
            // mesmo que o coelho ja esteja com 0 vidas). Na pratica isso so
            // faz diferenca quando rabbitLives chega a 0: nesse caso, aqui
            // a trava nunca liga, entao esse "if" continua sendo reavaliado
            // a cada frame enquanto durar a colisao (sem efeito pratico
            // grave, ja que o "rabbitLives--" tambem esta protegido, mas
            // vale considerar deixar igual a funcao da raposa para manter o
            // comportamento consistente entre as duas).
            if (rabbitLives > 0) {
            	rabbitLives--; // o jogador perde uma vida
            	aveJaTirouVida = true; // trava ate a ave se afastar de novo
            }
        }
    } else {
        // A ave se afastou o suficiente: libera a trava, para poder tirar
        // vida de novo caso ela volte a encostar no coelho mais pra frente
        aveJaTirouVida = false;
    }
}
