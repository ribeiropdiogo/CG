#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include "EngineMotion.h"

GLfloat mode = GL_FILL; // GL_FILL, GL_LINE; GL_POINT
GLfloat beta = 0.0f;
GLfloat alpha = 0.0f;
GLfloat raio = 5.0f;

GLfloat px = raio * sin(alpha)*cos(beta);
GLfloat py = raio * sin(beta);
GLfloat pz = raio * cos(alpha)*cos(beta);

GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat z = 0.0f;

void EngineMotion::handle_ascii(unsigned char key, int xn, int yn) {
    switch (key) {
        case '1':
            mode = GL_FILL;
            break;
        case '2':
            mode = GL_LINE;
            break;
        case '3':
            mode = GL_POINT;
            break;
        case 'w':
            beta += M_PI / 100;
            if (beta > M_PI / 2)
                beta -= M_PI / 100;
            break;
        case 's':
            beta -= M_PI / 100;
            if (beta < -M_PI / 2)
                beta += M_PI / 100;
            break;
        case 'd':
            alpha += M_PI / 100;
            break;
        case 'a':
            alpha -= M_PI / 100;
            break;
        case 'h':
            y += M_PI / 100;
            break;
        case 'j':
            y -= M_PI / 100;
            break;
        case 'r':
            x = y = z = 0.0f;
            raio = 5.0f;
            alpha = beta = 0.0f;
            break;
        default:;
    }

    px = raio * sin(alpha)*cos(beta);
    py = raio * sin(beta);
    pz = raio * cos(alpha)*cos(beta);

    glutPostRedisplay();
}

void EngineMotion::handle_special(int key, int xn, int yn) {

    switch (key) {
        case GLUT_KEY_LEFT :
            x += 2 * M_PI / 100;
            break;
        case GLUT_KEY_RIGHT :
            x -= 2 * M_PI / 100;
            break;
        case GLUT_KEY_UP :
            raio -= 2 * M_PI / 100;
            break;
        case GLUT_KEY_DOWN :
            raio += 2 * M_PI / 100;
            break;
    }

    px = raio * sin(alpha)*cos(beta);
    py = raio * sin(beta);
    pz = raio * cos(alpha)*cos(beta);

    glutPostRedisplay();
    }

void EngineMotion::place_camera() {
    glLoadIdentity();

    gluLookAt(z+px, y+py, z+pz,
              x, y, z,
              0.0f,1.0f,0.0f);

    glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void EngineMotion::projection_size(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}