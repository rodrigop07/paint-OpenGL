# 🎨 paint-OpenGL: Editor de Gráficos Vetoriais 2D

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg?style=for-the-badge&logo=c%2B%2B)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3%2B-orange.svg?style=for-the-badge&logo=opengl)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)

O **paint-OpenGL** é um editor de gráficos vetoriais 2D rústico desenvolvido em C++ utilizando OpenGL e a biblioteca GLUT. O projeto foi concebido e implementado como trabalho prático da disciplina de **Computação Gráfica** da **Professora Luana**.

Com este editor, o usuário pode interagir diretamente com a tela para criar pontos, retas e polígonos, bem como aplicar transformações geométricas e salvar/carregar o progresso de sua arte vetorial.

---

## Funcionalidades Principais

*   **Criação de Formas Geométricas**:
    *   **Pontos**: Adicione pontos com precisão na tela.
    *   **Retas**: Conecte dois pontos para formar segmentos de reta.
    *   **Polígonos**: Crie formas complexas com qualquer quantidade de vértices.
*   **Seleção de Objetos (Algoritmo do Tiro)**:
    *   Clique diretamente nas formas para selecioná-las (pontos, retas e polígonos).
    *   Seleção de polígonos robusta baseada no algoritmo de **Ray Casting (Algoritmo do Tiro)**, permitindo selecionar com precisão até mesmo polígonos côncavos complexos.
*   **Fecho Convexo (Dividir e Conquistar)**:
    *   Transformação automática de polígonos côncavos em polígonos convexos.
    *   Implementação eficiente baseada no paradigma de **Divisão e Conquista** ($O(N \log N)$), realizando o particionamento dos vértices, cálculo de orientação por produto vetorial e união através de tangentes superiores e inferiores.
*   **Transformações Geométricas**:
    *   **Translação**: Mova os objetos selecionados livremente nas direções X e Y.
    *   **Rotação**: Gire as formas em torno de seus respectivos baricentros.
    *   **Escalonamento**: Aumente ou diminua o tamanho das formas.
    *   **Espelhamento / Reflexão**: Inverta as formas nos eixos horizontal e vertical.
    *   **Cisalhamento**: Deforme os objetos em relação aos eixos X e Y.
*   **Manipulação Avançada**:
    *   **Exclusão**: Delete as formas selecionadas com um atalho rápido.
    *   **Animação**: Inicie ou pare uma animação de deslocamento contínuo dos elementos selecionados.
    *   **Persistência**: Salve o estado atual da tela em arquivo (`progresso.txt`) e carregue-o posteriormente para continuar a edição.

---

## ⌨️ Controles de Teclado e Mouse

O aplicativo é operado por meio de atalhos rápidos do teclado e cliques do mouse.

### Modos de Operação

| Tecla / Ação | Modo | Descrição |
| :---: | :---: | :--- |
| `v` | **Ponto** | Entra no modo de criação de pontos. |
| `r` | **Reta** | Entra no modo de criação de segmentos de reta. |
| `p` | **Polígono** | Entra no modo de criação de polígonos. |
| `s` | **Seleção** | Entra no modo de seleção de objetos existentes. |

### Interação com o Mouse

| Ação | Modo Relacionado | Função |
| :---: | :---: | :--- |
| `Clique Esquerdo` | Ponto / Reta / Polígono | Adiciona um ponto ou vértice na coordenada do clique. |
| `Clique Esquerdo` | Seleção | Clica sobre um objeto para selecioná-lo/arrastá-lo. |
| `Clique Direito` | Polígono | Finaliza o polígono atual conectando o último vértice ao primeiro. |

### Edição e Transformações (Modo de Seleção)

| Atalho | Tipo de Transformação | Ação Realizada |
| :--- | :---: | :--- |
| `d` | **Exclusão** | Exclui todos os objetos que estiverem selecionados. |
| `Ctrl` + `←` / `→` | **Translação X** | Move os objetos selecionados para a esquerda / direita. |
| `Ctrl` + `↑` / `↓` | **Translação Y** | Move os objetos selecionados para cima / baixo. |
| `Shift` + `←` / `→` | **Rotação** | Rotaciona as formas para a esquerda ou direita em torno de seu baricentro. |
| `Shift` + `↑` / `↓` | **Escala** | Aumenta ou diminui a escala das formas. |
| `Alt` + `←` ou `→` | **Reflexão Vertical** | Espelha os objetos selecionados em relação ao eixo vertical (inversão horizontal). |
| `Alt` + `↑` ou `↓` | **Reflexão Horizontal** | Espelha os objetos selecionados em relação ao eixo horizontal (inversão vertical). |
| `Shift` + `a` / `s` | **Cisalhamento X** | Aplica distorção cisalhante no eixo X (negativo / positivo). |
| `Shift` + `z` / `x` | **Cisalhamento Y** | Aplica distorção cisalhante no eixo Y (negativo / positivo). |
| `c` | **Fecho Convexo** | Aplica o algoritmo de Divisão e Conquista para transformar o polígono côncavo selecionado em convexo. |

### Sistema e Animação

| Atalho | Ação | Descrição |
| :--- | :---: | :--- |
| `Ctrl` + `s` | **Salvar** | Salva a lista de pontos, retas e polígonos no arquivo `progresso.txt`. |
| `Ctrl` + `o` | **Carregar** | Carrega o progresso salvo a partir do arquivo `progresso.txt`. |
| `Ctrl` + `a` | **Animação** | Ativa ou desativa a animação de movimento contínuo dos objetos selecionados. |

---

## 🛠️ Como Instalar e Compilar

### Pré-requisitos

Para compilar e executar o projeto, você precisará de um compilador compatível com C++17 e das bibliotecas de desenvolvimento do **OpenGL**, **GLU** e **GLUT**.

#### 🐧 No Linux (Debian / Ubuntu / Mint)
Instale as dependências executando o comando abaixo no terminal:

```bash
sudo apt update
sudo apt install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev libglew-dev libglfw3-dev
```

#### 🪟 No Windows (via MinGW e MSYS2)
1. Instale o [MSYS2](https://www.msys2.org/).
2. Abra o terminal do MSYS2 (UCRT64 ou MINGW64) e instale o compilador e as bibliotecas:
```bash
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-freeglut mingw-w64-ucrt-x86_64-glew
```

---

### Passo a Passo de Compilação

#### Opção 1: Usando `make` (Recomendado)
Se tiver a ferramenta `make` instalada, basta executar:
```bash
# Compilar o executável
make

# Executar a aplicação
./app
```

#### Opção 2: Compilando Diretamente via `g++`
Se não possuir o `make` instalado, compile diretamente pelo terminal:

**No Linux:**
```bash
g++ -O3 -Wall -std=c++17 src/main.cpp src/eventos.cpp src/Globais.c src/transformacoes.c -o app -lglut -lGLU -lGL -lm
./app
```

**No Windows:**
```bash
g++ -O3 -Wall -std=std=c++17 src/main.cpp src/eventos.cpp src/Globais.c src/transformacoes.c -o app.exe -lfreeglut -lglu32 -lopengl32 -lgdi32
app.exe
```

---

## 🚀 Como Usar: Exemplos Práticos

1.  **Desenhando um Triângulo (Polígono):**
    *   Pressione a tecla `p` para entrar no modo polígono.
    *   Dê 3 cliques com o **botão esquerdo do mouse** na tela para posicionar os 3 vértices do triângulo.
    *   Dê um clique com o **botão direito do mouse** para finalizar a forma.
2.  **Transladando e Rotacionando o Triângulo:**
    *   Pressione a tecla `s` para ativar a seleção.
    *   Clique em qualquer ponto interno do triângulo criado (ele ficará destacado).
    *   Mantenha pressionada a tecla `Ctrl` e use as setas do teclado (`←`, `→`, `↑`, `↓`) para movê-lo pela tela.
    *   Mantenha pressionada a tecla `Shift` e utilize `←` ou `→` para rotacionar o triângulo suavemente.
3.  **Salvar e Abrir:**
    *   Ao desenhar diversas formas, pressione `Ctrl` + `s`. O arquivo `progresso.txt` será criado na raiz do projeto contendo os dados matemáticos de todas as formas na tela.
    *   Feche o editor, abra-o novamente e aperte `Ctrl` + `o` para restaurar o estado salvo.
4.  **Transformando um Polígono Côncavo em Convexo:**
    *   Pressione a tecla `p` (Modo Polígono).
    *   Desenhe uma forma côncava na tela (como uma estrela, uma letra "C" ou um "Pac-Man") dando vários cliques com o **botão esquerdo**, e finalize com o **botão direito**.
    *   Pressione a tecla `s` para ativar a seleção.
    *   Clique dentro da forma côncava para selecioná-la (o algoritmo do tiro validará o clique).
    *   Pressione a tecla `c` no teclado. A forma automaticamente esticará suas extremidades, ligando as tangentes e se convertendo em um polígono perfeitamente convexo!
    
---

## 📂 Estrutura do Código

```text
paint-OpenGL/
│
├── headers/                 # Arquivos de cabeçalho (.h)
│   ├── eventos.h            # Declarações dos callbacks de eventos GLUT (mouse/teclado)
│   ├── geometria.h          # Definições de estruturas (Vertice, Ponto, Reta, Poligono, ModoDesenho)
│   ├── Globais.h            # Declarações das variáveis de estado global do sistema
│   └── transformacoes.h     # Declarações de cálculos matemáticos e transformações geométricas
│
├── src/                     # Código-fonte principal (.cpp / .c)
│   ├── eventos.cpp          # Implementação das funções de renderização, mouse, teclado e arquivos
│   ├── Globais.c            # Inicialização das variáveis globais da aplicação
│   ├── main.cpp             # Ponto de entrada (main) e configuração inicial do ambiente GLUT
│   └── transformacoes.c     # Algoritmos de ray casting, exclusão e fórmulas das transformações
│
├── assets/                  # Imagens e recursos adicionais de documentação
│   └── interface_mockup.png # Captura de tela ilustrativa da interface
│
├── Makefile                 # Arquivo de automação de compilação multi-plataforma
└── README.md                # Esta documentação
```

---

## 💻 Tecnologias Utilizadas

*   **C++ (Padrão 17)**: Linguagem principal estruturada para manipulação eficiente de memória e vetores.
*   **OpenGL 3.3+**: API gráfica para renderização de primitivas em tempo real.
*   **GLUT (FreeGLUT)**: Biblioteca auxiliar para criação de janelas gráficas e gerenciamento facilitado de eventos do sistema operacional.

---

## 👥 Integrantes do Grupo

* **Jonathas Levi Pascoal Palmeira** - 2024.1 
* **Fabrício Elízio Lima dos Santos** - 2024.1
* **Otávio da Silva Ferreira** - 2024.1
* **Rodrigo Pinheiro Alcantra** - 2024.1
* **Rafael da Silva Souza** - 2024.1

---

## 📄 Licença

Este projeto é disponibilizado sob a licença **MIT**. Veja o arquivo de licença correspondente para mais detalhes.

---
*Trabalho prático desenvolvido para fins educativos na disciplina de Computação Gráfica ministrada pela **Professora Luana**.*
