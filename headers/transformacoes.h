#ifndef TRANSFORMACOES_H
#define TRANSFORMACOES_H

#include "geometria.h"

float calcularDistancia(Vertice v1, Vertice v2);
void tentarSelecionar(int x, int y);
void excluirObjetos();
void rotacionarObjetos(float anguloG);
void transladarObjetos(float dx, float dy);
void refletirObjetos(bool horizontal, bool vertical);
void escalarObjetos(float fator);
void cisalharObjetos(float ciX, float ciY);

#endif // TRANSFORMACOES_H