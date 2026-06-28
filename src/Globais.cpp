#include "../headers/Globais.h"
#include "../headers/geometria.h"

// definições das variáveis globais
ModoDesenho modoAtual = SELECAO;
int larguraJanela = 800;
int alturaJanela = 600;

// lista para armazenar os pontos criados
std::vector<Ponto> listaPontos;
// lista para armazenar as retas criadas
std::vector<Reta> listaRetas;
// lista para armazenar os polígonos criados
std::vector<Poligono> listaPoligonos;

// variáveis temporárias para o desenho de retas e polígonos
// quantos vértices da reta foram inseridos
int pontosReta = 0;
// primeiro vértice da reta
Vertice cacheV1;
// lista de vértices temporária para o polígono
std::vector<Vertice> verticesPoligono;

// número de objetos selecionados
int objetosSelecionados = 0;
// variáveis da animação
bool animacao = false;
float velAnimacaoX = 1.0f;
float velAnimacaoY = 1.0f;

// variáveis para rastrear o arrasto de objetos
int arrastoX = 0;
int arrastoY = 0;