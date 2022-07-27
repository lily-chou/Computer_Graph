#include <GL/glut.h>
#include <GL/gl.h>
#include<iostream>
#include <cmath> 
using namespace std;

char type;
bool f_point = false;	//first_point
char last;
int pointx, pointy;

void display() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void clear() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

void drawpoint(int x, int y) {
	//glutSwapBuffers();
	cout << "The dot is at (" << x << " , " << y << ")" << endl;
	glPointSize(5);
	glColor3f(1.0f, 0.0f, 0.0f);	//red
	glutSwapBuffers();
	glBegin(GL_POINTS);
	glVertex2i(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
	glEnd();
	glutSwapBuffers();
}


//Mid-point algo.
void drawline(int x1, int y1, int x2, int y2) {
	glutSwapBuffers();
	glPointSize(2);
	if (type == 'l')
		glColor3f(1.0f, 1.0f, 0.0f);	//yellow
	else
		glColor3f(1.0f, 0.0f, 1.0f);	//purple
	glBegin(GL_POINTS);
	glVertex2i(x1, glutGet(GLUT_WINDOW_HEIGHT) - y1);
	glEnd();
	int x = x1;
	int y = y1;
	int a = y2 - y1;
	int b = x2 - x1;
	int stepx = abs(b);
	int stepy = abs(a);
	float m = float(a) / float(b);
	bool check = false;
	if (x1 <= x2)
		check = false;
	else
		check = true;

	if (-1 >= m) {
		int d = -b - a / 2;
		while (stepy > 0) {
			if (d >= 0) {		//E
				if (!check) {
					y--;
					d += -b;
				}
				else {
					y++;
					d += b;
				}
			}
			else {				//NE
				if (!check) {
					x++;
					y--;
					d += (-a - b);
				}
				else {
					x--;
					y++;
					d += (b + a);
				}
			}
			stepy--;
			glBegin(GL_POINTS);
			glVertex2i(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
			glEnd();
		}
	}
	else if (-1 <= m && m <= 0) {
		int d = a + b / 2;
		while (stepx > 0) {
			if (d >= 0) {	//E
				if (!check) {
					x++;
					d += a;
				}
				else {
					x--;
					d -= a;
				}
			}
			else {			//NE
				if (!check) {
					x++;
					y--;
					d += (a + b);
				}
				else {
					x--;
					y++;
					d += -(a + b);
				}
			}
			stepx--;
			glBegin(GL_POINTS);
			glVertex2i(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
			glEnd();
		}
	}
	else if (0 < m && m <= 1) {
		int d = a - b / 2;
		while (stepx > 0) {
			if (d <= 0) {		//E
				if (!check) {
					x++;
					d += a;
				}
				else {
					x--;
					d += -a;
				}
			}
			else {		//NE
				if (!check) {
					x++;
					y++;
					d += (a - b);
				}
				else {
					x--;
					y--;
					d += (b - a);
				}
			}
			stepx--;
			glBegin(GL_POINTS);
			glVertex2i(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
			glEnd();
		}
	}
	else if (m > 1) {
		int d = b - a / 2;
		while (stepy > 0) {
			if (d <= 0) {	//E
				if (!check) {
					y++;
					d += b;
				}
				else {
					y--;
					d += -b;
				}

			}
			else {		//NE
				if (!check) {
					x++;
					y++;
					d += (b - a);
				}
				else {
					x--;
					y--;
					d += (a - b);
				}
			}
			stepy--;
			glBegin(GL_POINTS);
			glVertex2i(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
			glEnd();
		}
	}
	glutSwapBuffers();
}


//Mid-point circle algo.
void drawcircle(int x1, int y1, int x2, int y2) {
	int r = sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
	cout << "The circle's center is (" << x1 << " , " << y1 << "), and its radius is " << r << endl;

	int x = 0;
	int y = r;
	int d = int(5 / 4 - r);

	glutSwapBuffers();
	glPointSize(2);
	glColor3f(0.0f, 0.0f, 1.0f);
	y1 = glutGet(GLUT_WINDOW_HEIGHT) - y1;

	while (x <= y) {
		if (d < 0) {
			d += (2 * x + 3);
			x++;
		}
		else {
			d += (2 * (x - y) + 5);
			x++;
			y--;
		}
		glBegin(GL_POINTS);
		glVertex2i(x1 + x, y1 + y);
		glVertex2i(x1 + x, y1 - y);
		glVertex2i(x1 - x, y1 + y);
		glVertex2i(x1 - x, y1 - y);
		glVertex2i(x1 + y, y1 + x);
		glVertex2i(x1 - y, y1 + x);
		glVertex2i(x1 + y, y1 - x);
		glVertex2i(x1 - y, y1 - x);
		glEnd();
	}
	glutSwapBuffers();
}


void mouse(int bin, int state, int x, int y) {
	if (type == 'o' && bin == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		f_point = false;
		drawcircle(pointx, pointy, x, y);
	}
	if (bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		switch (type) {
		case 'd':
			drawpoint(x, y);
			break;
		case 'l':
			if (!f_point) {
				pointx = x;
				pointy = y;
				f_point = true;
			}
			else {
				f_point = false;
				cout << "The line from (" << pointx << " , " << pointy << ") to (" << x << " , " << y << ")" << endl;
				drawline(pointx, pointy, x, y);
			}
			break;
		case 'p':
			if (f_point || last == 'p') {
				drawline(pointx, pointy, x, y);
				pointx = x;
				pointy = y;
			}
			else {
				pointx = x;
				pointy = y;
				f_point = true;
			}
			break;
		case 'o':
			if (!f_point) {
				pointx = x;
				pointy = y;
				f_point = true;
			}
			break;
		default:
			break;
		}
	}
}

void keyboard(unsigned char key, int x, int y) {
	last = type;
	type = key;
	switch (key) {
	case 'd':
		cout << "Function: Draw the dot." << endl;
		break;
	case 'l':
		cout << "Function: Draw the line." << endl;
		f_point = false;
		break;
	case 'p':
		f_point = false;
		cout << "Function: Draw the polygon." << endl;
		break;
	case 'o':
		cout << "Function: Draw the circle" << endl;
		f_point = false;
		break;
	case 'c':
		cout << "Function: Clear" << endl;
		if (last != 'c')
			clear();
		break;
	case 'r':
		cout << "Function: Redraw" << endl;
		if (last == 'c')
			glutSwapBuffers();
		break;
	case 'q':
		cout << "Function: Quit" << endl;
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	gluOrtho2D(0, 800, 0, 600);
	glutCreateWindow("Lab1 Window");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	gluOrtho2D(0, 800, 0, 600);
	glutSwapBuffers();
	glutMainLoop();

}
