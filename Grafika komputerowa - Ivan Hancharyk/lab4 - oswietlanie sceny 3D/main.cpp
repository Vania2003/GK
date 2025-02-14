# define M_PI 3.141592653589793238462643383279502884L
#include <gl/gl.h>
#include <gl/glut.h>
#include<iostream>
#include <cmath>
#include <GL/freeglut_ext.h>

using namespace std;

GLfloat yellow_light_position[] = { 10.0, 10.0, 10.0, 1.0 };
GLfloat blue_light_position[] = { 10.0, 0.0, 0.0, 1.0 };

static GLfloat scale = 10.0;
static GLfloat viewer[] = { 0.0, 0.0, scale };
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLfloat pix2angle;
static GLfloat pix2angle_x;
static GLfloat pix2angle_y;
static GLint statusLeft = 0;
static GLint statusRight = 0;

static int x_pos_old = 0;
static int delta_x = 0;
static int y_pos_old = 0;
static int delta_y = 0;

float rViewer = 10;
static GLfloat azymuth_light0 = 0;
static GLfloat elevation_light0 = 0;
static GLfloat azymuth_light1 = 0;
static GLfloat elevation_light1 = 0;

int task = 2;
int type = 2;
int model = 3;
typedef float point3[3];

struct Point3D {
    GLfloat  x;
    GLfloat  y;
    GLfloat  z;
};
GLfloat Xu, Xv, Yu, Yv, Zu, Zv;
GLfloat vectX, vectY, vectZ;

Point3D** points;
Point3D** colors;
point3** vectors;
int N = 10;

void Axes(void)
{
    glLineWidth(4.0);
    point3 x_min = { -5.0, 0.0, 0.0 };
    point3 x_max = { 5.0, 0.0, 0.0 };
    point3 y_min = { 0.0, -5.0, 0.0 };
    point3 y_max = { 0.0, 5.0, 0.0 };
    point3 z_min = { 0.0, 0.0, -5.0 };
    point3 z_max = { 0.0, 0.0, 5.0 };
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

void Points() {
    if (N < 5) N = 5;

    vectors = new point3*[N];
    points = new Point3D*[N];
    for (int i = 0; i < N; i++) {
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
            Zv = -M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * cos(M_PI * v);

            vectX = Yv * Zu - Yu * Zv;
            vectY = Zv * Xu - Zu * Xv;
            vectZ = Xv * Yu - Xu * Yv;

            //normalizacja
            float vectorLen = sqrt(pow(vectX, 2) + pow(vectY, 2) + pow(vectZ, 2));
            if (vectorLen == 0) vectorLen = 1;
            vectors[i][j][0] = vectX / vectorLen;
            vectors[i][j][1] = vectY / vectorLen;
            vectors[i][j][2] = vectZ / vectorLen;

            if (i < (N + 1) / 2) {
                vectors[i][j][0] *= -1;
                vectors[i][j][1] *= -1;
                vectors[i][j][2] *= -1;
            }
        }
    }
}

void Colors() {
    colors = new Point3D * [N];
    for (int i = 0; i <= N; i++) {
        colors[i] = new Point3D[N];
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            colors[i][j].x = (float(rand() % 11)) / 10;
            colors[i][j].y = (float(rand() % 11)) / 10;
            colors[i][j].z = (float(rand() % 11)) / 10;
        }
    }
}

void Egg() {
    switch (model) {
        case 3:
            for (int i = 0; i < N - 1; i++) {
                for (int j = 0; j < N - 1; j++) {
                    glBegin(GL_TRIANGLES);
                    glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
                    glNormal3fv(vectors[i][j]);
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glColor3f(colors[i + 1][j].x, colors[i + 1][j].y, colors[i + 1][j].z);
                    glNormal3fv(vectors[i + 1][j]);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    glColor3f(colors[i + 1][j + 1].x, colors[i + 1][j + 1].y, colors[i + 1][j + 1].z);
                    glNormal3fv(vectors[i + 1][j + 1]);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                    glEnd();

                    glBegin(GL_TRIANGLES);
                    glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
                    glNormal3fv(vectors[i][j]);
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glColor3f(colors[i][j + 1].x, colors[i][j + 1].y, colors[i][j + 1].z);
                    glNormal3fv(vectors[i][j + 1]);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                    glColor3f(colors[i + 1][j + 1].x, colors[i + 1][j + 1].y, colors[i + 1][j + 1].z);
                    glNormal3fv(vectors[i + 1][j + 1]);

                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                    glEnd();
                }
            }
            break;
    }
}

void Mouse(int btn, int state, int x, int y)
{
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        x_pos_old = x;
        y_pos_old = y;
        statusLeft = 1;
    }
    else
        statusLeft = 0;
    if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        y_pos_old = y;
        statusRight = 1;
    }
    else
        statusRight = 0;
}

void Wheel(int wheel, int direction, int x, int y)
{
    if (direction > 0) {
        scale -= 0.5f;
    }
    else {
        scale += 0.5f;
    }
    if (scale <= 0.1f) scale = 0.1f;
    if (scale >= 50.0f) scale = 50.0f;
    glutPostRedisplay();
}

void Motion(GLsizei x, GLsizei y) {
    delta_x = x - x_pos_old;
    x_pos_old = x;
    delta_y = y - y_pos_old;
    y_pos_old = y;
    glutPostRedisplay();
}

void task1() {
    if (statusLeft == 1){
        theta[0] -= delta_x * pix2angle;
        theta[1] += delta_y * pix2angle;
    }
    glRotatef(theta[0], 0.0, 1.0, 0.0);
    glRotatef(theta[1], 0.0, 0.0, 1.0);
}

void task2() {
    // Handle light 0 (yellow)
    if (statusLeft == 1) {
        azymuth_light0 -= delta_x * pix2angle / 10;
        elevation_light0 += delta_y * pix2angle / 10;

        if (azymuth_light0 <= -360.0)
            azymuth_light0 = 0.0;
        if (elevation_light0 >= 360.0)
            elevation_light0 = 0.0;

        // Update yellow light position based on the updated azimuth and elevation
        yellow_light_position[0] = rViewer * cos(azymuth_light0) * cos(elevation_light0);
        yellow_light_position[1] = rViewer * sin(elevation_light0);
        yellow_light_position[2] = rViewer * sin(azymuth_light0) * cos(elevation_light0);

        glLightfv(GL_LIGHT0, GL_POSITION, yellow_light_position);
    }

    // Handle light 1 (blue)
    if (statusRight == 1) {
        azymuth_light1 -= delta_x * pix2angle / 10;
        elevation_light1 += delta_y * pix2angle / 10;

        if (azymuth_light1 <= -360.0)
            azymuth_light1 = 0.0;
        if (elevation_light1 >= 360)
            elevation_light1 = 0.0;

        // Update blue light position based on the updated azimuth and elevation
        blue_light_position[0] = rViewer * cos(azymuth_light1) * cos(elevation_light1);
        blue_light_position[1] = rViewer * sin(elevation_light1);
        blue_light_position[2] = rViewer * sin(azymuth_light1) * cos(elevation_light1);

        glLightfv(GL_LIGHT1, GL_POSITION, blue_light_position);
    }
}

void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(viewer[0], viewer[1], scale, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    Axes();

    if (task == 1) {
        task1();  // Rotacja kamery
    } else if (task == 2) {
        task2();  // Sterowanie światłem
    }

    glPushMatrix();

    switch (type) {
        case 1:
            glutSolidTeapot(3.0);
            break;
        case 2:
            Egg();
            break;
    }

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void keys(unsigned char key, int x, int y) {
    if (key == '1') task = 1;
    if (key == '2') task = 2;
    if (key == 'c') type = 1;
    if (key == 'j') type = 2;

    if (key == '-') {
        if (N > 5) {
            N -= 1;
            Points();
            Colors();
        }
    }
    if (key == '+') {
        if (N < 100) {
            N += 1;
            Points();
            Colors();
        }
    }
    if (key == 27) {
        exit(0);
    }

    RenderScene();
}

void MyInit(void)
{
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess = { 20.0 };

    GLfloat light_ambient[] = { 0.1, 0.0, 0.0, 0.25 };
    GLfloat yellow_light_diffuse[] = { 1.0, 1.0, 0.0, 0.0 };
    GLfloat blue_light_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat att_constant = { 1.0 };
    GLfloat att_linear = { 0.05 };
    GLfloat att_quadratic = { 0.001 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow_light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, yellow_light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, blue_light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, blue_light_position);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
    pix2angle = 360.0 / (float)horizontal;
    pix2angle_x = 360.0 * 0.1 / (float)horizontal;
    pix2angle_y = 360.0 * 0.1 / (float)vertical;

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

int main(int argc, char** argv) {
    cout << " Klawisz 1 - sterowanie obrotem obiektu" << endl;
    cout << " Klawisz 2 - sterowanie ruchem swiatla" << endl;
    cout << " Lewy klawisz myszy - swiatlo zolte" << endl;
    cout << " Prawy klawisz myszy - swiatlo niebieskie" << endl;
    cout << " Klawisz c - czajnik" << endl;
    cout << " Klawisz j - jajko" << endl;
    cout << " Klawisz + zwiekszenie punktow" << endl;
    cout << " Klawisz - zmniejszenie punktow" << endl;
    cout << " Esc - zakonczenie dzialania programu" << endl;

    Points();
    Colors();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Oswietlanie scen 3-D");

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutKeyboardFunc(keys);
    glutMouseWheelFunc(Wheel);

    MyInit();
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 0;
}