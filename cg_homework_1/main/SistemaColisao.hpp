#pragma once
#include <vector>
#include "Cenario.hpp"

// O SistemaColisao recebe as variáveis por referência, isolando a lógica matemática
// e evitando dependências circulares (includes cruzados) entre Raposa, Coelho e Cenario.
void verificarColisaoComVegetais(float coelhoX, float coelhoY, bool coelhoEscondido,
                                 std::vector<Vegetal>& listaVegetais);

void verificarColisaoComRaposa(float coelhoX, float coelhoY, bool coelhoEscondido, int& rabbitLives,
                               float foxX, float foxY, bool foxActive, bool& foxJaTirouVidaNestaPassagem);
