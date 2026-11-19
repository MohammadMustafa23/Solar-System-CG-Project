#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

const float PI = 3.14159265f;
bool paused = false;
float speed = 1.0f;
float zoom = 32.0f;
float angle[8] = {0};

struct Planet {
    const char* name;
    float distance, size;
    float r, g, b;
    float orbitSpeed;
};

Planet planets[] = {
    {"Mercury", 3.0f, 0.25f, 0.6f, 0.6f, 0.6f, 4.7f},
    {"Venus", 4.5f, 0.38f, 0.9f, 0.6f, 0.2f, 3.5f},
    {"Earth", 6.0f, 0.42f, 0.1f, 0.4f, 1.0f, 2.9f},
    {"Mars", 7.7f, 0.32f, 1.0f, 0.2f, 0.1f, 2.4f},
    {"Jupiter", 10.5f, 1.0f, 0.8f, 0.5f, 0.2f, 1.3f},
    {"Saturn", 13.5f, 0.85f, 0.8f, 0.7f, 0.5f, 0.9f},
    {"Uranus", 16.5f, 0.65f, 0.2f, 0.8f, 0.7f, 0.6f},
    {"Neptune", 19.5f, 0.62f, 0.1f, 0.2f, 0.9f, 0.45f}
};

void drawText(float x, float y, float z, const char* str) {
    glColor3f(0.9f, 0.9f, 1.0f);
    glRasterPos3f(x, y, z);
    while (*str) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *str++);
}

void drawStars() {
    static bool initialized = false;
    if (!initialized) { srand(10); initialized = true; }
    glDisable(GL_LIGHTING);
    glPointSize(1.5f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 600; ++i) {
        float x = (rand() % 1000 - 500) / 15.0f;
        float y = (rand() % 500 - 250) / 15.0f;
        float z = (rand() % 1000 - 500) / 15.0f;
        glColor3f(0.7f, 0.7f, 0.9f);
        glVertex3f(x, y, z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawOrbit(float radius) {
    glDisable(GL_LIGHTING);
    glColor3f(0.20f, 0.30f, 0.42f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; ++i) {
        float a = i * PI / 180.0f;
        glVertex3f(radius * cos(a), 0.0f, radius * sin(a));
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawSun() {
    glPushMatrix();
    GLfloat emission[] = {1.0f, 0.35f, 0.02f, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    glColor3f(1.0f, 0.45f, 0.02f);
    glutSolidSphere(1.5, 50, 50);
    GLfloat noEmission[] = {0, 0, 0, 1};
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.8f, 0.3f);
    glRasterPos3f(0.4f, 1.8f, 0.0f);
    const char* name = "SUN";
    while (*name) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *name++);
    glEnable(GL_LIGHTING);
}

void drawSaturnRings() {
    glDisable(GL_LIGHTING);
    glColor3f(0.85f, 0.72f, 0.48f);
    for (int j = 0; j < 5; ++j) {
        float radius = 1.1f + j * 0.18f;
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 100; ++i) {
            float a = 2.0f * PI * i / 100.0f;
            glVertex3f(radius * cos(a), 0.0f, radius * sin(a));
        }
        glEnd();
    }
    glEnable(GL_LIGHTING);
}

void drawPlanets() {
    for (int i = 0; i < 8; ++i) {
        float a = angle[i] * PI / 180.0f;
        float x = planets[i].distance * cos(a);
        float z = planets[i].distance * sin(a);
        drawOrbit(planets[i].distance);
        glPushMatrix();
        glTranslatef(x, 0.0f, z);
        GLfloat color[] = {planets[i].r, planets[i].g, planets[i].b, 1.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
        glutSolidSphere(planets[i].size, 35, 35);
        if (i == 5) drawSaturnRings();
        glDisable(GL_LIGHTING);
        drawText(0.3f, planets[i].size + 0.4f, 0.0f, planets[i].name);
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }
}

void drawInstructions() {
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 700);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 0.8f, 0.3f);
    const char* title = "SOLAR SYSTEM";
    glRasterPos2i(20, 650);
    while (*title) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *title++);

    const char* instructions[] = {
        "SPACE: Pause / Resume",
        "+ / -: Increase / Decrease Speed",
        "W / S: Zoom In / Zoom Out",
        "R: Reset View",
        "ESC: Exit"
    };
    glColor3f(0.8f, 0.85f, 1.0f);
    for (int i = 0; i < 5; ++i) {
        glRasterPos2i(20, 620 - i * 25);
        for (const char* p = instructions[i]; *p; ++p)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, zoom * 0.65f, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    drawStars();
    GLfloat lightPosition[] = {0, 0, 0, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    drawSun();
    drawPlanets();
    drawInstructions();
    glutSwapBuffers();
}

void update(int) {
    if (!paused) {
        for (int i = 0; i < 8; ++i) {
            angle[i] += planets[i].orbitSpeed * speed;
            if (angle[i] >= 360.0f) angle[i] -= 360.0f;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case ' ': paused = !paused; break;
        case '+': case '=': speed += 0.2f; break;
        case '-': speed -= 0.2f; if (speed < 0.1f) speed = 0.1f; break;
        case 'w': case 'W': zoom -= 2.0f; if (zoom < 15.0f) zoom = 15.0f; break;
        case 's': case 'S': zoom += 2.0f; break;
        case 'r': case 'R': zoom = 32.0f; speed = 1.0f; break;
        case 27: std::exit(0);
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.005f, 0.005f, 0.02f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    GLfloat ambient[] = {0.25f, 0.25f, 0.25f, 1.0f};
    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glShadeModel(GL_SMOOTH);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 750);
    glutCreateWindow("Solar System - OpenGL / FreeGLUT");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
