#include "../headers/transformacoes.h"
#include "../headers/Globais.h"
#include <GL/glut.h>
#include <cmath>
#include <iostream>


float calcularDistancia(Vertice v1, Vertice v2){
    return std::sqrt(std::pow(v2.x - v1.x, 2) + std::pow(v2.y - v1.y, 2));
}

// função que verifica se um polígono foi selecionado utilizando o algorítmo de ray casting
bool cliqueDentroPoligono(float xClique, float yClique, const std::vector<Vertice> &vertices){
    bool interior = false;
    int numVertices = vertices.size();

    int j = numVertices - 1;
    for(int i = 0; i < numVertices; i++){
        // pega as coordenadas de dois vértices consecutivos
        float xi = vertices[i].x, yi = vertices[i].y;
        float xj = vertices[j].x, yj = vertices[j].y;

        // verifica se o eixo y do clique cruza a linha formada pelos vértices
        // se a coordenada y do clique não estiver abaixo ou acima de ambos os vértices, significa que cruza a linha em algum ponto
        bool tocaAresta = ((yi > yClique) != (yj > yClique));
        if(tocaAresta){
            // calcula a coordenada x de onde ocorreu o toque na aresta
            float xToque = (xj - xi) * (yClique - yi) / (yj - yi) + xi;
            // verifica se o toque ocorreu a direita do clique
            if (xClique < xToque){
                interior = !interior; // inverte o estado de interioridade
            }
        }
        // atualiza o vértice anterior para o vértice atual
        j = i;
    }
    return interior;
}

// função para tentar selecionar um objeto com o mouse
void tentarSelecionar(int x, int y){
    Vertice cliqueMouse = {float(x), float(y)};
    float tolerancia = 5.0f;

    // verifica se algum vértice foi selecionado
    for(auto &p: listaPontos){
        if(calcularDistancia(p.v, cliqueMouse) < tolerancia){
            // inverte o estado de seleção do vértice
            if(p.selecionado){
                p.selecionado = false;
                objetosSelecionados--;
                std::cout << "Ponto deselecionado em (" << x << ", " << y << ")" << std::endl;
            }else{
                p.selecionado = true;
                objetosSelecionados++;
                std::cout << "Ponto selecionado em (" << x << ", " << y << ")" << std::endl;
            }
            return;
        }
    }

    // verifica se alguma reta foi selecionada
    for(auto &r: listaRetas){
        float d1 = calcularDistancia(r.v1, cliqueMouse);
        float d2 = calcularDistancia(r.v2, cliqueMouse);
        float tamReta = calcularDistancia(r.v1, r.v2);
        // verifica se o clique está próximo o suficiente da reta
        if ((d1 + d2 - tamReta) <= tolerancia){
            // inverte o estado de seleção da reta
            if(r.selecionado){
                r.selecionado = false;
                objetosSelecionados--;
                std::cout << "Reta deselecionada em (" << x << ", " << y << ")" << std::endl;
            }else{
                r.selecionado = true;
                objetosSelecionados++;
                std::cout << "Reta selecionada em (" << x << ", " << y << ")" << std::endl;
            }
            return;
        }
    }

    // verifica se algum polígono foi selecionado
    for(auto &p: listaPoligonos){
        // chama a função que verifica se o clique ocorreu no interior do polígono
        if(cliqueDentroPoligono(cliqueMouse.x, cliqueMouse.y, p.vertices)){
            // inverte o estado de seleção do polígono
            if(p.selecionado){
                p.selecionado = false;
                objetosSelecionados--;
                std::cout << "Poligono deselecionado em (" << x << ", " << y << ")" << std::endl;
            }else{
                p.selecionado = true;
                objetosSelecionados++;
                std::cout << "Poligono selecionado em (" << x << ", " << y << ")" << std::endl;
            }
            return;
        }
    }
}

// função para excluir objetos selecionados
void excluirObjetos(){
    // percorre a lista de pontos
    for(auto it = listaPontos.begin(); it != listaPontos.end();){
        if(it->selecionado){
            it = listaPontos.erase(it);
            std::cout << "Ponto excluido em (" << it->v.x << ", " << it->v.y << ")" << std::endl;
        }else{
            ++it;
        }
    }

    // percorre a lista de retas
    for(auto it = listaRetas.begin(); it != listaRetas.end();){
        if(it->selecionado){
            it = listaRetas.erase(it);
            std::cout << "Reta excluida" << std::endl;
        }else{
            ++it;
        }
    }

    // percorre a lista de polígonos
    for(auto it = listaPoligonos.begin(); it != listaPoligonos.end();){
        if(it->selecionado){
            it = listaPoligonos.erase(it);
            std::cout << "Poligono excluido" << std::endl;
        }else{
            ++it;
        }
    }
}

// função para rotacionar objetos
void rotacionarObjetos(float anguloG, bool pontos, bool retas, bool poligonos){
    // converte o ângulo de graus para radianos
    float anguloR = anguloG * (3.14159265f / 180.0f);
    // calcula seno e cosseno do ângulo
    float cosA = std::cos(anguloR);
    float senA = std::sin(anguloR);

    // aplica a rotação dos pontos em torno da origem
    if(pontos){
        for(auto &p: listaPontos){
            if(p.selecionado){
                float xAnterior = p.v.x;
                float yAnterior = p.v.y;
                // aplica a fórmula de rotação
                p.v.x = xAnterior * cosA - yAnterior * senA;
                p.v.y = xAnterior * senA + yAnterior * cosA;
            }
        }
    }

    // aplica rotação nas retas
    if(retas){
        for(auto &r: listaRetas){
            if(r.selecionado){
                // calcula o centro da reta
                float cx = (r.v1.x + r.v2.x) / 2.0f;
                float cy = (r.v1.y + r.v2.y) / 2.0f;

                // aplica translação + rotação + translação inversa no vértice 1
                float x1 = r.v1.x;
                float y1 = r.v1.y;
                r.v1.x = (x1 - cx) * cosA - (y1 - cy) * senA + cx;
                r.v1.y = (x1 - cx) * senA + (y1 - cy) * cosA + cy;
            
                // aplica translação + rotação + translação inversa no vértice 2
                float x2 = r.v2.x;
                float y2 = r.v2.y;
                r.v2.x = (x2 - cx) * cosA - (y2 - cy) * senA + cx;
                r.v2.y = (x2 - cx) * senA + (y2 - cy) * cosA + cy;
            }
        }
    }

    // aplica rotação nos polígonos
    if(poligonos){
        for(auto &p: listaPoligonos){
            if(p.selecionado && !p.vertices.empty()){
                // calcula o centro do polígono
                float cx = 0, cy = 0;
                for(const auto &v: p.vertices){
                    cx += v.x;
                    cy += v.y;
                }
            
                cx /= p.vertices.size();
                cy /= p.vertices.size();

                // aplica translação + rotação + translação inversa em cada vértice do polígono
                for(auto &v: p.vertices){
                    float xAnterior = v.x;
                    float yAnterior = v.y;
                    v.x = (xAnterior - cx) * cosA - (yAnterior - cy) * senA + cx;
                    v.y = (xAnterior - cx) * senA + (yAnterior - cy) * cosA + cy;
                }
            }
        }
    }
}

void transladarObjetos(float dx, float dy){
    // aplica translação nos pontos
    for(auto &p: listaPontos){
        if(p.selecionado){
            p.v.x += dx;
            p.v.y += dy;
        }
    }

    // aplica translação nas retas
    for(auto &r: listaRetas){
        if(r.selecionado){
            r.v1.x += dx;
            r.v1.y += dy;
            r.v2.x += dx;
            r.v2.y += dy;
        }
    }

    // aplica translação nos polígonos
    for(auto &p: listaPoligonos){
        if(p.selecionado){
            for(auto &v: p.vertices){
                v.x += dx;
                v.y += dy;
            }
        }
    }
}

// função para aumentar/diminuir a escala de objetos
void escalarObjetos(float fator){
    // aplica transformação nas retas
    for(auto &r: listaRetas){
        if(r.selecionado){
            // calcula o centro da reta
            float cx = (r.v1.x + r.v2.x) / 2.0f;
            float cy = (r.v1.y + r.v2.y) / 2.0f;

            // aplica translação + escalonamento + translação inversa no vértice 1
            r.v1.x = (r.v1.x - cx) * fator + cx;
            r.v1.y = (r.v1.y - cy) * fator + cy;

            // aplica translação + escalonamento + translação inversa no vértice 2
            r.v2.x = (r.v2.x - cx) * fator + cx;
            r.v2.y = (r.v2.y - cy) * fator + cy;
        }
    }

    // aplica a transformação nos polígonos
    for(auto &p: listaPoligonos){
        if(p.selecionado && !p.vertices.empty()){
            // calcula o centro do polígono
            float cx = 0;
            float cy = 0;
            for(const auto &v: p.vertices){
                cx += v.x;
                cy += v.y;
            }
            cx /= p.vertices.size();
            cy /= p.vertices.size();

            // aplica translação + escalonamento + translação inversa em cada vértice do polígono
            for(auto &v: p.vertices){
                v.x = (v.x - cx) * fator + cx;
                v.y = (v.y - cy) * fator + cy;
            }
        }
    }
}

// função para refletir objetos em relação ao seu centro
void refletirObjetos(bool horizontal, bool vertical){
    // reflete as retas selecionadas
    for(auto &r: listaRetas){
        if(r.selecionado){
            // calcula o centro da reta
            float cx = (r.v1.x + r.v2.x) / 2.0f;
            float cy = (r.v1.y + r.v2.y) / 2.0f;

            // verifica o sentido e aplica a reflexão
            if(horizontal){
                r.v1.x = (r.v1.x - cx) * -1.0f + cx;
                r.v2.x = (r.v2.x - cx) * -1.0f + cx;
            }
            if(vertical){
                r.v1.y = (r.v1.y - cy) * -1.0f + cy;
                r.v2.y = (r.v2.y - cy) * -1.0f + cy;
            }
        }
    }

    // reflete os polígonos selecionados
    for(auto &p: listaPoligonos){
        if(p.selecionado && !p.vertices.empty()){
            float cx = 0;
            float cy = 0;
            for(const auto &v: p.vertices){
                cx += v.x;
                cy += v.y;
            }
            cx /= p.vertices.size();
            cy /= p.vertices.size();

            // aplica translação + reflexão + translação inversa em cada vértice do polígono
            for(auto &v: p.vertices){
                float xAnterior = v.x;
                float yAnterior = v.y;
                if(horizontal){
                    v.x = (xAnterior - cx) * -1.0f + cx;
                }
                if(vertical){
                    v.y = (yAnterior - cy) * -1.0f + cy;
                }
            }
        }
    }
}


void cisalharObjetos(float ciX, float ciY){
    for(auto &r: listaRetas){
        if(r.selecionado){
            float cx = (r.v1.x + r.v2.x) / 2.0f;
            float cy = (r.v1.y + r.v2.y) / 2.0f;
            
            float x1 = r.v1.x;
            float y1 = r.v1.y;
            r.v1.x = (x1 - cx) + ciX * (y1 - cy) + cx;
            r.v1.y = (y1 - cy) + ciY * (x1 - cx) + cy;

            float x2 = r.v2.x;
            float y2 = r.v2.y;
            r.v2.x = (x2 - cx) + ciX * (y2 - cy) + cx;
            r.v2.y = (y2 - cy) + ciY * (x2 - cx) + cy;
        }
    }

    // aplica cisalhamento nos polígonos
    for(auto &p: listaPoligonos){
        if(p.selecionado && !p.vertices.empty()){
            float cx = 0;
            float cy = 0;
            for(const auto &v: p.vertices){
                cx += v.x;
                cy += v.y;
            }
            cx /= p.vertices.size();
            cy /= p.vertices.size();

            // aplica translação + cisalhamento + translação inversa em cada vértice do polígono
            for(auto &v: p.vertices){
                float x1 = v.x;
                float y1 = v.y;
                v.x = (x1 - cx) + ciX * (y1 - cy) + cx;
                v.y = (y1 - cy) + ciY * (x1 - cx) + cy;
            }
        }
    }
}


// função para gerenciar o mvimento dos objetos durante a animação
void loopAnimacao(int id){
    if(!animacao)
        return;

    bool bateuParedeX = false;
    bool bateuParedeY = false;

    for(auto &p: listaPontos){
        if(p.selecionado){
            p.v.x += velAnimacaoX;
            p.v.y += velAnimacaoY;

            if(p.v.x <= 0 || p.v.x >= larguraJanela)
                bateuParedeX = true;

            if(p.v.y <= 0 || p.v.y >= alturaJanela)
                bateuParedeY = true;
        }
    }
    
    for(auto &r: listaRetas){
        if(r.selecionado){
            // translada os vértices da reta
            r.v1.x += velAnimacaoX;
            r.v1.y += velAnimacaoY;
            r.v2.x += velAnimacaoX;
            r.v2.y += velAnimacaoY;

            // verifica os limites da tela
            if(r.v1.x <= 0 || r.v1.x >= larguraJanela || r.v2.x <= 0 || r.v2.x >= larguraJanela)
                bateuParedeX = true;
            
            if(r.v1.y <= 0 || r.v1.y >= alturaJanela || r.v2.y <= 0 || r.v2.y >= alturaJanela)
                bateuParedeY = true;
        }
    }

    for(auto &p: listaPoligonos){
        if(p.selecionado && !p.vertices.empty()){
            for(auto &v: p.vertices){
                v.x += velAnimacaoX;
                v.y += velAnimacaoY;

                if(v.x <= 0 || v.x >= larguraJanela)
                    bateuParedeX = true;

                if(v.y <= 0 || v.y >= alturaJanela)
                    bateuParedeY = true;
            }
        }
    }

    rotacionarObjetos(-0.2f, false, true, true);
    // inverte a velocidade se bater na parede
    if(bateuParedeX){
        velAnimacaoX *= -1;
    }
    if(bateuParedeY){ 
        velAnimacaoY *= -1;
    }

    glutPostRedisplay();
    glutTimerFunc(16, loopAnimacao, 0);
}
