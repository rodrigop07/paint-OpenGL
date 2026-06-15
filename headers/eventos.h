#ifndef EVENTOS_H
#define EVENTOS_H

#include "geometria.h"
#include <vector>

extern int larguraJanela;
extern int alturaJanela;
extern ModoDesenho modoAtual;

extern std::vector<Ponto> listaPontos;
extern std::vector<Reta> listaRetas;
extern std::vector<Poligono> listaPoligonos;

extern int pontosReta;
extern Vertice cacheV1;
extern std::vector<Vertice> verticesPoligono;

// Declaração das funções de callback da GLUT
void display();
void handleMouse(int button, int state, int x, int y);
void handleKeyboard(unsigned char key, int x, int y);

#endif // EVENTOS_H