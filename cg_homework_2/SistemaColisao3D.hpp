#pragma once
#include <vector>
#include "Vegetais3D.hpp"
#include "Coelho3D.hpp"
#include "Raposa3D.hpp"
#include "AveRapina3D.hpp"

// Colisao do coelho com os vegetais de bonificacao
void verificarColisaoComVegetais(float coelhoX, float coelhoY, float coelhoZ, bool coelhoEscondido,
                                 std::vector<Vegetal>& listaVegetais);

// Colisao 3D entre a raposa (predador terrestre) e o coelho
void verificarColisaoComRaposa();

// Colisao 3D entre a ave de rapina / falcao (predador aereo) e o coelho
void verificarColisaoComAve();

