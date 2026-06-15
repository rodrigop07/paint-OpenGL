#include "../headers/eventos.h"
#include <GL/glut.h>
#include <iostream>
#include <cmath>

// definição das variáveis globais
ModoDesenho modoAtual = SELECAO;
int larguraJanela = 1280;
int alturaJanela = 720;

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


// calcula a distância euclidiana entre dois vértices
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

void excluiObjetosSelecionados(){
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


// função para desenhar todos os objetos atuais na tela
void display(){
    // limpa o buffer de cor da tela
    glClear(GL_COLOR_BUFFER_BIT);

    // inicia modo de renderização de pontos únicos
    glBegin(GL_POINTS);
    // percorre a lista de pontos e desenha cada um
    for(const auto &p: listaPontos){
        glVertex2f(p.v.x, p.v.y);
    }
    // encerra o modo de renderização
    glEnd();

    // inicia modo de renderização de linhas
    glBegin(GL_LINES);
    // desenha cada reta da lista
    for(const auto &r: listaRetas){
        glVertex2f(r.v1.x, r.v1.y);
        glVertex2f(r.v2.x, r.v2.y);
    }
    glEnd();

    // percorre a lista de polígonos
    for(const auto &p: listaPoligonos){
        // inicia modo de renderização de polígonos
        glBegin(GL_LINE_LOOP);
        // desenha cada vértice do polígono
        for(const auto &v: p.vertices){
            glVertex2f(v.x, v.y);
        }
        glEnd();
    }

    // executa todos os comandos de desenho pendentes
    glFlush();  
}

// função para gerenciar os eventos do mouse
void handleMouse(int button, int state, int x, int y){
    // verifica se o botão esquerdo do mouse foi pressionado
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        // corrige o valor de y, a origem do sistemas operacional é diferente da origem do OpenGL
        int yCorrigido = alturaJanela - y; 
        // verifica o modo atual e 
        if(modoAtual == SELECAO){
            tentarSelecionar(x, yCorrigido);
            glutPostRedisplay();
        }else if(modoAtual == PONTO){
            // cria um novo e pega as coordenadas do clique do mouse
            Ponto novoPonto;
            novoPonto.v.x = x;
            novoPonto.v.y = yCorrigido;
            // adiciona o novo ponto à lista
            listaPontos.push_back(novoPonto);
            // redesenha a tela
            std::cout << "Ponto criado em (" << x << ", " << yCorrigido << ")" << std::endl;
            glutPostRedisplay();
        }else if(modoAtual == RETA){
            // primeiro vértice da reta, guarda o ponto em uma variável temporária
            if(pontosReta == 0){
                cacheV1.x = x;
                cacheV1.y = yCorrigido;
                // atualiza a variável para aguardar o próximo clique do mouse
                pontosReta = 1;
                std::cout << "Primeiro vertice da reta definido em (" << x << ", " << yCorrigido << ")" << std::endl;
            }else if(pontosReta == 1){
                // segundo vértice da reta, cria e adiciona a lista
                Reta novaReta;
                novaReta.v1 = cacheV1;
                novaReta.v2.x = x;
                novaReta.v2.y = yCorrigido;
                // adiciona a reta à lista
                listaRetas.push_back(novaReta);
                std::cout << "Segundo vertice da reta definido em (" << x << ", " << yCorrigido << ")" << std::endl;
                // reseta o contador de pontos
                pontosReta = 0;
                // redesenha a tela
                glutPostRedisplay();
            }
        }else if(modoAtual == POLIGONO){
            Vertice novoVertice;
            novoVertice.x = x;
            novoVertice.y = yCorrigido;
            // adiciona o novo vértice à lista temporária
            verticesPoligono.push_back(novoVertice);
            std::cout << "Vertice do poligono definido em (" << x << ", " << yCorrigido << ")" << std::endl;
            // redesenha a tela
            glutPostRedisplay();
        }
    }else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        // fecha o polígono e adiciona na lista de polígonos
        if(modoAtual == POLIGONO){
            Poligono novoPoligono;
            // recebe a lista de vértices temporária
            novoPoligono.vertices = verticesPoligono;
            // adiciona o polígono à lista de polígonos
            listaPoligonos.push_back(novoPoligono);
            // limpa a lista de vértices temporária
            verticesPoligono.clear();
            std::cout << "Poligono criado" << std::endl;
            glutPostRedisplay();
        }
    }
}

// função para gerenciar os eventos do teclado
void handleKeyboard(unsigned char key, int x, int y){
    // usa as teclas para alternar entre os modos de desenho
    // troca para o modo ponto
    if (key == 'v' || key == 'V'){
        modoAtual = PONTO;
        std::cout << "Modo PONTO ativado" << std::endl;
    // troca para o modo reta
    }else if(key == 'r' || key == 'R'){
        modoAtual = RETA;
        std::cout << "Modo RETA ativado" << std::endl;
    // troca para o modo polígono
    }else if(key == 'p' || key == 'P'){
        modoAtual = POLIGONO;
        std::cout << "Modo POLIGONO ativado" << std::endl;
    // troca para o modo seleção
    }else if(key == 's' || key == 'S'){
        modoAtual = SELECAO;
        std::cout << "Modo SELECAO ativado" << std::endl;
    }
    // exclui os objetos selecionados se estiver no modo seleção
    if(modoAtual == SELECAO && (key == 'd' || key == 'D')){
        excluiObjetosSelecionados();
        glutPostRedisplay();
    }
}
