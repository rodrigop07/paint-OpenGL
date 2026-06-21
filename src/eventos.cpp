#include "../headers/Eventos.h"
#include "../headers/Globais.h"
#include "../headers/Geometria.h"
#include "../headers/Transformacoes.h"
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <fstream>


// função para salvar a cena em um arquivo
void salvarArquivo(){
    // prepara o arquivo para escrita
    std::ofstream arquivo("progresso.txt");

    if(!arquivo.is_open()){
        std:: cout << "ERRO: Nao foi possivel abrir o arquivo para escrita" << std::endl;
        return;
    }

    // salva os pontos
    arquivo << listaPontos.size() << "\n";
    for(const auto &p: listaPontos){
        arquivo << p.v.x << " " << p.v.y << "\n";
    }

    // salva as retas
    arquivo << listaRetas.size() << "\n";
    for(const auto &r: listaRetas){
        arquivo << r.v1.x << " " << r.v1.y << " " << r.v2.x << " " << r.v2.y << "\n";
    }

    // salva os polígonos
    arquivo << listaPoligonos.size() << "\n";
    for(const auto &p: listaPoligonos){
        arquivo << p.vertices.size() << "\n";
        // salva cada vértice do polígono
        for(const auto &v: p.vertices){
            arquivo << v.x << " " << v.y << "\n";
        }
    }

    // fecha o arquivo
    arquivo.close();
    std::cout << "Progresso salvo com sucesso em progresso.txt" << std::endl;
}

// função para carregar o arquivo e desenhar a cena
void abrirArquivo(){
    // prepara o arquivo para leitura
    std::ifstream arquivo("progresso.txt");

    if(!arquivo.is_open()){
        std::cout << "ERRO: Nao foi possivel abrir o arquivo para leitura" << std::endl;
        return;
    }

    // limpa os objetos atuais
    listaPontos.clear();
    listaRetas.clear();
    listaPoligonos.clear();

    // variável que será usada para ler a quantidade de cada tipo de objeto
    int qtd;

    // carrega os pontos
    arquivo >> qtd;
    for(int i = 0; i < qtd; i++){
        Ponto novoPonto;
        arquivo >> novoPonto.v.x >> novoPonto.v.y;
        listaPontos.push_back(novoPonto);
    }

    // carrega as retas
    arquivo >> qtd;
    for(int i = 0; i < qtd; i++){
        Reta novaReta;
        arquivo >> novaReta.v1.x >> novaReta.v1.y >> novaReta.v2.x >> novaReta.v2.y;
        listaRetas.push_back(novaReta);
    }

    // carrega os polígonos
    arquivo >> qtd;
    for(int i = 0; i < qtd; i++){
        Poligono novoPoligono;
        int qtdVertices;
        arquivo >> qtdVertices;

        for(int j = 0; j < qtdVertices; j++){
            Vertice novoVertice;
            arquivo >> novoVertice.x >> novoVertice.y;
            novoPoligono.vertices.push_back(novoVertice);
        }
        listaPoligonos.push_back(novoPoligono);
    }

    // fecha o arquivo
    arquivo.close();
    std::cout << "Progresso carregado com sucesso de progresso.txt" << std::endl;
    glutPostRedisplay();
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
        glBegin(GL_POLYGON);
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

    // exclui os objetos selecionados se estiver no modo seleção
    if(modoAtual == SELECAO && (key == 'd' || key == 'D')){
        excluirObjetos();
        glutPostRedisplay();
    }

    if(modificador == GLUT_ACTIVE_SHIFT){
        if(key == 'A'){
            cisalharObjetos(-0.1f, 0.0f);
            std::cout << "SHIFT + a pressionado, cisalhando objetos negativamente em x" << std::endl;
            glutPostRedisplay();
        }else if(key == 'S'){
            cisalharObjetos(0.1f, 0.0f);
            std::cout << "SHIFT + s pressionado, cisalhando objetos positivamente em x" << std::endl;
            glutPostRedisplay();
        }else if(key == 'Z'){
            cisalharObjetos(0.0f, -0.1f);
            std::cout << "SHIFT + z pressionado, cisalhando objetos negativamente em y" << std::endl;
            glutPostRedisplay();
        }else if(key == 'X'){
            cisalharObjetos(0.0f, 0.1f);
            std::cout << "SHIFT + x pressionado, cisalhando objetos positivamente em y" << std::endl;
            glutPostRedisplay();
        }
    }else if(modificador == GLUT_ACTIVE_CTRL){
        // 19 corresponde a tecla 'S' quando ctrl está pressionado
        if(key == 19){
            salvarArquivo();
            std::cout << "Ctrl + s pressionado, salvando progresso" << std::endl;
        // 15 corresponde a tecla 'O' quando ctrl está pressionado
        }else if(key == 15){
            abrirArquivo();
            std::cout << "Ctrl + o pressionado, abrindo arquivo" << std::endl;
        }else if(key == 1){
            animacao = !animacao;
            if(animacao && objetosSelecionados > 0){
                std::cout << "Animacao iniciada" << std::endl;
                glutTimerFunc(16, loopAnimacao, 0);
            }else if(!animacao && objetosSelecionados > 0){
                std::cout << "Animacao parada" << std::endl;
            }
        }
    }else{
        // usa as teclas para alternar entre os modos de desenho
        // troca para o modo ponto
        if(key == 'v' || key == 'V'){
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
                    rotacionarObjetos(rPasso, true, true, true);
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
                    rotacionarObjetos(-rPasso, true, true, true);
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
