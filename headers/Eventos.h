#ifndef EVENTOS_H
#define EVENTOS_H

// Declaração das funções de callback da GLUT
void display();
void handleMotion(int x, int y);
void handleMouse(int button, int state, int x, int y);
void handleKeyboard(unsigned char key, int x, int y);
void handleSpecialKeyboard(int key, int x, int y);

#endif // EVENTOS_H