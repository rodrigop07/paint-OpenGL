#ifndef GLOBAL_H
#define GLOBAL_H

#include "geometria.h"
#include <vector>

// variáveis globais
extern int larguraJanela;
extern int alturaJanela;
extern ModoDesenho modoAtual;

extern std::vector<Ponto> listaPontos;
extern std::vector<Reta> listaRetas;
extern std::vector<Poligono> listaPoligonos;

extern int arrastoX;
extern int arrastoY;

extern int pontosReta;
extern Vertice cacheV1;
extern std::vector<Vertice> verticesPoligono;

#endif // GLOBAL_H