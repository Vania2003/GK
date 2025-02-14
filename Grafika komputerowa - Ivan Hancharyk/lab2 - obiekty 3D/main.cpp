#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <math.h>

using namespace std;
typedef float point3[3];

struct Point3D {
    GLfloat  x;
    GLfloat  y;
    GLfloat  z;
};

static GLfloat theta[] = { 0.0, 0.0, 0.0 };
int model = 1;
const int N = 50;
bool rotation = false;
bool rot_X = false;
bool rot_Z = false;
Point3D points[N][N];
Point3D colors[N][N];

void Points() {
    for (int i = 0; i < N; i++) {
        float u = (float)i / (N - 1);
        for (int j = 0; j < N; j++) {
            float v = (float)j / (N - 1);

            points[i][j].x = (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * cos(M_PI * v);
            points[i][j].y = -5 + 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
            points[i][j].z = (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * sin(M_PI * v);
        }
    }
}

void Colors() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            colors[i][j].x = (float(rand() % 11)) / 10;
            colors[i][j].y = (float(rand() % 11)) / 10;
            colors[i][j].z = (float(rand() % 11)) / 10;
        }
    }
}

void Axes(void) {
    point3 x_min = { -1.0, 0.0, 0.0 };
    point3 x_max = { 1.0, 0.0, 0.0 };

    point3 y_min = { 0.0, -1.0, 0.0 };
    point3 y_max = { 0.0,  1.0, 0.0 };

    point3 z_min = { 0.0, 0.0, -1.0 };
    point3 z_max = { 0.0, 0.0,  1.0 };

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

void Draw() {
    switch (model) {
        case 1:
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
                    glBegin(GL_POINTS);
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glEnd();
                }
            }
            break;
        case 2:
            for (int i = 0; i < N - 1; i++) {
                for (int j = 0; j < N - 1; j++) {
                    glColor3f(0.1, 1.0, 1.0);
                    glBegin(GL_LINES);
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    glEnd();

                    glBegin(GL_LINES);
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                    glEnd();
                }
            }
            break;
        case 3:
            for (int i = 0; i < N - 1; i++) {
                for (int j = 0; j < N - 1; j++) {
                    glBegin(GL_TRIANGLES);
                    glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glColor3f(colors[i + 1][j].x, colors[i + 1][j].y, colors[i + 1][j].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    glColor3f(colors[i + 1][j + 1].x, colors[i + 1][j + 1].y, colors[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                    glEnd();

                    glBegin(GL_TRIANGLES);
                    glColor3f(colors[i][j].x, colors[i][j].y, colors [i][j].z);
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glColor3f(colors[i][j + 1].x, colors[i][j + 1].y, colors[i][j + 1].z);
                    glVertex3f(points[i][j + 1].x, points[i][j+1].y, points[i][j + 1].z);
                    glColor3f(colors[i+1][j + 1].x, colors[i+1][j+1].y, colors[i+1][j+1].z);
                    glVertex3f(points[i+1][j + 1].x, points[i+1][j + 1].y, points[i+1][j + 1].z);
                    glEnd();
                }
            }
            break;
        case 4:
            glutWireTeapot(5.0);
    }
}

void autospinEgg() {
    theta[0] -= 0.005;
    if (theta[0] > 360.0) theta[0] -= 360.0;

    theta[1] -= 0.005;
    if (theta[1] > 360.0) theta[1] -= 360.0;

    theta[2] -= 0.005;
    if (theta[2] > 360.0) theta[2] -= 360.0;

    glutPostRedisplay();
}

void spinX() {
    if (theta[0] > 360.0) theta[0] -= 360.0;
    if (theta[0] < -360.0) theta[0] += 360.0;
    glutPostRedisplay();
}

void spinZ() {
    if (theta[2] > 360.0) theta[2] -= 360.0;
    if (theta[2] < -360.0) theta[2] += 360.0;
    glutPostRedisplay();
}

void spinEgg() {
    if (rotation) autospinEgg();
    else {
        if (rot_X) spinX();
        if (rot_Z) spinZ();
    }
}

void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    Axes();
    glRotatef(theta[0], 1.0, 0.0, 0.0);
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    glRotatef(theta[2], 0.0, 0.0, 1.0);
    Draw();
    glFlush();
    glutSwapBuffers();
}

void MyInit(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical) {
    GLfloat AspectRatio;
    if (vertical == 0)
        vertical = 1;
    glViewport(0, 0, horizontal, vertical);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    if (horizontal <= vertical)
        glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
    else
        glOrtho(-7.5 * AspectRatio, 7.5 * AspectRatio, -7.5, 7.5, 10.0, -10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keys(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    if (key == 'p') model = 1;
    if (key == 's') model = 2;
    if (key == 't') model = 3;
    if (key == 'c') model = 4;
    if (key == 32) {
        rotation = !rotation;
    }
    RenderScene();
}

void processSpecialKeys(int key, int x, int y) {
    if (rotation) return; 
    if (key == GLUT_KEY_LEFT) {
        theta[2] += 1.0;
        rot_X = false;
        rot_Z = true;
    }

    if (key == GLUT_KEY_RIGHT) {
        theta[2] -= 1.0;
        rot_X = false;
        rot_Z = true;
    }

    if (key == GLUT_KEY_UP) {
        theta[0] += 1.0;
        rot_X = true;
        rot_Z = false;
    }

    if (key == GLUT_KEY_DOWN) {
        theta[0] -= 1.0;
        rot_X = true;
        rot_Z = false;
    }
    RenderScene();
}

int main(int argc, char** argv) {
    cout << " Klawisz p - punkty" << endl;
    cout << " Klawisz s - siatka" << endl;
    cout << " Klawisz t - trojkaty" << endl;
    cout << " Klawisz c - czajnik" << endl;
    cout << " Spacja - wlaczenie/wylaczenie automatycznej rotacji" << endl;
    cout << " Klawisze strzalek - sterowanie rotacja" << endl;
    cout << " Klawisz esc - skonczenie dzialania programu" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("3D Objects");

    Points();
    Colors();

    glutKeyboardFunc(keys);
    glutDisplayFunc(RenderScene);
    glutIdleFunc(spinEgg);
    glutSpecialFunc(processSpecialKeys);
    glutReshapeFunc(ChangeSize);
    MyInit();

    glEnable(GL_DEPTH_TEST);
    glutMainLoop();

    return 0;
}
