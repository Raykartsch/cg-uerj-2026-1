#pragma once
#include <vector>
#include "Vegetais3D.hpp"

// Como no 2D, o SistemaColisao recebe os dados por parametro, isolando a
// matematica e evitando includes cruzados entre Coelho e Vegetais.
void verificarColisaoComVegetais(float coelhoX, float coelhoY, float coelhoZ, bool coelhoEscondido,
                                 std::vector<Vegetal>& listaVegetais);
