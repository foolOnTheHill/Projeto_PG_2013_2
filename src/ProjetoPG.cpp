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

int state = 0;
int numpoints = 0;
int qpoints = 0;
int factor = 100;
int circle = 0;
GLfloat mouse_x, mouse_y;
Point colors[2];
Point control_points[50000];
Point ds[50000];
float us[50000];
Point curve_points[10000];
GLfloat window_width = 800.0;
GLfloat window_height = 600.0;
vector<Circle> curva;
bool hide_points = false;
bool hide_curve = false;
bool hide_lines = false;
bool hide_control_points = true;
bool hide_control_polygon = true;
bool hide_circle = false;

void control_points() {

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

//
void mydisplay() {

}
//

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
	
	if (state == IDLE && btn == GLUT_LEFT_BUTTON) {
		mouse_x = x;
		mouse_y = y;

		if (key_state == GLUT_DOWN) {
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
	} else if (state == IDLE && btn == GLUT_MIDDLE_BUTTON) {
		mouse_x = x;
		mouse_y = y;

		if (key_state == GLUT_DOWN) {
			for (int p = 0; p < qpoints; p++) {
				float t_x = pow(mouse_x - ds[p].x, 2.0f);
				float t_y = pow(mouse_y - ds[p].y, 2.0f);

				if (sqrt(t_x + t_y) <= 5) {
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
	} else if (state == IDLE && btn == GLUT_RIGHT_BUTTON) {
		
		if (key_state == GLUT_DOWN) {
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
	
	if(key == ESC){
		exit(0);
	} else if (key == 't'){
		hide_control_polygon = !hide_control_polygon;
	} else if (key == 'r'){
		hide_control_points = !hide_control_points;
	} else if(key == 'y'){
		hide_circle = !hide_circle;
	} else if(key == '+'){
		factor += 50;
	} else if(key == '-'){
		factor = max(factor-50, 50);
	} else if (key == 'q'){
        hide_points = !hide_points;
    } else if(key == 'w'){
        hide_lines = !hide_lines;
    } else if (key == 'e'){
        hide_curve = !hide_curve;
    }

    state = MODIFIED;
}

void hadleSpecialKeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_F5) {
		myinit();
	}
}

void loop(int id) {

	if (state == MODIFIED) {
		mydisplay();
		mydisplay();

		state = IDLE;
		circle = 0;
	} else if (state != IDLE) {
		mydisplay();
		mydisplay();
	} else if (!hide_circle && circle < curva.size()) {
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

	myinit();

	glutMainLoop();

	return 0;
}
