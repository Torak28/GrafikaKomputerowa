#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <iostream>

typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 0.0 };
GLfloat theta = 0.0;
GLfloat phi = 0.0;
GLfloat R = 10.0;
static GLfloat pix2angle;
static GLint status = 0;       
static int x_pos_old = 0;
static int y_pos_old = 0;
static int z_pos_old = 0;
float fix = 1.0;

#define M_PI 3.14159265358979323846
typedef float point3[3];

using namespace std;

static int delta_x = 0;        
static int delta_y = 0;        
static int delta_z = 0;

void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };
	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;
		y_pos_old = y;
		status = 1;
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		y_pos_old = y;
		status = 2;
	}
	else
		status = 0;
}

void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old;
	delta_y = y - y_pos_old;
	x_pos_old = x;
	y_pos_old = y;
	glutPostRedisplay();
}

float wyliczX(float u, float v) {
	float PIV = M_PI * v;
	return ((-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(PIV));
}
float wyliczY(float u, float v) {
	return (160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2));
}
float wyliczZ(float u, float v) {
	float PIV = M_PI  * v;
	return ((-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(PIV));
}

struct Punkt {
	float x;
	float y;
	float z;
	float xRGB;
	float yRGB;
	float zRGB;
};


/********************************OBSŁUGA********************************/
// 1- punkty, 2- siatka, 3 - wypełnione trójkąty, 4 - czjniczek
int model = 3;
int ruch = 1;
float KAT = 90.0;

// Ilosc podzialow boku kwadratu jednostkowego
const int N = 150;
// Obrot
static GLfloat obrot[] = { 0.0, 0.0, 0.0 };
// Tablica sluzaca do zapisywania punktow
struct Punkt punktyJaja[N][N];

float krok = 1.0 / N;

/********************************KONIEC********************************/

void nic() {
	// Liczenie Punktow
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			punktyJaja[i][j].x = wyliczX(i * krok, j * krok);
			punktyJaja[i][j].y = wyliczY(i * krok, j * krok);
			punktyJaja[i][j].z = wyliczZ(i * krok, j * krok);
			//Kolorki
			punktyJaja[i][j].xRGB = ((double)rand() / (RAND_MAX));
			punktyJaja[i][j].yRGB = ((double)rand() / (RAND_MAX));
			punktyJaja[i][j].zRGB = ((double)rand() / (RAND_MAX));
		}
	}
}

void Jajo() {
	// Punkty
	if (model == 1) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				glBegin(GL_POINTS);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
				glEnd();
			}
		}
	}
	// Linie
	else if (model == 2) {
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		//Poziome Linie
		for (int i = 0; i < N; i++) {
			int pom = N - i;
			for (int j = 0; j < N - 1; j++) {
				glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
				glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
			}
			// Problem z nie rysowaniem łączeń w okregu
			if (pom != N) {
				glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
				glVertex3f(punktyJaja[pom][N - 1].x, punktyJaja[pom][N - 1].y - 5, punktyJaja[pom][N - 1].z);
			}
		}

		//Poionowe Linie
		glColor3f(1.0f, 1.0f, 0.0f);
		for (int i = 0; i < N; i++) {
			int pom = N - i;
			for (int j = 0; j < N; j++) {
				// Problem w nierysowaniu łączenia z punktem [0][0]
				if (i == N - 1) {
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
				}
				else {
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glVertex3f(punktyJaja[i + 1][j].x, punktyJaja[i + 1][j].y - 5, punktyJaja[i + 1][j].z);
				}
			}
		}

		//Skośne Linie
		glColor3f(0.294f, 0.0f, 0.510f);
		for (int i = N / 2; i < N; i++) {
			int pom = N - i;
			for (int j = 0; j < N - 1; j++) {
				if (i == N - 1) {
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
				}
				else {
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glVertex3f(punktyJaja[i + 1][j + 1].x, punktyJaja[i + 1][j + 1].y - 5, punktyJaja[i + 1][j + 1].z);
				}
				if (pom != N) {
					glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
					glVertex3f(punktyJaja[pom + 1][N - 1].x, punktyJaja[pom + 1][N - 1].y - 5, punktyJaja[pom + 1][N - 1].z);
				}
			}
		}
		glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
		glVertex3f(punktyJaja[1][N - 1].x, punktyJaja[1][N - 1].y - 5, punktyJaja[1][N - 1].z);

		glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
		glVertex3f(punktyJaja[N - 1][N - 1].x, punktyJaja[N - 1][N - 1].y - 5, punktyJaja[N - 1][N - 1].z);
		for (int i = 1; i < N / 2 + 1; i++) {
			int pom = N - i;
			for (int j = 0; j < N - 1; j++) {
				glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
				glVertex3f(punktyJaja[i - 1][j + 1].x, punktyJaja[i - 1][j + 1].y - 5, punktyJaja[i - 1][j + 1].z);
				if (pom != N) {
					glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
					glVertex3f(punktyJaja[pom - 1][N - 1].x, punktyJaja[pom - 1][N - 1].y - 5, punktyJaja[pom - 1][N - 1].z);
				}
			}
		}

		glEnd();
	}
	// Kolorki
	else if (model == 3) {
		glBegin(GL_TRIANGLES);
		for (int i = N / 2; i < N; i++) {
			int pom = N - i;
			for (int j = 0; j < N - 1; j++) {
				if (i == N - 1) {
					glColor3f(punktyJaja[i][j].xRGB, punktyJaja[i][j].yRGB, punktyJaja[i][j].zRGB);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glColor3f(punktyJaja[i][j + 1].xRGB, punktyJaja[i][j + 1].yRGB, punktyJaja[i][j + 1].zRGB);
					glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
					glColor3f(punktyJaja[0][0].xRGB, punktyJaja[0][0].yRGB, punktyJaja[0][0].zRGB);
					glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
				}
				else {
					glColor3f(punktyJaja[i][j].xRGB, punktyJaja[i][j].yRGB, punktyJaja[i][j].zRGB);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glColor3f(punktyJaja[i + 1][j].xRGB, punktyJaja[i + 1][j].yRGB, punktyJaja[i + 1][j].zRGB);
					glVertex3f(punktyJaja[i + 1][j].x, punktyJaja[i + 1][j].y - 5, punktyJaja[i + 1][j].z);
					glColor3f(punktyJaja[i + 1][j + 1].xRGB, punktyJaja[i + 1][j + 1].yRGB, punktyJaja[i + 1][j + 1].zRGB);
					glVertex3f(punktyJaja[i + 1][j + 1].x, punktyJaja[i + 1][j + 1].y - 5, punktyJaja[i + 1][j + 1].z);

					glColor3f(punktyJaja[i][j].xRGB, punktyJaja[i][j].yRGB, punktyJaja[i][j].zRGB);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glColor3f(punktyJaja[i][j + 1].xRGB, punktyJaja[i][j + 1].yRGB, punktyJaja[i][j + 1].zRGB);
					glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
					glColor3f(punktyJaja[i + 1][j + 1].xRGB, punktyJaja[i + 1][j + 1].yRGB, punktyJaja[i + 1][j + 1].zRGB);
					glVertex3f(punktyJaja[i + 1][j + 1].x, punktyJaja[i + 1][j + 1].y - 5, punktyJaja[i + 1][j + 1].z);
				}
				if (pom != N) {
					glColor3f(punktyJaja[i][0].xRGB, punktyJaja[i][0].yRGB, punktyJaja[i][0].zRGB);
					glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
					glColor3f(punktyJaja[pom][N - 1].xRGB, punktyJaja[pom][N - 1].yRGB, punktyJaja[pom][N - 1].zRGB);
					glVertex3f(punktyJaja[pom][N - 1].x, punktyJaja[pom][N - 1].y - 5, punktyJaja[pom][N - 1].z);
					glColor3f(punktyJaja[pom + 1][N - 1].xRGB, punktyJaja[pom + 1][N - 1].yRGB, punktyJaja[pom + 1][N - 1].zRGB);
					glVertex3f(punktyJaja[pom + 1][N - 1].x, punktyJaja[pom + 1][N - 1].y - 5, punktyJaja[pom + 1][N - 1].z);

					if (i != N / 2) {
						glColor3f(punktyJaja[i][0].xRGB, punktyJaja[i][0].yRGB, punktyJaja[i][0].zRGB);
						glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
						glColor3f(punktyJaja[i - 1][0].xRGB, punktyJaja[i - 1][0].yRGB, punktyJaja[i - 1][0].zRGB);
						glVertex3f(punktyJaja[i - 1][0].x, punktyJaja[i - 1][0].y - 5, punktyJaja[i - 1][0].z);
						glColor3f(punktyJaja[pom + 1][N - 1].xRGB, punktyJaja[pom + 1][N - 1].yRGB, punktyJaja[pom + 1][N - 1].zRGB);
						glVertex3f(punktyJaja[pom + 1][N - 1].x, punktyJaja[pom + 1][N - 1].y - 5, punktyJaja[pom + 1][N - 1].z);
					}
				}
			}
		}
		glColor3f(punktyJaja[0][0].xRGB, punktyJaja[0][0].yRGB, punktyJaja[0][0].zRGB);
		glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
		glColor3f(punktyJaja[N - 1][0].xRGB, punktyJaja[N - 1][0].yRGB, punktyJaja[N - 1][0].zRGB);
		glVertex3f(punktyJaja[N - 1][0].x, punktyJaja[N - 1][0].y - 5, punktyJaja[N - 1][0].z);
		glColor3f(punktyJaja[1][N - 1].xRGB, punktyJaja[1][N - 1].yRGB, punktyJaja[1][N - 1].zRGB);
		glVertex3f(punktyJaja[1][N - 1].x, punktyJaja[1][N - 1].y - 5, punktyJaja[1][N - 1].z);


		glColor3f(punktyJaja[0][0].xRGB, punktyJaja[0][0].yRGB, punktyJaja[0][0].zRGB);
		glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
		glColor3f(punktyJaja[1][0].xRGB, punktyJaja[1][0].yRGB, punktyJaja[1][0].zRGB);
		glVertex3f(punktyJaja[1][0].x, punktyJaja[1][0].y - 5, punktyJaja[1][0].z);
		glColor3f(punktyJaja[N - 1][N - 1].xRGB, punktyJaja[N - 1][N - 1].yRGB, punktyJaja[N - 1][N - 1].zRGB);
		glVertex3f(punktyJaja[N - 1][N - 1].x, punktyJaja[N - 1][N - 1].y - 5, punktyJaja[N - 1][N - 1].z);


		for (int i = 0; i < N / 2 + 1; i++) {
			int pom = N - i;
			for (int j = 0; j < N - 1; j++) {
				if (i == 1) {
					glColor3f(punktyJaja[i][j].xRGB, punktyJaja[i][j].yRGB, punktyJaja[i][j].zRGB);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glColor3f(punktyJaja[i][j + 1].xRGB, punktyJaja[i][j + 1].yRGB, punktyJaja[i][j + 1].zRGB);
					glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
					glColor3f(punktyJaja[0][0].xRGB, punktyJaja[0][0].yRGB, punktyJaja[0][0].zRGB);
					glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
				}
				else {
					if (i != 0) {
						glColor3f(punktyJaja[i][j].xRGB, punktyJaja[i][j].yRGB, punktyJaja[i][j].zRGB);
						glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
						glColor3f(punktyJaja[i - 1][j].xRGB, punktyJaja[i - 1][j].yRGB, punktyJaja[i - 1][j].zRGB);
						glVertex3f(punktyJaja[i - 1][j].x, punktyJaja[i - 1][j].y - 5, punktyJaja[i - 1][j].z);
						glColor3f(punktyJaja[i - 1][j + 1].xRGB, punktyJaja[i - 1][j + 1].yRGB, punktyJaja[i - 1][j + 1].zRGB);
						glVertex3f(punktyJaja[i - 1][j + 1].x, punktyJaja[i - 1][j + 1].y - 5, punktyJaja[i - 1][j + 1].z);
					}


					glColor3f(punktyJaja[i][j].xRGB, punktyJaja[i][j].yRGB, punktyJaja[i][j].zRGB);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glColor3f(punktyJaja[i][j + 1].xRGB, punktyJaja[i][j + 1].yRGB, punktyJaja[i][j + 1].zRGB);
					glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
					glColor3f(punktyJaja[i - 1][j + 1].xRGB, punktyJaja[i - 1][j + 1].yRGB, punktyJaja[i - 1][j + 1].zRGB);
					glVertex3f(punktyJaja[i - 1][j + 1].x, punktyJaja[i - 1][j + 1].y - 5, punktyJaja[i - 1][j + 1].z);
				}
				if (pom != N) {
					glColor3f(punktyJaja[i][0].xRGB, punktyJaja[i][0].yRGB, punktyJaja[i][0].zRGB);
					glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
					glColor3f(punktyJaja[pom][N - 1].xRGB, punktyJaja[pom][N - 1].yRGB, punktyJaja[pom][N - 1].zRGB);
					glVertex3f(punktyJaja[pom][N - 1].x, punktyJaja[pom][N - 1].y - 5, punktyJaja[pom][N - 1].z);
					glColor3f(punktyJaja[pom - 1][N - 1].xRGB, punktyJaja[pom - 1][N - 1].yRGB, punktyJaja[pom - 1][N - 1].zRGB);
					glVertex3f(punktyJaja[pom - 1][N - 1].x, punktyJaja[pom - 1][N - 1].y - 5, punktyJaja[pom - 1][N - 1].z);


					glColor3f(punktyJaja[i][0].xRGB, punktyJaja[i][0].yRGB, punktyJaja[i][0].zRGB);
					glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
					glColor3f(punktyJaja[i + 1][0].xRGB, punktyJaja[i + 1][0].yRGB, punktyJaja[i + 1][0].zRGB);
					glVertex3f(punktyJaja[i + 1][0].x, punktyJaja[i + 1][0].y - 5, punktyJaja[i + 1][0].z);
					glColor3f(punktyJaja[pom - 1][N - 1].xRGB, punktyJaja[pom - 1][N - 1].yRGB, punktyJaja[pom - 1][N - 1].zRGB);
					glVertex3f(punktyJaja[pom - 1][N - 1].x, punktyJaja[pom - 1][N - 1].y - 5, punktyJaja[pom - 1][N - 1].z);
				}
			}
		}
		glEnd();
	}
	else if (model == 4) {
		glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
	}
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if (status == 1) {
		theta += delta_x*pix2angle / 20.0;
		phi += delta_y*pix2angle / 20.;
		if (phi>2 * M_PI)
			phi = 0;
		if (phi<0)
			phi = 2 * M_PI;
		if (phi>M_PI / 2)
			fix = -1.0;
		else
			fix = 1.0;
		if (phi>M_PI + (M_PI / 2))
			fix = 1.0;
	}
	else if (status == 2)
	{
		R += delta_y*pix2angle / 20.0;
	}
	viewer[0] = R * cos(theta) * cos(phi);
	viewer[1] = R * sin(phi);
	viewer[2] = R * sin(theta) * cos(phi);
	if (R > 18.00) {
		R = 9.00;
	}
	if (R < 0.50) {
		R = 9.00;
	}
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, fix, 0.0);

	glColor3f(1.0f, 1.0f, 1.0f);
	Axes();
	Jajo();
	glFlush();
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
	if (key == 'q') model = 1;
	if (key == 'w') model = 2;
	if (key == 'e') model = 3;
	if (key == 'r') model = 4;

	RenderScene();
}

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1.0, 1.0, 30.0);
	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	nic();
	glutCreateWindow("OPENGL - interakcja z użytkownikiem");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutKeyboardFunc(keys);
	glutMainLoop();
}