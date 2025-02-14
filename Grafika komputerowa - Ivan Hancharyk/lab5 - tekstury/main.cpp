#define _CRT_SECURE_NO_DEPRECATE
#define M_PI 3.141592653589793238462643383279502884L
#include <windows.h>
#include <commdlg.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <math.h>
using namespace std;

typedef float point3[3];
GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat);
void ApplyTexture(const char* filename);

struct Point3D {
	GLfloat  x;
	GLfloat  y;
	GLfloat  z;
};
GLfloat Xu, Xv, Yu, Yv, Zu, Zv;
GLfloat vectX, vectY, vectZ;
GLfloat vectX1, vectY1, vectZ1;

Point3D** points;
point3** vectors;
point3** vectorss;
int N = 10;

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat thetaArr[] = { 0.0, 0.0, 0.0 };
static GLfloat theta = 0.0;
static GLfloat alfa = 0.0;
static GLfloat pix2angle;
static GLint edge1=1;
static GLint edge2=1;
static GLint edge3=1;
static GLint edge4=1;
static GLint edge5=1;
static GLint status = 0;
static int x_pos_old = 0;
static int y_pos_old = 0;
static int scroll_old = 0;
static int delta_x = 0;
static int delta_y = 0;
static int delta_scroll = 0;
static int type = 0;

void spinEgg()
{
	thetaArr[0] -= 0.5;
	if (thetaArr[0] > 360.0) thetaArr[0] -= 360.0;

	thetaArr[1] -= 0.5;
	if (thetaArr[1] > 360.0) thetaArr[1] -= 360.0;

	thetaArr[2] -= 0.5;
	if (thetaArr[2] > 360.0) thetaArr[2] -= 360.0;

	glutPostRedisplay();
}

void Points() {
	vectors = new  point3 * [N];
	points = new  Point3D * [N];
	for (int i = 0; i <= N; i++) {
		points[i] = new Point3D[N];
		vectors[i] = new point3[N];
	}

	for (int i = 0; i < N; i++) {
		float u = ((float)i) / (N - 1);

		for (int j = 0; j < N; j++) {
			float v = ((float)j) / (N - 1);

			points[i][j].x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(M_PI * v);
			points[i][j].y = -5 + 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
			points[i][j].z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(M_PI * v);

			// obliczenie wektorów normalnych
			Xu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45) * cos(M_PI * v);
			Xv = M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * sin(M_PI * v);
			Yu = -5 + 640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u;
			Yv = -5;
			Zu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45) * sin(M_PI * v);
			Zv = -M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * cos(M_PI * v);;

			vectX = Yv * Zu - Yu * Zv;
			vectY = Zv * Xu - Zu * Xv;
			vectZ = Xv * Yu - Xu * Yv;

			//normalizacja
			float vectorLen = sqrt(pow(vectX, 2) + pow(vectY, 2) + pow(vectZ, 2));
			if (vectorLen == 0) vectorLen = 1;
			vectors[i][j][0] = vectX / vectorLen;
			vectors[i][j][1] = vectY / vectorLen;
			vectors[i][j][2] = vectZ / vectorLen;

			if (i < (N + 1) / 2)
			{
				vectors[i][j][0] *= -1;
				vectors[i][j][1] *= -1;
				vectors[i][j][2] *= -1;
			}
		}
	}
}

void Egg() {

	Points();

	for (int i = 0; i < N - 1; i++) {
		for (int j = 0; j < N - 1; j++) {
			glBegin(GL_TRIANGLES);
			glNormal3fv(vectors[i][j]);
			glTexCoord2f((float) i / (N-1), (float)j / (N - 1));
			glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
			glNormal3fv(vectors[i + 1][j]);
			glTexCoord2f((float)(i+1) / (N - 1), (float)j / (N - 1));
			glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
			glNormal3fv(vectors[i + 1][j + 1]);
			glTexCoord2f((float)(i+1) / (N - 1), (float)(j+1) / (N - 1));
			glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
			glEnd();

			glBegin(GL_TRIANGLES);
			glNormal3fv(vectors[i][j]);
			glTexCoord2f((float)i / (N - 1), (float)j / (N - 1));
			glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
			glNormal3fv(vectors[i][j + 1]);
			glTexCoord2f((float)i / (N - 1), (float)(j+1) / (N - 1));
			glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
			glNormal3fv(vectors[i + 1][j + 1]);
			glTexCoord2f((float)(i + 1) / (N - 1), (float)(j + 1) / (N - 1));
			glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
			glEnd();
		}
	}
}

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		x_pos_old = x; // Przypisanie pozycji kursora myszy
		y_pos_old = y;
		status = 1; // Wciśnięty został lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		scroll_old = y;
		status = 2; // Wciśnięty został prawy klawisz myszy
	}
	else if (btn == 3) { // Scroll up (zoom in)
		viewer[2] -= 0.5f;
	}
	else if (btn == 4) { // Scroll down (zoom out)
		viewer[2] += 0.5f;
	}
	else {
		status = 0; // Nie wciśnięto żadnego klawisza
	}
	glutPostRedisplay(); // Przerysowanie sceny
}

void Motion(GLsizei x, GLsizei y)
{
	if (status == 1) {
		delta_x = x - x_pos_old;     // obliczenie roznicy polozenia kursora myszy
		delta_y = y - y_pos_old;
		x_pos_old = x;            // podstawienie biezacego polozenia jako poprzednie
		y_pos_old = y;
	}
	else if (status == 2) {
		delta_scroll = y - scroll_old;
		scroll_old = y;
	}
	glutPostRedisplay();     // przerysowanie obrazu sceny
}

void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// poczatek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };
	// poczatek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
	//  poczatek i koniec obrazu osi y

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

void Teapot() {
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidTeapot(1.0);
	glPopMatrix();
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Kamera - zoom
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	if (status == 1) {
		theta += delta_x * pix2angle;
		alfa += delta_y * pix2angle;
	}

	glRotatef(thetaArr[0], 1.0, 0.0, 0.0);
	glRotatef(thetaArr[1], 0.0, 1.0, 0.0);
	glRotatef(thetaArr[2], 0.0, 0.0, 1.0);
	glRotatef(theta, 0.0, 1.0, 0.0);
	glRotatef(alfa, 1.0, 0.0, 0.0);

	glColor3f(1.0f, 1.0f, 1.0f);

	// Wyświetlanie obiektu w zależności od typu
	switch (type) {
		case 1: // Czajnik
			Teapot();
		break;
		case 2: // Jajko
			Egg();
		break;
	}

	glFlush();
	glutSwapBuffers();
}

char* ChooseTextureFile() {
	static char filename[MAX_PATH] = {0};
	OPENFILENAME ofn; // Struktura konfiguracji okna dialogowego
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "TGA Files\0*.tga\0All Files\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = "tga";
	if (GetOpenFileName(&ofn)) {
		return filename;
	}
	return nullptr; // Użytkownik anulował wybór
}

// Funkcja nakładająca teksturę
void ApplyTexture(const char* filename) {
	GLbyte* pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;

	pBytes = LoadTGAImage(filename, &ImWidth, &ImHeight, &ImComponents, &ImFormat);
	if (!pBytes) {
		cout << "Nie można załadować pliku tekstury: " << filename << endl;
		return;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);
	free(pBytes);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat)
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

	FILE* pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte* pbitsperpixel = NULL;

	// Wartosci domyslne zwracane w przypadku bledu
	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	pFile = fopen(FileName, "rb");
	if (pFile == NULL)
		return NULL;

	// Przeczytanie naglowka pliku
	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

	// Odczytanie szerokosci, wysokosci i glebi obrazu
	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;

	// Sprawdzenie, czy glebia spelnia zalozone warunki (8, 24, lub 32 bity)
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

void MyInit(void)
{
// Zmienne dla obrazu tekstury
	GLbyte* pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat mat_ambient[] = { 1.0,1.0, 1.0, 1 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess = { 20.0 };
	GLfloat light_position[] = { 0.0, 0.0, 30.0, 1.0 };
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat att_constant = { 1.0 };
	GLfloat att_linear = { (GLfloat)0.05 };
	GLfloat att_quadratic = { (GLfloat)0.001 };

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

	glShadeModel(GL_SMOOTH); // wlaczenie lagodnego cieniowania
	glEnable(GL_LIGHTING);   // wlaczenie systemu oswietlenia sceny
	glEnable(GL_LIGHT0);     // wlaczenie zrodla o numerze 0
	glEnable(GL_DEPTH_TEST); // wlaczenie mechanizmu z-bufora

	pBytes = LoadTGAImage("D1_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	free(pBytes);

	glEnable(GL_TEXTURE_2D);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	GLdouble aspect = (GLdouble)horizontal / vertical;

	gluPerspective(70, (float)aspect, 1.0, 30.0);

	glViewport(0, 0, horizontal, vertical);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void keys(unsigned char key, int x, int y) {
	if (key == 'c') type = 1; // Wyświetlenie czajnika
	if (key == 'j') type = 2; // Wyświetlenie jajka

	if (key == '-') { // Zmniejszenie punktów dla jajka
		N -= 1;
		if (N <= 1) N = 1;
		Points();
	}
	if (key == '+') { // Zwiększenie punktów dla jajka
		N += 1;
		Points();
	}
	if (key == 't') {
		char* textureFile = ChooseTextureFile();
		if (textureFile) {
			ApplyTexture(textureFile);
			cout << "Zaladowano teksture: " << textureFile << endl;
		}
	}
	RenderScene();
}

int main(int argc, char** argv)
{
	cout << " Klawisz c - czajnik" << endl;
	cout << " Klawisz j - jajko" << endl;
	cout << " Klawisz t - wybor tekstury " << endl;
	cout << " Klawisz + - zwiekszenie punktow (dla jajka)" << endl;
	cout << " Klawisz - - zmniejszenie punktow (dla jajka)" << endl;
	cout << " Scroll myszki - zoom" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Teksturowanie");

	glutDisplayFunc(RenderScene);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keys);

	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}
