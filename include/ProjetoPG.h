/*
-----------------------------------------------------------------------------
OpenGL Tutorial
VOXAR Labs
Computer Science Center - CIn
Federal University of Pernambuco - UFPE
http://www.cin.ufpe.br/~voxarlabs

@author Tullio Lucena - tjsl
@updated by George Oliveira - ghao
-----------------------------------------------------------------------------
*/

#ifndef _PROJETOPG_H_
#define _PROJETOPG_H_

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <math.h>
#include <gl/glut.h>

using namespace std;

// Constantes do programa
#define FPS			30	// Quantidade de atualizacoes por segundo do programa
#define ESC			27	// Valor da tecla ESC
#define IDLE		-2	// Nada a fazer
#define MODIFIED	-1	// A tela foi modificada

class Point {

public:
	GLfloat x, y, z; // Coordenadas

	void setCoordXY(GLfloat xp, GLfloat yp) {
		x = xp;
		y = yp;
	}

	void setCoordXYZ(GLfloat xp, GLfloat yp, GLfloat zp) {
		x = xp;
		y = yp;
		z = zp;
	}

};

/**
* Funcao para calcular os pontos de controle de uma B-Spline Cubica C2.
*/
void find_control_points();

/**
* Funcao para calcular os parametros auxiliares para determinar os pontos da de controle;
*/
void init_u();

/**
* Funcao para calcular os pontos de controle de uma B-Spline Cubica C2.
* @param control array com os pontos de controle
* @param i indice do ponto que se quer calcular
* @param r nivel atual do algoritmo
* @param t parametro da interpolacao
* @param start indice do ponto onde se inicia a computacao
*/
Point de_casteljau(Point* control, int i, int r, double t, int start);

/**
* Funcao que pinta a curva de Bezier na tela.
*/
void draw_curve();

/**
* Funcao para inicializacao do programa.
*/
void myinit();

/**
* Esta funcao vai ser chamada quando a tela for redimensionada.
* @param w Largura atual da tela.
* @param h Altura atual da tela.
*/
void myreshape (GLsizei w, GLsizei h);

/**
* Esta é a funcao responsavel por pintar os objetos na tela.
*/
void mydisplay();

/**
* Esta funcao vai ser chamada toda vez que o mouse se mover com uma tecla apertada.
* Vai atualizar a posicao do quadrado que esta sendo movido.
* @param x Coordenada x atual do mouse.
* @param y Coordenada y atual do mouse.
*/
void handleMotion(int x, int y);

/**
* Esta funcao vai ser chamada toda vez que uma tecla do mouse seja clicada ou levantada.
* Com o botao direito do mouse, um ponto pode ser criado na posicao atual do mouse.
* Com o botao esquerdo do mouse, um ponto pode ser movimentado.
* @param btn Tecla levantada. Ex: GLUT_LEFT_BUTTON.
* @param key_state Estado da tecla. Ex: GLUT_DOWN.
* @param x Coordenada x atual do mouse.
* @param y Coordenada y atual do mouse.
*/
void handleMouse(int btn, int key_state, int x, int y);

/**
* Esta funcao vai ser chamada toda vez que uma tecla comum seja levantada.
* @param key Tecla levantada.
* @param x Coordenada x atual do mouse.
* @param y Coordenada y atual do mouse.
*/
void hadleKeyboard(unsigned char key, int x, int y);

/**
* Esta funcao vai ser chamada toda vez que uma tecla especial seja levantada.
* Botao F5 o programa volta ao estado inicial.
* @param key Tecla levantada. Ex: GLUT_KEY_F5.
* @param x Coordenada x atual do mouse.
* @param y Coordenada y atual do mouse.
*/
void hadleSpecialKeyboard(int key, int x, int y);

/**
* Representa uma maquina de estados do programa. Vai ser executada FPS vezes por segundo.
* @param id Identificador da maquina de estados.
*/
void loop(int id);

#endif //_PROJETOPG_H_
