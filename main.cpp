#include <iostream>
#include <GL/glut.h>
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
    bool selecionada = false;
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
int pontosReta = 0;
Vertice cacheV1;
std::vector<Vertice> verticesPoligono;

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

void handleMouse(int button, int state, int x, int y){
    // verifica se o botão esquerdo do mouse foi pressionado
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        // corrige o valor de y, a origem do sistemas operacional é diferente da origem do OpenGL
        int yCorrigido = alturaJanela - y; 
        // verifica o modo atual e cria o objeto correspondente
        if(modoAtual == PONTO){
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
                std::cout << "Primeiro vértice da reta definido em (" << x << ", " << yCorrigido << ")" << std::endl;
            }else if(pontosReta == 1){
                // segundo vértice da reta, cria e adiciona a lista
                Reta novaReta;
                novaReta.v1 = cacheV1;
                novaReta.v2.x = x;
                novaReta.v2.y = yCorrigido;
                // adiciona a reta à lista
                listaRetas.push_back(novaReta);
                std::cout << "Segundo vértice da reta definido em (" << x << ", " << yCorrigido << ")" << std::endl;
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
            std::cout << "Vértice do polígono definido em (" << x << ", " << yCorrigido << ")" << std::endl;
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
            std::cout << "Polígono criado" << std::endl;
            glutPostRedisplay();
        }
    }
}

void handleKeyboard(unsigned char key, int x, int y){
    // usa as teclas para alternar entre os modos de desenho
    if (key == 'q' || key == 'Q'){
        modoAtual = PONTO;
        std::cout << "Modo PONTO ativado" << std::endl;
    }else if(key == 'w' || key == 'W'){
        modoAtual = RETA;
        std::cout << "Modo RETA ativado" << std::endl;
    }else if(key == 'e' || key == 'E'){
        modoAtual = POLIGONO;
        std::cout << "Modo POLIGONO ativado" << std::endl;
    }
}


void init(int argc, char** argv, int largura, int altura){
    // carrega as bibliotecas do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // tamanho da janela
    glutInitWindowSize(largura, altura);
    //posição inicial da janela
    glutInitWindowPosition(100, 100);
    // título da janela
    glutCreateWindow("Paint 1.0");

    // inicia modo de matriz de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // configura uma projeção ortográfica 2d
    gluOrtho2D(0.0, largura, 0.0, altura);

    // função que será chamada sempre que a janela precisar ser redesenhada
    glutDisplayFunc(display);
    glutMouseFunc(handleMouse);
    glutKeyboardFunc(handleKeyboard);


    // deixa a janela aberta esperando eventos
    glutMainLoop();
};


int main(int argc, char** argv){
    init(argc, argv, larguraJanela, alturaJanela);


    return 0;
}



