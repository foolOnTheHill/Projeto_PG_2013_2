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

// Posicao do mouse
GLfloat mouse_x, mouse_y;

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

float d_u(int i) {
	return us[i] - us[i-1];
}

void init_u() {

	// Calcula os us utilizados na parametrização
	for (int i = 0; i <= qpoints-3; i++) {
		double d_x = pow(double(ds[i+2].x - ds[i].x), 2.0);
		double d_y = pow(double(ds[i+2].y - ds[i].y), 2.0);

		us[i] = sqrt(d_x + d_y); // Corda

		if (i > 0) {
			us[i] += us[i-1];
		}

	}

}

void find_control_points() {

    int L = qpoints-3, k = 6;
    float b, a;

    Point H, I, J, P, Q, R;

    init_u();
          
    control_points[0] = ds[0];
    control_points[1] = ds[1];
    control_points[(3*L) - 1] = ds[L+1];
    control_points[3*L] = ds[L+2];

    a = d_u(1)/(d_u(1) + d_u(2));    
    b = d_u(2)/(d_u(1) + d_u(2));
    H.x = ds[1].x * b + ds[2].x * a;
    H.y = ds[1].y * b + ds[2].y * a;
    control_points[2] = H;
               
    a = d_u(L-1)/(d_u(L-1) + d_u(L));
    b = d_u(L)/(d_u(L-1) + d_u(L));
    I.x = ds[L].x * b + ds[L+1].x * a;
    I.y = ds[L].y * b + ds[L+1].y * a;
    control_points[(3*L)-2] = I;

	a = d_u(L-1)/(d_u(L-1) + d_u(L));
    b = d_u(L)/(d_u(L-1) + d_u(L));
    J.x = (control_points[(3*L) - 4].x * b) + (control_points[(3*L) - 2].x * a);
    J.y = (control_points[(3*L) - 4].y * b) + (control_points[(3*L) - 2].y * a);
    control_points[(3*L)-3] = J;
    
    for(int i = 1; i <= L - 2; i++){

        a = d_u(i)/(d_u(i) + d_u(i+1));    
        b = d_u(i+1)/(d_u(i) + d_u(i+1));
        P.x = (control_points[(3*i) - 1].x * b) + (control_points[(3*i) + 1].x * a);
        P.y = (control_points[(3*i) - 1].y * b) + (control_points[(3*i) + 1].y * a);
        control_points[3*i] = P;
         
        a = d_u(i)/(d_u(i) + d_u(i+1) + d_u(i+2));
        b = (d_u(i+1) + d_u(i+2))/(d_u(i) + d_u(i+1) + d_u(i+2));
        Q.x = (ds[i+1].x * b) + (ds[i+2].x * a);
        Q.y = (ds[i+1].y * b) + (ds[i+2].y * a);
        control_points[(3*i)+1] = Q;
         
        a = (d_u(i) + d_u(i+1))/(d_u(i) + d_u(i+1) + d_u(i+2));
        b = d_u(i+2)/(d_u(i) + d_u(i+1) + d_u(i+2));
        R.x = (ds[i+1].x * b) + (ds[i+2].x * a);
        R.y = (ds[i+1].y * b) + (ds[i+2].y * a);
        control_points[(3*i)+2] = R;

		k += 3;
	}

}

Point de_casteljau(Point* control, int i, int r, double t, int start) {

	if (r == 0) {
		return control[i+start]; // Ponto no nivel adequado
	} else {
		Point g = de_casteljau(control, i, r-1, t, start);
		Point h = de_casteljau(control, i+1, r-1, t, start);
		Point k;

		k.setCoordXY((1-t)*g.x + t*h.x, (1-t)*g.y + t*h.y);

		return k;
	}
}

void draw_curve() {

	Point P, Q;

	int L = qpoints - 3;

	if (numpoints <= 4) {

		float par = 0;

		while (par <= 1) {
			Q = de_casteljau(control_points, 0, numpoints-1, par, 0);

			glBegin(GL_POINTS);
			glColor3f(0.0f, 0.0f, 237.0f);
			glVertex2f(Q.x, Q.y);
			glEnd();

			if (par > 0) {
				glBegin(GL_LINES);
				glColor3f(0.0f, 0.0f, 237.0f);
				glVertex2f(Q.x, Q.y);
				glVertex2f(P.x, P.y);
				glEnd();		
			}

			P.setCoordXY(Q.x, Q.y);

			par += float(1)/float(factor);
		} 

	} else {

		float par = 0;
		GLfloat u;

		for (int i = 0; i < L; i++) {

			u = us[i];
			while (u <= us[i+1]) {

				par = (u - us[i])/(us[i+1] - us[i]); // Parametrização
	
				if (i != L-1) {
					Q = de_casteljau(control_points, 0, 3, par, 3*i);
				} else {
					Q = de_casteljau(control_points, 0, numpoints - (1 + 3*i), par, 3*i);
				}

				glBegin(GL_POINTS);
				glColor3f(0.0f, 0.0f, u);
				glVertex2f(Q.x, Q.y);
				glEnd();

				// Liga um segmento de reta entre dois pontos consecutivos da curva
				if (u > us[0]) {
					glBegin(GL_LINES);
					glColor3f(0.0f, 0.0f, u);
					glVertex2f(Q.x, Q.y);
					glVertex2f(P.x, P.y);
					glEnd();		
				}

				P.setCoordXY(Q.x, Q.y);

				u += (us[L]-us[0])/float(factor); // Divide o intervalo de acordo com o fator escolhido pelo usuario apertando (+/-)
			}

		}

		// Ultimo ponto da curva
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, u);
		glVertex2f(control_points[(3*L)].x, control_points[(3*L)].y);
		glVertex2f(P.x, P.y);
		glEnd();
                  
	}

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

	loop(0);
}

void mydisplay() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glPointSize(6.0);

	for (int p = 0; p < qpoints; p++) {

		// Mostra os pontos determinados pelo usuario
		if (!hide_points) {
			glBegin(GL_POINTS);
			glColor3f(0, 0, 0);
			glVertex2f(ds[p].x, ds[p].y);
			glEnd();
		}

		// Poligonal de controle dos pontos escolhidos pelo usuario
		if (qpoints > 1 && !hide_lines && p > 0) {
			glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex2f(ds[p].x, ds[p].y);
			glVertex2f(ds[p-1].x, ds[p-1].y);
			glEnd();
		}

	}

	// Pinta a curva com no max. 4 pontos escolhidos pelo usuario.
	if (!hide_curve && qpoints > 1 && qpoints <= 4) {

		numpoints = qpoints;

		// Os pontos de controle são os mesmos do usuario
		for (int p = 0; p < qpoints; p++) {
			control_points[p] = ds[p];
		}

		glPointSize(1.5);
		draw_curve();
		glPointSize(6.0);

	// Pinta a curva com mais de 4 pontos
	} else if (qpoints > 4) {

		numpoints = 3*(qpoints-3) + 1;
        
		find_control_points(); // Calcula os pontos de controle para a B-Spline

		if (!hide_control_points) {

			// Pinta os pontos de controle de verde na tela
			for (int p = 0; p < numpoints; p++) {
				glBegin(GL_POINTS);
				glColor3f(0.0, 255.0, 0.0);
				glVertex2f(control_points[p].x, control_points[p].y);
				glEnd();
			}

		}

		// Pinta a poligonal de controle da B-spline
		if (!hide_control_polygon) {

			for (int p = 1; p < numpoints; p++) {
				glBegin(GL_LINES);
				glColor3f(0.0, 250.0, 0.0);
				glVertex2f(control_points[p].x, control_points[p].y);
				glVertex2f(control_points[p-1].x, control_points[p-1].y);
				glEnd();
			}			

		}

		// Pinta a B-spline
		if (!hide_curve) {
			glPointSize(1.5);
			draw_curve();
			glPointSize(6.0);
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
						ds[q-1] = ds[q];
					}

					qpoints -= 1;
					
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
	} else if (key == 'q'){ // Toggle na Poligonal de controle
		hide_control_polygon = !hide_control_polygon;
	} else if (key == 'w'){ // Toggle nos Pontos de controle
		hide_control_points = !hide_control_points;
	} else if (key == 'e'){ // Toggle os pontos do usuario
        hide_points = !hide_points;
    } else if(key == 'r'){ // Toggle nas linhas entre os pontos
        hide_lines = !hide_lines;
    } else if (key == 't'){ // Toggle na curva
        hide_curve = !hide_curve;
    } else if(key == '+'){ // Mais pontos na curva
		factor += 50;
	} else if(key == '-'){ // Menos pontos na curva
		factor = max(factor-50, 50);
	} 

    state = MODIFIED;
}

void hadleSpecialKeyboard(int key, int x, int y) {
	// Atualiza a tela
	if (key == GLUT_KEY_F5) {
		myinit();
	}
}

void loop(int id) {

	if (state == MODIFIED) { // A curva deve ser modificada
		mydisplay();
		mydisplay();
		state = IDLE;
	} else if (state != IDLE) {
		mydisplay();
		mydisplay();
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
