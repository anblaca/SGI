/***************************************************
ISGI::Practica 9 - Interacción
Antonio Blasco Calafat
***************************************************/
#define PROYECTO "ISGI::S1E01::Interfaz de conducción"

#include <iostream>           
#include <freeglut.h>         
#include <sstream>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdlib.h>
#include <../Utilidades.h>

#define vIncr 0.1
#define PI  3.1415926

// CARRETERA practica 6
const GLfloat amplitud = 8;
const GLfloat periodo = 150;
const GLfloat ancho = 4;
const GLint numQuads = 50;
// quan inicial y vertices para crear quad
GLfloat quad0 = 0.0;
//[componente X o Z][vertice][valor]
GLfloat vertices[2][2][numQuads + 1]; 
// Definicion del coche
GLfloat cam[3] = { 0.0 , 1.0, 0.0 };
GLfloat vDir[2];
GLfloat velocidad = 0;
//ILUMINACION
GLfloat lucesFarolas[] = { GL_LIGHT2, GL_LIGHT3, GL_LIGHT4 , GL_LIGHT5 };
GLfloat directorFarolas[] = { 0.0, -1.0, 0.0 };
GLfloat far0 = 0.0;
bool ini = true;
float f = 0.0;

GLfloat difCarretera[] = { 0.5,0.5,0.5,1.0 };
GLfloat espCarretera[] = { 0.3,0.3,0.3,1.0 };

GLfloat difCartel[] = { 0.5,0.5,0.5,1.0 };
GLfloat espCartel[] = { 0.3,0.3,0.3,1.0 };

GLfloat far1[4] = { 0,4.0,0,1 };
GLfloat far2[4] = { 0,4.0,0,1 };;

//TEXTURAS
GLuint text[5];
GLint piso;
char img1[] = "definitive.jpg";
char img2[] = "ruta66.jpg";
char img3[] = "desierto.jpg";
char img4[] = "piso.jpg";
char img5[] = "madera.jpg";
const GLint quadsFondo = 20;
float alturaFondo = 300;
float radFondo = 100;
GLfloat verticesFondo[2][quadsFondo];
float verticesCarteles[2][4]; // para guardar la posicion de los 4 vertices
GLfloat verticesPoste[2][2][2];  // [componente X o Z] [Vertice del Poste] [Poste]
static GLint paisaje;
//CONTROL
// 0 [NORMAL] 1[ALAMBRICO] 2[LUCES] 3[NIEBLA] 4[HUD]
int estado = 0; 
bool largas = false;

//Para fijar el piso
float aux = 15;
float reinicio[3];

//Creacion del hud
static GLint barra;
float nozar = 0;

//Creacion de la captura de pantalla
bool captura = false;
float altura = 0;
float anchura = 0;
char salida[] = "salida.png";

/*Funcion de la practica 6*/
float sen(float u) {
	return (amplitud * sin(u * ((2 * PI) / periodo)));
}
/*Funcion de la practica 6*/
float dsen(float u) {
	return (((2 * PI * amplitud) * cos(u * ((2 * PI) / periodo))) / periodo);
}

/*Practica 8 inicializacion de las texturas*/
void texturas() {
	glGenTextures(5, text);
	glBindTexture(GL_TEXTURE_2D, text[0]);
	loadImageFile(img1);
	glBindTexture(GL_TEXTURE_2D, text[1]);
	loadImageFile(img2);
	glBindTexture(GL_TEXTURE_2D, text[2]);
	loadImageFile(img3);
	glBindTexture(GL_TEXTURE_2D, text[3]);
	loadImageFile(img4);
	glBindTexture(GL_TEXTURE_2D, text[4]);
	loadImageFile(img5);
}
/*Practica 7 ilumninacion, luna faros y farolas definicion y inicializacion*/
void luces() {
	// inicializar luces luna
	GLfloat ambL[] = { 0.5, 0.5, 0.5, 1.0 }; GLfloat difL[] = { 0.1, 0.1, 0.1, 1.0 }; GLfloat espL[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambL); glLightfv(GL_LIGHT0, GL_DIFFUSE, difL); glLightfv(GL_LIGHT0, GL_SPECULAR, espL);


	// inicializar luces faro
	GLfloat ambC[] = { 0.15, 0.15, 0.15, 1.0 }; GLfloat difC[] = { 1.0, 1.0, 1.0, 1.0 }; GLfloat espC[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambC); glLightfv(GL_LIGHT1, GL_DIFFUSE, difC); glLightfv(GL_LIGHT1, GL_SPECULAR, espC);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0); glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 1.0);


	// inicializar luces farolas
	GLfloat ambF[] = { 0.0, 0.0, 0.0, 1.0 }; GLfloat difF[] = { 0.5 ,0.5 ,0.2 ,1.0 }; GLfloat espF[] = { 0.0, 0.0, 0.0, 1.0 };
	for (int i = 0; i < 4; i++) {
		glLightfv(lucesFarolas[i], GL_AMBIENT, ambF); glLightfv(lucesFarolas[i], GL_DIFFUSE, difF); glLightfv(lucesFarolas[i], GL_SPECULAR, espF);
		glLightf(lucesFarolas[i], GL_SPOT_CUTOFF, 45.0); glLightf(lucesFarolas[i], GL_SPOT_EXPONENT, 10.0);
	}

}

/* Crear la carretera practica 6*/
void iniV(float u, int i, int mode) {
	if (mode == 0) {
		float aux = (1 / sqrt(1 + pow(dsen(u), 2)));
		vertices[0][0][i] = u - aux * (-dsen(u)) * (ancho / 2);   //componente X (0) , v0 (0) 
		vertices[1][0][i] = sen(u) - aux * (ancho / 2);
		vertices[0][1][i] = u + aux * (-dsen(u)) * (ancho / 2);
		vertices[1][1][i] = sen(u) + aux * (ancho / 2);            //componente Y(1) , v3(1)
	}
	if (mode == 1) {
		float aux = (1 / sqrt(1 + pow(dsen(u), 2)));
		verticesCarteles[0][i] = u - aux * (-dsen(u)) * (ancho / 2);      //v0 
		verticesCarteles[1][i] = sen(u) - aux * (ancho / 2);
		verticesCarteles[0][i + 1] = u + aux * (-dsen(u)) * (ancho / 2);
		verticesCarteles[1][i + 1] = sen(u) + aux * (ancho / 2);            //v3
	}
}

void fondo() {
	float alfa = 0;
	float sector = (2 * PI) / quadsFondo;
	int j = 10; 
	for (int i = 0; i <= quadsFondo; i++) {
		alfa = j * sector;
		j++;
		verticesFondo[0][i] = radFondo * cos(alfa); //X
		verticesFondo[1][i] = radFondo * sin(alfa); //Z
	}
	//Creo una lista donde se guarda el paisaje que se usa de fondo
	paisaje = glGenLists(1);
	glNewList(paisaje, GL_COMPILE);
	GLfloat coordenadasTextura = 0;
	for (int i = 0; i < quadsFondo; i++) {
		//añado los vertices
		glPushMatrix();
		GLfloat v0[3] = { verticesFondo[0][i],(-alturaFondo / 2), verticesFondo[1][i] };
		GLfloat v1[3] = { verticesFondo[0][i],(alturaFondo / 2) , verticesFondo[1][i] };
		GLfloat v2[3] = { verticesFondo[0][i + 1],(alturaFondo / 2) , verticesFondo[1][i + 1] };
		GLfloat v3[3] = { verticesFondo[0][i + 1],(-alturaFondo / 2) , verticesFondo[1][i + 1] };
		glPolygonMode(GL_FRONT, GL_LINE);
		//En caso de que se quiera el modo alambrico simplemente se pintan los quads del fondo
		if (estado == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
		quadtex(v3, v0, v1, v2, coordenadasTextura + (1.0 / quadsFondo), coordenadasTextura, 0, 1, 100, 100);
		coordenadasTextura = coordenadasTextura + (1.0 / quadsFondo);
		glPopMatrix();
	}
	glEndList();
}
/*Metodo que genera un suelo por debajo del circuito*/
void suelo() {
	piso = glGenLists(5);
	glNewList(piso, GL_COMPILE); 
	float x = -100; // Posicion de inicio de la x
	float ix = (2 * (-x)) / 5;
	for (int i = 0; i < 6; i++) {          
		float z = -3; // Posicion de inicio de la z
		float iz = (150+(-1*z)) / 5;
		GLfloat b0[3] = { 0,-0.067,0 };
		GLfloat b1[3] = { 0,-0.067,0 };
		GLfloat b2[3] = { 0,-0.067,0 };
		GLfloat b3[3] = { 0,-0.067,0 };
		for (int j = 0; j < 6; j++) {      
			b0[0] = x; b0[2] = z;
			b1[0] = x + ix; b1[2] = z;
			b2[0] = x + ix; b2[2] = z + iz;
			b3[0] = x; b3[2] = z + iz;
			
			quadtex(b0, b1, b2, b3, 0, 1, 0, 1, 50, 50);
			z += iz;
		}
		x += ix;
	}
	glEndList();
}

/*Metodo que calcula la posicion de los carteles, los palos que la aguantan, y ademas los ilumina en su creacion*/
void cartelesIluminados() {
	if (cam[0] >= far0) {
		float incr = 40; //cada 50 metros se dibujaran los carteles
		float x[2] = { far0 + incr, far0 + 2 * incr }; // array con las x de cada farola
		far0 = far0 + incr;  //actualizamos la posicion inicial de la siguiente farola
		//calculamos la posicion de las farolas
		far1[0] = x[0]; far1[2] = sen(x[0]);
		far2[0] = x[1]; far2[2] = sen(x[1]);
		//calculamos la posicion de los carteles
		int j = 0;
		for (int i = 0; i < 4; i = i + 2) {
			iniV(x[j], i, 1);
			float xP = x[j]; float zP = sen(xP);
			float x0 = verticesCarteles[0][i]; float z0 = verticesCarteles[1][i];
			float x3 = verticesCarteles[0][i + 1]; float z3 = verticesCarteles[1][i + 1];
			verticesPoste[0][0][j] = x0 + (xP - x0) / 3;
			verticesPoste[1][0][j] = z0 - (z0 - zP) / 3;

			verticesPoste[0][1][j] = x3 - (x3 - xP) / 3;
			verticesPoste[1][1][j] = z3 + (zP - z3) / 3;
			j++;
		}
	} // fin del IF
	glLightfv(GL_LIGHT2, GL_POSITION, far1);  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, directorFarolas);
	glLightfv(GL_LIGHT3, GL_POSITION, far2);  glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, directorFarolas);
	int k = 0;
	for (int i = 0; i < 4; i = i + 2) {
		glPushMatrix();
		GLfloat v0[3] = { verticesCarteles[0][i],3,verticesCarteles[1][i] };
		GLfloat v1[3] = { verticesCarteles[0][i + 1],3,verticesCarteles[1][i + 1] };
		GLfloat v2[3] = { verticesCarteles[0][i],2,verticesCarteles[1][i] };
		GLfloat v3[3] = { verticesCarteles[0][i + 1],2,verticesCarteles[1][i + 1] };

		GLfloat v4[3] = { verticesPoste[0][0][k] ,2,verticesPoste[1][0][k] };
		GLfloat v5[3] = { verticesPoste[0][1][k] ,2,verticesPoste[1][1][k] };
		GLfloat v6[3] = { verticesCarteles[0][i],0,verticesCarteles[1][i] };
		GLfloat v7[3] = { verticesPoste[0][0][k] ,0,verticesPoste[1][0][k] };
		GLfloat v8[3] = { verticesPoste[0][1][k] ,0,verticesPoste[1][1][k] };
		GLfloat v9[3] = { verticesCarteles[0][i + 1],0,verticesCarteles[1][i + 1] };
		k++;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (estado == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
		glBindTexture(GL_TEXTURE_2D, text[4]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		quadtex(v2, v4, v7, v6, 0, 1, 0, 1, 10, 10);
		quadtex(v5, v3, v9, v8, 0, 1, 0, 1, 10, 10);
		//CARTEL
		glBindTexture(GL_TEXTURE_2D, text[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		quadtex(v2, v3, v1, v0, 0, 1, 0, 1, 40, 40);
		glPopMatrix();
	}
}

/*Metodo que crea una barra del hud que se llama en el display*/
void elHud() {
	if (estado==4) {               
		glPushAttrib(GL_CURRENT_BIT);
		glEnable(GL_BLEND);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		//creo una camara antes de la camara original para que los vertices sean sobre los puntos originales
		glOrtho(-1, 1, -1, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_TEXTURE_2D);  // Deshabilitar texturas
		glDisable(GL_LIGHTING); // Deshablilitar luces
		glDisable(GL_FOG); // Deshabilitar niebla
		// Habilitar lo que se necesite


		if (velocidad < 9) {
			glColor3f(0.0, 1.0, 0.0);
		} else {
			glColor3f(1.0, 0.0, 0.0);
		}
		//Lo pongo en la esquina inferior izquierda
		glTranslatef(-1, -1.0, 0);
		//Para que no crezca de mas
		if (0.19 * velocidad < 0.19*10.5) {
			glScalef(0.1 * nozar, 0.19 * velocidad, 0);
		}
		else {
			glScalef(0.1 * nozar, 0.19 * 10.5, 0);
		}

		glCallList(barra);


		glEnable(GL_TEXTURE_2D);  // Deshabilitar texturas
		glEnable(GL_LIGHTING); // Deshablilitar luces
		glEnable(GL_FOG); // Deshabilitar niebla
		// Habilitar lo que se necesite
		glPopAttrib();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glDisable(GL_BLEND);
		glPopAttrib();
	}
}

void init()
{
	float aux = (1 / sqrt(1 + pow(dsen(quad0), 2)));
	//se inicializa el vector director
	vDir[0] = aux;   vDir[1] = aux * (dsen(quad0));

	glEnable(GL_DEPTH_TEST);
	//se habilitan las luces
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); glEnable(GL_LIGHT1); glEnable(GL_LIGHT2); glEnable(GL_LIGHT3); glEnable(GL_LIGHT4); glEnable(GL_LIGHT5);
	glShadeModel(GL_SMOOTH);
	
	luces();//Se inicializan las luces
	texturas(); //Se inicializan las texturas
	fondo(); //Se inicializan los quads del fondo
	suelo(); //Se inicializa el suelo
	// Se inicializa una call list pequeña para el hud asi luego se llama en el display
	barra = glGenLists(1);
	glNewList(barra, GL_COMPILE);
	//creo los puntos sobre el eje de coordenadas y en el display lo traslado
	GLfloat vDerechaDebajo[3] = { 1.0 , 0.0, 0.0 };  
	GLfloat vIzquierdaAbajo[3] = { 0 ,0.0,0.0 };
	GLfloat vDerechaArriba[3] = { 1.0,1.0,0.0 };
	GLfloat vIzquierdaArriba[3] = { 0,1.0,0.0 };
	quad(vDerechaDebajo, vDerechaArriba, vIzquierdaArriba, vIzquierdaAbajo, 1, 1);
	glEndList();
}

void display()         
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_FOG);

	switch (estado) {
	case 1: // ALAMBRICO
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glColor3f(0.0, 0.0, 0.0);
		break;
	case 2: // LUCES
		glEnable(GL_LIGHTING);
		break;
	case 3: // NIEBLA
		glEnable(GL_FOG);
		glFogfv(GL_FOG_COLOR, BRONCE);
		glFogf(GL_FOG_DENSITY, 0.06);
	case 4: // HUD
		elHud();
		break;
	}
	//cargamos la modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Luces del coche inicializar antes de la camara
	GLfloat lucesCoche[4] = { 0, -0.3, 0, 1.0 };
	GLfloat dirCoche[3] = { 0.0 , -1.0 , -1.5 };
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0);
	//Codigo para añadir las largas a las luces
	if (estado == 2 && largas == true) { 
		dirCoche[2] = -2;
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0);
	}

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dirCoche);
	glLightfv(GL_LIGHT1, GL_POSITION, lucesCoche);


	if ((cam[0] == 0 && estado == 6) || estado == 5) { cam[1] = 5; }
	else { cam[1] = 1; }

	
	//Definicion de la camara
	GLfloat dir[] = { cam[0] + vDir[0] , 1.0 , cam[2] + vDir[1] };
	gluLookAt(cam[0], cam[1], cam[2], dir[0], dir[1], dir[2], 0, 1, 0);

	//EJES
	if (estado == 6) {
		glPushMatrix();
		glScalef(2.0, 2.0, 2.0);
		ejes();
		glPopMatrix();
	}

	//Se situa la luna 
	GLfloat luna[4] = { 0.0 , 9.0 , 0.0 , 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, luna);

	// CARRETERA ---------------------------------------------------------------------------
	if (cam[0] > quad0 || ini) {                // 1 - si corresponde calculamos los vertices
		ini = false;
		float incr = periodo / numQuads;
		float anterior = quad0;
		for (int i = 0; i <= numQuads; i++) {
			iniV((anterior), i, 0);
			anterior += incr;
		}
		quad0 += periodo / 10;   // cada x (periodo) metros dibujamos la carretera 10 veces 
	}
	for (int i = 0; i < numQuads; i++) {                    // 2 - se dibujan los vertices
		glPushMatrix();
		GLfloat v0[] = { vertices[0][0][i], 0, vertices[1][0][i] };
		GLfloat v1[] = { vertices[0][0][i + 1], 0, vertices[1][0][i + 1] };
		GLfloat v2[] = { vertices[0][1][i + 1], 0, vertices[1][1][i + 1] };
		GLfloat v3[] = { vertices[0][1][i], 0, vertices[1][1][i] };
		// luces
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, BLANCO); // difCarretera
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, espCarretera); 
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 3);
		// textura
		glBindTexture(GL_TEXTURE_2D, text[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		int resolucion = 10;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (i > ((numQuads / 3) + 1)) { resolucion = 70; }  // un tercio de la carretera aparecera en alta resolucion
		if (estado == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
		quadtex(v0, v3, v2, v1, 1, 0, 1, 0, resolucion, resolucion);
		glPopMatrix();
	}
	// FIN CARRETERA 

	// FONDO / PAISAJE
	glBindTexture(GL_TEXTURE_2D, text[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glPushMatrix();
	glTranslatef(cam[0], cam[1], cam[2]);
	glCallList(paisaje);
	glPopMatrix();
	// FIN FONDO

	// FAROLAS + CARTEL
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, espCartel);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difCartel);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 3);
	cartelesIluminados();  
	//FIN FAROLAS + CARTEL

	//Suelo
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, BLANCO); // difCarretera
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, espCarretera);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);

	glBindTexture(GL_TEXTURE_2D, text[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	//Fijo el suelo para que no se mueva todo el rato
	glPushMatrix();
	if (cam[0] > aux) {
		aux += 15;
		reinicio[0] = cam[0];
		reinicio[1] = -0.067;
		reinicio[2] = cam[2];
	}
	glTranslatef(reinicio[0], 0, reinicio[2]);
	glRotatef(90, 0, 1, 0);
	glCallList(piso);
	glPopMatrix();
	//FIN DEL PISO

	//Captura de pantalla
	if (captura) {
		captura = false;
		saveScreenshot(salida, anchura, altura);
	}
	glutSwapBuffers();
}

void reshape(GLint w, GLint h)  // FUNCION DE ATENCION AL REDIMENSIONAMIENTO
{
	nozar = (float)h / w;
	anchura = w;
	altura = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float razon = (float)w / h;
	gluPerspective(45, razon, 1, 100);
}

void onSpecialKey(int specialKey, int x, int y) {
	switch (specialKey)
	{
	case GLUT_KEY_UP:
		velocidad = velocidad + vIncr;        // se cambia el modulo de la velocidad actual
		break;

	case GLUT_KEY_DOWN:
		if (velocidad - vIncr < 0) { velocidad = 0; }
		else { velocidad = velocidad - vIncr; }
		break;

	case GLUT_KEY_LEFT:
		vDir[1] -= rad(1);     // se cambia la componente Z del vector director para rotar respecto al eje Y
		break;

	case GLUT_KEY_RIGHT:
		vDir[1] += rad(1);
		break;
	}
	glutPostRedisplay();
}

void onKey(unsigned char tecla, int x, int y)
{
	switch (tecla) {
	//Modo Alambrico
	case 's':
		if (estado == 1) { estado = 0; }
		else { estado = 1; }
		break;
	//Modo Noche
	case 'l':
		if (estado == 2) { estado = 0; }
		else { estado = 2; }
		break;
	//Modo Niebla
	case 'n':
		if (estado == 3) { estado = 0; }
		else { estado = 3; }
		break;
	//Modo HUD
	case 'c':
		if (estado == 4) { estado = 0; }
		else { estado = 4; }
		break;
	//Modo largas
	case 'z':
		largas = not largas;
		break;
	//Modo Camara
	case 'k':
		if (estado == 5) { estado = 0; }
		else { estado = 5; }
		break;
	//Modo captura
	case 'p':
		captura = true;
		break;

	//Modo ejes
	case 'e':
		if (estado == 6) { estado = 0; }
		else { estado = 6; }
		if (cam[0] > 0) { if ((cam[0] > 0 && estado == 6)) { printf("para mostrar los ejes debemos estar en el inicio de la recta(0.0) \n"); } }
		break;

		//Modo Alambrico
	case 'S':
		if (estado == 1) { estado = 0; }
		else { estado = 1; }
		break;
		//Modo Noche
	case 'L':
		if (estado == 2) { estado = 0; }
		else { estado = 2; }
		break;
		//Modo Niebla
	case 'N':
		if (estado == 3) { estado = 0; }
		else { estado = 3; }
		break;
		//Modo HUD
	case 'C':
		if (estado == 4) { estado = 0; }
		else { estado = 4; }
		break;
		//Modo largas
	case 'Z':
		largas = not largas;
		break;
		//Modo Camara
	case 'K':
		if (estado == 5) { estado = 0; }
		else { estado = 5; }
		break;
		//Modo captura
	case 'P':
		captura = true;
		break;
		//Modo ejes
	case 'E':
		if (estado == 6) { estado = 0; }
		else { estado = 6; }
		if (cam[0] > 0) { if ((cam[0] > 0 && estado == 6)) { printf("para mostrar los ejes debemos estar en el inicio de la recta(0.0) \n"); } }
		break;


	//EXIT
	case 27:
		exit(0);
	}
}

void onTimer(int val) {
	//la velocidad actual debe ser minimo 0
	if (velocidad < 0) { velocidad = 0; }
	// Actualizar posicion camara
	int ahora;
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	float tiempo_transcurrido;
	ahora = glutGet(GLUT_ELAPSED_TIME);
	tiempo_transcurrido = ahora - antes;
	//1000 ms son 1 s
	cam[0] += velocidad / 1000.0 * vDir[0] * (tiempo_transcurrido / 1000.0);
	cam[2] += velocidad / 1000.0 * vDir[1] * (tiempo_transcurrido / 1000.0);

	// Añadimos el modulo de la velocidad al titulo de la ventana
	stringstream titulo;
	titulo << "Velocidad: " << velocidad << "m/s";
	glutSetWindowTitle(titulo.str().c_str());
	// Llamamos a la funcion de dibujo
	glutTimerFunc(val, onTimer, val);
	glutPostRedisplay();
}

void main(int argc, char** argv)       // PROGRAMA PRINCIPAL
{
	glutInit(&argc, argv);         // Inicializacion de GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);    // Alta de buffers a usar
	glutInitWindowSize(600, 600);        // Tamanyo inicial de la ventana
	glutCreateWindow(PROYECTO);        // Creacion de la ventana con su titulo
	glutKeyboardFunc(onKey);
	glutSpecialFunc(onSpecialKey);
	init(); // Inicializacion 
	std::cout << PROYECTO << " running" << std::endl;  // Mensaje por consola
	printf("Funcionalidades de la aplicacion:\n");
	printf("     -l/L : Activa/Desactiva las luces\n");
	printf("     -n/N : Activa/Desactiva la niebla\n");
	printf("     -z/Z : Activa/Desactiva las largas si están encendidas las luces\n");
	printf("     -s/S : Activa/Desactiva el modo alámbrico\n");
	printf("     -c/C : Activa/Desactiva el HUD\n");
	printf("     -p/P : Realiza una captura de pantalla\n");
	printf("     -k/K : Activa/Desactiva la posición de la cámara\n");
	printf("     -e/E : Muestra los ejes\n");

	glutDisplayFunc(display);        // Alta de la funcion de atencion a display
	glutReshapeFunc(reshape);        // Alta de la funcion de atencion a reshape
	glutTimerFunc(1000 / 60, onTimer, 1000 / 60); // 60 FPS
	glutMainLoop();          // Puesta en marcha del programa
}