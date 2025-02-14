    #include <gl/gl.h>
    #include <gl/glut.h>
    #include <iostream>
    #include <math.h>
    #include <cstdlib>
    #include <ctime>

    using namespace std;
    typedef float point3[3];

    struct Point3D {
        GLfloat  x;
        GLfloat  y;
        GLfloat  z;
    };

    static GLfloat theta[] = { 0.0, 0.0, 0.0 };
    GLfloat pix2angle_x = 360.0 / 600.0, pix2angle_y = 360.0 / 600.0;
    int statusLeft = 0;
    int statusRight = 0;
    int x_pos_old = 0;
    int y_pos_old = 0;
    int delta_x = 0;
    int delta_y = 0;

    float azymuth = 0.0;
    float elevation = 0.0;
    float rViewer = 15.0;
    GLfloat scale = 1.0;

    int model = 1;
    int N;
    Point3D** points;
    Point3D** colors;
    bool showAxes = true;
    const float MIN_VIEWER_DISTANCE = 5.0;
    const float MAX_VIEWER_DISTANCE = 50.0;

    void Points() {
        points = new Point3D*[N];
        for (int i = 0; i < N; i++) {
            points[i] = new Point3D[N / 2 + 1];
        }
        for (int i = 0; i < N; i++) {
            float u = (float)i / (N - 1);
            for (int j = 0; j <= N / 2; j++) {
                float v = (float)j / (N - 1);

                points[i][j].x = (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * cos(M_PI * v);
                points[i][j].y = -5 + 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
                points[i][j].z = (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * sin(M_PI * v);
            }
        }
    }

    void Colors() {
        colors = new Point3D*[N];
        for (int i = 0; i < N; i++) {
            colors[i] = new Point3D[N / 2 + 1];
        }
        for (int i = 0; i < N; i++) {
            for (int j = 0; j <= N / 2; j++) {
                colors[i][j].x = (float(rand() % 11)) / 10;
                colors[i][j].y = (float(rand() % 11)) / 10;
                colors[i][j].z = (float(rand() % 11)) / 10;
            }
        }
    }

    void Axes(void) {
        if (!showAxes) return;

        float axis_length = 5.0f;

        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex3f(-axis_length, 0.0f, 0.0f);
        glVertex3f(axis_length, 0.0f, 0.0f);
        glEnd();

        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex3f(0.0f, -axis_length, 0.0f);
        glVertex3f(0.0f, axis_length, 0.0f);
        glEnd();

        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, -axis_length);
        glVertex3f(0.0f, 0.0f, axis_length);
        glEnd();
    }

    void Draw() {
        srand(time(NULL));
        switch (model) {
            case 1: // Rysowanie punktów
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j <= N / 2; j++) {
                        glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
                        glBegin(GL_POINTS);
                        glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                        glEnd();

                        // Lustrzane odbicie
                        glBegin(GL_POINTS);
                        glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                        glEnd();
                    }
                }
                break;

            case 2:
                for (int i = 0; i < N - 1; i++) {
                    for (int j = 0; j < N / 2; j++) {
                        glColor3f(0.1, 1.0, 1.0);
                        glBegin(GL_LINES);
                        glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                        glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                        glEnd();

                        glBegin(GL_LINES);
                        glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                        glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                        glEnd();

                        // Lustrzane odbicie
                        glBegin(GL_LINES);
                        glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                        glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
                        glEnd();

                        glBegin(GL_LINES);
                        glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                        glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                        glEnd();
                    }
                }
                break;

            case 3:
                for (int i = 0; i < N - 1; i++) {
                    for (int j = 0; j < N / 2; j++) {
                        glBegin(GL_TRIANGLES);

                        // Pierwszy trójkąt
                        glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
                        glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);

                        glColor3f(colors[i + 1][j].x, colors[i + 1][j].y, colors[i + 1][j].z);
                        glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);

                        glColor3f(colors[i][j + 1].x, colors[i][j + 1].y, colors[i][j + 1].z);
                        glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);

                        glEnd();

                        glBegin(GL_TRIANGLES);

                        // Drugi trójkąt
                        glColor3f(colors[i][j + 1].x, colors[i][j + 1].y, colors[i][j + 1].z);
                        glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);

                        glColor3f(colors[i + 1][j].x, colors[i + 1][j].y, colors[i + 1][j].z);
                        glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);

                        glColor3f(colors[i + 1][j + 1].x, colors[i + 1][j + 1].y, colors[i + 1][j + 1].z);
                        glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);

                        glEnd();

                        // Lustrzane odbicie trójkątów
                        glBegin(GL_TRIANGLES);

                        // Pierwszy trójkąt odbity
                        glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
                        glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);

                        glColor3f(colors[i + 1][j].x, colors[i + 1][j].y, colors[i + 1][j].z);
                        glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);

                        glColor3f(colors[i][j + 1].x, colors[i][j + 1].y, colors[i][j + 1].z);
                        glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);

                        glEnd();

                        glBegin(GL_TRIANGLES);

                        // Drugi trójkąt odbity
                        glColor3f(colors[i][j + 1].x, colors[i][j + 1].y, colors[i][j + 1].z);
                        glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);

                        glColor3f(colors[i + 1][j].x, colors[i + 1][j].y, colors[i + 1][j].z);
                        glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);

                        glColor3f(colors[i + 1][j + 1].x, colors[i + 1][j + 1].y, colors[i + 1][j + 1].z);
                        glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);

                        glEnd();
                    }
                }
                break;

            case 4:
                glutWireTeapot(5.0);
                break;
        }
    }

    void RenderScene(void) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Obliczenie pozycji kamery na podstawie azymutu, elewacji i odległości
        float camX = rViewer * cos(azymuth) * cos(elevation);
        float camY = rViewer * sin(elevation);
        float camZ = rViewer * sin(azymuth) * cos(elevation);

        // Ustawienie kamery z użyciem gluLookAt() - zawsze skierowana na środek sceny
        gluLookAt(camX, camY, camZ,
                  0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0);

        // Ustawienie światła
        GLfloat light_position[] = { 0.0, 10.0, 10.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        Axes(); // Narysowanie osi
        glScalef(scale, scale, scale);
        glRotatef(theta[0], 1.0, 0.0, 0.0);
        glRotatef(theta[1], 0.0, 1.0, 0.0);
        glRotatef(theta[2], 0.0, 0.0, 1.0);
        Draw();
        glFlush();
        glutSwapBuffers();
    }

    void MyInit(void) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glPointSize(1.0);
        glLineWidth(2.0);

        GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    }

    void ChangeSize(GLsizei horizontal, GLsizei vertical) {
        GLfloat AspectRatio;
        if (vertical == 0) {
            vertical = 1;
        }
        glViewport(0, 0, horizontal, vertical);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

        gluPerspective(45.0, AspectRatio, 1.0, 100.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void Mouse(int btn, int state, int x, int y) {
        if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            x_pos_old = x; // przypisanie aktualnie odczytanej pozycji kursora
            y_pos_old = y;
            statusLeft = 1; // wciśnięty został lewy klawisz myszy
        } else {
            statusLeft = 0; // lewy klawisz nie jest wciśnięty
        }

        // Obsługa kółka myszy do zoomowania
        if (btn == 3) { // Scroll up (zoom in)
            rViewer -= 1.0f;
            if (rViewer < MIN_VIEWER_DISTANCE) {
                rViewer = MIN_VIEWER_DISTANCE;
            }
            glutPostRedisplay();
        }
        if (btn == 4) { // Scroll down (zoom out)
            rViewer += 1.0f;
            if (rViewer > MAX_VIEWER_DISTANCE) {
                rViewer = MAX_VIEWER_DISTANCE;
            }
            glutPostRedisplay();
        }
    }

    void Motion(GLsizei x, GLsizei y) {
        delta_x = x - x_pos_old;
        delta_y = y - y_pos_old;
        x_pos_old = x;
        y_pos_old = y;

        if (statusLeft == 1) { // Obrót obiektu
            azymuth += delta_x * pix2angle_x / 20.0;
            elevation += delta_y * pix2angle_y / 20.0;

            // Ograniczenia obrotu w pionie, aby kamera nie obracała się zbyt daleko
            if (elevation > 1.5) elevation = 1.5;
            if (elevation < -1.5) elevation = -1.5;
        }

        glutPostRedisplay();
    }

    void keys(unsigned char key, int x, int y) {
        if (key == 'r') {
            if (points != nullptr && colors != nullptr) {
                for (int i = 0; i < N; i++) {
                    delete[] points[i];
                    delete[] colors[i];
                }
                delete[] points;
                delete[] colors;
            }

            cout << "Podaj nowa wartosc N: ";
            cin >> N;
            if (N < 2) {
                cout << "Wartosc N musi byc wieksza lub rowna 2. Ustawiono N = 2." << endl;
                N = 2;
            }

            Points();
            Colors();
            RenderScene();
            return;
        }
        if (key == 27) exit(0);
        if (key == 'p') model = 1;
        if (key == 's') model = 2;
        if (key == 't') model = 3;
        if (key == 'c') model = 4;
        if (key == 'y') showAxes = !showAxes;

        glutPostRedisplay();
    }

    int main(int argc, char** argv) {
        points = nullptr;
        colors = nullptr;
        cout << "Podaj wartosc N: ";
        cin >> N;
        if (N < 2) {
            cout << "Wartosc N musi byc wieksza lub rowna 2. Ustawiono N = 2." << endl;
            N = 2;
        }
        cout << " Klawisz p - punkty" << endl;
        cout << " Klawisz s - siatka" << endl;
        cout << " Klawisz t - trojkaty" << endl;
        cout << " Klawisz c - czajnik" << endl;
        cout << " Klawisz y - ukryc/pokazac osie wspolrzednych" << endl;
        cout << " Lewy przycisk myszy - rotacja obiektu" << endl;
        cout << " Kolko myszy do zoomowania" << endl;
        cout << " Klawisz esc - zakonczenie programu" << endl;
        cout << " Klawisz r - powrot do wyboru wartosci N" << endl;

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(600, 600);
        glutCreateWindow("3D Objects");

        Points();
        Colors();

        glutKeyboardFunc(keys);
        glutDisplayFunc(RenderScene);
        glutMouseFunc(Mouse);
        glutMotionFunc(Motion);
        glutReshapeFunc(ChangeSize);
        MyInit();

        glEnable(GL_DEPTH_TEST);
        glutMainLoop();

        for (int i = 0; i < N; i++) {
            delete[] points[i];
            delete[] colors[i];
        }
        delete[] points;
        delete[] colors;

        return 0;
    }
