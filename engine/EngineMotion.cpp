#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <iostream>
#include <Vec3.h>
#include "EngineMotion.h"

GLfloat mode = GL_FILL; // GL_FILL, GL_LINE; GL_POINT
GLfloat speed = 1.0f;

// angle of rotation for the camera direction
float alpha=0.0, beta=0.0f;
// actual vector representing the camera's direction
Vec3 l(0.0f, 0.0f, -1.0f);
//float lx=0.0f,lz=-1.0f, ly=0.0f;
// XZ position of the camera
Vec3 p(0.0, 1.0f,5.0f);
//float x=0.0f,z=5.0f,y=1.0f;

void EngineMotion::handle_ascii(unsigned char key, int xn, int yn) {
    float fraction = 0.5f;
    float rotation = M_PI/100;
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
            p = p + (l * fraction * speed);
            break;
        case 's':
            p = p - (l * fraction * speed);
            break;
        case 'd':
            //x += lx * fraction;
            p = p + (l.crossprod((*new Vec3(0.0f,1.0f, 0.0f))) * fraction * speed);
            break;
        case 'a':
            //x -= lx*fraction;
            p = p - (l.crossprod((*new Vec3(0.0f,1.0f, 0.0f))) * fraction * speed);
            break;
        case 'z':
            speed -= 0.1f;
            if (speed <= 0.0f)
                speed = 0.1f;
            break;
        case 'x':
            speed += 0.1f;
            break;
        case 'r':
            speed = 1.0f;
            beta = 0.0f;
            l = *(new Vec3(0.0f, 0.0f, -1.0f));
            p = *(new Vec3(0.0f, 1.0f, 5.0f));
            break;

        default:;
    }

    glutPostRedisplay();
}

void EngineMotion::handle_special(int key, int xn, int yn) {
    float fraction = M_PI/100;
    switch (key) {
        case GLUT_KEY_LEFT :
            alpha -= fraction * speed;
            break;
        case GLUT_KEY_RIGHT :
            alpha += fraction * speed;
            break;
        case GLUT_KEY_UP :
            beta -= fraction * speed;
            break;
        case GLUT_KEY_DOWN :
            beta += fraction * speed;
            break;
    }

    l = (*new Vec3(sin(alpha), -sin(beta), -cos(alpha)));

    glutPostRedisplay();
}

void EngineMotion::place_camera() {
    glLoadIdentity();

    Vec3 fut = p + l;

    gluLookAt(	p.getX(), p.getY(), p.getZ(),
                  fut.getX(), fut.getY(),  fut.getZ(),
                  0.0f, 1.0f,  0.0f);

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