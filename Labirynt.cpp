#include <windows.h>
#include "gl/gl.h"
#include "gl/glut.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <ctime>

#define OS_X 40
#define OS_Y 40

#define POLNOC 0
#define POLUDNIE 1
#define WSCHOD 2
#define ZACHOD 3

using namespace std;

int X = 1;
int Y = 1;
int szerokosc = 1;

char wolna = ' ';
char zajeta = '#';
char sciana = '|';
char start = 'S';
char koniec = 'K';

int iloscDobrych = 0;

void wydrukuj(char labirynt[OS_Y][OS_X]) {
	//system("cls");
	/*for (int i = 0; i < OS_Y; i++) {
		for (int j = 0; j < OS_X; j++) {
			cout << labirynt[i][j];
		}
		cout << "\n";
	}*/

	for (int i = 0; i < OS_Y; i++) {
		for (int j = 0; j < OS_X; j++) {
			if (labirynt[i][j] == zajeta) {
				glColor3f(0.5f, 0.5f, 0.5f);
				glRectf((i - floor(szerokosc / 2)) * szerokosc, (j - floor(szerokosc / 2)) * szerokosc, ((i - floor(szerokosc / 2)) * szerokosc) + szerokosc, ((j - floor(szerokosc / 2)) * szerokosc) + szerokosc);
			}
			else if (labirynt[i][j] == wolna) {
				glColor3f(1.0f, 1.0f, 1.0f);
				glRectf((i - floor(szerokosc / 2)) * szerokosc, (j - floor(szerokosc / 2)) * szerokosc, ((i - floor(szerokosc / 2)) * szerokosc) + szerokosc, ((j - floor(szerokosc / 2)) * szerokosc) + szerokosc);
			}
			//cout << labirynt[j][i];
		}
		//cout << "\n";
	}
	glFlush();
}

int ruchBok(int strona, int kor_x) {
	if (strona == WSCHOD) {
		return kor_x + 1;
	}
	else if (strona == ZACHOD) {
		return kor_x - 1;
	}
	else {
		return kor_x;
	}
}

int ruchPion(int strona, int kor_y) {
	if (strona == POLNOC) {
		return kor_y - 1;
	}
	else if (strona == POLUDNIE) {
		return kor_y + 1;
	}
	else {
		return kor_y;
	}
}

bool czyWolna(int kor_x, int kor_y, int strona, char labirynt[OS_Y][OS_X]) {
	kor_x = ruchBok(strona, kor_x);
	kor_y = ruchPion(strona, kor_y);

	if (labirynt[kor_y][kor_x] == wolna || kor_x >= (OS_X - 1) || kor_x <= 0 || kor_y <= 0 || kor_y >= (OS_Y - 1)) {
		return false;
	}

	// wszystkie strony sprawdzamy
	if (strona == POLNOC) {
		if (labirynt[kor_y][kor_x - 1] != wolna &&  labirynt[kor_y - 1][kor_x] != wolna && labirynt[kor_y][kor_x + 1] != wolna &&  labirynt[kor_y - 1][kor_x - 1] != wolna && labirynt[kor_y - 1][kor_x + 1] != wolna) {
			return true;
		}
	}
	if (strona == POLUDNIE) {
		if (labirynt[kor_y][kor_x - 1] != wolna &&  labirynt[kor_y + 1][kor_x] != wolna && labirynt[kor_y][kor_x + 1] != wolna && labirynt[kor_y + 1][kor_x - 1] != wolna && labirynt[kor_y + 1][kor_x + 1] != wolna) {
			return true;
		}
	}
	if (strona == WSCHOD) {
		if (labirynt[kor_y][kor_x + 1] != wolna &&  labirynt[kor_y - 1][kor_x] != wolna && labirynt[kor_y + 1][kor_x] != wolna && labirynt[kor_y - 1][kor_x + 1] != wolna && labirynt[kor_y + 1][kor_x + 1] != wolna) {
			return true;
		}
	}
	if (strona == ZACHOD) {
		if (labirynt[kor_y][kor_x - 1] != wolna &&  labirynt[kor_y - 1][kor_x] != wolna && labirynt[kor_y + 1][kor_x] != wolna && labirynt[kor_y - 1][kor_x - 1] != wolna && labirynt[kor_y + 1][kor_x - 1] != wolna) {
			return true;
		}
	}
	return false;
}

int stworz_Labirynt() {
	return 0;
}

void RenderScene(void)
{
	char labirynt[OS_Y][OS_X];

	srand(time(0));

	for (int i = 0; i < OS_Y; i++) {
		for (int j = 0; j < OS_X; j++) {
			labirynt[i][j] = zajeta;
		}
	}

	stack<int> WartosciX;
	stack<int> WartosciY;

	int strona = 0;

	do {
		for (int i = 0; i < 4; i++) {
			if (czyWolna(X, Y, i, labirynt)) {
				iloscDobrych++;
			}
		}

		if (iloscDobrych == 0) {
			X = WartosciX.top();
			Y = WartosciY.top();
			WartosciX.pop();
			WartosciY.pop();
		}
		else if (iloscDobrych == 1) {
			if (czyWolna(X, Y, POLNOC, labirynt)) {
				Y = ruchPion(POLNOC, Y);
			}
			else if (czyWolna(X, Y, POLUDNIE, labirynt)) {
				Y = ruchPion(POLUDNIE, Y);
			}
			else if (czyWolna(X, Y, WSCHOD, labirynt)) {
				X = ruchBok(WSCHOD, X);
			}
			else if (czyWolna(X, Y, ZACHOD, labirynt)) {
				X = ruchBok(ZACHOD, X);
			}
		}
		else if (iloscDobrych > 1) {
			WartosciX.push(X);
			WartosciY.push(Y);

			do {
				strona = rand() % 4;
			} while (!czyWolna(X, Y, strona, labirynt));

			X = ruchBok(strona, X);
			Y = ruchPion(strona, Y);
		}

		labirynt[Y][X] = wolna;
		wydrukuj(labirynt);
		iloscDobrych = 0;

	} while (!WartosciX.empty());

	wydrukuj(labirynt);
	glColor3f(0.0f, 1.0f, 0.0f);
	glRectf((X - (floor(szerokosc / 2)) )* szerokosc, (Y - (floor(szerokosc / 2))) * szerokosc, ((X - (floor(szerokosc / 2)))* szerokosc) + szerokosc, ((Y - (floor(szerokosc / 2))) * szerokosc) + szerokosc);
	glColor3f(0.0f, 0.0f, 1.0f);
	glRectf(((OS_X - 2) * szerokosc) - ((floor(szerokosc / 2)) * szerokosc), ((OS_Y - 2) * szerokosc) - ((floor(szerokosc / 2)) * szerokosc), ((OS_X - 2) * szerokosc) + szerokosc - ((floor(szerokosc / 2)) * szerokosc), ((OS_Y - 2) * szerokosc) + szerokosc - ((floor(szerokosc / 2)) * szerokosc));
	
	glFlush();

	for (int i = 0; i < OS_Y; i++) {
		for (int j = 0; j < OS_X; j++) {
			if (i == 0 || i == OS_Y - 1) {
				labirynt[i][j] = sciana;
			}
			labirynt[i][0] = sciana;
			labirynt[i][OS_X - 1] = sciana;
		}
	}

	labirynt[X][Y] = start;
	labirynt[OS_X - 2][OS_Y - 2] = koniec;

	wydrukuj(labirynt);
}

void MyInit(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	// Kolor okna wnętrza okna - ustawiono na szary
}


void ChangeSize(GLsizei horizontal, GLsizei vertical)
// Parametry horizontal i vertical (szeroko�� i wysoko�� okna) s�
// przekazywane do funkcji za ka�dym razem, gdy zmieni si� rozmiar okna
{
	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio okre�laj�cej proporcj� wymiar�w okna

	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0
		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ciokna okna urz�dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	// Okre�lenie uk�adu wsp��rz�dnych obserwatora
	glLoadIdentity();
	// Okre�lenie przestrzeni ograniczaj�cej
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wsp��czynnika proporcji okna
	// Gdy okno na ekranie nie jest kwadratem wymagane jest 
	// okre�lenie okna obserwatora. 
	// Pozwala to zachowa� w�a�ciwe proporcje rysowanego obiektu
	// Do okre�lenia okna obserwatora s�u�y funkcja glOrtho(...)

	if (horizontal <= vertical)
		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);
	else
		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	// Okre�lenie uk�adu wsp��rz�dnych    
	glLoadIdentity();
}
/************************************************************************************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli

void main(void)
{
srand( time( NULL ) );
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
// Ustawienie trybu wy�wietlania
// GLUT_SINGLE - pojedynczy bufor wy�wietlania
// GLUT_RGBA - model kolor�w RGB

glutInitWindowSize(1000, 1000);

glutCreateWindow("Labirynt");
// Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna

glutDisplayFunc(RenderScene);
// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn� (callback)
// Biblioteka GLUT b�dzie wywo�ywa�a t� funkcj� za ka�dym razem, gdy
// trzeba b�dzie przerysowa� okno


glutReshapeFunc(ChangeSize);
// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn� za
// zmiany rozmiaru okna

MyInit();
// Funkcja MyInit (zdefiniowana powy�ej) wykonuje wszelkie
// inicjalizacje konieczneprzed przyst�pieniem do renderowania

glutMainLoop();
// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/