#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <iostream>
#include "EngineMotion.h"

GLfloat mode = GL_FILL; // GL_FILL, GL_LINE; GL_POINT
GLfloat speed = 1.0f;

GLfloat tx = 0.0f;
GLfloat ty = 0.0f;
GLfloat tz = 0.0f;

// angle of rotation for the camera direction
float alpha=0.0, beta=0.0f;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f, ly=0.0f;
// XZ position of the camera
float x=0.0f,z=5.0f,y=1.0f;



void EngineMotion::rotate(){
}

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
            x += lx * fraction * speed;
            z += lz * fraction * speed;
            y += ly * fraction * speed;
            break;
        case 's':
            x -= lx * fraction * speed;
            z -= lz * fraction * speed;
            y -= ly * fraction * speed;
            break;
        case 'd':
            //x += lx * fraction;
            break;
        case 'a':
            //x -= lx*fraction;
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
            beta = tx = ty = tz = 0.0f;
            lx=0.0f;
            lz=-1.0f;
            ly=0.0f;
            x=0.0f;z=5.0f;y=1.0f;
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

    lx = sin(alpha);
    lz = -cos(alpha);
    ly = -sin(beta);

    glutPostRedisplay();
}

void EngineMotion::place_camera() {
    glLoadIdentity();

    gluLookAt(	x, y, z,
                  x+lx, y+ly,  z+lz,
                  0.0f, 1.0f,  0.0f);

    glPolygonMode(GL_FRONT_AND_BACK, mode);

    /*glTranslatef(0,0,tz);

    glRotatef(-beta, 1.0f, 0.0f, 0.0f);

    glTranslatef(tx,0,0);*/


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