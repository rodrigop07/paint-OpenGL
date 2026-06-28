#ifndef TRANSFORMACOES_H
#define TRANSFORMACOES_H

#include "geometria.h"
#include <vector>

float calcularDistancia(Vertice v1, Vertice v2);
bool cliqueDentroPoligono(float xClique, float yClique, const std::vector<Vertice> &vertices);
void tentarSelecionar(int x, int y);
void excluirObjetos();
void rotacionarObjetos(float anguloG, bool pontos, bool retas, bool poligonos);
void transladarObjetos(float dx, float dy);
void refletirObjetos(bool horizontal, bool vertical);
void escalarObjetos(float fator);
void cisalharObjetos(float ciX, float ciY);
void loopAnimacao(int id);

float orientacao(Vertice p, Vertice q, Vertice r);
std::vector<Vertice> mergeFechos(std::vector<Vertice>& esq, std::vector<Vertice>& dir);
std::vector<Vertice> divideEConquista(std::vector<Vertice>& pontos);
std::vector<Vertice> fechoConvexoDivConq(std::vector<Vertice> pontos);
void aplicarFechoConvexoSelecionados();

#endif // TRANSFORMACOES_H