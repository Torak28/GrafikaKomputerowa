#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <iostream>
#include <fstream>

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

GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{
#pragma pack(1)           
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;
#pragma pack(8)


	FILE *pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte    *pbitsperpixel = NULL;
	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;
	fopen_s(&pFile, FileName, "rb");
	if (pFile == NULL)
		return NULL;
	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);
	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;
	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;
	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
	if (pbitsperpixel == NULL)
		return NULL;
	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}
	switch (sDepth)
	{
	case 3:
		*ImFormat = GL_BGR_EXT;
		*ImComponents = GL_RGB8;
		break;
	case 4:
		*ImFormat = GL_BGRA_EXT;
		*ImComponents = GL_RGBA8;
		break;
	case 1:
		*ImFormat = GL_LUMINANCE;
		*ImComponents = GL_LUMINANCE8;
		break;
	};
	fclose(pFile);
	return pbitsperpixel;
}

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
	float i;
	float j;
};


/********************************OBSŁUGA********************************/
// 1- punkty, 2- siatka, 3 - wypełnione trójkąty, 4 - czjniczek
int model = 3;
int ruch = 1;
float KAT = 90.0;

// Ilosc podzialow boku kwadratu jednostkowego
const int N = 250;
// Obrot
static GLfloat obrot[] = { 0.0, 0.0, 0.0 };
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
			punktyJaja[i][j].i = i * krok;
			punktyJaja[i][j].j = j * krok;
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
					glNormal3f(-punktyJaja[i][j].xV, -punktyJaja[i][j].yV, -punktyJaja[i][j].zV);
					glTexCoord2f(-punktyJaja[i][j].i, -punktyJaja[i][j].j);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glNormal3f(-punktyJaja[i][j + 1].xV, -punktyJaja[i][j + 1].yV, -punktyJaja[i][j + 1].zV);
					glTexCoord2f(-punktyJaja[i][j + 1].i, -punktyJaja[i][j + 1].j);
					glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
					glNormal3f(-punktyJaja[0][0].xV, -punktyJaja[0][0].yV, -punktyJaja[0][0].zV);
					glTexCoord2f(-punktyJaja[0][0].i, -punktyJaja[0][0].j);
					glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
				}
				else {
					glNormal3f(-punktyJaja[i][j].xV, -punktyJaja[i][j].yV, -punktyJaja[i][j].zV);
					glTexCoord2f(-punktyJaja[i][j].i, -punktyJaja[i][j].j);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glNormal3f(-punktyJaja[i + 1][j].xV, -punktyJaja[i + 1][j].yV, -punktyJaja[i + 1][j].zV);
					glTexCoord2f(-punktyJaja[i + 1][j].i, -punktyJaja[ i+ 1][j].j);
					glVertex3f(punktyJaja[i + 1][j].x, punktyJaja[i + 1][j].y - 5, punktyJaja[i + 1][j].z);
					glNormal3f(-punktyJaja[i + 1][j + 1].xV, -punktyJaja[i + 1][j + 1].yV, -punktyJaja[i + 1][j + 1].zV);
					glTexCoord2f(-punktyJaja[i + 1][j].i, -punktyJaja[i + 1][j].j);
					glVertex3f(punktyJaja[i + 1][j + 1].x, punktyJaja[i + 1][j + 1].y - 5, punktyJaja[i + 1][j + 1].z);

					glNormal3f(-punktyJaja[i][j].xV, -punktyJaja[i][j].yV, -punktyJaja[i][j].zV);
					glTexCoord2f(-punktyJaja[i][j].i, -punktyJaja[i][j].j);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glNormal3f(-punktyJaja[i][j + 1].xV, -punktyJaja[i][j + 1].yV, -punktyJaja[i][j + 1].zV);
					glTexCoord2f(-punktyJaja[i][j + 1].i, -punktyJaja[i][j + 1].j);
					glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
					glNormal3f(-punktyJaja[i + 1][j + 1].xV, -punktyJaja[i + 1][j + 1].yV, -punktyJaja[i + 1][j + 1].zV);
					glTexCoord2f(-punktyJaja[i + 1][j + 1].i, -punktyJaja[i + 1][j + 1].j);
					glVertex3f(punktyJaja[i + 1][j + 1].x, punktyJaja[i + 1][j + 1].y - 5, punktyJaja[i + 1][j + 1].z);
				}
				if (pom != N) {
					glNormal3f(-punktyJaja[i][0].xV, -punktyJaja[i][0].yV, -punktyJaja[i][0].zV);
					glTexCoord2f(-punktyJaja[i][0].i, -punktyJaja[i][0].j);
					glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
					glNormal3f(punktyJaja[pom][N - 1].xV, punktyJaja[pom][N - 1].yV, punktyJaja[pom][N - 1].zV);
					glTexCoord2f(punktyJaja[pom][N - 1].i, punktyJaja[pom][N - 1].j);
					glVertex3f(punktyJaja[pom][N - 1].x, punktyJaja[pom][N - 1].y - 5, punktyJaja[pom][N - 1].z);
					glNormal3f(punktyJaja[pom + 1][N - 1].xV, punktyJaja[pom + 1][N - 1].yV, punktyJaja[pom + 1][N - 1].zV);
					glTexCoord2f(punktyJaja[pom + 1][N - 1].i, punktyJaja[pom + 1][N - 1].j);
					glVertex3f(punktyJaja[pom + 1][N - 1].x, punktyJaja[pom + 1][N - 1].y - 5, punktyJaja[pom + 1][N - 1].z);

					if (i != N / 2) {
						glNormal3f(-punktyJaja[i][0].xV, -punktyJaja[i][0].yV, -punktyJaja[i][0].zV);
						glTexCoord2f(-punktyJaja[i][0].i, -punktyJaja[i][0].j);
						glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
						glNormal3f(-punktyJaja[i - 1][0].xV, -punktyJaja[i - 1][0].yV, -punktyJaja[i - 1][0].zV);
						glTexCoord2f(-punktyJaja[i - 1][0].i, -punktyJaja[i - 1][0].j);
						glVertex3f(punktyJaja[i - 1][0].x, punktyJaja[i - 1][0].y - 5, punktyJaja[i - 1][0].z);
						glNormal3f(punktyJaja[pom + 1][N - 1].xV, punktyJaja[pom + 1][N - 1].yV, punktyJaja[pom + 1][N - 1].zV);
						glTexCoord2f(punktyJaja[pom + 1][N - 1].i, punktyJaja[pom + 1][N - 1].j);
						glVertex3f(punktyJaja[pom + 1][N - 1].x, punktyJaja[pom + 1][N - 1].y - 5, punktyJaja[pom + 1][N - 1].z);
					}
				}
			}
		}
		glNormal3f(-punktyJaja[0][0].xV, -punktyJaja[0][0].yV, -punktyJaja[0][0].zV);
		glTexCoord2f(-punktyJaja[0][0].i, -punktyJaja[0][0].j);
		glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
		glNormal3f(-punktyJaja[N - 1][0].xV, -punktyJaja[N - 1][0].yV, -punktyJaja[N - 1][0].zV);
		glTexCoord2f(-punktyJaja[N - 1][0].i,- punktyJaja[N - 1][0].j);
		glVertex3f(punktyJaja[N - 1][0].x, punktyJaja[N - 1][0].y - 5, punktyJaja[N - 1][0].z);
		glNormal3f(punktyJaja[1][N - 1].xV, punktyJaja[1][N - 1].yV, punktyJaja[1][N - 1].zV);
		glTexCoord2f(punktyJaja[1][N - 1].i, punktyJaja[1][N - 1].j);
		glVertex3f(punktyJaja[1][N - 1].x, punktyJaja[1][N - 1].y - 5, punktyJaja[1][N - 1].z);

		glNormal3f(punktyJaja[0][0].xV, punktyJaja[0][0].yV, punktyJaja[0][0].zV);
		glTexCoord2f(punktyJaja[0][0].i, punktyJaja[0][0].j);
		glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
		glNormal3f(punktyJaja[1][0].xV, punktyJaja[1][0].yV, punktyJaja[1][0].zV);
		glTexCoord2f(punktyJaja[1][0].i, punktyJaja[1][0].j);
		glVertex3f(punktyJaja[1][0].x, punktyJaja[1][0].y - 5, punktyJaja[1][0].z);
		glNormal3f(-punktyJaja[N - 1][N - 1].xV, -punktyJaja[N - 1][N - 1].yV, -punktyJaja[N - 1][N - 1].zV);
		glTexCoord2f(-punktyJaja[N - 1][N - 1].i, -punktyJaja[N - 1][N - 1].j);
		glVertex3f(punktyJaja[N - 1][N - 1].x, punktyJaja[N - 1][N - 1].y - 5, punktyJaja[N - 1][N - 1].z);

		for (int i = 0; i < N/ 2 + 1; i++) {
			int pom = N - i;
			for (int j = 0; j < N - 1; j++) {
				if (i == 1) {
					glNormal3f(punktyJaja[i][j].xV, punktyJaja[i][j].yV, punktyJaja[i][j].zV);
					glTexCoord2f(punktyJaja[i][j].i, punktyJaja[i][j].j);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glNormal3f(punktyJaja[i][j + 1].xV, punktyJaja[i][j + 1].yV, punktyJaja[i][j + 1].zV);
					glTexCoord2f(punktyJaja[i][j + 1].i, punktyJaja[i][j + 1].j);
					glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
					glNormal3f(punktyJaja[0][0].xV, punktyJaja[0][0].yV, punktyJaja[0][0].zV);
					glTexCoord2f(punktyJaja[0][0].i, punktyJaja[0][0].j);
					glVertex3f(punktyJaja[0][0].x, punktyJaja[0][0].y - 5, punktyJaja[0][0].z);
				}
				else {
					if (i != 0) {
						glNormal3f(punktyJaja[i][j].xV, punktyJaja[i][j].yV, punktyJaja[i][j].zV);
						glTexCoord2f(punktyJaja[i][j].i, punktyJaja[i][j].j);
						glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
						glNormal3f(punktyJaja[i - 1][j].xV, punktyJaja[i - 1][j].yV, punktyJaja[i - 1][j].zV);
						glTexCoord2f(punktyJaja[i - 1][j].i, punktyJaja[i - 1][j].j);
						glVertex3f(punktyJaja[i - 1][j].x, punktyJaja[i - 1][j].y - 5, punktyJaja[i - 1][j].z);
						glNormal3f(punktyJaja[i - 1][j + 1].xV, punktyJaja[i - 1][j + 1].yV, punktyJaja[i - 1][j + 1].zV);
						glTexCoord2f(punktyJaja[i - 1][j + 1].i, punktyJaja[i - 1][j + 1].j);
						glVertex3f(punktyJaja[i - 1][j + 1].x, punktyJaja[i - 1][j + 1].y - 5, punktyJaja[i - 1][j + 1].z);
					}
					glNormal3f(punktyJaja[i][j].xV, punktyJaja[i][j].yV, punktyJaja[i][j].zV);
					glTexCoord2f(punktyJaja[i][j].i, punktyJaja[i][j].j);
					glVertex3f(punktyJaja[i][j].x, punktyJaja[i][j].y - 5, punktyJaja[i][j].z);
					glNormal3f(punktyJaja[i][j + 1].xV, punktyJaja[i][j + 1].yV, punktyJaja[i][j + 1].zV);
					glTexCoord2f(punktyJaja[i][j + 1].i, punktyJaja[i][j + 1].j);
					glVertex3f(punktyJaja[i][j + 1].x, punktyJaja[i][j + 1].y - 5, punktyJaja[i][j + 1].z);
					glNormal3f(punktyJaja[i - 1][j + 1].xV, punktyJaja[i - 1][j + 1].yV, punktyJaja[i - 1][j + 1].zV);
					glTexCoord2f(punktyJaja[i - 1][j + 1].i, punktyJaja[i - 1][j + 1].j);
					glVertex3f(punktyJaja[i - 1][j + 1].x, punktyJaja[i - 1][j + 1].y - 5, punktyJaja[i - 1][j + 1].z);
				}
				if (pom != N) {
					glNormal3f(punktyJaja[i][0].xV, punktyJaja[i][0].yV, punktyJaja[i][0].zV);
					glTexCoord2f(punktyJaja[i][0].i, punktyJaja[i][0].j);
					glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
					glNormal3f(-punktyJaja[pom][N - 1].xV, -punktyJaja[pom][N - 1].yV, -punktyJaja[pom][N - 1].zV);
					glTexCoord2f(-punktyJaja[pom][N - 1].i, -punktyJaja[pom][N - 1].j);
					glVertex3f(punktyJaja[pom][N - 1].x, punktyJaja[pom][N - 1].y - 5, punktyJaja[pom][N - 1].z);
					glNormal3f(-punktyJaja[pom - 1][N - 1].xV, -punktyJaja[pom - 1][N - 1].yV, -punktyJaja[pom - 1][N - 1].zV);
					glTexCoord2f(-punktyJaja[pom - 1][N - 1].i, -punktyJaja[pom - 1][N - 1].j);
					glVertex3f(punktyJaja[pom - 1][N - 1].x, punktyJaja[pom - 1][N - 1].y - 5, punktyJaja[pom - 1][N - 1].z);
					glNormal3f(punktyJaja[i][0].xV, punktyJaja[i][0].yV, punktyJaja[i][0].zV);
					glTexCoord2f(punktyJaja[i][0].i, punktyJaja[i][0].j);
					glVertex3f(punktyJaja[i][0].x, punktyJaja[i][0].y - 5, punktyJaja[i][0].z);
					glNormal3f(punktyJaja[i + 1][0].xV, punktyJaja[i + 1][0].yV, punktyJaja[i + 1][0].zV);
					glTexCoord2f(punktyJaja[i + 1][0].i, punktyJaja[i + 1][0].j);
					glVertex3f(punktyJaja[i + 1][0].x, punktyJaja[i + 1][0].y - 5, punktyJaja[i + 1][0].z);
					glNormal3f(-punktyJaja[pom - 1][N - 1].xV, -punktyJaja[pom - 1][N - 1].yV, -punktyJaja[pom - 1][N - 1].zV);
					glTexCoord2f(-punktyJaja[pom - 1][N - 1].i, -punktyJaja[pom - 1][N - 1].j);
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

void MyInit(void)
{
	GLbyte *pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;
	glColor3f(1.0f, 0.0f, 0.0f);
	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess = { 50.0 };
	GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat att_constant = 1.0;
	GLfloat att_linear = 0.05;
	GLfloat att_quadratic = 0.001;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	nic();
	//glEnable(GL_CULL_FACE);
	pBytes = LoadTGAImage("mojapisanka3.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);
	free(pBytes);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void keys(unsigned char key, int x, int y)
{
	if (key == 'q') model = 1;
	if (key == 'w') model = 2;
	if (key == 'e') model = 3;
	if (key == 'r') model = 4;

	RenderScene();
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
	int foo = 1;
	char * bar[1] = { " " };
	glutInit(&foo, bar);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("OpenGL - teksturowanie powierzchni obiektów");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutKeyboardFunc(keys);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}