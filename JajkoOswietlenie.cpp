/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj� osi  
//  uk�adu wsp�rz�dnych dla rzutowania perspektywicznego
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <iostream>

typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat theta1 = 0.0;   // k�t obrotu obiektu
static GLfloat theta2 = 0.0;   // k�t obrotu obiektu
static GLfloat theta3 = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy 
// 0 - nie naci�ni�to �adnego klawisza
// 1 - naci�ni�ty zosta� lewy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int z_pos_old = 0;

#define M_PI 3.14159265358979323846
typedef float point3[3];

using namespace std;

static int delta_x = 0;        // r�nica pomi�dzy pozycj� bie��c�
// i poprzedni� kursora myszy 
static int delta_y = 0;        // r�nica pomi�dzy pozycj� bie��c�
// i poprzedni� kursora myszy 
// inicjalizacja po�o�enia obserwatora
static int delta_z = 0;
/*************************************************************************************/
// Funkcja rysuj�ca osie uk�adu wsp�?rz?dnych

void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
	//  pocz?tek i koniec obrazu osi y
	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y
	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z
	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}
/*************************************************************************************/
// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana, gdy trzeba 
// przerysowa� scen�)

/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia warto�ci odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
		y_pos_old = y;
		status = 1;           // wci�ni�ty zosta� lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		y_pos_old = y;
		status = 2;
		//cout << y << "\n";
	}
	else

		status = 0;          // nie zosta� wci�ni�ty �aden klawisz 
}

/*************************************************************************************/
// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich 
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
	delta_y = y - y_pos_old;
	x_pos_old = x;
	y_pos_old = y;            // podstawienie bie��cego po�o�enia jako poprzednie

	glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/
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

float wyliczXu(float u, float v) {
	float PIV = M_PI * v;
	return (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45) * cos(PIV);
}
float wyliczXv(float u, float v) {
	float PIV = M_PI * v;
	return M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * sin(PIV);
}
float wyliczYu(float u, float v) {
	return (640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u);
}
float wyliczYv(float u, float v) {
	return 0;
}
float wyliczZu(float u, float v) {
	float PIV = M_PI * v;
	return (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)  * sin(PIV);
}
float wyliczZv(float u, float v) {
	float PIV = M_PI * v;
	return -M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * cos(PIV);
}
float dlugoscWektora(float xv, float yv, float zv) {
	return sqrt(pow(xv, 2) + pow(yv, 2) + pow(zv, 2));
}

struct Punkt {
	float x;
	float y;
	float z;
	float xRGB;
	float yRGB;
	float zRGB;
	float xV;
	float yV;
	float zV;
};

/********************************OBS�UGA********************************/
// 1- punkty, 2- siatka, 3 - wype�nione tr�jk�ty, 4 - czjniczek
int model = 3;
int ruch = 1;
float KAT = 0.0;

// Ilosc podzialow boku kwadratu jednostkowego
const int N = 90;
// Obrot
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
// Tablica sluzaca do zapisywania punktow
struct Punkt punktyJaja[N][N];

float krok = 1.0 / N;

float x_swiatla = 0;
float y_swiatla = 0;
float x_swiatla2 = 0;
float y_swiatla2 = 0;

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

			float xU = wyliczXu(i * krok, j * krok);
			float xV = wyliczXv(i * krok, j * krok);
			float yU = wyliczYu(i * krok, j * krok);
			float yV = wyliczYv(i * krok, j * krok);
			float zU = wyliczZu(i * krok, j * krok);
			float zV = wyliczZv(i * krok, j * krok);
			float xVector = (yU * zV - zU * yV);
			float yVector = (zU * xV - xU * zV);
			float zVector = (xU * yV - yU * xV);
			float dl = dlugoscWektora(xVector, yVector, zVector);
			punktyJaja[i][j].xV = xVector / dl;
			punktyJaja[i][j].yV = yVector / dl;
			punktyJaja[i][j].zV = zVector / dl;
		}
	}
}

void Jajo() {
	nic();
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
			// Problem z nie rysowaniem ��cze� w okregu
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
				// Problem w nierysowaniu ��czenia z punktem [0][0]
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

		//Sko�ne Linie
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
					glNormal3f(-punktyJaja[i][j].xV, -punktyJaja[i][j].yV, -punktyJaja[i][j].zV);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glNormal3f(-punktyJaja[i][j + 1].xV, -punktyJaja[i][j + 1].yV, -punktyJaja[i][j + 1].zV);
					glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
					glNormal3f(-punktyJaja[0][0].xV, -punktyJaja[0][0].yV, -punktyJaja[0][0].zV);
					glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
				}
				else {
					glNormal3f(-punktyJaja[i][j].xV, -punktyJaja[i][j].yV, -punktyJaja[i][j].zV);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glNormal3f(-punktyJaja[i + 1][j].xV, -punktyJaja[i + 1][j].yV, -punktyJaja[i + 1][j].zV);
					glVertex3f(punktyJaja[i + 1][j].x, punktyJaja[i + 1][j].y - 5, punktyJaja[i + 1][j].z);
					glNormal3f(-punktyJaja[i + 1][j + 1].xV, -punktyJaja[i + 1][j + 1].yV, -punktyJaja[i + 1][j + 1].zV);
					glVertex3f(punktyJaja[i + 1][j + 1].x, punktyJaja[i + 1][j + 1].y - 5, punktyJaja[i + 1][j + 1].z);

					glNormal3f(-punktyJaja[i][j].xV, -punktyJaja[i][j].yV, -punktyJaja[i][j].zV);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glNormal3f(-punktyJaja[i][j + 1].xV, -punktyJaja[i][j + 1].yV, -punktyJaja[i][j + 1].zV);
					glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
					glNormal3f(-punktyJaja[i + 1][j + 1].xV, -punktyJaja[i + 1][j + 1].yV, -punktyJaja[i + 1][j + 1].zV);
					glVertex3f(punktyJaja[i + 1][j + 1].x, punktyJaja[i + 1][j + 1].y - 5, punktyJaja[i + 1][j + 1].z);
				}
				if (pom != N) {
					glNormal3f(-punktyJaja[i][0].xV, -punktyJaja[i][0].yV, -punktyJaja[i][0].zV);
					glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
					glNormal3f(punktyJaja[pom][N - 1].xV, punktyJaja[pom][N - 1].yV, punktyJaja[pom][N - 1].zV);
					glVertex3f(punktyJaja[pom][N - 1].x, punktyJaja[pom][N - 1].y - 5, punktyJaja[pom][N - 1].z);
					glNormal3f(punktyJaja[pom + 1][N - 1].xV, punktyJaja[pom + 1][N - 1].yV, punktyJaja[pom + 1][N - 1].zV);
					glVertex3f(punktyJaja[pom + 1][N - 1].x, punktyJaja[pom + 1][N - 1].y - 5, punktyJaja[pom + 1][N - 1].z);

					if (i != N / 2){
						glNormal3f(-punktyJaja[i][0].xV, -punktyJaja[i][0].yV, -punktyJaja[i][0].zV);
						glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
						glNormal3f(-punktyJaja[i - 1][0].xV, -punktyJaja[i - 1][0].yV, -punktyJaja[i - 1][0].zV);
						glVertex3f(punktyJaja[i - 1][0].x, punktyJaja[i - 1][0].y - 5, punktyJaja[i - 1][0].z);
						glNormal3f(punktyJaja[pom + 1][N - 1].xV, punktyJaja[pom + 1][N - 1].yV, punktyJaja[pom + 1][N - 1].zV);
						glVertex3f(punktyJaja[pom + 1][N - 1].x, punktyJaja[pom + 1][N - 1].y - 5, punktyJaja[pom + 1][N - 1].z);
					}
				}
			}
		}
		glNormal3f(-punktyJaja[0][0].xV, -punktyJaja[0][0].yV, -punktyJaja[0][0].zV);
		glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
		glNormal3f(-punktyJaja[N - 1][0].xV, -punktyJaja[N - 1][0].yV, -punktyJaja[N - 1][0].zV);
		glVertex3f(punktyJaja[N - 1][0].x, punktyJaja[N - 1][0].y - 5, punktyJaja[N - 1][0].z);
		glNormal3f(punktyJaja[1][N - 1].xV, punktyJaja[1][N - 1].yV, punktyJaja[1][N - 1].zV);
		glVertex3f(punktyJaja[1][N - 1].x, punktyJaja[1][N - 1].y - 5, punktyJaja[1][N - 1].z);


		glNormal3f(punktyJaja[0][0].xV, punktyJaja[0][0].yV, punktyJaja[0][0].zV);
		glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
		glNormal3f(punktyJaja[1][0].xV, punktyJaja[1][0].yV, punktyJaja[1][0].zV);
		glVertex3f(punktyJaja[1][0].x, punktyJaja[1][0].y - 5, punktyJaja[1][0].z);
		glNormal3f(-punktyJaja[N - 1][N - 1].xV, -punktyJaja[N - 1][N - 1].yV, -punktyJaja[N - 1][N - 1].zV);
		glVertex3f(punktyJaja[N - 1][N - 1].x, punktyJaja[N - 1][N - 1].y - 5, punktyJaja[N - 1][N - 1].z);


		for (int i = 0; i < N / 2 + 1; i++) {
			int pom = N - i;
			for (int j = 0; j < N - 1; j++) {
				if (i == 1) {
					glNormal3f(punktyJaja[i][j].xV, punktyJaja[i][j].yV, punktyJaja[i][j].zV);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glNormal3f(punktyJaja[i][j + 1].xV, punktyJaja[i][j + 1].yV, punktyJaja[i][j + 1].zV);
					glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
					glNormal3f(punktyJaja[0][0].xV, punktyJaja[0][0].yV, punktyJaja[0][0].zV);
					glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
				}
				else {
					if (i != 0) {
						glNormal3f(punktyJaja[i][j].xV, punktyJaja[i][j].yV, punktyJaja[i][j].zV);
						glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
						glNormal3f(punktyJaja[i - 1][j].xV, punktyJaja[i - 1][j].yV, punktyJaja[i - 1][j].zV);
						glVertex3f(punktyJaja[i - 1][j].x, punktyJaja[i - 1][j].y - 5, punktyJaja[i - 1][j].z);
						glNormal3f(punktyJaja[i - 1][j + 1].xV, punktyJaja[i - 1][j + 1].yV, punktyJaja[i - 1][j + 1].zV);
						glVertex3f(punktyJaja[i - 1][j + 1].x, punktyJaja[i - 1][j + 1].y - 5, punktyJaja[i - 1][j + 1].z);
					}


					glNormal3f(punktyJaja[i][j].xV, punktyJaja[i][j].yV, punktyJaja[i][j].zV);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glNormal3f(punktyJaja[i][j + 1].xV, punktyJaja[i][j + 1].yV, punktyJaja[i][j + 1].zV);
					glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
					glNormal3f(punktyJaja[i - 1][j + 1].xV, punktyJaja[i - 1][j + 1].yV, punktyJaja[i - 1][j + 1].zV);
					glVertex3f(punktyJaja[i - 1][j + 1].x, punktyJaja[i - 1][j + 1].y - 5, punktyJaja[i - 1][j + 1].z);
				}
				if (pom != N) {
					glNormal3f(punktyJaja[i][0].xV, punktyJaja[i][0].yV, punktyJaja[i][0].zV);
					glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
					glNormal3f(-punktyJaja[pom][N - 1].xV, -punktyJaja[pom][N - 1].yV, -punktyJaja[pom][N - 1].zV);
					glVertex3f(punktyJaja[pom][N - 1].x, punktyJaja[pom][N - 1].y - 5, punktyJaja[pom][N - 1].z);
					glNormal3f(-punktyJaja[pom - 1][N - 1].xV, -punktyJaja[pom - 1][N - 1].yV, -punktyJaja[pom - 1][N - 1].zV);
					glVertex3f(punktyJaja[pom - 1][N - 1].x, punktyJaja[pom - 1][N - 1].y - 5, punktyJaja[pom - 1][N - 1].z);


					glNormal3f(punktyJaja[i][0].xV, punktyJaja[i][0].yV, punktyJaja[i][0].zV);
					glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
					glNormal3f(punktyJaja[i + 1][0].xV, punktyJaja[i + 1][0].yV, punktyJaja[i + 1][0].zV);
					glVertex3f(punktyJaja[i + 1][0].x, punktyJaja[i + 1][0].y - 5, punktyJaja[i + 1][0].z);
					glNormal3f(-punktyJaja[pom - 1][N - 1].xV, -punktyJaja[pom - 1][N - 1].yV, -punktyJaja[pom - 1][N - 1].zV);
					glVertex3f(punktyJaja[pom - 1][N - 1].x, punktyJaja[pom - 1][N - 1].y - 5, punktyJaja[pom - 1][N - 1].z);
				}
			}
		}
		glEnd();
	}
	else if (model == 4) {
		glutSolidTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
	}
}



void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// Zdefiniowanie po�o�enia obserwatora
	//Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy�ej

	if (status == 1)                     // je�li lewy klawisz myszy wci�ni�ty
	{
		x_swiatla += delta_x*pix2angle;
		y_swiatla -= delta_y*pix2angle;
	}
	else if (status == 2) {
		x_swiatla2 += delta_x*pix2angle;
		y_swiatla2 -= delta_y*pix2angle;
	}
	else if (status == 3){
		theta1 += 5 * pix2angle;
	}
	else if (status == 4){
		theta2 += 10*pix2angle;
	}
	else if (status == 5){
		viewer[2] += 5*0.05;
		if (viewer[2] > 14.00) {
		viewer[2] = 9.00;
		}
	}
	else if (status == 6){
		viewer[2] -= 5 * 0.05;
		if (viewer[2] > 14.00) {
			viewer[2] = 9.00;
		}
	}

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glRotatef(theta1, 0.0, 1.0, 0.0);  //obr�t obiektu o nowy k�t
	glRotatef(theta2, 1.0, 0.0, 0.0);

	glColor3f(1.0f, 1.0f, 1.0f);
	// Ustawienie koloru rysowania na bia�y
	GLfloat light_position[] = { x_swiatla / 15, y_swiatla / 15, 0.0f, 1.0f };
	GLfloat light_position2[] = { x_swiatla2 / 15, y_swiatla2 / 15, 0.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position2);
	Jajo();
	// Narysowanie czajnika
	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania
	glutSwapBuffers();

}
/*************************************************************************************/
// Funkcja ustalaj�ca stan renderowania

void keys(unsigned char key, int x, int y)
{
	if (key == 'q') model = 1;
	if (key == 'w') model = 2;
	if (key == 'e') model = 3;
	if (key == 'r') model = 4;
	if (key == 'a') status = 3;
	if (key == 's') status = 4;
	if (key == 'd') status = 5;
	if (key == 'f') status = 6;

	RenderScene(); // przerysowanie obrazu sceny
}

void MyInit(void)
{
	/*************************************************************************************/

	//  Definicja materia�u z jakiego zrobiony jest czajnik
	//  i definicja �r�d�a �wiat�a

	/*************************************************************************************/
	// Definicja materia�u z jakiego zrobiony jest czajnik

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki ka =[kar,kag,kab] dla �wiat�a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki kd =[kdr,kdg,kdb] �wiat�a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki ks =[ksr,ksg,ksb] dla �wiat�a odbitego               

	GLfloat mat_shininess = { 10.0 };
	// wsp�czynnik n opisuj�cy po�ysk powierzchni

	/*************************************************************************************/
	// Definicja �r�d�a �wiat�a

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	// po�o�enie �r�d�a


	GLfloat light_ambient[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 1.0f };
	// sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	GLfloat att_linear = { 0.05f };
	// sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	GLfloat att_quadratic = { 0.001f };
	// sk�adowa kwadratowa dq dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a


	GLfloat light_position2[] = { 10.0f, 0.0f, 0.0f, 1.0f };
	GLfloat light_ambient2[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat light_diffuse2[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat light_specular2[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat att_constant2 = { 1.0f };
	GLfloat att_linear2 = { 0.05f };
	GLfloat att_quadratic2 = { 0.001f };
	/*************************************************************************************/
	// Ustawienie parametr�w materia�u i �r�d�a �wiat�a

	/*************************************************************************************/
	// Ustawienie patrametr�w materia�u


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/*************************************************************************************/
	// Ustawienie parametr�w 1 �r�d�a

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	// Ustawienie parametr�w 2 �r�d�a

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular2);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant2);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear2);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic2);


	/*************************************************************************************/
	// Ustawienie opcji systemu o�wietlania sceny

	glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
	glEnable(GL_LIGHTING);   // w�aczenie systemu o�wietlenia sceny
	glEnable(GL_LIGHT0);     // w��czenie �r�d�a o numerze 0
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora

	/*************************************************************************************/
}

// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych 
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s� 
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie

	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie��cej 

	gluPerspective(70, 1.0, 1.0, 30.0);
	// Ustawienie parametr�w dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej 

}
/*************************************************************************************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli

void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1000, 1000);

	glutCreateWindow("Rzutowanie perspektywiczne");

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  B�dzie ona wywo�ywana za ka�dym razem 
	// gdy zajdzie potrzeba przerysowania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// za zmiany rozmiaru okna      

	glutMouseFunc(Mouse);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie ruchu myszy

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania niewidocznych element�w sceny

	glutKeyboardFunc(keys);

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/