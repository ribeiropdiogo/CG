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

#define MAX_LIGHT_UNITS 20

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

unordered_map<string, GLuint> loadedTextures;

float bruh[16] = {
        0.5f, 0.0, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
        1.0, 1.0f, 1.0f, 1.0f, 0.0f
};

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

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2

int usedLights = 0;

typedef struct light {
    // Color components
    float diffuse[4];
    float ambient[4] ;
    float specular[4];

    // extrinsic light props
    float position[4] = {0.0, 0.0, 0.0};
    float direction[4] = {0.0, 0.0, 0.0,0.0};
    int isOn;
    int type;

    float emissionAngle = M_PI_2;

    // Attenuation of light
    float att_constant;
    float att_linear;
    float att_quadratic;
    // Just to conform to layout std140.
    float trash[2];
} Light;

Light lighting[MAX_LIGHT_UNITS];

void setupColorComponents(glm::vec3 diffuse,
        glm::vec3 ambient, glm::vec3 specular) {
    int i;
    Light* light = lighting + usedLights;
    for(i = 0; i < 3; i++) {
        light->diffuse[i] = diffuse[i];
        light->ambient[i] = ambient[i];
        light->specular[i] = specular[i];
    }
    light->diffuse[i] = 1;
    light->ambient[i] = 1;
    light->specular[i] = 1;
}

void setupAttenuation(Light * light, float att_constant, float att_linear, float att_quadratic) {
    light->att_constant = att_constant;
    light->att_linear = att_linear;
    light->att_quadratic = att_quadratic;
}

void Engine::addPointLight(glm::vec3 position, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular
                , float att_constant, float att_linear, float att_quadratic) {
    Light* light = lighting + usedLights;
    //lighting[usedLights].isOn = 1;
    light->isOn = 1;
    //cout << light.isOn << endl;
    light->type = POINT_LIGHT;
    setupColorComponents(diffuse, ambient, specular);
    for(int i = 0; i < 3; i++) {
        light->position[i] = position[i];
    }
    light->position[3] = 1;
    setupAttenuation(light, att_constant, att_linear, att_quadratic);
    usedLights++;
}

void Engine::addDirectionalLight(glm::vec3 direction, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular
                , float att_constant, float att_linear, float att_quadratic) {
    Light* light = lighting + usedLights;

    light->isOn = 1;
    light->type = DIRECTIONAL_LIGHT;
    setupColorComponents(diffuse, ambient, specular);
    for(int i = 0; i < 3; i++) {
        light->direction[i] = direction[i];
    }
    light->direction[3] = 1;
    setupAttenuation(light, att_constant, att_linear, att_quadratic);
    usedLights++;
}

void Engine::addSpotLight(glm::vec3 position, glm::vec3 direction,
        glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular, float emissionAngle
        , float att_constant, float att_linear, float att_quadratic) {
    Light* light = lighting + usedLights;

    light->isOn = 1;
    light->type = SPOT_LIGHT;
    setupColorComponents(diffuse, ambient, specular);
    for(int i = 0; i < 3; i++) {
        light->position[i] = position[i];
        light->direction[i] = direction[i];
    }
    light->position[3] = 1;
    light->direction[3] = 1;
    light->emissionAngle = emissionAngle;
    setupAttenuation(light, att_constant, att_linear, att_quadratic);
    usedLights++;
}

void Engine::setupLights() {
    // Initially all lights are turned off.
    for(int i = 0; i < MAX_LIGHT_UNITS; i++) {
        lighting[i].isOn = 0;
    }

}

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

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        ilLoadImage((ILstring)faces[i].c_str());
        width = ilGetInteger(IL_IMAGE_WIDTH);
        height = ilGetInteger(IL_IMAGE_HEIGHT);
        ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
        unsigned char *data = ilGetData();
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

DrawEvent Engine::newDrawing(const string& file, float * materialOptions,const string& texture){
    Object3d * newObj = nullptr;
    DrawEvent * event = nullptr;

    auto iter = loadedObj.find(file);

    if(iter != loadedObj.end()) {
        // Existe já armazenado em memoria
        newObj = &(iter->second);
    }
    else {
        newObj = new Object3d();
        newObj->loadObject(file);
        loadedObj.insert({file, *newObj});
    }

    event = new DrawEvent(numObjs++, *newObj, materialOptions, texture);
    loadedEvents.push_back(*event);

    return *event;
}

void Engine::loadTexture(int idx, string texture_name, GLuint *textures){
    if(!texture_name.empty()) {
        if(loadedTextures.find(texture_name) == loadedTextures.end()) {
            // Doesnt exist any texture by that name.
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
            loadedTextures[texture_name] = textures[idx];
        }
        else {
            // If a texture with that name already exists.
            textures[idx] = loadedTextures[texture_name];
        }
        }

}


void Engine::bindLights() {
    glBindBuffer(GL_UNIFORM_BUFFER, lights);

    glBufferData(GL_UNIFORM_BUFFER, MAX_LIGHT_UNITS*sizeof(Light), lighting, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Engine::bindMaterials() {
    GLuint N = loadedEvents.size();
    for(int i = 0; i < N; i++) {
        float * tmp = loadedEvents[i].getMaterialOptions();

        //unsigned int block_index = glGetUniformBlockIndex(progs[0].getID(), "Materials");

        //glUniformBlockBinding(progs[0].getID(), block_index, i);
        glBindBuffer(GL_UNIFORM_BUFFER, materials[i]);

        glBufferData(GL_UNIFORM_BUFFER, 13*sizeof(float),tmp, GL_DYNAMIC_DRAW);
        //glBindBufferBase(GL_UNIFORM_BUFFER, 1, materials[i]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

void Engine::bindAllObjects() {
    float *vertices;
    GLuint blockIndex, idx, N = loadedEvents.size();
    VAOs = (GLuint *) malloc(sizeof(GLuint)*N);
    VBOs = (GLuint *) malloc(sizeof(GLuint)*N);
    textures = (GLuint *) malloc(sizeof(GLuint)*N);
    indexes = (GLuint *) malloc(sizeof(GLuint)*N);
    materials = (GLuint *) malloc(sizeof(GLuint)*N);

    glGenVertexArrays(N, VAOs);
    glGenBuffers(N, VBOs);
    glGenBuffers(N, indexes);
    glGenBuffers(N, materials);
    glGenBuffers(1, &lights);
    glGenTextures(N, textures);


    // Bind all lights.
    bindLights();

    // Bind all object materials.
    bindMaterials();

    for(auto elem : loadedEvents) {
        Object3d obj = elem.getObj();
        idx = elem.getBufferId();
        vector<GLfloat> tmp = obj.getPontos();
        vertices = tmp.data();

        glBindVertexArray(VAOs[idx]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[idx]);
        glBufferData(GL_ARRAY_BUFFER, tmp.size() * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

        //glBindBuffer(GL_UNIFORM_BUFFER, lights);
        //glBufferData(GL_UNIFORM_BUFFER, MAX_NUM_LIGHTS * )

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes[idx]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.getIndices().size() * sizeof(GLuint),
                     obj.getIndices().data(), GL_STATIC_DRAW);

        loadTexture(idx, elem.getTexture(), textures);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
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
        cubemapTexture = loadCubemap(skyBoxFaces);
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

        tmp = group->publish(VAOs, textures, materials, lights, progs, milis);

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

    mt::identity();//    glLoadIdentity();

    motion.place_camera(focus,lookX,lookY,lookZ);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, lights);

    runGroups(0, timeT, progs);

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
        //groups[idxFocus]->adjustCenter(groups,timeT);
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
                //groups[idxFocus]->adjustCenter(groups,motion.checkSysTime(glutGet(GLUT_ELAPSED_TIME)));
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
    setupLights();
    glutInit(eargc, argv);

    glutInitContextVersion(4, 0);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_LIGHT0);

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

    //skyBoxShader->use();

    bindAllObjects();

    glutMainLoop();
}

void Engine::close(){

}

void Engine::newGroup(){
    groups.push_back(new Group());
}

void Engine::newObj(const string& file,float * materialOptions, string texture){

    DrawEvent event = newDrawing(file,materialOptions,texture);

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