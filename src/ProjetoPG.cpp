/*
-----------------------------------------------------------------------------
Computer Science Center - CIn
Federal University of Pernambuco - UFPE

@author George Oliveira - ghao
@author Lucas Souza - lns2
@author Nicolas Nascimento - nogn
-----------------------------------------------------------------------------
*/

#include "ProjetoPG.h"

int state = 0; // P/ a maquina de estados
int numpoints = 0; // Quantidade de pontos para desenhar a curva
int qpoints = 0; // Quantidade de pontos colocados pelo usuario
int factor = 100;

// Auxiliares para desenhar o circulo que desliza pela curva
int circle = 0;
vector<Circle> curva;

// Posicao do mouse
GLfloat mouse_x, mouse_y;

// Cores dos pontos
Point colors[2];

// Para determinar a curva
Point control_points[50000]; // Pontos de controle da B-spline
Point ds[50000]; // Pontos determinados pelo usuario
float us[50000]; // Calculados pela aplicacao

Point curve_points[10000]; // Representa a curva

// Dimensoes da janela
GLfloat window_width = 800.0;
GLfloat window_height = 600.0;

// Controle do programa
bool hide_points = false;
bool hide_curve = false;
bool hide_lines = false;
bool hide_control_points = true;
bool hide_control_polygon = true;
bool hide_circle = false;

void find_control_points() {

}

void init_u() {

}

Point de_casteljau(Point* control_points, int i, int r, double t, int start) {

}

void draw_curve() {

}

void draw_circle(Circle c) {
	/*From the tutorial at http://slabode.exofire.net/circle_draw.shtml*/
	int num_segments = 20;

	float theta = (2 * 3.14159)/float(20);
	float c = cosf(theta), s = sinf(theta), t;

	float x = 20; // x = r
	float y = 0;

	GLfloat cx = c.center.x;
	GLfloat cy = c.center.y;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < num_segments; i++) {
		glColor3f(c.r, c.g, c.b);
		glVertex2f(x + cx, y + cy);

		t = x;
		x = c*x - s*y;
		y = s*t + c*y;
	}
	glEnd();
}

void myreshape (GLsizei w, GLsizei h) {
	// Tamanho da tela
	glViewport(0, 0, w, h);
    
    // Tipo da projecao
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
             
    // Projecao Ortogonal
    glOrtho(0, w, h, 0, 0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void myinit() {
	srand(time(NULL));
	state = MODIFIED;

	myreshape(window_width, window_height);

	colors[0].setCoordXYZ(255.0/255.0, 36.0/255.00, 0/255);
	colors[1].setCoordXYZ(1, 215.0/255.0, 0);
	
	loop(0);
}

void mydisplay() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glPointSize(6.0);

	for (int p = 0; p < qpoints; p++) {
		
		if (!hide_points) {
			glBegin(GL_POINTS);
			glColor3f(0, 0, 0);
			glVertex2f(ds[i].x, ds[i].y);
			glEnd();
		}

		if (qpoints > 1 && !hide_lines && i > 0) {
			glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex2f(ds[i].x, ds[i].y);
			glVertex2f(ds[i-1].x, ds[i-1].y);
			glEnd();
		}

	}

	if (!hide_curve && qpoints > 1 && qpoints <= 4) {

		numpoints = qpoints;

		for (int p = 0; p < qpoints; p++) {
			control_points[p] = ds[p];
		}

		glPointSize(1.5);
		draw_curve();
		glPointSize(6.0);

	} else if (qpoints > 4) {

		find_control_points();

		if (!hide_control_points) {

			for (int p = 0; p < numpoints; p++) {
				glBegin(GL_POINTS);
				glColor3f(0.0, 255.0, 0.0);
				glVertex2f(control_points[p].x, control_points[p].y);
				glEnd();
			}

		}

		if (!hide_control_polygon) {

			for (int p = 1; p < numpoints; p++) {
				glBegin(GL_LINES);
				glColor3f(0.0, 255.0, 0.0);
				glVertex2f(control_points[p].x, control_points[p].y);
				glVertex2f(control_points[p-1].x, control_points[p-1].y);
				glEnd();
			}			

		}

		if (!hide_curve) {
			glPointSize(1.5);
			draw_curve();
			glPointSize(6.0);
		}

		/*Talvez nao seja necessario...*/
		if (!hide_circle) {
			draw_circle(curve[circle]);
		}

	}

	glFlush();
}

void handleMotion(int x, int y) {
	mouse_x = x;
	mouse_y = y;

	if ((state != MODIFIED) && (state != IDLE)) {
		ds[state].setCoordXY(x, y);
		mydisplay();
		mydisplay();
	}

}

void handleMouse(int btn, int key_state, int x, int y) {
	
	if (state == IDLE && btn == GLUT_LEFT_BUTTON) { // Usuario clicou na tela com o botao esquerdo
		mouse_x = x;
		mouse_y = y;

		if (key_state == GLUT_DOWN) {
			// Verifica se clicou em algum ponto
			for (int p = 0; p < qpoints; p++) {
				float t_x = pow(mouse_x - ds[p].x, 2.0f);
				float t_y = pow(mouse_y - ds[p].y, 2.0f);

				if (sqrt(t_x + t_y) <= 5) {
					state = p;
					break;
				}
			}
		}
	} else if (btn == GLUT_LEFT_BUTTON && key_state == GLUT_UP) {
		state = MODIFIED;
	} else if (state == IDLE && btn == GLUT_MIDDLE_BUTTON) { // Usuario clicou na tela 
		mouse_x = x;
		mouse_y = y;

		if (key_state == GLUT_DOWN) {
			// Verifica se clicou em algum ponto
			for (int p = 0; p < qpoints; p++) {
				float t_x = pow(mouse_x - ds[p].x, 2.0f);
				float t_y = pow(mouse_y - ds[p].y, 2.0f);

				if (sqrt(t_x + t_y) <= 5) {

					// Apaga o ponto clicado
					for (int q = p+1; q < qpoints; q++) {
						ds[q-i] = ds[q];
					}

					qpoints -= 1;
					if (qpoints <= 1) {
						curva.clear();
					}

					state = MODIFIED;
					break;
				}

			}
		}
	} else if (state == IDLE && btn == GLUT_RIGHT_BUTTON) { // Usuario clicou na tela com o botao direito
		
		if (key_state == GLUT_DOWN) {
			// Adiciona um ponto na posição clicada
			GLfloat px = x, py = y;
			Point p;
			p.setCoordXY(px, py);
			ds[qpoints] = p;
			qpoints += 1;
			glFinish();
			state = MODIFIED;
		}
	}

}

void hadleKeyboard(unsigned char key, int x, int y) {
	
	if(key == ESC){ // Finaliza o programa
		exit(0);
	} else if (key == 't'){ // Toggle na Poligonal de controle
		hide_control_polygon = !hide_control_polygon;
	} else if (key == 'r'){ // Toggle nos Pontos de controle
		hide_control_points = !hide_control_points;
	} else if(key == 'y'){ // Toggle o circulo que desliza pela curva
		hide_circle = !hide_circle;
	} else if(key == '+'){
		factor += 50;
	} else if(key == '-'){
		factor = max(factor-50, 50);
	} else if (key == 'q'){ // Toggle os pontos do usuario
        hide_points = !hide_points;
    } else if(key == 'w'){ // Toggle nas linhas entre os pontos
        hide_lines = !hide_lines;
    } else if (key == 'e'){ // Toggle na curva
        hide_curve = !hide_curve;
    }

    state = MODIFIED;
}

void hadleSpecialKeyboard(int key, int x, int y) {
	// Limpa a tela
	if (key == GLUT_KEY_F5) {
		myinit();
	}
}

void loop(int id) {

	if (state == MODIFIED) { // A curva deve ser modificada
		mydisplay();
		mydisplay();

		state = IDLE;
		circle = 0;
	} else if (state != IDLE) {
		mydisplay();
		mydisplay();
	} else if (!hide_circle && circle < curva.size()) { // Move o circulo pela curva.
		mydisplay();
		mydisplay();

		circle += 1;
		if (circle >= curva.size()) {
			circle = 0;
		}
		glFlush();
	}

	glutTimerFunc(1000/FPS, loop, id);
}

int main(int argc, char **argv) {

	/* Inicia a janela da aplicacao */
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(0, 100);
	glutCreateWindow("B-Spline Cubica C2");

	glutDisplayFunc(mydisplay);
	glutReshapeFunc(myreshape);
	glutMouseFunc(handleMouse);
	glutMotionFunc(handleMotion);
	glutKeyboardUpFunc(hadleKeyboard);
	glutSpecialUpFunc(hadleSpecialKeyboard);
	//

	myinit();

	glutMainLoop();

	return 0;
}
