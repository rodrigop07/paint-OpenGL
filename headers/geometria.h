#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include <vector>

// definição dos structs para a criação dos objetos geométricos
// struct Vertice representa um ponto no espaço 2D
struct Vertice{
    float x, y;
};

// struct Ponto representa um ponto selecionável, contendo um vértice e um estado de seleção
struct Ponto{
    Vertice v;
    bool selecionado = false;
};

// struct Reta representa uma linha entre dois vértices, contendo os vértices e um estado de seleção
struct Reta{
    Vertice v1, v2;
    bool selecionado = false;
};

// struct Poligono representa um polígono definido por uma lista de vértices, contendo os vértices e um estado de seleção
struct Poligono{
    std::vector<Vertice> vertices;
    bool selecionado = false;
};

enum ModoDesenho{
    SELECAO,
    PONTO,
    RETA,
    POLIGONO
};

#endif // GEOMETRIA_H