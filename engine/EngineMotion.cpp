#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <iostream>
#include "EngineMotion.h"

GLfloat mode = GL_FILL; // GL_FILL, GL_LINE; GL_POINT
GLfloat alpha = 0.0f;
GLfloat speed = 1.0f;

GLfloat px = 0.0f;//raio * sin(alpha)*cos(beta);
GLfloat py = 0.0f;//raio * sin(beta);
GLfloat pz = 5.0f;//raio * cos(alpha)*cos(beta);

GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat z = 0.0f;

GLfloat upX = 0.0f;
GLfloat upY = 1.0f;
GLfloat upZ = 0.0f;

GLfloat tx = 0.0f;
GLfloat ty = 0.0f;
GLfloat tz = 0.0f;




void EngineMotion::rotate(){
}

void EngineMotion::handle_ascii(unsigned char key, int xn, int yn) {
    float fraction = M_PI/100;
    float rotation = M_PI/4;
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
            tz += fraction * speed;
            break;
        case 's':
            tz -= fraction * speed;
            break;
        case 'd':
            tx -= fraction * speed;
            break;
        case 'a':
            tx += fraction * speed;
            break;
        case 'q':
            alpha += rotation * speed;
            break;
        case 'e':
            alpha -= rotation * speed;
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
            alpha = tx = ty = tz = 0.0f;
            break;

        default:;
    }

    glutPostRedisplay();
}

void EngineMotion::handle_special(int key, int xn, int yn) {
    float fraction = M_PI/100;
    switch (key) {
        case GLUT_KEY_LEFT :
            break;
        case GLUT_KEY_RIGHT :
            break;
        case GLUT_KEY_UP :
            ty -= fraction*speed;
            break;
        case GLUT_KEY_DOWN :
            ty += fraction*speed;
            break;
    }


    glutPostRedisplay();
    }

void EngineMotion::place_camera() {
    glLoadIdentity();

    gluLookAt(px, py, pz, // onde esta a camara
              x, y, z,    // para onde estas a olhar
              upX,upY,upZ); // vetor up

    glPolygonMode(GL_FRONT_AND_BACK, mode);

    glTranslatef(tx,ty,tz);
    glRotatef(alpha,0.0f,1.0f,0.0f);
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