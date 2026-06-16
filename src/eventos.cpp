#include "../headers/eventos.h"
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <numbers>

// definição das variáveis globais
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

// variáveis para rastrear o arrasto de objetos
int arrastoX = 0;
int arrastoY = 0;


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
            //r.v1.x = x1 + ciX * (y1 - cy);
            //r.v1.y = y1 + ciY * (x1 - cx);

            float x2 = r.v2.x;
            float y2 = r.v2.y;
            r.v2.x = (x2 - cx) + ciX * (y2 - cy) + cx;
            r.v2.y = (y2 - cy) + ciY * (x2 - cx) + cy;
            //r.v2.x = x2 + ciX * (y2 - cy);
            //r.v2.y = y2 + ciY * (x2 - cx);
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
                //v.x = x1 + ciX * (y1 - cy);
                //v.y = y1 + ciY * (x1 - cx);
            }
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
            arrastoX = x;
            arrastoY = yCorrigido;
            // atualiza a tela
            glutPostRedisplay();
        }else if(modoAtual == PONTO){
            // cria um novo e pega as coordenadas do clique do mouse
            Ponto novoPonto;
            novoPonto.v.x = x;
            novoPonto.v.y = yCorrigido;
            // adiciona o novo ponto à lista
            listaPontos.push_back(novoPonto);
            std::cout << "Ponto criado em (" << x << ", " << yCorrigido << ")" << std::endl;
            // redesenha a tela
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
    int modificador = glutGetModifiers();
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
        excluirObjetos();
        glutPostRedisplay();
    }

    if(modificador == GLUT_ACTIVE_SHIFT){
        if(key == 'a' || key == 'A'){
            cisalharObjetos(-0.1f, 0.0f);
            std::cout << "SHIFT + a pressionado, cisalhando objetos negativamente em x" << std::endl;
            glutPostRedisplay();
        }else if(key == 's' || key == 'S'){
            cisalharObjetos(0.1f, 0.0f);
            std::cout << "SHIFT + s pressionado, cisalhando objetos positivamente em x" << std::endl;
            glutPostRedisplay();
        }else if(key == 'z' || key == 'Z'){
            cisalharObjetos(0.0f, -0.1f);
            std::cout << "SHIFT + z pressionado, cisalhando objetos negativamente em y" << std::endl;
            glutPostRedisplay();
        }else if(key == 'x' || key == 'X'){
            cisalharObjetos(0.0f, 0.1f);
            std::cout << "SHIFT + x pressionado, cisalhando objetos positivamente em y" << std::endl;
            glutPostRedisplay();
        }
    }
}

// função para gerenciar o movimento ativo do mouse
void handleMotion(int x, int y){
    if(modoAtual != SELECAO)
        return;

    int yCorrigido = alturaJanela - y;

    // calcula a distância que o mouse se moveu em relação à posição anterior
    float dX = x - arrastoX;
    float dY = yCorrigido - arrastoY;

    // aplica translação nos objetos selecionados
    transladarObjetos(dX, dY);

    // atualiza a posição anterior do mouse
    arrastoX = x;
    arrastoY = yCorrigido;
    glutPostRedisplay();
}

// função para gerenciar eventos especiais do teclado, como teclas fora do padrão ASCII
void handleSpecialKeyboard(int key, int x, int y){
    if(modoAtual == SELECAO){
        // define o valor de translação a cada movimento
        float tPasso = 5.0f;
        // define o ângulo de rotação a cada movimento
        float rPasso = 5.0f;
        // define o fator de encolhimento a cada clique 
        float fatorEncolhimento = 0.9f;
        // define o fator de crescimento a cada clique
        float fatorCrescimento = 1.1f;

        // verifica se alguma tecla modificadora (shift, ctrl, alt) foi pressionada
        int modificador = glutGetModifiers();

        switch(key){
            case GLUT_KEY_LEFT:
                if(modificador == GLUT_ACTIVE_CTRL){
                    transladarObjetos(-tPasso, 0);
                    std::cout << "CTRL + Seta esquerda pressionada, transladando objetos" << std::endl;
                    glutPostRedisplay();
                }else if(modificador == GLUT_ACTIVE_SHIFT){
                    rotacionarObjetos(rPasso);
                    std::cout << "SHIFT + Seta esquerda pressionada, rotacionando objetos 5 graus para a esquerda" << std::endl;
                    glutPostRedisplay();
                }else if(modificador == GLUT_ACTIVE_ALT){
                    refletirObjetos(true, false);
                    std::cout << "ALT + Seta esquerda pressionada, refletindo objetos horizontalmente" << std::endl;
                    glutPostRedisplay();
                }
                break;
            case GLUT_KEY_UP:
                if(modificador == GLUT_ACTIVE_CTRL){
                    transladarObjetos(0, tPasso);
                    std::cout << "CTRL + Seta para cima pressionada, transladando objetos" << std::endl;
                    glutPostRedisplay();
                }else if(modificador == GLUT_ACTIVE_SHIFT){
                    escalarObjetos(fatorCrescimento);
                    std::cout << "SHIFT + Seta para cima pressionada, escalando objetos em 10%" << std::endl;
                    glutPostRedisplay();
                }else if(modificador == GLUT_ACTIVE_ALT){
                    refletirObjetos(false, true);
                    std::cout << "ALT + Seta para cima pressionada, refletindo objetos verticalmente" << std::endl;
                    glutPostRedisplay();
                }
                break;
            case GLUT_KEY_DOWN:
                if(modificador == GLUT_ACTIVE_CTRL){
                    transladarObjetos(0, -tPasso);
                    std::cout << "CTRL + Seta para baixo pressionada, transladando objetos" << std::endl;
                    glutPostRedisplay();
                }else if(modificador == GLUT_ACTIVE_SHIFT){
                    escalarObjetos(fatorEncolhimento);
                    std::cout << "SHIFT + Seta para baixo pressionada, encolhendo objetos em 10%" << std::endl;
                    glutPostRedisplay();
                }else if(modificador == GLUT_ACTIVE_ALT){
                    refletirObjetos(false, true);
                    std::cout << "ALT + Seta para baixo pressionada, refletindo objetos verticalmente" << std::endl;
                    glutPostRedisplay();
                }
                break;
            case GLUT_KEY_RIGHT:
                if(modificador == GLUT_ACTIVE_CTRL){
                    transladarObjetos(tPasso, 0);
                    std::cout << "CTRL + Seta direita pressionada, transladando objetos" << std::endl;
                    glutPostRedisplay();
                }else if(modificador == GLUT_ACTIVE_SHIFT){
                    rotacionarObjetos(-rPasso);
                    std::cout << "SHIFT + Seta direita pressionada, rotacionando objetos 5 graus para a direita" << std::endl;
                    glutPostRedisplay();
                }else if(modificador == GLUT_ACTIVE_ALT){
                    refletirObjetos(true, false);
                    std::cout << "ALT + Seta direita pressionada, refletindo objetos horizontalmente" << std::endl;
                    glutPostRedisplay();
                }
                break;
            default:
                break;
        }
    }
}
