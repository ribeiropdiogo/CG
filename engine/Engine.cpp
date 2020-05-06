#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "Engine.h"
#include "IL/il.h"

EngineMotion Engine::motion;
vector<Group*> Engine::groups;
GLuint * Engine::buffers;
GLuint * Engine::indexes;
GLuint * Engine::texCoords;

bool focus = false;
float lookX=0.0,lookY=0.0,lookZ=0.0;
int frame = 0, timebase = 0;
int idxFocus=-1;
int mudaCor=-1;
int timeT=0;
unsigned int * textures;

void Engine::wrap_proj(int w, int h) {
    motion.projection_size(w, h);
}

void Engine::wrap_ascii(unsigned char key, int x, int y) {
    if (key == '.')
        focus=false;
    else
    motion.handle_ascii(key, x, y);
}

void Engine::wrap_special(int key, int x, int y) {
    motion.handle_special(key, x, y);
}

void Engine::wrap_up_ascii(unsigned char key, int x, int y) {
    motion.up_ascii(key, x, y);
}

void Engine::wrap_up_special(int key, int x, int y) {
    motion.up_special(key, x, y);
}

DrawEvent Engine::newDrawing(const string& file, int r, int g, int b,
        float diffR, float diffG, float diffB, const string& texture){
    DrawEvent * event = nullptr;
    auto iter = loadedEvents.find(file);

    if(iter != loadedEvents.end()) {
        // Existe já armazenado em memoria
        event = &(iter->second);
    }
    else {
        // Não existe em memoria;
        Object3d newObj;
        newObj.loadObject(file);
        event = new DrawEvent(numObjs++, newObj, r, g, b, diffR, diffG, diffB, texture);
        loadedEvents.insert({file, *event});
    }

    return *event;
}

void Engine::loadTexture(int idx, string texture_name, GLuint *textures){
    if(!texture_name.empty()) {
        unsigned int t, tw, th;
        unsigned char *texData;
        ilGenImages(1, &t);
        ilBindImage(t);

        ilLoadImage((ILstring) texture_name.c_str());
        tw = ilGetInteger(IL_IMAGE_WIDTH);
        th = ilGetInteger(IL_IMAGE_HEIGHT);
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        texData = ilGetData();

        glBindTexture(GL_TEXTURE_2D, textures[idx]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Engine::bindAllObjects() {
    unsigned int idx, N = loadedEvents.size();
    buffers = (GLuint *) malloc(sizeof(GLuint) * N);
    indexes = (GLuint *) malloc(sizeof(GLuint) * N);
    texCoords = (GLuint *) malloc(sizeof(GLuint) * N);
    textures = (unsigned int *) malloc(sizeof(unsigned int) * N);


    glGenBuffers(N, buffers);
    glGenBuffers(N, indexes);
    glGenBuffers(N, texCoords);

    ;
    glGenTextures(N, textures);

    for(auto elem : loadedEvents) {
        Object3d obj = elem.second.getObj();
        idx = elem.second.getBufferId();

        loadTexture(idx, elem.second.getTexture(), textures);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[idx]);
        glBufferData(GL_ARRAY_BUFFER, obj.getPontos().size() * sizeof(GLfloat),
                     obj.getPontos().data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes[idx]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.getIndices().size() * sizeof(GLuint),
                     obj.getIndices().data(), GL_STATIC_DRAW);
        /*
        glBindBuffer(GL_ARRAY_BUFFER, normals[idx]);
        glBufferData(GL_ARRAY_BUFFER, obj.getNormals().size() * sizeof(GLuint),
                obj.getNormals().data(), GL_STATIC_DRAW);
                */

        glBindBuffer(GL_ARRAY_BUFFER, texCoords[idx]);
        glBufferData(GL_ARRAY_BUFFER, obj.getTexCoords().size() * sizeof(GLuint),
                obj.getTexCoords().data(), GL_STATIC_DRAW);
    }
}

int Engine::runGroups(int idx, int milis, float *viewMatrix) {
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
        glPushMatrix();

        glStencilFunc(GL_ALWAYS,idx+1,-1);

        tmp = group->publish(buffers, indexes, texCoords, textures, milis, viewMatrix);

        for(int j = 0; j < tmp; j++) {
            nprocd += runGroups(idx + nprocd, milis, viewMatrix);
        }

        glPopMatrix();
    }

    return nprocd;
}

void Engine::renderScene(){
    float viewMatrix[16];
    //glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);

    glLoadIdentity();
    motion.place_camera(focus,lookX,lookY,lookZ);
    glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
    glLoadIdentity();
    //drawAxes();

    runGroups(0, timeT, viewMatrix);

    glLoadMatrixf(viewMatrix);
    for(int i = 0; i < groups.size(); i++) {
        groups[i]->drawTracing();
    }

    float lpos[4] = { 4,4,4,0 };
    //glLightfv(GL_LIGHT0, GL_POSITION, lpos);


    glutSwapBuffers();
}

void Engine::idleFunc()
{
    char title[100];
    float fps;
    frame++;
    timeT = motion.checkSysTime(glutGet(GLUT_ELAPSED_TIME));
    if (timeT - timebase > 1000){
        fps = frame*1000.0/(timeT-timebase);
        timebase = timeT;
        frame = 0;
        sprintf(title,"FPS: %8.2f",fps);//motion.getFrustumState(),fps);
        glutSetWindowTitle(title);
    }
    if (focus){
        groups[idxFocus]->adjustCenter(groups,timeT);
        lookX=groups[idxFocus]->getCenterX();
        lookY=groups[idxFocus]->getCenterY();
        lookZ=groups[idxFocus]->getCenterZ();}
    glutPostRedisplay();
}

void Engine::processMouseButtons(int button, int state, int xx, int yy) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            int window_width = glutGet(GLUT_WINDOW_WIDTH);
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
                groups[idxFocus]->adjustCenter(groups,motion.checkSysTime(glutGet(GLUT_ELAPSED_TIME)));
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
    glutInit(eargc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow(WIN_NAME.c_str());

    glutFullScreen();

    motion.build_key_mappers();
    motion.build_special_mappers();

    glutDisplayFunc(renderScene);
    glutIdleFunc(idleFunc);
    glutReshapeFunc(wrap_proj);
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(wrap_ascii);
    glutSpecialFunc(wrap_special);
    glutMouseFunc(processMouseButtons);
    glutKeyboardUpFunc(wrap_up_ascii);
    glutSpecialUpFunc(wrap_up_special);

    #ifndef __APPLE__
        glewInit();
    #endif

    ilInit();
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glEnableClientState(GL_NORMAL_ARRAY);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void Engine::loop() {
    bindAllObjects();

    glutMainLoop();
}

void Engine::close(){

}

void Engine::newGroup(){
    groups.push_back(new Group());
}

void Engine::newObj(const string& file, int r, int g, int b,
        float diffR, float diffG, float diffB, string texture){

    DrawEvent event = newDrawing(file,r,g,b,diffR,diffG,diffB,texture);

    if(!groups.empty()) {
        Group * last = groups.back();
        last->pushDraw(event);
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
    motion.setCamera(x,y,z);
}