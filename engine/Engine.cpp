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

EngineMotion Engine::motion;
vector<Group*> Engine::groups;
GLuint * Engine::buffers;
GLuint * Engine::indexes;
GLuint * Engine::texCoords;
GLuint * Engine::VAOs;
GLuint * Engine::VBOs;
GLuint * Engine::materials;
GLuint Engine::lights;

unsigned int skyboxVAO;
unsigned int cubemapTexture;

bool focus = false;
float lookX=0.0,lookY=0.0,lookZ=0.0;
int frame = 0, timebase = 0;
int idxFocus=-1;
int mudaCor=-1;
int timeT=0;
unsigned int * textures;

glm::vec3 * bgcolor = new glm::vec3(0.0f, 0.0f, 0.0f);

vector<Shader> progs;

vector<string> skyBoxFaces;
Shader *skyBoxShader= nullptr;

float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
};

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

Object3d Engine::newDrawing(const string& file, float * materialOptions,const string& texture){
    Object3d newObj(0);

    auto iter = loadedObj.find(file);

    if(iter != loadedObj.end()) {
        // Existe já armazenado em memoria
        newObj = (iter->second);
    }
    else {
        newObj = ObjLoader::loadFile(file);
        loadedObj.insert({file, newObj});
    }

    loadedEvents.push_back(newObj);

    return newObj;
}

void Engine::bindAllObjects() {

    for(auto elem : loadedEvents) {
        elem.bind();
    }

    if(skyBoxFaces.size() == 6) {
        unsigned int skyboxVBO;
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        cubemapTexture = TexLoader::loadCubemapTexture(skyBoxFaces);
    }
}

int Engine::runGroups(int idx, int milis, vector<Shader> progs) {
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
        mt::pushMatrix();//glPushMatrix();

        glStencilFunc(GL_ALWAYS,idx+1,-1);
        cout << "before publish " << endl;
        tmp = group->publish(VAOs, textures, materials, lights, progs, milis);
        cout << "after publish " << endl;

        for(int j = 0; j < tmp; j++) {
            nprocd += runGroups(idx + nprocd, milis, progs);
        }

        mt::popMatrix();//glPopMatrix();
    }

    return nprocd;
}

void Engine::renderScene(){
    GLuint id;
    glClearColor(bgcolor->x,bgcolor->y,bgcolor->z,1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_STENCIL_TEST);

    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);

    mt::identity();

    motion.place_camera(focus,lookX,lookY,lookZ);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, lights);

    cout << "before groups " << endl;

    runGroups(0, timeT, progs);

    cout << "after groups " << endl;

    if(skyBoxFaces.size() == 6) {
        glEnable(GL_DEPTH_CLAMP);
        id = skyBoxShader->getID();
        glDepthFunc(GL_LEQUAL);
        skyBoxShader->use();

        mt::undoViewTranslation();
        mt::bindView(id);
        mt::bindProj(id);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        glDisable(GL_DEPTH_CLAMP);
    }

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

    //glutFullScreen();

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

    glEnable(GL_CULL_FACE);
}

void Engine::loop() {

    progs.push_back(* new Shader(
            "../../resources/shaders/base.vs",
            "../../resources/shaders/base.fs"));

    if(skyBoxFaces.size() == 6) {
        skyBoxShader = new Shader("../../resources/shaders/skybox.vs",
                        "../../resources/shaders/skybox.fs");
        glUniform1i(glGetUniformLocation(skyBoxShader->getID(), "skybox"), 0);
    }

    bindAllObjects();

    glutMainLoop();
}

void Engine::close(){

}

void Engine::newGroup(){
    groups.push_back(new Group());
}

void Engine::newObj(const string& file,float * materialOptions, string texture){

    Object3d event = newDrawing(file,materialOptions,texture);

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

void Engine::appendCubeMapFace(string face) {
    string file = "../../resources/textures/skyboxes/";
    file = file.append(face);
    skyBoxFaces.push_back(file);
}

void Engine::setBackgroundColor(float r, float g, float b) {
    bgcolor = new glm::vec3(r, g, b);
}