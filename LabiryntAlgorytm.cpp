/*
0. DFS - Deep First Search
1. Generuje Labirynt zapelniony scianami po czym wybieram
lokazje startu i oznaczam ja jako odwiedzona
2. Dopoki sa nieodwiedzone sciany/komorki
	1. Jesli aktualnie rozpatrywana sciana/komorka ma sasiada ktory nie jest
odwiedzony:
		1. Randomowo wybieram jakiegos nieodwiedzonego sasiada
		2. Trafia on na stos
		3. Usuwam sciae miedzy Nasza komorka/sciana a sasiadem
		4. Robie z sasiada aktualnie rozpatrywana sciane i oznaczam jako
		   odiwedzonego
	2. Jesli nie to:
		1. Zwracam komorke ze stosu
		2. Robie z niej aktualnie rozpatrywana
*/

#include <iostream>
#include <cstdlib>
#include <stack>
#include <ctime>

#define OS_X 20
#define OS_Y 20

#define POLNOC 0
#define POLUDNIE 1
#define WSCHOD 2
#define ZACHOD 3

using namespace std;

int X = 1;
int Y = 1;

char wolna = ' ';
char zajeta = '#';
char sciana = '|';
char start = 'S';
char koniec = 'K';

int iloscDobrych = 0;

void wydrukuj(char labirynt[OS_Y][OS_X]) {
	system("cls");
	for (int i = 0; i < OS_Y; i++) {
		for (int j = 0; j < OS_X; j++) {
			cout << labirynt[i][j];
		}
		cout << "\n";
	}
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

void stworz_Labirynt() {
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

int main() {
	stworz_Labirynt();
	system("pause");
	return 0;
}