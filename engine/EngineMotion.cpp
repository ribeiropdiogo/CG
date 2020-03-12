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


GLfloat zoom = 5.0f;
GLfloat left = 0.0f;
GLfloat right = 0.0f;

void EngineMotion::handle_ascii(unsigned char key, int x, int y) {
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
        default:;
    }

    glutPostRedisplay();
}

void EngineMotion::handle_special(int key, int x, int y) {

    switch (key) {
        case GLUT_KEY_LEFT :
            left += 2 * M_PI / 100;
            right -= 2 * M_PI / 100;
            break;
        case GLUT_KEY_RIGHT :
            right += 2 * M_PI / 100;
            left -= 2 * M_PI / 100;
            break;
        case GLUT_KEY_UP :
            zoom -= 2 * M_PI / 100;
            break;
        case GLUT_KEY_DOWN :
            zoom += 2 * M_PI / 100;
            break;
    }

    glutPostRedisplay();
    }

void EngineMotion::place_camera() {
    glLoadIdentity();

    gluLookAt(zoom * sin(alpha)*cos(beta), zoom * sin(beta), zoom * cos(alpha)*cos(beta),
              left, 0.0f, right,
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