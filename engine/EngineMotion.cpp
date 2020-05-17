#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#endif
#include <math.h>
#include <iostream>
#include <Vec3.h>
#include "EngineMotion.h"
#include <unordered_map>
#include <unordered_set>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "mathAct.h"

using namespace std;

typedef void (*EMKeyProc)();

GLfloat mode = GL_FILL; // GL_FILL, GL_LINE; GL_POINT
GLfloat speed = 0.1f;
float angle = 45;
int frustumOn = 1;
bool inFocus=false, paused=false;

// angle of rotation for the camera direction
float alpha=0.0, beta=0.0f;

// actual vector representing the camera's direction
glm::vec3 l(0.0f, 0.0f, -1.0f);

glm::vec3 p(0.0, 1.0f,5.0f);
glm::vec3 up(0.0f,1.0f,0.0f);

glm::vec3 focusV(1.0,0.0,0.0);

float range=1;

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
void apply_active_keys_focused();
void go_fill();
void go_line();
void go_point();
void move_front();
void move_back();
void move_left();
void move_right();
void move_down();
void move_up();
void inc_speed();
void dec_speed();
void reset_world();
void look_up();
void look_down();
void look_left();
void look_right();
void frustummode();
void pauseplay();

/*
###################################################################
###################################################################
###################################################################
*/

void EngineMotion::handle_ascii(unsigned char key, int , int ) {
    if(keyboardMappers.find(key) != keyboardMappers.end()) {
        keyboardActive.insert(key);

        glutPostRedisplay();
    }
}

void EngineMotion::handle_special(int key, int, int ) {
    if(specialMappers.find(key) != specialMappers.end()) {
        specialActive.insert(key);

        glutPostRedisplay();
    }
}

void EngineMotion::up_ascii(unsigned char key, int , int) {
    if(keyboardMappers.find(key) != keyboardMappers.end()) {
        keyboardActive.erase(key);

        glutPostRedisplay();
    }
}

void EngineMotion::up_special(int key, int, int ) {
    if(specialMappers.find(key) != specialMappers.end()) {
        specialActive.erase(key);

        glutPostRedisplay();
    }
}

void EngineMotion::place_camera(bool focused,float lookX,float lookY,float lookZ) {
    glLoadIdentity();
    if (focused)
    {
        if (!inFocus)
        {
            range=1;
            inFocus=true;
        }
        focusV = glm::vec3(lookX, lookY, lookZ);
        l = glm::normalize(focusV * (-1.0f));
        p = focusV * range;
        apply_active_keys_focused();
    }
    else
    {
        if (inFocus)
        {
            reset_world();
            inFocus=false;
        }
        apply_active_keys();
    }
    glm::vec3 fut = p + l;

    mt::lookat(	p, fut, up);


    glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void EngineMotion::projection_size(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratioz = w * 1.0 / h;

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    mt::perspective(angle * M_PI / 180.0f,ratioz,1.0f,1000.0f);
}

void EngineMotion::build_key_mappers() {
    keyboardMappers['1'] = go_fill;
    keyboardMappers['2'] = go_line;
    keyboardMappers['3'] = go_point;
    keyboardMappers['w'] = move_front;
    keyboardMappers['a'] = move_left;
    keyboardMappers['s'] = move_back;
    keyboardMappers['d'] = move_right;
    keyboardMappers['o'] = move_up;
    keyboardMappers['p'] = move_down;
    keyboardMappers['z'] = inc_speed;
    keyboardMappers['x'] = dec_speed;
    keyboardMappers['r'] = reset_world;
    keyboardMappers['f'] = frustummode;
    keyboardMappers[' '] = pauseplay;
}

void EngineMotion::build_special_mappers() {
    specialMappers[GLUT_KEY_UP] = look_up;
    specialMappers[GLUT_KEY_LEFT] = look_left;
    specialMappers[GLUT_KEY_DOWN] = look_down;
    specialMappers[GLUT_KEY_RIGHT] = look_right;
}

int EngineMotion::checkSysTime(int milis) {
    return milis;
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

void apply_active_keys_focused() {
    for(unsigned char key : keyboardActive) {
        switch (key) {
            case 'w':
                range -= speed/1000.0;
                break;
            case 's':
                range += speed/1000.0;
                break;

        }
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
    glm::vec3 dir = glm::normalize(glm::cross(l, up));
    p = p - (dir * speed);
}

void move_right() {
    glm::vec3 dir = glm::normalize(glm::cross(l, up));
    p = p + (dir * speed);
}

void move_down() {
    p = p - (up * speed);
}

void move_up() {
    p = p + (up * speed);
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
    speed = 0.1f;
    beta = 0.0f;
    alpha = 0.0f;
    l = glm::vec3(0.0f, 0.0f, -1.0f);
    p = glm::vec3(0.0f, 1.0f, 5.0f);
}

void look_up() {
    beta -= speed / 10;
    l = glm::vec3(sin(alpha), -sin(beta), -cos(alpha));
}

void look_down() {
    beta += speed / 10;
    l = glm::vec3(sin(alpha), -sin(beta), -cos(alpha));
}

void look_left() {
    alpha -= speed / 10;
    l = glm::vec3(sin(alpha), -sin(beta), -cos(alpha));
}

void look_right() {
    alpha += speed / 10;
    l = glm::vec3(sin(alpha), -sin(beta), -cos(alpha));
}

void frustummode(){
    frustumOn = !frustumOn;
}

void pauseplay() {
    paused = !paused;
}

void EngineMotion::setCamera(float x, float y, float z){
    p = glm::vec3(x, y, z);
}