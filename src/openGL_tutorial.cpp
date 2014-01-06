/*
-----------------------------------------------------------------------------
OpenGL Tutorial
VOXAR Labs
Computer Science Center - CIn
Federal University of Pernambuco - UFPE
http://www.cin.ufpe.br/~voxarlabs

@author Tullio Lucena - tjsl
@Updated by Ermano Arruda - eaa3

Este programa foi feito para mostrar conceitos basicos de OpenGl e glut para
a turma de Processamento Grafico do Centro de Informatica da UFPE.

Funcionalidades:
1-	Criar um quadrado com tamanho e cor aleatoria ao clicar com o botao
	direito do mouse. A posicao e definida pela posicao atual do mouse.
2-	Movimentar o quadrado segurando o botao esquerdo do mouse e arrastando.
3-	Limpar a tela apertando a tecla F5.
4-	Sair do programa apertando a tecla ESC.

Referencias:
Funcoes de C/C++:
	http://www.cplusplus.com/
Copia online do Red Book (OpenGL Programming Guide):
	http://fly.cc.fer.hr/~unreal/theredbook/
Referencia online para os comandos OpenGL (Man pages):
	http://www.opengl.org/sdk/docs/man/

-----------------------------------------------------------------------------
*/

#include "openGL_tutorial.h"

int qtdQuadrados;
int estado;
GLfloat mouse_x, mouse_y;
Quadrado quad[1000];
GLfloat window_width = 800.0;
GLfloat window_height = 600.0;

void myinit()
{
	srand(time(NULL));
	qtdQuadrados = 0;
	estado = MODIFIED;

	//Imagem projetada no near plane sera desenhada na seguinte area da tela: (0,0,width,height)
	glViewport(0, 0, window_width, window_height);

	//Setando tipo de projecao..
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Projecao sera ortogonal
	glOrtho(0, window_width, window_height, 0, 0, 5.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	loop(0);
}

void myreshape (GLsizei w, GLsizei h)
{

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	window_width = (GLfloat) w;
	window_height = (GLfloat) h;
	glOrtho(0, window_width, window_height, 0, 0, 5.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mydisplay()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	for(int i = 0; i < qtdQuadrados; i++){
		glColor3f(quad[i].r, quad[i].g, quad[i].b);
		glBegin(GL_QUADS);
			glVertex2f(quad[i].x, quad[i].y);
			glVertex2f(quad[i].x+quad[i].lado, quad[i].y);
			glVertex2f(quad[i].x+quad[i].lado, quad[i].y+quad[i].lado);
			glVertex2f(quad[i].x, quad[i].y+quad[i].lado);
		glEnd();
	}
	
	glFlush();
}

void handleMotion(int x, int y)
{
	mouse_x = x;
	mouse_y = y;
	if((estado != MODIFIED) && (estado != IDLE)){

		printf("[Mover quad] pos = mouse(%0.2f,%0.2f)\n",mouse_x,mouse_y);

		quad[estado].x = mouse_x - quad[estado].lado/2;
		quad[estado].y = mouse_y - quad[estado].lado/2;
	}
}

void handleMouse(int btn, int state, int x, int y)
{
	if (estado == IDLE && btn == GLUT_LEFT_BUTTON){
		mouse_x = x;
		mouse_y = y;
		if (state == GLUT_DOWN){
			for(int i = qtdQuadrados-1; i >= 0; i--){
				if((mouse_x >= quad[i].x) && (mouse_x <= (quad[i].x+quad[i].lado)) && (mouse_y >= quad[i].y) && (mouse_y <= (quad[i].y+quad[i].lado))){

					estado = i;
					break;
				}
			}
		}
	}
	else if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP){
		estado = MODIFIED;
	}
	else if(estado == IDLE && btn == GLUT_RIGHT_BUTTON){
		if (state == GLUT_DOWN){

			//Posicao do quadrado
			GLfloat x2 = x;
			GLfloat y2 = y;

			//Tamanho do lado aleatorio
			GLfloat lado = (((GLfloat)(rand()%200)))+10; 
			//Cor
			GLfloat r = ((GLfloat)(rand()%256))/255.0;
			GLfloat g = ((GLfloat)(rand()%256))/255.0;
			GLfloat b = ((GLfloat)(rand()%256))/255.0;

			printf("[Add quad] posicao = mouse(%d,%d), lado = %0.2f, cor = rgb(%0.2f,%0.2f,%0.2f)\n",x,y, lado,r,g,b);
			quad[qtdQuadrados++] = Quadrado(lado, x2, y2, r, g, b);
			estado = MODIFIED;
		}
	}
}

void hadleKeyboard(unsigned char key, int x, int y)
{
	if(key == ESC){
		exit(0);
	}
}

void hadleSpecialKeyboard(int key, int x, int y)
{
	if(key == GLUT_KEY_F5){
		myinit();
	}
}

void loop(int id)
{
	if(estado == MODIFIED){
		mydisplay();
		estado = IDLE;
	}
	else if(estado != IDLE){
		mydisplay();
	}
	glutTimerFunc(1000/FPS, loop, id);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(0,100);
	glutCreateWindow("PG - OpenGL Template");
	
	glutDisplayFunc(mydisplay);
	glutReshapeFunc(myreshape);
	glutMouseFunc(handleMouse);
	glutMotionFunc(handleMotion);
	glutKeyboardUpFunc(hadleKeyboard);
	glutSpecialUpFunc(hadleSpecialKeyboard);

	myinit();

	glutMainLoop();
	return 0;
}
