#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "Engine.h"

EngineMotion Engine::motion;
vector<Group*> Engine::groups;
GLuint * Engine::buffers;
GLuint * Engine::indexes;


int frame = 0, timebase = 0;
int mudaCor=-123;

void Engine::wrap_proj(int w, int h) {
    motion.projection_size(w, h);
}

void Engine::wrap_ascii(unsigned char key, int x, int y) {
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

Group * Engine::latestGroup(){
    return groups.back();
}

DrawEvent Engine::newDrawing(const string& file){
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
        event = new DrawEvent(numObjs++, newObj);
        loadedEvents.insert({file, *event});
    }

    return *event;
}

void Engine::bindAllObjects() {
    unsigned int idx, N = loadedEvents.size();
    buffers = (GLuint *) malloc(sizeof(GLuint) * N);
    indexes = (GLuint *) malloc(sizeof(GLuint) * N);

    glGenBuffers(N, buffers);
    glGenBuffers(N, indexes);

    for(auto elem : loadedEvents) {
        Object3d obj = elem.second.getObj();
        idx = elem.second.getBufferId();

        glBindBuffer(GL_ARRAY_BUFFER, buffers[idx]);
        glBufferData(GL_ARRAY_BUFFER, obj.getPontos().size() * sizeof(GLfloat),
                     obj.getPontos().data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes[idx]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.getIndices().size() * sizeof(GLuint),
                     obj.getIndices().data(), GL_STATIC_DRAW);
    }
}

int Engine::runGroups(int idx, int milis) {
    int tmp, nprocd = 0;

    int r=0,g=0,b=255;
    if(idx < groups.size()) {
        nprocd = 1;
        if (idx==mudaCor)
        {
            r=255;
            g=0;
            b=0;
        }
        glColor3ub(r,g,b);

        Group * group = groups[idx];
        glPushMatrix();



        //if (!motion.getFrustumState() || (motion.getGeometricFrustum().sphereInFrustum(a,0.5) != GeometricFrustum::OUTSIDE)) {

        glStencilFunc(GL_ALWAYS,idx+1,-1);
        tmp = group->publish(buffers, indexes, milis);

        for(int j = 0; j < tmp; j++) {
            nprocd += runGroups(idx + nprocd, milis);
        }
        //}
        glPopMatrix();
    }

    return nprocd;
}

void Engine::drawAxes(){
    glBegin(GL_LINES);
    // X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f( 100.0f, 0.0f, 0.0f);
    // Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    // Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
}

void Engine::renderScene(){

    char title[100];
    float fps,time;

    glClearColor(1,1,1,1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    motion.place_camera();
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    drawAxes();
    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000){
        fps = frame*1000.0/(time-timebase);
        timebase = time;
        frame = 0;
        sprintf(title,"Frustum: %d | FPS: %8.2f",motion.getFrustumState(),fps);
        glutSetWindowTitle(title);
    }


    // Por aqui funcao de renderizacao.
    runGroups(0, glutGet(GLUT_ELAPSED_TIME));



    glutSwapBuffers();
}

void Engine::processMouseButtons(int button, int state, int xx, int yy) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_MIDDLE_BUTTON) {
            int window_width = glutGet(GLUT_WINDOW_WIDTH);
            int window_height = glutGet(GLUT_WINDOW_HEIGHT);

            GLbyte color[4];
            GLfloat depth;
            GLuint index;

            glReadPixels(xx, window_height - yy - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
            glReadPixels(xx, window_height - yy - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
            glReadPixels(xx, window_height - yy - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

            printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
                   xx, yy, color[0], color[1], color[2], color[3], depth, index);
            mudaCor = index - 1;
        }
    }
}

void Engine::start(int *eargc, char **argv){
    glutInit(eargc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow(WIN_NAME.c_str());

    //glutFullScreen();

    motion.build_key_mappers();
    motion.build_special_mappers();

    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
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

    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_CULL_FACE);
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

void Engine::newObj(const string& file){
    DrawEvent event = newDrawing(file);

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
