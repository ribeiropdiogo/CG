#ifdef __APPLE__
#include <GL/freeglut.h>
#else
#include <GL/glew.h>
#endif
#include "Engine.h"
#include <IL/il.h>
#include <Shader.h>
#include <GL/freeglut.h>
#include <mathAct.h>
#include <unordered_map>
#include <TexLoader.h>
#include "ObjLoader.h"
#include "lightSystem.h"

vector<Group*> Engine::groups;

bool focus = false;
float lookX=0.0,lookY=0.0,lookZ=0.0;
int frame = 0, timebase = 0;
int idxFocus=-1;
int timeT=0;

glm::vec3 * bgcolor = new glm::vec3(0.0f, 0.0f, 0.0f);

Shader *normalShader = nullptr;

Object3d* Engine::newDrawing(const string& file, const string& texture, Material material){
    Object3d *newObj = nullptr;
    GLuint id_tex = 0;
    string key = "(" + file + "," + texture + ")";

    auto iter = loadedObj.find(key);

    if(iter != loadedObj.end()) {
        newObj = &(iter->second);
    }
    else {
        id_tex = TexLoader::loadTexture(texture.c_str());
        newObj = ObjLoader::loadFile(file, id_tex, material);

        loadedObj.insert({key, *newObj});
    }

    loadedEvents.push_back(*newObj);

    return newObj;
}

void Engine::bindAllObjects() {
    lightSystem::bind();

    for(auto elem : loadedEvents) {
        elem.bind();
    }

    TexLoader::bindSkybox();
}

int Engine::runGroups(int idx, int milis) {
    int tmp, nprocd = 0;
    int r=0,g=0,b=255;
    if(idx < groups.size()) {
        nprocd = 1;
        if (idxFocus==idx && focus)
        {
            r=255;
            b=0;
        }
        glColor3ub(r,g,b);

        Group * group = groups[idx];
        mt::pushMatrix();

        glStencilFunc(GL_ALWAYS,idx+1,-1);

        tmp = group->publish(*normalShader, milis);

        for(int j = 0; j < tmp; j++) {
            nprocd += runGroups(idx + nprocd, milis);
        }

        mt::popMatrix();
    }

    return nprocd;
}

void Engine::renderScene(){
    glClearColor(bgcolor->x,bgcolor->y,bgcolor->z,1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_STENCIL_TEST);

    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);

    mt::identity();

    EngineMotion::place_camera(focus,lookX,lookY,lookZ);

    lightSystem::activate(normalShader->getID());

    runGroups(0, timeT);

    TexLoader::renderSkybox();

    glutSwapBuffers();
}

void Engine::idleFunc()
{
    char title[100];
    float fps;
    frame++;
    timeT = EngineMotion::checkSysTime(glutGet(GLUT_ELAPSED_TIME));
    if (timeT - timebase > 1000){
        fps = frame*1000.0/(timeT-timebase);
        timebase = timeT;
        frame = 0;
        sprintf(title,"FPS: %8.2f",fps);
        glutSetWindowTitle(title);
    }
    if (focus){
        groups[idxFocus]->adjustCenter(groups,timeT);
        lookX=groups[idxFocus]->getCenterX();
        lookY=groups[idxFocus]->getCenterY();
        lookZ=groups[idxFocus]->getCenterZ();
    }
    glutPostRedisplay();
}

void Engine::processMouseButtons(int button, int state, int xx, int yy) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            int window_height = glutGet(GLUT_WINDOW_HEIGHT);

            GLbyte color[4];
            GLfloat depth;
            GLuint index;

            glReadPixels(xx, window_height - yy - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
            glReadPixels(xx, window_height - yy - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
            glReadPixels(xx, window_height - yy - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
            if (index > 0)
            {
                idxFocus=index-1;
                groups[idxFocus]->adjustCenter(groups,timeT);
                printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u,center x %f,center y %f,center z %f\n",
                       xx, yy, color[0], color[1], color[2], color[3], depth, index,groups[idxFocus]->getCenterX(),groups[idxFocus]->getCenterY(),groups[idxFocus]->getCenterZ());
                focus= true;
                lookX=groups[idxFocus]->getCenterX();
                lookY=groups[idxFocus]->getCenterY();
                lookZ=groups[idxFocus]->getCenterZ();
            }
        }
    }
}

void Engine::start(int *eargc, char **argv){
    lightSystem::setup();
    glutInit(eargc, argv);

    glutInitContextVersion(4, 0);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow(WIN_NAME.c_str());

    glutFullScreen();

    EngineMotion::build_key_mappers();
    EngineMotion::build_special_mappers();

    glutDisplayFunc(renderScene);
    glutIdleFunc(idleFunc);
    glutReshapeFunc(EngineMotion::projection_size);
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(EngineMotion::handle_ascii);
    glutSpecialFunc(EngineMotion::handle_special);
    glutMouseFunc(processMouseButtons);
    glutKeyboardUpFunc(EngineMotion::up_ascii);
    glutSpecialUpFunc(EngineMotion::up_special);

    #ifndef __APPLE__
        glewInit();
    #endif

    ilInit();
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
}

void Engine::loop() {
    normalShader = new Shader(
            "../../resources/shaders/base.vs",
            "../../resources/shaders/base.fs");

    bindAllObjects();

    glutMainLoop();
}

void Engine::newGroup(){
    groups.push_back(new Group());
}

void Engine::newObj(const string& file, string texture, Material material){
    Object3d * event = newDrawing(file, texture, material);

    if(!groups.empty()) {
        Group * last = groups.back();
        last->pushDraw(*event);
    }
}

void Engine::newTransform(TransformEvent te){
    if(!groups.empty()) {
        Group * last = groups.back();
        last->pushTransformation(te);
    }
}

void Engine::addSubgroup(int group) {
    groups[group]->addSubgroup();
}

vector<int> Engine::addUpgroup(int upGroup,int group) {
    groups[group]->setUpgroup(upGroup,groups[upGroup]->getUpgroup());
    return groups[group]->getUpgroup();
}


void Engine::initialCamera(float x, float y, float z){
    EngineMotion::setCamera(x,y,z);
}

void Engine::appendCubeMapFace(string face) {
    TexLoader::addSkyboxFace(face);
}

void Engine::setBackgroundColor(float r, float g, float b) {
    bgcolor = new glm::vec3(r, g, b);
}