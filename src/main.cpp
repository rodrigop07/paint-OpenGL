#include <GL/glut.h>
#include "../headers/eventos.h"

void init(int argc, char** argv, int largura, int altura){
    // carrega as bibliotecas do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // tamanho da janela
    glutInitWindowSize(largura, altura);
    //posição inicial da janela
    glutInitWindowPosition(100, 100);
    // título da janela
    glutCreateWindow("Paint 2.0");

    // inicia modo de matriz de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // configura uma projeção ortográfica 2d
    gluOrtho2D(0.0, largura, 0.0, altura);

    // função que será chamada sempre que a janela precisar ser redesenhada
    glutDisplayFunc(display);
    // função para eventos do mouse
    glutMouseFunc(handleMouse);
    // função para eventos do teclado
    glutKeyboardFunc(handleKeyboard);
    // função para eventos de movimento do mouse
    glutMotionFunc(handleMotion);
    // função para eventos de teclado especial
    glutSpecialFunc(handleSpecialKeyboard);

    // deixa a janela aberta esperando eventos
    glutMainLoop();
}


int main(int argc, char** argv){
    init(argc, argv, larguraJanela, alturaJanela);
    return 0;
}