#include <iostream>
#include "includes/tinyxml.h"
#include "includes/tinystr.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
#include <Engine.h>

/*
GLuint * buffers;
GLuint * indexes;


int main(int argc, char **argv) {
    char * file = strdup(argv[1]);
    char * workdir = strdup("../../samples/XML/");
    workdir = (char*) realloc(workdir, strlen(workdir) + strlen(file) + 1);
    char * dir = strcat(workdir, file);
    //objs.reserve(1);

    TiXmlDocument doc(dir);
    if (!doc.LoadFile()) return 1;
    TiXmlHandle hDoc(&doc);
    TiXmlElement *pElem;
    TiXmlHandle hRoot(0);
    pElem = hDoc.FirstChildElement().Element();
    if (!pElem) return 1;
    const char *string = pElem->Value();
    hRoot = TiXmlHandle(pElem);

// init GLUT and the window

// Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(handle_rotation);

    glewInit();

//  OpenGL settings
   glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_CULL_FACE);

// Read Objects
    if (strcmp(string, "scene") == 0) {
        pElem = hRoot.FirstChildElement().Element();
        for (pElem; pElem; pElem = pElem->NextSiblingElement()) {
            objs.resize(n+1);
            objs[n].loadObject(n, (char *) pElem->Attribute("file"));
            n++;
        }
        //printf("%p %p",objs[0].getIndices().data(),objs[1].getIndices().data());
        buffers = (GLuint *) malloc(sizeof(GLuint) * n);
        indexes = (GLuint *) malloc(sizeof(GLuint) * n);
        glGenBuffers(n,buffers);
        glGenBuffers(n,indexes);
        for (int i=0;i<n;i++) {
            glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
            glBufferData(GL_ARRAY_BUFFER, objs[i].getPontos().size() * sizeof(GLfloat),
                         objs[i].getPontos().data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, objs[i].getIndices().size() * sizeof(GLuint),
                         objs[i].getIndices().data(), GL_STATIC_DRAW);
        }
    }

// enter GLUT's main cycle
    glutMainLoop();
        return 1;

    }*/

int main(int argc, char ** argv) {
    Engine e;

    e.start(&argc, argv);

    e.newGroup();

    e.newTransform(*(new TransformEvent(1,1,0,0,45)));

    e.newObj("loles2.3d");

    e.newGroup();

    e.addSubgroup(0);

    e.newTransform(*(new TransformEvent(2,2,0,0)));

    e.newObj("loles2.3d");

    e.newGroup();

    e.addSubgroup(0);

    e.newTransform(*(new TransformEvent(2,-2,0,0)));

    e.newObj("loles2.3d");


    e.loop();

    return 0;
}