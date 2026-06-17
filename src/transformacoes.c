#include "../headers/transformacoes.h"
#include "../headers/global.h"
//#include "../headers/geometria.h"
#include <cmath>
#include <iostream>


float calcularDistancia(Vertice v1, Vertice v2){
    return std::sqrt(std::pow(v2.x - v1.x, 2) + std::pow(v2.y - v1.y, 2));
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
                std::cout << "Ponto deselecionado em (" << x << ", " << y << ")" << std::endl;
            }else{
                p.selecionado = true;
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
                std::cout << "Reta deselecionada em (" << x << ", " << y << ")" << std::endl;
            }else{
                r.selecionado = true;
                std::cout << "Reta selecionada em (" << x << ", " << y << ")" << std::endl;
            }
            return;
        }
    }

    // verifica se algum polígono foi selecionado
    for(auto &p: listaPoligonos){
        int qtdVertices = p.vertices.size();
        for(int i = 0; i < qtdVertices; i++){
            // pega dois vértices consecutivos do polígono
            Vertice v1 = p.vertices[i];
            Vertice v2 = p.vertices[(i + 1) % qtdVertices];
            // verifica se o clique está próximo o suficiente da reta
            float d1 = calcularDistancia(v1, cliqueMouse);
            float d2 = calcularDistancia(v2, cliqueMouse);
            float tamReta = calcularDistancia(v1, v2);
            if ((d1 + d2 - tamReta) <= tolerancia){
                // inverte o estado de seleção do polígono
                if(p.selecionado){
                    p.selecionado = false;
                    std::cout << "Poligono deselecionado em (" << x << ", " << y << ")" << std::endl;
                }else{
                    p.selecionado = true;
                    std::cout << "Poligono selecionado em (" << x << ", " << y << ")" << std::endl;
                }
                return;
            }   
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
void rotacionarObjetos(float anguloG){
    // converte o ângulo de graus para radianos
    float anguloR = anguloG * (3.14159265f / 180.0f);
    // calcula seno e cosseno do ângulo
    float cosA = std::cos(anguloR);
    float senA = std::sin(anguloR);

    // aplica a rotação dos pontos em torno da origem
    for(auto &p: listaPontos){
        if(p.selecionado){
            float xAnterior = p.v.x;
            float yAnterior = p.v.y;
            // aplica a fórmula de rotação
            p.v.x = xAnterior * cosA - yAnterior * senA;
            p.v.y = xAnterior * senA + yAnterior * cosA;
        }
    }

    // aplica rotação nas retas
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

    // aplica rotação nos polígonos
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
