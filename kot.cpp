/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi  
//  układu współrzędnych dla rzutowania perspektywicznego
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <iostream>

typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat theta1 = 0.0;   // kąt obrotu obiektu
static GLfloat theta2 = 0.0;   // kąt obrotu obiektu
static GLfloat theta3 = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy 
// 0 - nie naciśnięto żadnego klawisza
// 1 - naciśnięty zostać lewy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int z_pos_old = 0;

#define M_PI 3.14159265358979323846
typedef float point3[3];

using namespace std;

static int delta_x = 0;        // różnica pomiędzy pozycją bieżącą
// i poprzednią kursora myszy 
static int delta_y = 0;        // różnica pomiędzy pozycją bieżącą
// i poprzednią kursora myszy 
// inicjalizacja położenia obserwatora
static int delta_z = 0;
/*************************************************************************************/
// Funkcja rysująca osie układu wspó?rz?dnych

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
// Funkcja określająca co ma być rysowane (zawsze wywoływana, gdy trzeba 
// przerysować scenę)

/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia wartości odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
		y_pos_old = y; 		
		status = 1;           // wcięnięty został lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		y_pos_old = y;
		status = 2;
	}
	else

		status = 0;          // nie został wcięnięty żaden klawisz 
}

/*************************************************************************************/
// Funkcja "monitoruje" położenie kursora myszy i ustawia wartości odpowiednich 
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie różnicy położenia kursora myszy
	delta_y = y - y_pos_old;
	x_pos_old = x;
	y_pos_old = y;            // podstawienie bieżącego położenia jako poprzednie

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

struct Punkt {
	float x;
	float y;
	float z;
};


/********************************OBSŁUGA********************************/
// 1- punkty, 2- siatka, 3 - wypełnione trójkąty, 4 - czjniczek
int model = 2;
int ruch = 1;
float KAT = 90.0;

// Ilosc podzialow boku kwadratu jednostkowego
const int N = 20;
// Obrot
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
// Tablica sluzaca do zapisywania punktow
struct Punkt punktyJaja[N][N];
struct Punkt koloryJaja[N][N];

float krok = 1.0 / N;

/********************************KONIEC********************************/

void nic() {
	// Liczenie Punktow
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			punktyJaja[i][j].x = wyliczX(i * krok, j * krok);
			punktyJaja[i][j].y = wyliczY(i * krok, j * krok);
			punktyJaja[i][j].z = wyliczZ(i * krok, j * krok);
			if (i == N)
				cout << "[" << i << "][" << j << "]:\tx = " << punktyJaja[i][j].x << "\ty = " << punktyJaja[i][j].y << "\tz = " << punktyJaja[i][j].z << "\n";
		}
	}
	// Kolorki
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			koloryJaja[i][j].x = ((double)rand() / (RAND_MAX));
			koloryJaja[i][j].y = ((double)rand() / (RAND_MAX));
			koloryJaja[i][j].z = ((double)rand() / (RAND_MAX));
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
				glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
				glEnd();
			}
		}
	}
	// Linie
	else if (model == 2) {
		glBegin(GL_LINES);
		//Poziome Linie
		/*for (int i = 0; i < N; i++) {
		int pom = N - i;
		for (int j = 0; j < N - 1; j++) {
		glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
		glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
		}
		if (pom != N) {
		glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
		glVertex3f(punktyJaja[pom][N - 1].x, punktyJaja[pom][N - 1].y - 5, punktyJaja[pom][N - 1].z);
		}
		}

		//Poionowe Linie
		for (int i = 0; i < N; i++) {
		int pom = N - i;
		for (int j = 0; j < N ; j++) {
		if (i == N-1) {
		glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
		glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
		}
		else {
		glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
		glVertex3f(punktyJaja[i + 1][j].x, punktyJaja[i + 1][j].y - 5, punktyJaja[i + 1][j].z);
		}
		}
		}*/

		//Skośne Linie
		for (int i = N / 2; i < N - 1; i++) {
			int pom = N - i;
			for (int j = 0; j < N - 1; j++) {
				glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
				glVertex3f(punktyJaja[i + 1][j + 1].x, punktyJaja[i + 1][j + 1].y - 5, punktyJaja[i + 1][j + 1].z);
			}
			if (pom != N) {
				glVertex3f(punktyJaja[i + 1][0].x, punktyJaja[i + 1][0].y - 5, punktyJaja[i + 1][0].z);
				glVertex3f(punktyJaja[pom][N - 1].x, punktyJaja[pom][N - 1].y - 5, punktyJaja[pom][N - 1].z);
			}
		}
		/*for (int i = 0; i < N/2 - 1; i++) {
		int pom = N - i;
		for (int j = 0; j < N - 1; j++) {
		glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
		glVertex3f(punktyJaja[i + 1][j + 1].x, punktyJaja[i + 1][j + 1].y - 5, punktyJaja[i + 1][j + 1].z);
		}
		if (pom != N) {
		glVertex3f(punktyJaja[i - 1][0].x, punktyJaja[i - 1][0].y - 5, punktyJaja[i - 1][0].z);
		glVertex3f(punktyJaja[pom][N - 1].x, punktyJaja[pom][N - 1].y - 5, punktyJaja[pom][N - 1].z);
		}

		}*/
		glEnd();
	}
	// Kolorki
	/*else if (model == 3) {

	}
	glEnd();
	}*/
	else if (model == 4) {
		glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
	}
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszczącym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// Zdefiniowanie położenia obserwatora
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powyżej

	if (status == 1)                     // jeśli lewy klawisz myszy wcięnięty
	{
		theta1 += delta_x*pix2angle;
		theta2 += delta_y*pix2angle;

		// modyfikacja kąta obrotu o kat proporcjonalny
	}
	else if (status == 2) {
		viewer[2] += delta_y*0.05;
	}

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glRotatef(theta1, 0.0, 1.0, 0.0);  //obrót obiektu o nowy kąt
	glRotatef(theta2, 1.0, 0.0, 0.0);

	glColor3f(1.0f, 1.0f, 1.0f);
	// Ustawienie koloru rysowania na biały

	Jajo();
	// Narysowanie czajnika
	glFlush();
	// Przekazanie poleceń rysujących do wykonania
	glutSwapBuffers();

}
/*************************************************************************************/
// Funkcja ustalająca stan renderowania

void keys(unsigned char key, int x, int y)
{
	if (key == 'q') model = 1;
	if (key == 'w') model = 2;
	if (key == 'e') model = 3;
	if (key == 'r') model = 4;

	RenderScene(); // przerysowanie obrazu sceny
}

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszczący (wypełnienia okna) ustawiono na czarny
}
/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są 
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie

	glMatrixMode(GL_PROJECTION);
	// Przełączenie macierzy bieżącej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bieżącej 

	gluPerspective(70, 1.0, 1.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielkości okna okna widoku (viewport) w zależności
	// relacji pomiędzy wysokością i szerokością okna

	glMatrixMode(GL_MODELVIEW);
	// Przełączenie macierzy bieżącej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bieżącej 

}
/*************************************************************************************/
// Główny punkt wejścia programu. Program działa w trybie konsoli

void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1000, 1000);

	glutCreateWindow("Rzutowanie perspektywiczne");

	glutDisplayFunc(RenderScene);
	// Określenie, że funkcja RenderScene będzie funkcją zwrotną
	// (callback function).  Będzie ona wywoływana za każdym razem 
	// gdy zajdzie potrzeba przerysowania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
	// za zmiany rozmiaru okna      

	glutMouseFunc(Mouse);
	// Ustala funkcję zwrotną odpowiedzialną za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcję zwrotną odpowiedzialną za badanie ruchu myszy

	MyInit();
	// Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przystąpieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// Włączenie mechanizmu usuwania niewidocznych elementów sceny

	glutKeyboardFunc(keys);

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/