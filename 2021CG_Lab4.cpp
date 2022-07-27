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
int winx = 500, winy = 500;
float trans[4][4] = {};
float em[4][4] = {};
float pm[4][4] = {};
float wvm[4][4] = {};
float back_r = 0, back_g = 0, back_b = 0;
float ambient_r = 0, ambient_g = 0, ambient_b = 0;
float Ir = 0, Ig = 0, Ib = 0;
float eyex = 0, eyey = 0, eyez = 0;
float AR = 0;
int l_x = 0, top = 0, r_x = 0, bottom = 0;
int num = 0;
vector< vector< vector<float> >> ascvec;
vector< vector< vector<float> >> ascface;
vector< vector< vector<float> >> objvec;
vector< vector<float> > value;
//float light_vec[7];
vector< vector <float> > light_vec;
vector< vector<int> > points;
float Zbuffer [600][600];
float Cbuffer_r[600][600];
float Cbuffer_g[600][600];
float Cbuffer_b[600][600];

void print(float a[][4]) {
	for (int i = 0; i < 4; i++) {
		cout << "[ ";
		for (int j = 0; j < 4; j++) {
			cout << a[i][j] << " ";
		}
		cout << "]" << endl;
	}
}

void readfile() {
	string line;
	ifstream myfile(infile);
	
	if (myfile.is_open()) {
		cout << "infile" << endl;
		while (getline(myfile, line)) {
			select(line);
		}
		myfile.close();
	}
}

void readobject(string obfile, float Or, float Og, float Ob, float Kd, float Ks, float N) {
	//cout << "OOOOOOOOOOOOOOOOOOOOOOOOOO" << endl;
	//cout << Or << " " << Og << " " << Ob << " " << Kd << " " << Ks << " " << N << endl;
	string line;
	ifstream filein(obfile);
	int num_v = 0, num_f = 0;
	if (filein.is_open()) {
		//cout << "in";
		filein >> num_v;
		filein >> num_f;
	}
	else {
		cout << "not in the file" << endl;
	}
	vector<vector<float>> input1;
	for (int i = 0; i < num_v; i++) {
		input1.push_back({ 0,0,0,0 });
		for (int j = 0; j < 3; j++) {
			filein >> input1[i][j];
		}
		input1[i][3] = 1;
	}
	objvec.push_back(input1);
	vector<vector<float>> input2;
	int n;
	for (int i = 0; i < num_f; i++) {
		filein >> n;
		input2.push_back({ 0,0,0,0,0,0,0 });
		for (int j = 0; j < n; j++) {
			filein >> input2[i][j];
		}
		if (n == 3) {
			input2[i][3] = 0;
		}
	}
	filein.close();
	//print(trans);
	float tmp = 0;
	float str[4] = {};
	for (int i = 0; i < num_v; i++) {
		tmp = 0;
		for (int j = 0; j < 4; j++) {
			tmp += input1[i][0] * trans[j][0] + input1[i][1] * trans[j][1] + input1[i][2] * trans[j][2] + input1[i][3] * trans[j][3];
			str[j] = tmp;
			tmp = 0;
		}
		input1[i][0] = str[0];
		input1[i][1] = str[1];
		input1[i][2] = str[2];
		input1[i][3] = str[3];
	}
	ascvec.push_back(input1);
	//objvec.push_back(input1);
	ascface.push_back(input2);
	value.push_back({ Or, Og, Ob, Kd, Ks, float(N) });
	//cout << ascvec.size() << endl;
	num++;
	//cout << num << endl;
}

void display_fun() {
	glClearColor(back_r, back_g, back_b, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	readfile();
}

void clear() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

void mul_matrix(float a[4][4], float b[4][4]) {
	float tmp[4][4] = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp[i][j] = 0;
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				tmp[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			b[i][j] = tmp[i][j];
		}
	}
}

void scale(float a, float b, float c) {
	float matrix[4][4] = { {a,0,0,0},
						   {0,b,0,0},
						   {0,0,c,0},
						   {0,0,0,1} };
	mul_matrix(matrix, trans);
	//print(trans);
}

void rotate(float degreex, float degreey, float degreez) {
	if (degreex != 0) {
		float theta = degreex / 180 * PI;
		float matrix[4][4] = { {1,0,0,0},
							   {0,cos(theta),-sin(theta),0},
							   {0,sin(theta),cos(theta),0},
							   {0,0,0,1} };
		mul_matrix(matrix, trans);
	}
	if (degreez != 0) {
		float theta = degreez / 180 * PI;
		float matrix[4][4] = { {cos(theta),-sin(theta),0,0},
							   {sin(theta),cos(theta),0,0},
							   {0,0,1,0},
							   {0,0,0,1} };
		mul_matrix(matrix, trans);
	}
	if (degreey != 0) {
		float theta = degreey / 180 * PI;
		float matrix[4][4] = { {cos(theta),0,sin(theta),0},
							   {0,1,0,0},
							   {-sin(theta),0,cos(theta),0},
							   {0,0,0,1} };
		mul_matrix(matrix, trans);
	}
	//print(trans);
}

void translate(float a, float b, float c) {
	float matrix[4][4] = { {1,0,0,a},
						   {0,1,0,b},
						   {0,0,1,c},
						   {0,0,0,1} };
	mul_matrix(matrix, trans);
	//print(trans);
}

void viewport(float vxl, float vxr, float vyb, float vyt) {
	AR = (vxr - vxl) / (vyt - vyb);
	pm[1][1] = AR;

	l_x = (vxl + 1) * winx / 2;
	r_x = (vxr + 1) * winx / 2;
	top = (vyt + 1) * winy / 2;
	bottom = (vyb + 1) * winy / 2;

	float sx = (r_x - l_x) / 2;
	float sy = (top - bottom) / 2;

	float tmp[4][4] = { {sx,0,0,sx + l_x},
						  {0,sy,0,sy + bottom},
						  {0,0,1,0},
						  {0,0,0,1} };
	
	cout << "WVM" << endl;
	for (int i = 0; i < 4; i++) {
		cout << "[";
		for (int j = 0; j < 4; j++) {
			wvm[i][j] = tmp[i][j];
			cout << wvm[i][j] << " ";
		}
		cout << "]" << endl;
	}
	
}

void normalize(float a[]) {
	float tmp = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	for (int i = 0; i < 3; i++) {
		a[i] = a[i] / tmp;
	}
}

void cal_cross(float a[], float b[], float c[]) {
	a[0] = b[1] * c[2] - b[2] * c[1];
	a[1] = b[2] * c[0] - b[0] * c[2];
	a[2] = b[0] * c[1] - b[1] * c[0];
}

void eyematrix(float ex, float ey, float  ez, float coix, float coiy, float coiz, float tilt) {
	cout << "EM" << endl;
	float transmatrix[4][4] = { {1,0,0,-ex},
								{0,1,0,-ey},
								{0,0,1,-ez},
								{0,0,0,1} };
	float mirrow[4][4] = { {-1,0,0,0},
						   {0,1,0,0},
						   {0,0,1,0},
						   {0,0,0,1} };
	float degree = tilt / 180 * PI;
	float tiltmatrix[4][4] = { {cos(degree),sin(degree),0,0},
							   {-sin(degree),cos(degree),0,0},
							   {0,0,1,0},
							   {0,0,0,1} };
	float topvec[3] = { 0,1,0 };
	float arr[3][3] = { {0,0,0},{0,0,0},{coix - ex,coiy - ey,coiz - ez} };
	normalize(arr[2]);
	cal_cross(arr[0], topvec, arr[2]);
	normalize(arr[0]);
	cal_cross(arr[1], arr[2], arr[0]);
	normalize(arr[1]);
	float grm[4][4] = { {arr[0][0],arr[0][1],arr[0][2],0},
						{arr[1][0],arr[1][1],arr[1][2],0},
						{arr[2][0],arr[2][1],arr[2][2],0},
						{0,0,0,1} };
	float eye[4][4] = { {1,0,0,0},
						{0,1,0,0},
						{0,0,1,0},
						{0,0,0,1} };
	mul_matrix(transmatrix, eye);
	mul_matrix(grm, eye);
	mul_matrix(mirrow, eye);
	mul_matrix(tiltmatrix, eye);
	
	for (int i = 0; i < 4; i++) {
		cout << "[";
		for (int j = 0; j < 4; j++) {
			em[i][j] = eye[i][j];
			cout << em[i][j] << " ";
		}
		cout << "]" << endl;
	}
	
}

void projection_matrix(float hither, float yon, float hav) {
	cout << "PM" << endl;
	float degree = hav / 180 * PI;
	float n1 = tan(degree);
	float n2 = (yon / (yon - hither)) * n1;
	float n3 = ((hither * yon) / (hither - yon)) * n1;
	float tmp[4][4] = { {1,0,0,0},
						{0,1,0,0},
						{0,0,n2,n3},
						{0,0,n1,0} };
	
	for (int i = 0; i < 4; i++) {
		cout << "[";
		for (int j = 0; j < 4; j++) {
			pm[i][j] = tmp[i][j];
			cout << pm[i][j] << " ";
		}
		cout << "]" << endl;
	}
	
}

void observer(float ex, float ey, float  ez, float coix, float coiy, float coiz, float tilt, float hither, float yon, float hav) {
	eyematrix(ex, ey, ez, coix, coiy, coiz, tilt);
	projection_matrix(hither, yon, hav);
	eyex = ex;
	eyey = ey;
	eyez = ez;
}

void ambient(float KaIar, float KaIag, float KaIab) {
	ambient_r = KaIar;
	ambient_g = KaIag;
	ambient_b = KaIab;
}

void background(float Br, float Bg, float Bb) {
	back_r = Br;
	back_g = Bg;
	back_b = Bb;
}

void light(int index, float Ipr, float Ipg, float Ipb, float Ix, float Iy, float Iz) {
	if (index > light_vec.size())
		light_vec.push_back({ Ipr, Ipg, Ipb, Ix, Iy, Iz });
	else
		light_vec[index - 1] = { Ipr, Ipg, Ipb, Ix, Iy, Iz };
}

float dot(float a[3], float b[3]) {
	float sum = 0;
	for (int i = 0; i < 3; i++) {
		sum += a[i] * b[i];
	}
	return sum;
}

void display() {
	clear();
	
	//cout << "11111111111" << endl;
	//cout << l_x << " " << r_x << " " << bottom << top << endl;
	for (int i = l_x; i <= r_x; i++) {
		for (int j = bottom; j <= top; j++) {
			Zbuffer[i][j] = 1000000;
			Cbuffer_r[i][j] = back_r;
			Cbuffer_g[i][j] = back_g;
			Cbuffer_b[i][j] = back_b;
		}
	}
	//cout << "displaysize:" << ascvec.size() << endl;
	//cout << "222222222222222222" << endl;
	vector< vector<float>> vec;
	vector< vector<float>> vec1;
	vector< vector<float>> vec2;
	
	//cout << ascvec.size() << endl;
	//cout << ascface.size() << endl;
	for (int i = 0; i < num; i++) {
		vec = ascvec[i];
		vec1 = objvec[i];
		vec2 = ascface[i];
		for (int j = 0; j<vec2.size(); j++) {
			
			//get normal vector
			float normalvector[3];
			float point1[3];
			float point2[3];
			float point3[3];
			float v1[3];
			float v2[3];
			for (int k = 0; k < 3; k++) {
				point1[k] = vec[vec2[j][0] - 1][k];
				point2[k] = vec[vec2[j][1] - 1][k];
				point3[k] = vec[vec2[j][2] - 1][k];
				v1[k] = point2[k] - point1[k];
				v2[k] = point3[k] - point1[k];
			}
			cal_cross(normalvector, v1, v2);
			normalize(normalvector);

			//get the polygon color
			float lightvector[3];
			float rvector[3];
			float vvector[3];
			float NL = 0, RV = 0;
			Ir = ambient_r * value[i][0];
			Ig = ambient_g * value[i][1];
			Ib = ambient_b * value[i][2];
			
			lightvector[0] = light_vec[light_vec.size() - 1][3] - vec[vec2[j][0] - 1][0];
			lightvector[1] = light_vec[light_vec.size() - 1][4] - vec[vec2[j][0] - 1][1];
			lightvector[2] = light_vec[light_vec.size() - 1][5] - vec[vec2[j][0] - 1][2];
			normalize(lightvector);
			vvector[0] = eyex - vec[vec2[j][0] - 1][0];
			vvector[1] = eyey - vec[vec2[j][0] - 1][1];
			vvector[2] = eyez - vec[vec2[j][0] - 1][2];
			rvector[0] = lightvector[0] + vvector[0];
			rvector[1] = lightvector[1] + vvector[1];
			rvector[2] = lightvector[2] + vvector[2];
			//normalize(lightvector);
			normalize(rvector);
			NL = dot(normalvector, lightvector);
			RV = dot(normalvector, rvector);
			RV = pow(RV, value[i][5]);
			
			Ir += (value[i][3] * light_vec[light_vec.size() - 1][0] * NL) * value[i][0] + value[i][4] * light_vec[light_vec.size() - 1][0] * RV;
			Ig += (value[i][3] * light_vec[light_vec.size() - 1][1] * NL) * value[i][1] + value[i][4] * light_vec[light_vec.size() - 1][1] * RV;
			Ib += (value[i][3] * light_vec[light_vec.size() - 1][2] * NL) * value[i][2] + value[i][4] * light_vec[light_vec.size() - 1][2] * RV;
			
			/*
			
			lightvector[0] = light_vec[light_vec.size() - 1][3] - vec1[vec2[j][0] - 1][0];
			lightvector[1] = light_vec[light_vec.size() - 1][4] - vec1[vec2[j][0] - 1][1];
			lightvector[2] = light_vec[light_vec.size() - 1][5] - vec1[vec2[j][0] - 1][2];
			//cout << "lightvector " << lightvector[0] << " " << lightvector[1] << " " << lightvector[2] << endl;
			normalize(lightvector);
			vvector[0] = eyex - vec1[vec2[j][0] - 1][0];
			vvector[1] = eyey - vec1[vec2[j][0] - 1][1];
			vvector[2] = eyez - vec1[vec2[j][0] - 1][2];
			//cout << "vvector " << vvector[0] << " " << vvector[1] << " " << vvector[2] << endl;
			NL = dot(lightvector, normalvector);
			rvector[0] = 2 * NL * normalvector[0] - lightvector[0];
			rvector[1] = 2 * NL * normalvector[1] - lightvector[1];
			rvector[2] = 2 * NL * normalvector[2] - lightvector[2];
			//rvector[0] = lightvector[0] + vvector[0];
			//rvector[1] = lightvector[1] + vvector[1];
			//rvector[2] = lightvector[2] + vvector[2];
			normalize(vvector);
			normalize(rvector);
			normalize(lightvector);

			RV = dot(vvector, rvector);
			RV = pow(RV, value[i][5]);
			Ir += value[i][3] * light_vec[light_vec.size() - 1][0] * NL * value[i][0] + value[i][4] * light_vec[light_vec.size() - 1][0] * RV;
			Ig += value[i][3] * light_vec[light_vec.size() - 1][1] * NL * value[i][1] + value[i][4] * light_vec[light_vec.size() - 1][1] * RV;
			Ib += value[i][3] * light_vec[light_vec.size() - 1][2] * NL * value[i][2] + value[i][4] * light_vec[light_vec.size() - 1][2] * RV;
			*/
			/*
			for (int m = 0; m < light_vec.size(); m++) {
				//cout << "point  " << vec[vec2[j][0] - 1][0] << " " << vec[vec2[j][0] - 1][1] << " " << vec[vec2[j][0] - 1][2] << endl;
				lightvector[0] = light_vec[m][3] - vec1[vec2[j][0] - 1][0];
				lightvector[1] = light_vec[m][4] - vec1[vec2[j][0] - 1][1];
				lightvector[2] = light_vec[m][5] - vec1[vec2[j][0] - 1][2];
				//cout << "lightvector " << lightvector[0] << " " << lightvector[1] << " " << lightvector[2] << endl;
				normalize(lightvector);
				vvector[0] = eyex - vec1[vec2[j][0] - 1][0];
				vvector[1] = eyey - vec1[vec2[j][0] - 1][1];
				vvector[2] = eyez - vec1[vec2[j][0] - 1][2];
				//cout << "vvector " << vvector[0] << " " << vvector[1] << " " << vvector[2] << endl;
				NL = dot(lightvector, normalvector);
				rvector[0] = 2 * NL * normalvector[0] - lightvector[0];
				rvector[1] = 2 * NL * normalvector[1] - lightvector[1];
				rvector[2] = 2 * NL * normalvector[2] - lightvector[2];
				//rvector[0] = lightvector[0] + vvector[0];
				//rvector[1] = lightvector[1] + vvector[1];
				//rvector[2] = lightvector[2] + vvector[2];
				normalize(vvector);
				normalize(rvector);
				normalize(lightvector);

				RV = dot(vvector, rvector);
				RV = pow(RV, value[i][5]);
				Ir += value[i][3] * light_vec[m][0] * NL * value[i][0] + value[i][4] * light_vec[m][0] * RV;
				Ig += value[i][3] * light_vec[m][1] * NL * value[i][1] + value[i][4] * light_vec[m][1] * RV;
				Ib += value[i][3] * light_vec[m][2] * NL * value[i][2] + value[i][4] * light_vec[m][2] * RV;
			}
			*/						
			vec2[j][4] = Ir;
			vec2[j][5] = Ig;
			vec2[j][6] = Ib;
		}
	
		float tmp = 0;
		//EM
		//cout << "eye" << endl;
		float str[4] = {};
		for (int j = 0; j < vec.size(); j++) {
			for (int k = 0; k < 4; k++) {
				tmp += vec[j][0] * em[k][0] + vec[j][1] * em[k][1] + vec[j][2] * em[k][2] + vec[j][3] * em[k][3];
				str[k] = tmp;
				tmp = 0;
			}
			vec[j][0] = str[0];
			vec[j][1] = str[1];
			vec[j][2] = str[2];
			vec[j][3] = str[3];
		}
		//cout << "point  " << vec[0][0] << " " << vec[0][1] << " " << vec[0][2] << endl;

		//PM
		//cout << "projection" << endl;
		for (int j = 0; j < vec.size(); j++) {
			for (int k = 0; k < 4; k++) {
				tmp += vec[j][0] * pm[k][0] + vec[j][1] * pm[k][1] + vec[j][2] * pm[k][2] + vec[j][3] * pm[k][3];
				str[k] = tmp;
				tmp = 0;
			}
			vec[j][0] = str[0];
			vec[j][1] = str[1];
			vec[j][2] = str[2];
			vec[j][3] = str[3];
		}
		
		//perspective divide
		for (int j = 0; j < vec.size(); j++) {
			for (int k = 0; k < 4; k++) {
				vec[j][k] /= vec[j][3];
			}
		}
		//cout << "point  " << vec[0][0] << " " << vec[0][1] << " " << vec[0][2] << endl;

		//WVM
		//cout << "view" << endl;
		for (int j = 0; j < vec.size(); j++) {
			for (int k = 0; k < 4; k++) {
				tmp += vec[j][0] * wvm[k][0] + vec[j][1] * wvm[k][1] + vec[j][2] * wvm[k][2] + vec[j][3] * wvm[k][3];
				vec[j][k] = tmp;
				str[k] = tmp;
				tmp = 0;
			}
			vec[j][0] = str[0];
			vec[j][1] = str[1];
			vec[j][2] = str[2];
			vec[j][3] = str[3];
		}
		//cout << "3333333333333" << endl;
		//cout << "point  " << vec[0][0] << " " << vec[0][1] << " " << vec[0][2] << endl;
		//cout <<"OR "<< value[i][1] << endl;
		//face

		int n;
		for (int j = 0; j < vec2.size(); j++) {
			//get normal vector from the plane
			/*
			float normalvector[3];
			float point1[3];
			float point2[3];
			float point3[3];
			float v1[3];
			float v2[3];
			for (int k = 0; k < 3; k++) {
				point1[k] = vec[vec2[j][0] - 1][k];
				point2[k] = vec[vec2[j][1] - 1][k];
				point3[k] = vec[vec2[j][2] - 1][k];
				v1[k] = point2[k] - point1[k];
				v2[k] = point3[k] - point1[k];
			}
			cal_cross(normalvector, v1, v2);
			normalize(normalvector);
			
			//get the polygon color
			float lightvector[3];
			float rvector[3];
			float vvector[3];
			float NL = 0, RV = 0;
			//cout << "lightsize" << light_vec.size() << endl;
			//cout << "normalvector " << normalvector[0]<<" " << normalvector[1] << " " << normalvector[2] << endl;
			
			Ir = ambient_r * value[i][0];
			Ig = ambient_g * value[i][1];
			Ib = ambient_b * value[i][2];
			
			lightvector[0] = light_vec[light_vec.size() - 1][3] - vec[vec2[j][0] - 1][0];
			lightvector[1] = light_vec[light_vec.size() - 1][4] - vec[vec2[j][0] - 1][1];
			lightvector[2] = light_vec[light_vec.size() - 1][5] - vec[vec2[j][0] - 1][2];
			vvector[0] = eyex - vec[vec2[j][0] - 1][0];
			vvector[1] = eyey - vec[vec2[j][0] - 1][1];
			vvector[2] = eyez - vec[vec2[j][0] - 1][2];
			rvector[0] = lightvector[0] + vvector[0];
			rvector[1] = lightvector[1] + vvector[1];
			rvector[2] = lightvector[2] + vvector[2];
			normalize(lightvector);
			normalize(rvector);
			NL = dot(normalvector, lightvector);
			RV = dot(normalvector, rvector);
			RV = pow(RV, value[i][5]);
			
			Ir += (value[i][3] * light_vec[light_vec.size() - 1][0] * NL) * value[i][0] + value[i][4] * light_vec[light_vec.size() - 1][0] * RV;
			Ig += (value[i][3] * light_vec[light_vec.size() - 1][1] * NL) * value[i][1] + value[i][4] * light_vec[light_vec.size() - 1][1] * RV;
			Ib += (value[i][3] * light_vec[light_vec.size() - 1][2] * NL) * value[i][2] + value[i][4] * light_vec[light_vec.size() - 1][2] * RV;
			*/
			/*
			Ir = ambient_r * value[i][0];
			Ig = ambient_g * value[i][1];
			Ib = ambient_b * value[i][2];
			lightvector[0] = light_vec[light_vec.size() - 1][3] - vec[vec2[j][0] - 1][0];
			lightvector[1] = light_vec[light_vec.size() - 1][4] - vec[vec2[j][0] - 1][1];
			lightvector[2] = light_vec[light_vec.size() - 1][5] - vec[vec2[j][0] - 1][2];
			//cout << "lightvector " << lightvector[0] << " " << lightvector[1] << " " << lightvector[2] << endl;
			normalize(lightvector);
			vvector[0] = eyex - vec[vec2[j][0] - 1][0];
			vvector[1] = eyey - vec[vec2[j][0] - 1][1];
			vvector[2] = eyez - vec[vec2[j][0] - 1][2];
			//cout << "vvector " << vvector[0] << " " << vvector[1] << " " << vvector[2] << endl;
			NL = dot(lightvector, normalvector);
			rvector[0] = 2 * NL * normalvector[0] - lightvector[0];
			rvector[1] = 2 * NL * normalvector[1] - lightvector[1];
			rvector[2] = 2 * NL * normalvector[2] - lightvector[2];
			//rvector[0] = lightvector[0] + vvector[0];
			//rvector[1] = lightvector[1] + vvector[1];
			//rvector[2] = lightvector[2] + vvector[2];
			normalize(vvector);
			normalize(rvector);
			normalize(lightvector);

			RV = dot(vvector, rvector);
			RV = pow(RV, value[i][5]);
			Ir += value[i][3] * light_vec[light_vec.size() - 1][0] * NL * value[i][0] + value[i][4] * light_vec[light_vec.size() - 1][0] * RV;
			Ig += value[i][3] * light_vec[light_vec.size() - 1][1] * NL * value[i][1] + value[i][4] * light_vec[light_vec.size() - 1][1] * RV;
			Ib += value[i][3] * light_vec[light_vec.size() - 1][2] * NL * value[i][2] + value[i][4] * light_vec[light_vec.size() - 1][2] * RV;
			*/


			/*
			for (int m = 0; m < light_vec.size(); m++) {
				cout << "point  " << vec[vec2[j][0] - 1][0] << " " << vec[vec2[j][0] - 1][1] << " " << vec[vec2[j][0] - 1][2] << endl;
				lightvector[0] = light_vec[m][3] - vec[vec2[j][0] - 1][0];
				lightvector[1] = light_vec[m][4] - vec[vec2[j][0] - 1][1];
				lightvector[2] = light_vec[m][5] - vec[vec2[j][0] - 1][2];
				cout << "lightvector " << lightvector[0] << " " << lightvector[1] << " " << lightvector[2] << endl;
				normalize(lightvector);
				vvector[0] = eyex - vec[vec2[j][0] - 1][0];
				vvector[1] = eyey - vec[vec2[j][0] - 1][1];
				vvector[2] = eyez - vec[vec2[j][0] - 1][2];
				cout << "vvector " << vvector[0] << " " << vvector[1] << " " << vvector[2] << endl;
				NL = dot(lightvector, normalvector);
				rvector[0] = 2 * NL * normalvector[0] - lightvector[0];
				rvector[1] = 2 * NL * normalvector[1] - lightvector[1];
				rvector[2] = 2 * NL * normalvector[2] - lightvector[2];
				//rvector[0] = lightvector[0] + vvector[0];
				//rvector[1] = lightvector[1] + vvector[1];
				//rvector[2] = lightvector[2] + vvector[2];
				normalize(vvector);
				normalize(rvector);
				normalize(lightvector);
				
				RV = dot(vvector, rvector);
				RV = pow(RV, value[i][5]);
				Ir += value[i][3] * light_vec[m][0] * NL * value[i][0] + value[i][4] * light_vec[m][0] * RV;
				Ig += value[i][3] * light_vec[m][1] * NL * value[i][1] + value[i][4] * light_vec[m][1] * RV;
				Ib += value[i][3] * light_vec[m][2] * NL * value[i][2] + value[i][4] * light_vec[m][2] * RV;
			}*/
			

			//cout << Ir << " " << Ig << " " << Ib << endl;
			float normalvector[3];
			float point1[3];
			float point2[3];
			float point3[3];
			float v1[3];
			float v2[3];
			for (int k = 0; k < 3; k++) {
				point1[k] = vec[vec2[j][0] - 1][k];
				point2[k] = vec[vec2[j][1] - 1][k];
				point3[k] = vec[vec2[j][2] - 1][k];
				v1[k] = point2[k] - point1[k];
				v2[k] = point1[k] - point3[k];
			}
			//v1[0] = vec[vec2[j][1] - 1][0] - vec[vec2[j][0] - 1][0];
			//v1[1] = vec[vec2[j][1] - 1][1] - vec[vec2[j][0] - 1][1];
			//v1[2] = vec[vec2[j][1] - 1][2] * 10 - vec[vec2[j][0] - 1][2]*10;
			//v1[0] = vec[vec2[j][0] - 1][0] - vec[vec2[j][2] - 1][0];
			//v1[1] = vec[vec2[j][0] - 1][1] - vec[vec2[j][2] - 1][1];
			//v1[2] = vec[vec2[j][0] - 1][2] * 10 - vec[vec2[j][2] - 1][2] * 10;
			//v1[2] *= 100;
			//v2[2] *= 100;
			cal_cross(normalvector, v1, v2);
			//normalize(normalvector);
			n = 3;
			if (vec2[j][3] == 0) {
				n = 2;
			}
			// Ax+By+Cz+D=0
			float A = normalvector[0];
			float B = normalvector[1];
			float C = normalvector[2];
			float D = -(A * vec[vec2[j][0] - 1][0]*10 + B * vec[vec2[j][0] - 1][1] * 10 + C * vec[vec2[j][0] - 1][2]) * 10;
			int x1, y1, x2, y2;
			points.clear();
			for (int k = 0; k < n; k++) {
				x1 = vec[vec2[j][k] - 1][0];
				y1 = vec[vec2[j][k] - 1][1];
				x2 = vec[vec2[j][k + 1] - 1][0];
				y2 = vec[vec2[j][k + 1] - 1][1];
				//cout << "draw" << endl;
				drawline(x1, y1, x2, y2);
			}
			x1 = vec[vec2[j][0] - 1][0];
			y1 = vec[vec2[j][0] - 1][1];
			x2 = vec[vec2[j][n] - 1][0];
			y2 = vec[vec2[j][n] - 1][1];
			drawline(x1, y1, x2, y2);
			//cout << "points " << points.size() << endl;
			for (int k = 0; k < points.size(); k++) {
				int tmp = k;
				for (int m = k + 1; m < points.size(); m++) {
					if (points[m][0] < points[tmp][0]) {
						tmp = m;
					}
				}
				int temp = points[tmp][0];
				points[tmp][0] = points[k][0];
				points[k][0] = temp;
				temp = points[tmp][1];
				points[tmp][1] = points[k][1];
				points[k][1] = temp;
			}
			//cout << "finish" << endl;
			float z;
			for (int k = 0; k < points.size(); k++) {
				x1 = points[k][0];
				y1 = points[k][1];
				y2 = points[k][1];
				for (int m = k + 1; m < points.size(); m++) {
					if (points[m][0] == x1) {
						if (y1 > points[m][1]) {
							y1 = points[m][1];
						}
						else if (y2<points[m][1]) {
							y2 = points[m][1];
						}
					}
					else {
						break;
					}
				}
				//cout << "finish" << endl;
				for (int m = y1; m <= y2; m++) {
					z = (-(A * x1 + B * m + D) / C);
					/*
					glutSwapBuffers();
					glBegin(GL_POINTS);
					glColor3d(vec2[j][4], vec2[j][5], vec2[j][6]);
					glVertex2i(x1, m);
					glEnd();
					glFlush();
					glutSwapBuffers();
					*/
					
					if (z < Zbuffer[x1][m]) {
						Zbuffer[x1][m] = z;
						Cbuffer_r[x1][m] = vec2[j][4];
						Cbuffer_g[x1][m] = vec2[j][5];
						Cbuffer_b[x1][m] = vec2[j][6];
					}
					
				}
			}
		}
	}
	
	glutSwapBuffers();
	for (int i = l_x; i <= r_x;i++) {
		for (int j = bottom; j <= top; j++) {
			//cout << "int";
			glBegin(GL_POINTS);
			glColor3d(Cbuffer_r[i][j], Cbuffer_g[i][j], Cbuffer_b[i][j]);
			glVertex2i(i, j);
			glEnd();
		}
	}
	glFlush();
	glutSwapBuffers();
}

void reset() {
	//cout << "reset" << endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
				trans[i][j] = 1;
			else
				trans[i][j] = 0;
		}
	}
}

//Mid-point algo.
void drawline(int x1, int y1, int x2, int y2) {
	//glutSwapBuffers();
	//glPointSize(1);
	if (x1 >= l_x && x1 <= r_x && y1 >= bottom && y1 <= top) {
		//glBegin(GL_POINTS);
		//glVertex2i(x1, y1);
		//glEnd();
		points.push_back({x1,y1});
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
			if (x >= l_x && x <= r_x && y >= bottom && y <= top) {
				//glBegin(GL_POINTS);
				//glVertex2i(x, y);
				//glEnd();
				points.push_back({ x,y });
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
			if (x >= l_x && x <= r_x && y >= bottom && y <= top) {
				//glBegin(GL_POINTS);
				//glVertex2i(x, y);
				//glEnd();
				points.push_back({ x,y});
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
			if (x >= l_x && x <= r_x && y >= bottom && y <= top) {
				//glBegin(GL_POINTS);
				//glVertex2i(x, y);
				//glEnd();
				points.push_back({ x,y });
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
			if (x >= l_x && x <= r_x && y >= bottom && y <= top) {
				//glBegin(GL_POINTS);
				//glVertex2i(x, y);
				//glEnd();
				points.push_back({ x,y });
			}
		}
	}
	//glutSwapBuffers();
}

void select(string line) {
	//cout << "size: " << ascvec.size() << endl;
	stringstream input(line);
	string text;
	vector<string> input_list;
	float a, b, c, d, e, f;
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
		c = stof(input_list[3]);
		scale(a, b, c);
	}
	else if (input_list[0] == "rotate") {
		cout << "Rotate" << endl;
		a = stof(input_list[1]);
		b = stof(input_list[2]);
		c = stof(input_list[3]);
		//cout << a << b << c << endl;
		rotate(a, b, c);
	}
	else if (input_list[0] == "translate") {
		cout << "Translate" << endl;
		a = stof(input_list[1]);
		b = stof(input_list[2]);
		c = stof(input_list[3]);
		translate(a, b, c);
	}
	else if (input_list[0] == "viewport") {
		cout << "Viewport" << endl;
		//cout << input_list.size() << endl;
		a = stof(input_list[1]);
		b = stof(input_list[2]);
		c = stof(input_list[3]);
		d = stof(input_list[4]);
		viewport(a, b, c, d);
	}
	else if (input_list[0] == "object") {
		cout << "object" << endl;
		a = stof(input_list[2]);
		b = stof(input_list[3]);
		c = stof(input_list[4]);
		d = stof(input_list[5]);
		e = stof(input_list[6]);
		f = stof(input_list[7]);
		readobject(input_list[1], a, b, c, d, e, f);
	}
	else if (input_list[0] == "observer") {
		float g, h, i, j;
		a = stof(input_list[1]);
		b = stof(input_list[2]);
		c = stof(input_list[3]);
		d = stof(input_list[4]);
		e = stof(input_list[5]);
		f = stof(input_list[6]);
		g = stof(input_list[7]);
		h = stof(input_list[8]);
		i = stof(input_list[9]);
		j = stof(input_list[10]);
		observer(a, b, c, d, e, f, g, h, i, j);
	}
	else if (input_list[0] == "display") {
		cout << "display" << endl;
		display();
		system("pause");
	}
	else if (input_list[0] == "reset") {
		//cout << "reset" << endl;
		reset();
	}
	else if (input_list[0] == "end") {
		cout << "end" << endl;
		exit(0);
	}
	else if (input_list[0] == "ambient") {
		cout << "Ambient" << endl;
		a = stof(input_list[1]);
		b = stof(input_list[2]);
		c = stof(input_list[3]);
		ambient(a, b, c);
	}
	else if (input_list[0] == "light") {
		float g;
		a = stoi(input_list[1]);
		b = stof(input_list[2]);
		c = stof(input_list[3]);
		d = stof(input_list[4]);
		e = stof(input_list[5]);
		f = stof(input_list[6]);
		g = stof(input_list[7]);
		light(a, b, c, d, e, f, g);
	}
	else if (input_list[0] == "background") {
		cout << "background" << endl;
		a = stof(input_list[1]);
		b = stof(input_list[2]);
		c = stof(input_list[3]);
		background(a, b, c);
	}
	else {	//command
		cout << "##" << line << " ###" << endl;
	}
}

int main(int argc, char** argv) {
	reset();
	system("pause");
	string line = "";
	if (argc == 2) {
		infile = argv[1];
		ifstream ff(infile);
		//cout << "inininininin" << endl;
		ff >> winx;
		ff >> winy;
	}
	//cout << winx << " " << winy << endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(winx, winy);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Lab4 Window");
	glutDisplayFunc(display_fun);
	gluOrtho2D(0, winx, 0, winy);
	glutSwapBuffers();
	glutMainLoop();
}
