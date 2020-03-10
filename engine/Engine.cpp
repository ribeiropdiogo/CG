#include <GL/glew.h>
#include <GL/glut.h>
#include "Engine.h"

EngineMotion Engine::motion;
vector<Group> Engine::groups;
GLuint * Engine::buffers;
GLuint * Engine::indexes;

void Engine::wrap_proj(int w, int h) {
    motion.projection_size(w, h);
}

void Engine::wrap_ascii(unsigned char key, int x, int y) {
    motion.handle_ascii(key, x, y);
}

void Engine::wrap_special(int key, int x, int y) {
    motion.handle_special(key, x, y);
}

Group Engine::latestGroup(){
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
        Object3d newObj(file);
        event = new DrawEvent(numObjs++, &newObj);
    }

    return *event;
}

void Engine::bindAllObjects() {
    Object3d * obj = nullptr;
    unsigned int idx, N = loadedEvents.size();

    buffers = (GLuint *) malloc(sizeof(GLuint) * N);
    indexes = (GLuint *) malloc(sizeof(GLuint) * N);

    glGenBuffers(N, buffers);
    glGenBuffers(N, indexes);

    for(auto elem : loadedEvents) {
        obj = elem.second.getObj();
        idx = elem.second.getBufferId();

        glBindBuffer(GL_ARRAY_BUFFER, buffers[idx]);
        glBufferData(GL_ARRAY_BUFFER, obj->getPontos().size() * sizeof(GLfloat),
                     obj->getPontos().data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes[idx]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->getIndices().size() * sizeof(GLuint),
                     obj->getIndices().data(), GL_STATIC_DRAW);
    }
}

int Engine::runGroups(int idx) {
    int tmp, nprocd = 0;

    if(idx < groups.size()) {
        nprocd = 1;

        Group group = groups[idx];

        glPushMatrix();

        tmp = group.publish(buffers, indexes);

        for(int j = 0; j < tmp; j++) {
            nprocd += runGroups(idx + nprocd);
        }

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
    glClearColor(1,1,1,1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    motion.place_camera();

    drawAxes();

    // Por aqui funcao de renderizacao.
    runGroups(0);

    glutSwapBuffers();
}

void Engine::start(int *eargc, char **argv){
    glutInit(eargc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow(WIN_NAME.c_str());

    glutDisplayFunc(renderScene);
    glutReshapeFunc(wrap_proj);
    glutKeyboardFunc(wrap_ascii);
    glutSpecialFunc(wrap_special);

    glewInit();

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
    Group g;
    groups.push_back(g);
}

void Engine::newObj(const string& file){
    DrawEvent event = newDrawing(file);

    if(!groups.empty()) {
        Group last = groups.back();
        last.pushDraw(event);
    }
}

void Engine::newTransform(TransformEvent te){
    if(!groups.empty()) {
        Group last = groups.back();
        last.pushTransformation(te);
    }
}