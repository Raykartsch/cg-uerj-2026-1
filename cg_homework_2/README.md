# Relatório Técnico Unificado: Adaptação do Jogo 2D para 3D (Etapa 2)

**Disciplina:** Computação Gráfica  
**Instituição:** Universidade do Estado do Rio de Janeiro (UERJ)  
**Linguagem de Programação:** C++  
**Bibliotecas Utilizadas:** OpenGL clássico (GL, GLU, FreeGLUT / GLUT)  

---

## Sumário
1. [Introdução e Visão Geral](#1-introdução-e-visão-geral)
2. [Estrutura do Código e Arquitetura Modular](#2-estrutura-do-código-e-arquitetura-modular)
3. [Sistema de Coordenadas, Iluminação e Pipeline Gráfico 3D](#3-sistema-de-coordenadas-iluminação-e-pipeline-gráfico-3d)
4. [Cenário Tridimensional: Gramado, Rolagem e Sombras](#4-cenário-tridimensional-gramado-rolagem-e-sombras)
5. [Modelagem e Animação do Personagem Principal (Coelho 3D)](#5-modelagem-e-animação-do-personagem-principal-coelho-3d)
6. [Movimentação Orientada do Personagem - Tank Controls (Item 3)](#6-movimentação-orientada-do-personagem---tank-controls-item-3)
7. [Sistema de Câmeras e Alternância de Visualização (Item 4)](#7-sistema-de-câmeras-e-alternância-de-visualização-item-4)
8. [Rotação Orbital da Câmera na Visão Geral (Item 5)](#8-rotação-orbital-da-câmera-na-visão-geral-item-5)
9. [Predadores Tridimensionais: Raposa e Ave de Rapina](#9-predadores-tridimensionais-raposa-e-ave-de-rapina)
10. [Vegetais de Bonificação (Power-Ups 3D)](#10-vegetais-de-bonificação-power-ups-3d)
11. [Sistema de Colisões Volumétricas 3D e Interface (HUD)](#11-sistema-de-colisões-volumétricas-3d-e-interface-hud)
12. [Mapeamento Completo de Controles e Atalhos](#12-mapeamento-completo-de-controles-e-atalhos)
13. [Instruções de Compilação e Execução](#13-instruções-de-compilação-e-execução)

---

## 1. Introdução e Visão Geral

Na primeira etapa da disciplina, foi requisitado o desenvolvimento de um jogo bidimensional (2D) em OpenGL com rolagem lateral, no qual o personagem principal (um coelho) corria por um campo, desviava de predadores (uma raposa terrestre e uma ave de rapina) e recolhia vegetais para ativar bonificações temporárias (turbo de velocidade, vida extra e pulo reforçado).

Nesta **Etapa 2**, o objetivo central foi a expansão e migração integral do jogo para um **ambiente tridimensional (3D) interativo em tempo real**. O desafio de engenharia e modelagem consistiu em:
- Preservar rigorosamente a identidade visual, a paleta de cores original, a física de saltos e a dinâmica de jogo consagrada no 2D;
- Substituir primitivas planas 2D por sólidos tridimensionais volumétricos da GLUT com transformações hierárquicas (*Scene Graph*);
- Expandir o plano de locomoção para o quadrilátero $XZ$, adicionando profundidade física ao mundo;
- Implementar os requisitos formais de avaliação: controle orientado do personagem (*Item 3*), alternância de câmeras entre terceira pessoa panorâmica e primeira pessoa nos olhos do coelho (*Item 4*) e órbita livre da câmera geral ao redor do eixo vertical (*Item 5*).

Todo o código foi implementado em **C++** sem qualquer dependência de bibliotecas externas adicionais, respeitando as restrições estritas do enunciado (apenas GL, GLU e GLUT).

---

## 2. Estrutura do Código e Arquitetura Modular

O projeto foi organizado de forma modular, dividindo as responsabilidades de modelagem, física e renderização em arquivos de cabeçalho (`.hpp`) e implementação (`.cpp`), interligados pelo padrão de compilação unificada (**Unity Build**):

```
cg_homework_2/
├── main.cpp                  # Loop principal, GLUT callbacks, câmeras, iluminação e HUD
├── Coelho3D.hpp / .cpp       # Modelagem hierárquica do coelho, trote, pulo e movimentação orientada
├── Raposa3D.hpp / .cpp       # Modelagem 3D da raposa, cauda articulada, trote e colisão terrestre
├── AveRapina3D.hpp / .cpp    # Modelagem da ave, asas 3D, mergulho parabólico e pitch dinâmico
├── Vegetais3D.hpp / .cpp     # Modelagem de cenoura, alface e rabanete, rotação e geração aleatória
├── Cenario3D.hpp / .cpp      # Gramado quadriculado, rolagem no eixo X, cerca e projeção de sombras
└── SistemaColisao3D.hpp / .cpp # Colisões volumétricas euclidianas 3D e aplicação de bonificações
```

### O Padrão Unity Build
Assim como no código da Etapa 1, o arquivo `main.cpp` inclui diretamente os arquivos de implementação:
```cpp
#include "Cenario3D.cpp"
#include "Coelho3D.cpp"
#include "Vegetais3D.cpp"
#include "SistemaColisao3D.cpp"
#include "Raposa3D.cpp"
#include "AveRapina3D.cpp"
```
Essa estratégia garante compilação rápida em um único passo, eliminando a necessidade de gerenciadores de dependência complexos ou Makefiles proprietários. Para compilar, basta passar unicamente o `main.cpp` ao compilador.

---

## 3. Sistema de Coordenadas, Iluminação e Pipeline Gráfico 3D

### 3.1. Convenção de Eixos Tridimensionais
A transição do plano 2D para o espaço 3D estabeleceu a seguinte convenção no espaço do mundo:
- **Eixo X (Largura / Corrida):** Varia de $-8.0$ a $+8.0$ na área jogável delimitada pela cerca. A rolagem do cenário desliza para $-X$, simulando o avanço para a frente.
- **Eixo Y (Altura do Mundo):** O nível do gramado repousa exatamente no plano $Y = 0.0$. O centro do coelho apoia-se em $Y = 0.5$ (de modo que as patas toquem o chão em $Y = 0.0$), enquanto saltos elevam o personagem até $Y = 2.5$ (ou $Y = 3.5$ com bônus).
- **Eixo Z (Profundidade):** Varia de $-6.0$ a $+6.0$. A coordenada $+Z$ aponta na direção do observador e $-Z$ para o fundo do campo. O coelho e os predadores agora manobram livremente em toda a superfície do plano $XZ$.

### 3.2. Projeção em Perspectiva (`gluPerspective`)
Diferente da projeção ortográfica estática do 2D (`glOrtho`), o 3D utiliza a matriz de projeção em perspectiva:
```cpp
gluPerspective(55.0, (double)largura / (double)altura, 0.1, 100.0);
```
O campo de visão (*FOV*) de $55^\circ$ proporciona percepção natural de distância, fazendo com que objetos distantes (como vegetais no horizonte ou predadores se aproximando) pareçam menores e ganhem volume conforme se aproximam.

### 3.3. Teste de Profundidade (Z-Buffer)
No jogo 2D, a sobreposição dependia exclusivamente da ordem em que os comandos de desenho eram chamados no código (*Painter's Algorithm*). No 3D, ativou-se o teste de profundidade via hardware:
```cpp
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
glEnable(GL_DEPTH_TEST);
```
A cada frame, tanto o buffer de cor quanto o de profundidade são limpos simultaneamente:
```cpp
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

### 3.4. Iluminação e Materiais (`GL_LIGHT0` e `GL_NORMALIZE`)
Para conferir volume, realce e contraste aos sólidos da GLUT, o pipeline de iluminação foi configurado da seguinte forma:
- **Habilitação:** `glEnable(GL_LIGHTING)` e `glEnable(GL_LIGHT0)`.
- **Rastreamento de Cores:** `glEnable(GL_COLOR_MATERIAL)` com `glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE)`. Isso permite que as chamadas tradicionais `glColor3f` continuem definindo as cores difusas dos materiais sem sobrecarga de código.
- **Normalização de Vetores Normais:** Como os sólidos da GLUT são intensamente escalados via `glScalef` para transformar esferas unitárias em elipsoides, o vetor normal à superfície sofreria distorções de magnitude. Ativou-se obrigatoriamente `glEnable(GL_NORMALIZE)`, garantindo que o OpenGL recalcule as normais unitárias em tempo de execução, prevenindo manchas escuras.
- **Brilho Especular:** Aplicou-se um acabamento com expoente especular (`GL_SHININESS = 25.0`) na pelagem do coelho e nos olhos, gerando reflexos pontuais conforme a câmera se movimenta.
- **Posicionamento no Mundo:** A fonte de luz direcional solar $(0.4, 1.0, 0.6, 0.0)$ é enviada via `glLightfv` **após** a chamada do `gluLookAt`. Isso fixa a luz no referencial do mundo, fazendo com que o sombreamento e os reflexos mudem realisticamente sobre os corpos ao orbitar a câmera.

---

## 4. Cenário Tridimensional: Gramado, Rolagem e Sombras

### 4.1. Gramado Quadriculado e Rolagem Suave
O chão do cenário foi construído como uma malha de ladrilhos quadrados de $1.0 \times 1.0$ unidades no plano $XZ$ ($Y = 0.0$), alternando dois tons de verde (claro e escuro).
- A rolagem do cenário foi adaptada do 2D somando o deslocamento da variável `bgPos` (que decrementa por `bgSpeed` a cada frame com retorno cíclico via `std::fmod`).
- O gramado se estende além da cerca delimitadora (de $-20.0$ a $+20.0$ em $X$) para que o surgimento de vegetais e predadores no horizonte não ocorra sobre o vazio.

### 4.2. Cerca Delimitadora
Ao redor do quadrilátero de movimentação ($X \in [-8.0, 8.0]$, $Z \in [-6.0, 6.0]$), uma moldura de madeira marrom espessa foi desenhada em $Y = 0.02$, visualizando com clareza os limites do campo.

### 4.3. Sistema de Projeção de Sombras no Solo
Em um ambiente 3D, a ausência de sombras dificulta a percepção de altura e de alinhamento em profundidade. Para solucionar isso:
- Criou-se a função `drawDiscoNoChao()`, que renderiza discos escuros semitransparentes no plano horizontal.
- **Prevenção de Z-Fighting:** As sombras são desenhadas em $Y = 0.02$ com *blending* alfa (`GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA`) e iluminação temporariamente desligada, eliminando cintilações com a malha da grama.
- **Comportamento Dinâmico:**
  - Quando o coelho pula, o raio da sua sombra encolhe proporcionalmente à altura $Y$, indicando visualmente a elevação e a posição exata onde ele irá aterrissar;
  - A sombra da ave de rapina começa ampla e tênue no céu e se torna pequena, nítida e escura à medida que o predador mergulha em direção ao chão;
  - Raposa e vegetais possuem sombras próprias fixadas no solo.

---

## 5. Modelagem e Animação do Personagem Principal (Coelho 3D)

### 5.1. Modelagem Geométrica com Sólidos GLUT
O coelho da Etapa 1 era desenhado de perfil usando discos e triângulos planos. No 3D, ele foi transformado em um modelo volumétrico articulado hierárquico através de transformações locais (`glPushMatrix` / `glPopMatrix`):

| Parte do Corpo | Primitiva e Escala GLUT | Posição Relativa | Cor Adotada |
| :--- | :--- | :--- | :--- |
| **Corpo (Tronco)** | `glutSolidSphere` escalada para $(0.42, 0.30, 0.28)$ | Origem local $(0.0, 0.0, 0.0)$ | Creme claro $(0.96, 0.93, 0.89)$ |
| **Cabeça** | `glutSolidSphere` escalada para $(0.24, 0.22, 0.21)$ | $(+0.32, +0.18, 0.0)$ | Creme claro $(0.96, 0.93, 0.89)$ |
| **Bochechas** | Duas esferas simétricas de raio $0.10$ | Deslocadas em $\pm Z$ na face | Branco puro $(1.0, 1.0, 1.0)$ |
| **Focinho e Nariz** | Microesfera na ponta da cabeça | Frente da face | Rosa pastel $(0.90, 0.42, 0.54)$ |
| **Dentes** | Dois `glutSolidCube` escalados | Abaixo do focinho | Branco puro $(1.0, 1.0, 1.0)$ |
| **Olhos** | Esferas pretas com microesfera branca | Laterais da face em $\pm Z$ | Preto com reflexo especular |
| **Orelhas (Dupla Camada)** | Elipsoides alongados externos e camada interna côncava | Topo da cabeça com inclinação | Externa: Creme / Interna: Rosa |
| **4 Patas e Coxas** | Esferas escaladas cilíndricas | Dianteiras e traseiras com coxas | Bege macio $(0.90, 0.85, 0.79)$ |
| **Rabo Pom-pom** | Esfera fofa de algodão de raio $0.13$ | Traseira do corpo $(-0.42, 0.05, 0.0)$ | Creme claro $(0.96, 0.93, 0.89)$ |

As 4 patas foram posicionadas geometricamente para que, quando o coelho estiver no solo ($Y = 0.5$), a base de cada pé toque com precisão cirúrgica o plano da grama ($Y = 0.0$).

### 5.2. Animação de Marcha, Trote e Balanço de Orelhas
A animação reaproveita a lógica senoidal introduzida na Etapa 1, expandida para os membros 3D:
- **Trote Alternado:** A variável `walkPhase` avança durante a movimentação. Quando $\sin(\text{walkPhase}) > 0$, a pata dianteira esquerda e a traseira direita se elevam enquanto a orelha direita oscila para frente. Quando $\sin(\text{walkPhase}) < 0$, o par oposto executa o movimento.
- **Efeito do Turbo (Cenoura):** Ao recolher a cenoura, a velocidade de locomoção passa de $0.05$ para $0.20$ e o avanço da fase de trote é acelerado por um multiplicador de $1.8\times$, gerando passadas rápidas e balanço vigoroso nas orelhas.
- **Parada Suave:** Ao soltar os comandos de locomoção, `walkPhase` decai por fator amortecedor $0.8$ até zerar, assegurando que o coelho pouse todas as quatro patinhas suavemente no chão.
- **Dinâmica do Salto:** Ao acionar a barra de espaço, o coelho ascende parabolicamente; durante o voo, as 4 patas se recolhem contra o corpo ($+0.10$ em $Y$) e as orelhas inclinam $10^\circ$ adicionais para trás pela resistência do ar.

---

## 6. Movimentação Orientada do Personagem - Tank Controls (Item 3)

O enunciado do Trabalho 2 estabelece no Item 3:
> *"Permita que o personagem principal seja controlado por meio das setas do teclado, de acordo com as seguintes regras:  
> ● Seta para cima: movimenta o personagem para frente;  
> ● Seta para baixo: movimenta o personagem para trás;  
> ● Seta para esquerda: rotaciona o personagem em torno do seu próprio eixo;  
> ● Seta para direita: rotaciona o personagem em torno do seu próprio eixo.  
> O deslocamento para frente e para trás deve respeitar a direção para a qual o personagem está orientado."*

### 6.1. Rotação no Próprio Eixo (Setas Esquerda e Direita)
- A orientação é controlada pela variável angular `anguloCoelho` em graus (convenção: $0^\circ$ olha para $+X$, exatamente como o desenho de perfil do coelho original).
- Seta Esquerda: gira no sentido anti-horário (`anguloCoelho += 3.5f`).
- Seta Direita: gira no sentido horário (`anguloCoelho -= 3.5f`).
- O ângulo é mantido continuamente normalizado no intervalo cíclico $[0^\circ, 360^\circ)$.

### 6.2. Conversão Trigonométrica em Vetor Frontal (Setas Cima e Baixo)
Para transladar o personagem na direção em que ele aponta, o ângulo em graus é convertido para radianos ($\theta = \text{anguloCoelho} \cdot \pi / 180$) e decompõe-se no vetor frontal unitário $\vec{f} = (f_X, 0, f_Z)$ no plano $XZ$:
$$f_X = \cos(\theta)$$
$$f_Z = -\sin(\theta)$$
*(O sinal negativo em $Z$ deve-se à convenção do OpenGL, na qual girar no sentido anti-horário leva de $+X$ para $-Z$).*

O deslocamento é aplicado diretamente às coordenadas do coelho:
- **Avanço (Seta Cima):**  
  `coelhoX += fx * characterSpeed;`  
  `coelhoZ += fz * characterSpeed;`
- **Marcha a Ré (Seta Baixo):**  
  `coelhoX -= fx * (characterSpeed * 0.75f);`  
  `coelhoZ -= fz * (characterSpeed * 0.75f);`

### 6.3. Contenção nos Limites da Cerca
As coordenadas do coelho são filtradas pela função `limitar()`, descontando o raio da esfera envoltória ($0.5$), impedindo que o corpo do coelho atravesse os limites da cerca:
```cpp
coelhoX = limitar(coelhoX, CAMPO_X_MIN + RAIO_ESFERA, CAMPO_X_MAX - RAIO_ESFERA);
coelhoZ = limitar(coelhoZ, CAMPO_Z_MIN + RAIO_ESFERA, CAMPO_Z_MAX - RAIO_ESFERA);
```

---

## 7. Sistema de Câmeras e Alternância de Visualização (Item 4)

O enunciado exige no Item 4 a alternância entre duas câmeras pela tecla **`c`**:
1. Uma **visão geral panorâmica** da cena;
2. Uma **visão em primeira pessoa**, posicionada e orientada de acordo com o personagem.

### 7.1. Modo 1: Visão Geral da Cena
Inicialmente posicionada no ponto $(0.0, 10.0, 10.0)$ olhando para a origem $(0.0, 0.0, 0.0)$ com vetor vertical $(0.0, 1.0, 0.0)$. Proporciona uma visão tática completa de todo o gramado, permitindo antecipar o surgimento de vegetais e a chegada de predadores.

### 7.2. Modo 2: Visão em Primeira Pessoa (Olhos do Coelho)
Neste modo, a matriz `gluLookAt` é parametrizada em tempo real com base na posição e no ângulo de visão do coelho:
1. **Posição dos Olhos (`eye`):** A partir do centro $(X, Y, Z)$, a câmera é elevada na altura da cabeça ($+0.26$) e projetada ligeiramente à frente ($+0.35$ na direção $\vec{f}$):
   ```cpp
   camOlhoX = coelhoX + 0.35f * dirOlharX;
   camOlhoY = coelhoY + 0.26f;
   camOlhoZ = coelhoZ + 0.35f * dirOlharZ;
   ```
2. **Ponto de Mira (`target`):** Projetado 5 unidades à frente na linha de visada:
   ```cpp
   alvoOlharX = camOlhoX + dirOlharX * 5.0f;
   alvoOlharY = camOlhoY;
   alvoOlharZ = camOlhoZ + dirOlharZ * 5.0f;
   ```
3. **Chamada:** `gluLookAt(camOlhoX, camOlhoY, camOlhoZ, alvoOlharX, alvoOlharY, alvoOlharZ, 0.0, 1.0, 0.0);`

### 7.3. Ocultação Inteligente do Modelo em Primeira Pessoa
Se o coelho fosse desenhado enquanto a câmera ocupa o interior de sua própria cabeça, os polígonos faciais estariam mais próximos do que o plano de corte (*near clipping plane* = $0.1$), obstruindo a visão do jogador com artefatos visuais.  
No `display()`, o modelo `drawRabbit()` só é renderizado quando `cameraPrimeiraPessoa == false`. Isso entrega uma experiência em primeira pessoa limpa e imersiva.

### 7.4. Inclinação Vertical em Primeira Pessoa - Olhar para Cima (Item 8 - Bonificação)
O enunciado propõe como item adicional de bonificação:
> *"Como item adicional de bonificação, implemente uma modificação na visão do personagem principal, permitindo que, quando estiver na visão em primeira pessoa, o jogador possa olhar para cima, alterando a orientação da câmera verticalmente. A implementação deverá respeitar a posição e a orientação do personagem e ser integrada à dinâmica de movimentação e visualização já desenvolvida."*

#### a) Modelagem Matemática do Ângulo de Pitch (Arfagem):
Seja $\theta = \text{anguloCoelho}$ o ângulo horizontal de rotação do coelho (*yaw*) e $\phi = \text{pitchOlharPrimeiraPessoa}$ a inclinação vertical da câmera (*pitch*), ambos em graus convertidos para radianos.  
O vetor unitário tridimensional de visada $\vec{d} = (d_X, d_Y, d_Z)$ no espaço 3D é modelado por coordenadas esféricas:
$$d_X = \cos(\phi) \cdot \cos(\theta)$$
$$d_Y = \sin(\phi)$$
$$d_Z = \cos(\phi) \cdot (-\sin(\theta))$$

Essa formulação garante que:
- O vetor $\vec{d}$ tem norma estritamente unitária ($\|\vec{d}\| = 1.0$) para qualquer par $(\theta, \phi)$;
- Quando $\phi = 0^\circ$, o vetor é horizontal puro $(\cos\theta, 0, -\sin\theta)$;
- Conforme $\phi$ aumenta para cima ($>0^\circ$), o componente vertical $d_Y = \sin(\phi)$ se eleva, permitindo que o jogador observe o céu diretamente e localize a ave de rapina durante o voo rasante;
- Quando o coelho gira no próprio eixo com as setas Esquerda/Direita, o ângulo $\theta$ muda e o olhar vertical $\phi$ gira em conjunto para a nova orientação corporal;
- Quando o coelho corre ou pula, a posição de origem dos olhos segue o personagem em $(X, Y, Z)$, integrando-se à física do salto e das passadas.

#### b) Limites Angulares e Prevenção de Singularidades:
Para evitar colinearidade exata entre o vetor de visada $\vec{d}$ e o vetor Up $(0, 1, 0)$ no `gluLookAt` (o que geraria produto vetorial nulo e divisão por zero na matriz de visualização), o ângulo de pitch é limitado ao intervalo seguro:
- **Limite Superior:** $+85.0^\circ$ (quase zênite, suficiente para enxergar o céu verticalmente);
- **Limite Inferior:** $-30.0^\circ$ (permite baixar a vista em direção ao gramado para enxergar vegetais próximos).

#### c) Controles e Atalhos:
- **Teclas `W` ou `Page Up`:** Inclina a câmera verticalmente para cima ($+5^\circ$ por toque);
- **Teclas `S` ou `Page Down`:** Inclina a câmera verticalmente para baixo ($-5^\circ$ por toque);
- **Tecla `X`:** Centraliza instantaneamente o olhar de volta à linha do horizonte ($0^\circ$).
- **Feedback no HUD:** Quando em primeira pessoa, o topo direito da tela exibe o ângulo de inclinação em tempo real: `Camera: 1a Pessoa (Olhar: +45 deg) [C] [W/S: olhar]`.

---

## 8. Rotação Orbital da Câmera na Visão Geral (Item 5)

O Item 5 do enunciado especifica:
> *"Quando a visão geral estiver acionada, permita que, ao pressionar a tecla 'r', a posição da câmera seja rotacionada em relação ao eixo Y, possibilitando observar a cena a partir de diferentes ângulos."*

### 8.1. Fundamentação Matemática da Órbita
A posição original da visão geral é $(0, 10, 10)$, o que define:
- Altura constante: $Y = 10.0$;
- Raio orbital horizontal no plano $XZ$: $R = \sqrt{0^2 + 10^2} = 10.0$.

Ao rotacionar em torno do eixo vertical $Y$ por um ângulo $\theta_{\text{cam}}$:
$$\begin{pmatrix} X \\ Y \\ Z \end{pmatrix} = \begin{pmatrix} \cos\theta & 0 & \sin\theta \\ 0 & 1 & 0 \\ -\sin\theta & 0 & \cos\theta \end{pmatrix} \begin{pmatrix} 0 \\ 10 \\ 10 \end{pmatrix} = \begin{pmatrix} 10 \cdot \sin\theta \\ 10 \\ 10 \cdot \cos\theta \end{pmatrix}$$

Coordenadas paramétricas da câmera:
$$X_{\text{cam}} = R \cdot \sin(\theta_{\text{cam}})$$
$$Y_{\text{cam}} = 10.0$$
$$Z_{\text{cam}} = R \cdot \cos(\theta_{\text{cam}})$$

- Para $\theta = 0^\circ \implies (0, 10, 10)$ (posição padrão do Item 4);
- Para $\theta = 90^\circ \implies (10, 10, 0)$ (visão lateral direita);
- Para $\theta = 180^\circ \implies (0, 10, -10)$ (visão traseira da cena);
- Para $\theta = 270^\circ \implies (-10, 10, 0)$ (visão lateral esquerda).

O ponto de mira permanece na origem $(0, 0, 0)$ e o vetor "para cima" é $(0, 1, 0)$. Como a inclinação da visada é constante em $45^\circ$, nunca ocorre colinearidade entre o vetor de visão e o vetor Up, eliminando qualquer risco de *Gimbal Lock*.

### 8.2. Regra de Acionamento e HUD
- **Controle:** No `keyboard_callback`, ao pressionar `'r'` ou `'R'`, o ângulo avança em passos de $5^\circ$ (`anguloCameraGeral += 5.0f`), permitindo rotações com toques rápidos ou segurando a tecla para órbita contínua.
- **Bloqueio em 1ª Pessoa:** A rotação é condicionada a `if (!cameraPrimeiraPessoa)`. Se o jogador estiver na visão em primeira pessoa, a tecla não move a câmera geral, mantendo seu ângulo congelado até que se retorne à visão panorâmica.
- **Feedback Visual:** O HUD exibe dinamicamente o ângulo atual no topo direito:  
  `Camera: Geral (45 deg) [C] [R: girar]`

---

## 9. Predadores Tridimensionais: Raposa e Ave de Rapina

### 9.1. Raposa (Predador Terrestre)
A raposa patrulha a superfície da grama correndo horizontalmente pelo campo:
- **Modelagem 3D:** Tronco esguio em tom laranja vivo $(0.95, 0.45, 0.10)$, peito e ventre com mancha branca, pescoço inclinado a $+30^\circ$, cabeça afilada com bochechas brancas e trufa preta, orelhas triangulares alertas e 4 patas com meias escuras apoiadas no solo.
- **Cauda Articulada de Dupla Fase:** Dividida em dois elipsoides (base laranja e ponta branca pura). Cada segmento oscila com frequências e amplitudes senoidais independentes (`foxTailPhase` e `foxTailTipPhase`), conferindo ondulação natural.
- **Trote Diagonal de Canídeo:** As patas operam em pares cruzados simétricos (Grupo A e Grupo B) com base em $\sin(\text{foxWalkPhase})$.
- **Mecânica de Esquiva e Dano:** A raposa corre de $-X$ para $+X$ em profundidade $Z$ sorteada. Se o coelho pular alto ($Y > 1.35$), ele passa por cima da raposa em segurança; se colidir no nível do chão, perde 1 vida (com trava booleana para evitar danos múltiplos em uma mesma travessia).

### 9.2. Ave de Rapina (Predador Aéreo)
A ave de rapina ataca descendo velozmente dos céus em um mergulho calculado:
- **Modelagem 3D:** Corpo fusiforme aerodinâmico marrom escuro com ventre claro, crista de penachos escuros, bico adunco modelado com `glutSolidCone` inclinado para baixo em formato de gancho, olhos ferozes amarelos com pupila preta e reflexo branco, e garras amarelas recurvadas recolhidas no ventre.
- **Asas Tridimensionais com Envergadura no Eixo Z:** As asas abrem-se lateralmente em $\pm Z$. O bater de asas opera com rotação tridimensional em torno do eixo longitudinal $X$ (`wingAngle = sin(avePhase) * 38.0f`), simulando o voo real de um falcão.
- **Trajetória Parabólica 3D:** A ave surge em $X = 14.0$ e $Y = 7.5$, grava a posição do coelho no instante do surgimento $(X_{\text{alvo}}, Z_{\text{alvo}})$ e executa um rasante guiado pela função quadrática:
  $$Y(X) = a(X - X_{\text{alvo}})^2 + 0.55$$
  onde $a = (7.5 - 0.55) / (14.0 - X_{\text{alvo}})^2$.
- **Inclinação Dinâmica (Pitch):** A cada frame, o código calcula a derivada instantânea da parábola:
  $$\frac{dY}{dX} = 2a(X - X_{\text{alvo}})$$
  $$\text{inclinacao} = \arctan\left(\frac{dY}{dX}\right) \cdot \frac{180}{\pi}$$
  A ave aponta o bico para baixo na descida, nivela-se no rasante e aponta para cima na subida de retorno aos céus.
- **Sombra Dinâmica de Altitude:** No solo, a sombra da ave é desenhada em $(X_{\text{ave}}, Z_{\text{ave}})$. Conforme ela mergulha, a sombra reduz seu raio de $0.70$ para $0.35$ e intensifica sua opacidade de $0.12$ para $0.40$, avisando o jogador de que um ataque aéreo está em curso.
- **Esquiva 3D:** O jogador pode se esquivar manobrando no plano $XZ$ (saindo da coordenada de profundidade $Z_{\text{alvo}}$ ou acelerando em $X$).

---

## 10. Vegetais de Bonificação (Power-Ups 3D)

Os três vegetais foram construídos combinando sólidos GLUT e mantendo uma rotação contínua sobre o eixo vertical $Y$ (`vegetalGiro`):
1. **Cenoura (Turbo de Velocidade):** Corpo cônico laranja (`glutSolidCone`) invertido com três anéis de relevo escuros (`glutSolidTorus`) e ramagem de quatro cones verdes abertos em leque. Concede velocidade $4\times$ maior e trote acelerado por tempo limitado.
2. **Alface (Recuperação de Vida):** Modelada em roseta côncava com 6 folhas externas verde-escuras, 5 intermediárias mais claras e miolo esférico. Recupera 1 vida do coelho (máximo de 3).
3. **Rabanete (Super Pulo):** Bulbo esférico carmim brilhante, raiz cônica fina branca voltada para baixo e folhas verdes no topo. Concede pulo elevado de até $Y = 3.5$.

Os vegetais surgem na borda direita do mundo com $Z$ aleatório, divididos probabilisticamente entre vegetais no solo ($70\%$) e vegetais aéreos flutuantes ($30\%$), exigindo o salto para coleta.

---

## 11. Sistema de Colisões Volumétricas 3D e Interface (HUD)

### 11.1. Detecção de Colisão Euclidiana em 3 Eixos
No jogo 2D, as colisões utilizavam círculos de distância plana $\sqrt{\Delta X^2 + \Delta Y^2}$. No 3D, a detecção opera no espaço tridimensional com esferas delimitadoras:
$$\text{distância} = \sqrt{(X_1 - X_2)^2 + (Y_1 - Y_2)^2 + (Z_1 - Z_2)^2}$$
- **Coleta de Vegetais:** Se $\text{distância} \le 1.1$, o item é capturado, aciona o efeito sonoro/gráfico e aplica o temporizador de bônus.
- **Colisão com Predadores:** Se $\text{distância} \le 1.0$ (raposa) ou $\le 0.95$ (ave) e o coelho não estiver em altitude segura de esquiva, desconta-se 1 vida.

### 11.2. Interface e HUD em Projeção Ortogonal 2D
O painel informativo com vidas, alertas de bônus e instruções de teclas foi implementado com a técnica de sobreposição ortográfica 2D:
1. Conclui-se a renderização de toda a cena 3D;
2. Desabilitam-se `GL_DEPTH_TEST` e `GL_LIGHTING`;
3. Chaveia-se a matriz de projeção para coordenadas de tela em pixels:
   ```cpp
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   gluOrtho2D(0, janelaLargura, 0, janelaAltura);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   ```
4. Renderizam-se os textos com `glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ...)`;
5. Restauram-se as matrizes e reativam-se o Z-buffer e a iluminação.

---

## 12. Mapeamento Completo de Controles e Atalhos

| Tecla / Comando | Função no Jogo | Detalhes do Funcionamento |
| :---: | :--- | :--- |
| **Seta Cima** | Avançar para frente | Desloca o coelho na direção do seu vetor frontal $(\cos\theta, -\sin\theta)$ |
| **Seta Baixo** | Marcha a ré | Desloca o coelho no sentido oposto ao seu olhar a $75\%$ da velocidade |
| **Seta Esquerda** | Rotacionar para a esquerda | Gira o coelho no próprio eixo vertical $Y$ no sentido anti-horário |
| **Seta Direita** | Rotacionar para a direita | Gira o coelho no próprio eixo vertical $Y$ no sentido horário |
| **Barra de Espaço** | Pular | Executa o salto vertical, recolhe patinhas e encolhe a sombra no solo |
| **C** | Alternar Câmera | Chaveia entre Visão Geral elevada e Visão em 1ª Pessoa nos olhos do coelho |
| **R** | Rotacionar Visão Geral | Orbita a câmera da Visão Geral em $360^\circ$ ao redor do eixo $Y$ (passos de $5^\circ$) |
| **W ou Page Up** | Olhar para Cima *(1ª Pessoa)* | Inclina a visão verticalmente para cima até $+85^\circ$ (permite avistar a ave no céu) |
| **S ou Page Down** | Olhar para Baixo *(1ª Pessoa)* | Inclina a visão verticalmente para baixo até $-30^\circ$ |
| **X** | Nivelar Olhar *(1ª Pessoa)* | Centraliza instantaneamente o olhar vertical na linha do horizonte ($0^\circ$) |
| **F** | *(Teste)* Invocar Raposa | Dispara a travessia imediata da raposa pelo campo para testes |
| **A** | *(Teste)* Invocar Ave | Dispara o mergulho imediato da ave de rapina mirando no coelho |
| **V** | *(Teste)* Reduzir 1 Vida | Subtrai uma vida intencionalmente para possibilitar testar a cura da alface |
| **ESC** | Sair do Jogo | Encerra a aplicação imediatamente |

---

## 13. Instruções de Compilação e Execução

### No Windows (MSYS2 / MinGW / TDM-GCC):
Abra o prompt de comando ou PowerShell na pasta do projeto e compile:
```powershell
g++ main.cpp -o main.exe -lfreeglut -lglu32 -lopengl32
./main.exe
```

### No Linux (Ubuntu / Debian / Fedora):
Certifique-se de que os pacotes de desenvolvimento do FreeGLUT e GLU estejam instalados (`sudo apt install freeglut3-dev libglu1-mesa-dev`):
```bash
g++ -o main main.cpp -lglut -lGLU -lGL -lm
./main
```

---

## 14. Conclusão

A migração do jogo da Etapa 1 para o ambiente 3D atingiu plenamente todos os objetivos pedagógicos e técnicos da disciplina de Computação Gráfica. A transição não se limitou a acrescentar coordenadas de profundidade, mas implementou uma estrutura hierárquica completa de sólidos GLUT com normais consistentes, iluminação física no espaço do mundo, sombras dinâmicas projetadas, câmeras com alternância de perspectiva e órbita suave, controle orientado por vetores trigonométricos e inteligência de trajetória realista para os predadores.
