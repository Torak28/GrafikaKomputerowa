#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>

void rysuj_kwadrat(float x, float y, float a) {
	float def = (rand() % 100)*0.01;
	glBegin(GL_POLYGON);
		glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01)); //
		//wierzchołek czerwony
		glVertex2f(x + def, y + def);
		glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01)); //
		//wierzchołek niebieski
		glVertex2f(x + def, y + a + def);
		glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01)); //
		//wierzchołek niebieski
		glVertex2f(x + a + def, y + a + def);
		glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01)); //
		//wierzchołek czerwony
		glVertex2f(x + a + def, y + def);
	glEnd();
}
void rysuj_dywan(float x, float y, float a, int poziom) {
	float szerokosc = a / 3;
	if (poziom > 0) { 
		rysuj_dywan(x, y, szerokosc, poziom - 1); 
		rysuj_dywan(x + szerokosc, y, szerokosc, poziom - 1); 
		rysuj_dywan(x + 2 * szerokosc, y, szerokosc, poziom - 1); 
		rysuj_dywan(x, y - szerokosc, szerokosc, poziom - 1); 
		rysuj_dywan(x + 2 * szerokosc, y - szerokosc, szerokosc, poziom - 1); 
		rysuj_dywan(x, y - 2 * szerokosc, szerokosc, poziom - 1); 
		rysuj_dywan(x + szerokosc, y - 2 * szerokosc, szerokosc, poziom - 1); 
		rysuj_dywan(x + 2 * szerokosc, y - 2 * szerokosc, szerokosc, poziom - 1); 
	}
	else { 
		rysuj_kwadrat(x, y, szerokosc); 
		rysuj_kwadrat(x + szerokosc, y, szerokosc); 
		rysuj_kwadrat(x + 2 * szerokosc, y, szerokosc); 
		rysuj_kwadrat(x, y - szerokosc, szerokosc); 
		rysuj_kwadrat(x + 2 * szerokosc, y - szerokosc, szerokosc);
		rysuj_kwadrat(x, y - 2 * szerokosc, szerokosc); 
		rysuj_kwadrat(x + szerokosc, y - 2 * szerokosc, szerokosc); 
		rysuj_kwadrat(x + 2 * szerokosc, y - 2 * szerokosc, szerokosc); 
	}
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	rysuj_dywan(-60, 60, 120, 3);
	glFlush();
}

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio;

	if (vertical == 0)
		vertical = 1;

	glViewport(0, 0, horizontal, vertical);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	if (horizontal <= vertical)
		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);
	else
		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(void)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("OpenGL - podstawy");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glutMainLoop();
}