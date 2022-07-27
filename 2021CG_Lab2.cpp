#include <GL/glut.h>
#include <GL/gl.h>
#include<iostream>
#include<fstream>
#include<string>
#include <sstream>
#include<vector>
#include <cmath> 
using namespace std;

#define PI 3.1415926

void select(string);
void drawline(int, int, int, int);

string infile;
float trans[3][3] = {};
float wxl, wxr, wyb, wyt, vxl, vxr, vyb, vyt;
int num = 0, squ_num = 0, tria_num = 0;
vector< vector<float> > vec;

void readfile() {
	string line;
	ifstream myfile(infile);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			select(line);
		}
		myfile.close();
	}
}

void display() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	readfile();
}

void clear() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

void mul_matrix(float a[3][3]) {
	float tmp[3][3] = { 0 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			tmp[i][j] = 0;
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				tmp[i][j] += a[i][k] * trans[k][j];
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		cout << "[ ";
		for (int j = 0; j < 3; j++) {
			trans[i][j] = tmp[i][j];
			cout << trans[i][j] << " ";
		}
		cout << "]" << endl;
	}
	/*for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			cout<<trans[i][j]<<" ";
		cout << endl;
	}*/
}


void scale(float a, float b) {
	//cout << a << " " << b << endl;
	float matrix[3][3] = { {a,0,0},
						   {0,b,0},
						   {0,0,1} };
	mul_matrix(matrix);
}

void rotate(float degree) {
	//cout << "degree " << degree << endl;
	float theta = degree / 180 * PI;
	float matrix[3][3] = { {cos(theta),-sin(theta),0},
						   {sin(theta),cos(theta),0},
						   {0,0,1} };
	/*for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}*/
	mul_matrix(matrix);
}

void translate(float a, float b) {
	float matrix[3][3] = { {1,0,a},
						   {0,1,b},
						   {0,0,1} };
	mul_matrix(matrix);
}

void view() {
	float sx = (vxr - vxl) / (wxr - wxl);
	float sy = (vyt - vyb) / (wyt - wyb);
	float matrix1[3][3] = { {sx,0,vxl},
						  {0,sy,vyb},
						  {0,0,1} };
	float matrix2[3][3] = { {1,0,-wxl},
						  {0,1,-wyb},
						  {0,0,1} };
	float wvm[3][3] = { 0 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				wvm[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}
	cout << "WVM" << endl;
	for (int i = 0; i < 3; i++) {
		cout << "[";
		for (int j = 0; j < 3; j++)
			cout << wvm[i][j] << " ";
		cout << "]" << endl;
	}
	glColor3f(1.0f, 1.0f, 1.0f);	//white
	drawline(int(vxl), int(vyt), int(vxr), int(vyt));
	drawline(int(vxl), int(vyb), int(vxr), int(vyb));
	drawline(int(vxl), int(vyt), int(vxl), int(vyb));
	drawline(int(vxr), int(vyt), int(vxr), int(vyb));

	//glColor3f(1.0f, 1.0f, 0.0f);	//yellow
	for (int i = 0; i < vec.size(); i += 2) {
		glColor3f(1.0f, 1.0f, 0.0f);
		float x1, x2, y1, y2 = 0;
		x1 = wvm[0][0] * vec[i][0] + wvm[0][1] * vec[i][1] + wvm[0][2] * vec[i][2];
		y1 = wvm[1][0] * vec[i][0] + wvm[1][1] * vec[i][1] + wvm[1][2] * vec[i][2];
		x2 = wvm[0][0] * vec[i + 1][0] + wvm[0][1] * vec[i + 1][1] + wvm[0][2] * vec[i + 1][2];
		y2 = wvm[1][0] * vec[i + 1][0] + wvm[1][1] * vec[i + 1][1] + wvm[1][2] * vec[i + 1][2];
		drawline((int)x2, (int)y2, (int)x1, (int)y1);
		//cout << "x1,y1 " << x1 << " " << y1 << endl;
		//cout << "x2,y2 " << x2 << " " << y2 << endl;
	}
	/*
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			cout << wvm[i][j] << " ";
		cout << endl;
	}*/

}

void addpoint(float arr[3]) {
	vector<float> v = { arr[0], arr[1], arr[2] };
	vec.push_back(v);
	return;
}

void drawsquare() {
	squ_num++;
	cout << "You have " << squ_num << " square." << endl;
	float initial_point[4][3] = { {1,1,1,},
								  {1,-1,1},
								  {-1,1,1},
								  {-1,-1,1} };
	float cur_point[4][3] = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			cur_point[i][j] += initial_point[i][0] * trans[j][0] + initial_point[i][1] * trans[j][1] + initial_point[i][2] * trans[j][2];
		}
	}
	/*for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				cur_point[i][j] += trans[j][k] * initial_point[i][j];
			}
		}
	}*/
	addpoint(cur_point[0]);
	addpoint(cur_point[1]);
	num++;
	addpoint(cur_point[0]);
	addpoint(cur_point[2]);
	num++;
	addpoint(cur_point[2]);
	addpoint(cur_point[3]);
	num++;
	addpoint(cur_point[3]);
	addpoint(cur_point[1]);
	num++;
}

void drawtriangle() {
	tria_num++;
	cout << "You have " << tria_num << " triangle." << endl;
	float initial_point[3][3] = { {0,1,1,},
								  {1,-1,1},
								  {-1,-1,1} };
	float cur_point[3][3] = { 0 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cur_point[i][j] += initial_point[i][0] * trans[j][0] + initial_point[i][1] * trans[j][1] + initial_point[i][2] * trans[j][2];
		}
	}

	addpoint(cur_point[0]);
	addpoint(cur_point[1]);
	num++;
	//cout << num << endl;
	addpoint(cur_point[1]);
	addpoint(cur_point[2]);
	num++;
	addpoint(cur_point[2]);
	addpoint(cur_point[0]);
	num++;
}

void cleardata() {
	vec.clear();
	num = 0;
	squ_num = 0;
	tria_num = 0;
}

void reset() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == j)
				trans[i][j] = 1;
			else
				trans[i][j] = 0;
		}
	}
}

//Mid-point algo.
void drawline(int x1, int y1, int x2, int y2) {
	glutSwapBuffers();
	glPointSize(1);
	//glColor3f(1.0f, 0.0f, 1.0f);	//purple
	if (x1 >= vxl && x1 <= vxr && y1 >= vyb && y1 <= vyt) {
		glBegin(GL_POINTS);
		glVertex2i(x1, y1);
		glEnd();
	}
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
			if (x >= vxl && x <= vxr && y >= vyb && y <= vyt) {
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
			}
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
			if (x >= vxl && x <= vxr && y >= vyb && y <= vyt) {
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
			}
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
			if (x >= vxl && x <= vxr && y >= vyb && y <= vyt) {
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
			}
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
			if (x >= vxl && x <= vxr && y >= vyb && y <= vyt) {
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
			}
		}
	}
	glutSwapBuffers();
}


void select(string line) {
	stringstream input(line);
	string text;
	vector<string> input_list;
	float a, b;
	while (input >> text) {
		input_list.push_back(text);
	}
	if (input_list.empty()) {
		return;
	}
	if (input_list[0] == "scale") {
		cout << "Scale" << endl;
		a = stof(input_list[1]);
		b = stof(input_list[2]);
		scale(a, b);
	}
	else if (input_list[0] == "rotate") {
		cout << "Rotate" << endl;
		a = stof(input_list[1]);
		rotate(a);
	}
	else if (input_list[0] == "translate") {
		cout << "Translate" << endl;
		a = stof(input_list[1]);
		b = stof(input_list[2]);
		translate(a, b);
	}
	else if (input_list[0] == "square") {
		//cout << "square" << endl;
		drawsquare();
	}
	else if (input_list[0] == "triangle") {
		//cout << "triangle" << endl;
		drawtriangle();
	}
	else if (input_list[0] == "view") {
		cout << "View" << endl;
		//cout << input_list.size() << endl;
		wxl = stof(input_list[1]);
		wxr = stof(input_list[2]);
		wyb = stof(input_list[3]);
		wyt = stof(input_list[4]);
		vxl = stof(input_list[5]);
		vxr = stof(input_list[6]);
		vyb = stof(input_list[7]);
		vyt = stof(input_list[8]);
		view();
		system("pause");
	}
	else if (input_list[0] == "clearData") {
		//cout << "data" << endl;
		cleardata();
	}
	else if (input_list[0] == "clearScreen") {
		clear();
	}
	else if (input_list[0] == "reset") {
		//cout << "reset" << endl;
		reset();
	}
	else if (input_list[0] == "end") {
		cout << "end" << endl;
		exit(0);
	}
	else {	//command
		cout << "##" << line << " ###" << endl;
	}

}

int main(int argc, char** argv) {
	system("pause");

	string line = "";
	if (argc == 2) {
		infile = argv[1];
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	gluOrtho2D(0, 800, 0, 600);
	glutCreateWindow("Lab2 Window");

	glutDisplayFunc(display);
	//glutKeyboardFunc(keyboard);
	//glutMouseFunc(mouse);
	gluOrtho2D(0, 800, 0, 600);
	glutSwapBuffers();
	glutMainLoop();
}
