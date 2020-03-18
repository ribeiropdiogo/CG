#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <iostream>
#include <Vec3.h>
#include "EngineMotion.h"
#include <unordered_map>
#include <unordered_set>

using namespace std;

typedef void (*EMKeyProc)();

GLfloat mode = GL_FILL; // GL_FILL, GL_LINE; GL_POINT
GLfloat speed = 0.1f;

// angle of rotation for the camera direction
float alpha=0.0, beta=0.0f;
// actual vector representing the camera's direction
Vec3 l(0.0f, 0.0f, -1.0f);
//float lx=0.0f,lz=-1.0f, ly=0.0f;
// XZ position of the camera
Vec3 p(0.0, 1.0f,5.0f);
//float x=0.0f,z=5.0f,y=1.0f;

unordered_map<unsigned char, EMKeyProc> keyboardMappers;
unordered_map<int, EMKeyProc> specialMappers;

unordered_set<unsigned char> keyboardActive;
unordered_set<int> specialActive;

/*
###################################################################
################## KEY MAPPERS ####################################
###################################################################
*/
void apply_active_keys();

void go_fill();
void go_line();
void go_point();
void move_front();
void move_back();
void move_left();
void move_right();
void inc_speed();
void dec_speed();
void reset_world();
void look_up();
void look_down();
void look_left();
void look_right();

/*
###################################################################
###################################################################
###################################################################
*/

void EngineMotion::handle_ascii(unsigned char key, int xn, int yn) {
    if(keyboardMappers.find(key) != keyboardMappers.end()) {
        cout << "down key" << endl;
        keyboardActive.insert(key);

        glutPostRedisplay();
    }
}

void EngineMotion::handle_special(int key, int xn, int yn) {
    if(specialMappers.find(key) != specialMappers.end()) {
        cout << "down key" << endl;
        specialActive.insert(key);

        glutPostRedisplay();
    }
}

void EngineMotion::up_ascii(unsigned char key, int xn, int yn) {
    if(keyboardMappers.find(key) != keyboardMappers.end()) {
        cout << "Up key" << endl;
        keyboardActive.erase(key);

        glutPostRedisplay();
    }
}

void EngineMotion::up_special(int key, int xn, int yn) {
    if(specialMappers.find(key) != specialMappers.end()) {
        cout << "Up key" << endl;
        specialActive.erase(key);

        glutPostRedisplay();
    }
}

void EngineMotion::place_camera() {
    glLoadIdentity();

    apply_active_keys();

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

void EngineMotion::build_key_mappers() {
    keyboardMappers['1'] = go_fill;
    keyboardMappers['2'] = go_line;
    keyboardMappers['3'] = go_point;
    keyboardMappers['w'] = move_front;
    keyboardMappers['a'] = move_left;
    keyboardMappers['s'] = move_back;
    keyboardMappers['d'] = move_right;
    keyboardMappers['z'] = inc_speed;
    keyboardMappers['x'] = dec_speed;
    keyboardMappers['r'] = reset_world;
}

void EngineMotion::build_special_mappers() {
    specialMappers[GLUT_KEY_UP] = look_up;
    specialMappers[GLUT_KEY_LEFT] = look_left;
    specialMappers[GLUT_KEY_DOWN] = look_down;
    specialMappers[GLUT_KEY_RIGHT] = look_right;
}

void apply_active_keys() {
    for(unsigned char key : keyboardActive) {
        auto it = keyboardMappers.find(key);
        (*it->second)();
    }

    for(int key : specialActive) {
        auto it = specialMappers.find(key);
        (*it->second)();
    }
}

void go_fill() {
    mode = GL_FILL;
}

void go_line() {
    mode = GL_LINE;
}

void go_point() {
    mode = GL_POINT;
}

void move_front() {
    p = p + (l * speed);
}

void move_back() {
    p = p - (l * speed);
}

void move_left() {
    p = p - (l.crossprod((*new Vec3(0.0f,1.0f, 0.0f))) * speed);
}

void move_right() {
    p = p + (l.crossprod((*new Vec3(0.0f,1.0f, 0.0f))) * speed);
}

void inc_speed() {
    speed += 0.1f;
}

void dec_speed() {
    speed -= 0.1f;

    if (speed <= 0.0f)
        speed = 0.1f;
}

void reset_world() {
    speed = 1.0f;
    beta = 0.0f;
    alpha = 0.0f;
    l = *(new Vec3(0.0f, 0.0f, -1.0f));
    p = *(new Vec3(0.0f, 1.0f, 5.0f));
}

void look_up() {
    beta -= speed / 10;
    l = (*new Vec3(sin(alpha), -sin(beta), -cos(alpha)));
}

void look_down() {
    beta += speed / 10;
    l = (*new Vec3(sin(alpha), -sin(beta), -cos(alpha)));
}

void look_left() {
    alpha -= speed / 10;
    l = (*new Vec3(sin(alpha), -sin(beta), -cos(alpha)));
}

void look_right() {
    alpha += speed / 10;
    l = (*new Vec3(sin(alpha), -sin(beta), -cos(alpha)));
}