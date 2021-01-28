/***************************************************
ISGI:: Reloj 3D
***************************************************/
#define TITULO "Reloj analogico"
#define _USE_MATH_DEFINES
#include <iostream>
#include <freeglut.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <math.h>
#include <ctime>
#include <cmath>
#include <chrono>

int posicionSegundos;
int posicionMinutos;
int posicionHoras;
int posicionHorass;
static GLfloat anguloSec;
static GLfloat anguloMin;
static GLfloat anguloHora;

static const float velocidad = 24.0 * M_PI / 180;
bool crecer = true;

static GLint estrellaSimple;
static GLint varilla;

static GLfloat valorX = 0.2;

int contadorSeg = 0;
int contadorMin = 0;
int contadorHora = 0;

int tiempoGlobal = 0;

#pragma warning(disable : 4996
void init()
{
	// Calculo del tiempo inicial y los angulos correspondientes
	auto start = std::chrono::system_clock::now();
	time_t current_time;
	struct tm* time_info;

	time(&current_time);
	time_info = localtime(&current_time);

	posicionSegundos = time_info->tm_sec;
	//std::cout << time_info->tm_sec;
	anguloSec = posicionSegundos * 6;
	//contadorSeg = 360 / anguloSec;
	contadorSeg = posicionSegundos;

	posicionMinutos = time_info->tm_min;
	anguloMin = posicionMinutos * 6;
	contadorMin = posicionMinutos;
	//std::cout << contadorMin << "\n";

	posicionHoras = time_info->tm_hour;
	//std::cout << posicionHoras << "\n";
	posicionHoras %= 12;
	//std::cout << posicionHoras << "\n";
	anguloHora = posicionHoras * 30;
	contadorHora = posicionHoras;

	// Dibujo de una estrella de David
	estrellaSimple = glGenLists(1);
	glNewList(estrellaSimple, GL_COMPILE);

	glBegin(GL_TRIANGLE_STRIP);

	glVertex3f(1.0 * cos(0 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(1.0 * cos(2 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(2 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(2 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(2 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(1.0 * cos(4 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(4 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(4 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(4 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(1.0 * cos(0 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(0 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);

	glEnd();

	//triangulo 2
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(1.0 * cos(1 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(1.0 * cos(3 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(3 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(3 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(3 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(1.0 * cos(5 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(5 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(5 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(5 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(1.0 * cos(1 * (2 * M_PI / 6) + (M_PI / 2)), 1.0 * sin(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glVertex3f(0.7 * cos(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.7 * sin(1 * (2 * M_PI / 6) + (M_PI / 2)), 0.0);
	glEnd();
	glEndList();

	//Varilla de reloj
	varilla = glGenLists(3);
	glNewList(varilla, GL_COMPILE);
	glBegin(GL_LINES);
	glVertex3f(0, 0.6, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	glEndList();

}


void display()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Borra la pantalla
	glMatrixMode(GL_MODELVIEW); // Selecciona la modelview
	glLoadIdentity(); // Carga la matriz identidad
	gluLookAt(0, 0, 8.2, 0, 0, 0, 0, 1, 0); // posicion a donde mira la camara
	glEnable(GL_DEPTH_TEST);
	
	//Estrellas de david separadas 30 grados
	for (int i = 1; i <= 6; i++) {
		glPushAttrib(GL_CURRENT_BIT);
		glColor3f(0.3 - (i / 10.0), 1 - (i / 10.0), 0.4 + (i / 10.0));
		glPushMatrix();
		glScalef(valorX, valorX, valorX);
		glRotatef(30*i, 0, 1, 0);
		glCallList(estrellaSimple);
		glPopMatrix();
		glPopAttrib();
	}
	
	//varilla que marca los segundos
	
	glColor3f(1, 0, 0); //varillas de color rojo
	glPushMatrix();
	glLineWidth(3); // grosor de las lineas al 2
	glScalef(0.9, 0.9, 0.9); 
	glRotatef(-anguloSec, 0, 0, 1);
	glCallList(varilla);
	glPopMatrix();

	//varilla que marca los minutos
	glPushMatrix();
	glColor3f(0, 0, 1);
	glScalef(0.7, 0.7, 0.7); 
	glRotatef(-anguloMin, 0, 0, 1);
	glCallList(varilla);
	glPopMatrix();

	//varilla que marca las horas
	glPushMatrix();
	glColor3f(1, 0, 1); //varillas de color rojo
	glScalef(0.5, 0.5, 0.5);
	glRotatef(-anguloHora, 0, 0, 1);
	glCallList(varilla);
	glPopMatrix();

	//marcadores de la hora de las 12
	glColor3f(0, 1, 0);
	glPushMatrix();
	glScalef(0.3, 0.3, 0.3);
	glTranslatef(0, 2.5, 0);
	glCallList(varilla);
	glPopMatrix();

	//marcadores de la hora de las 3
	glColor3f(0, 1, 0);
	glPushMatrix();
	glScalef(0.3, 0.3, 0.3);
	glTranslatef(3, -0.3, 0);
	glCallList(varilla);
	glPopMatrix();
	
	//marcadores de la hora de las 6
	glColor3f(0, 1, 0);
	glPushMatrix();
	glScalef(0.3, 0.3, 0.3);
	glTranslatef(0, -3, 0);
	glCallList(varilla);
	glPopMatrix();

	//marcadores de la hora de las 9
	glColor3f(0, 1, 0);
	glPushMatrix();
	glScalef(0.3, 0.3, 0.3);
	glTranslatef(-3, -0.3, 0);
	glCallList(varilla);
	glPopMatrix();

	//esfera de fondo
	glColor3f(0, 0, 0.5);
	glPushMatrix();
	glutWireSphere(1.0, 0, 0);
	glPopAttrib();
	glPopMatrix();

	glutSwapBuffers();
}
void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h); // todo el area de dibujo
	float razon = (float)w / h;

	// matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	float distancia, fovy;
	distancia = sqrt(pow(5, 2) + pow(5, 2) + pow(5, 2));
	fovy = (asin(1 / (distancia / 2.0))) * 180 / M_PI;
	gluPerspective(fovy, razon, 1, 10);
}

void onTimer(int valor)
//Funcion de atencion al timer periodico
{
	static int antes = 0;
	int ahora, tiempo_transcurrido;
	ahora = glutGet(GLUT_ELAPSED_TIME); // Tiempo transcurrido desde el inicio
	tiempo_transcurrido = ahora - antes; // Tiempo transcurrido desde antes msg

	tiempoGlobal += tiempo_transcurrido;
	
	//valorX += velocidad * tiempo_transcurrido / 1000.0;
	float tiempo_trans = (float) tiempo_transcurrido;

	if (crecer == false) {
		//std::cout << valorX;
		//std::cout << "he entrado";
		valorX -= velocidad * tiempo_trans / 1000.0;
		if (valorX <= 0.2) { crecer = true; }
		
	} 

	if (crecer) {
		//std::cout << valorX;
		valorX += velocidad * tiempo_trans / 1000.0;
		if (valorX >= 0.6) { crecer = false; }
	}
	
	if (tiempoGlobal > 1000) {
		tiempoGlobal %= 1000;
		anguloSec = anguloSec + 6;
		contadorSeg += 1;
		//std::cout << contadorSeg;
		if (anguloSec >= 360) {
			anguloSec = (int)anguloSec % 360;		
		}
		
	}

	if (contadorSeg == 60) {
		contadorSeg = 0;
		anguloMin = anguloMin + 6;
		contadorMin += 1;
		if (anguloMin >= 360) {
			anguloMin = (int)anguloMin % 360;
		}
	}

	if (contadorMin == 60) {
		contadorMin = 0;
		anguloHora = anguloHora + 30;
		contadorHora += 1;
		if (anguloHora >= 360) {
			anguloHora = (int)anguloHora % 360;
		}
	}

	antes = ahora;

	glutTimerFunc(1000 / 60, onTimer, 60); //Se encola un nuevo timer
	glutPostRedisplay(); // se manda el dibujo
}


void main(int argc, char** argv)
// Programa principal
{
	glutInit(&argc, argv); // Inicializacion de GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Alta de buffers a usar
	glutInitWindowSize(400, 400); // Tamanyo inicial de la ventana
	glutCreateWindow(TITULO); // Creacion de la ventana con su titulo
	init(); // Inicializacion propia.
	std::cout << TITULO << " running" << std::endl; // Mensaje por consola
	glutDisplayFunc(display); // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape); // Alta de la funcion de atencion a reshape
	glutTimerFunc(1000 / 60, onTimer, 60); //Se encola un nuevo timer
	glutMainLoop(); // Puesta en marcha del programa
}