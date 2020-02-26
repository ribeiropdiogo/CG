//
// Created by ruimendes on 26/02/20.
//

#ifndef TP_OBJECT3D_H
#define TP_OBJECT3D_H


#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <GL/glut.h>

using namespace std;

class Object3d {
private:
    int numVertices;
    float * pontos;
public:
    void loadObject(char * filePath)
    {
        char * string = strdup(filePath);
        char * useThis = strcat(strdup("../"),string);
        ifstream inFile(useThis);
        inFile >> numVertices;
        pontos = (float *) malloc(sizeof(float)*3*numVertices);
        for (int i=0;i<numVertices*3;i+=3)
            inFile >> pontos[i] >> pontos[i+1] >> pontos[i+2];
        inFile.close();
    }
    void destroyObject()
    {
        numVertices=0;
        free(pontos);
    }

    void drawObject() {
        glPolygonMode(GL_FRONT,GL_FILL);
        glBegin(GL_TRIANGLES);
        for (int i=0;i<numVertices*3;i+=3)
            glVertex3f(pontos[i],pontos[i+1],pontos[i+2]);
        glEnd();
    }
};


#endif //TP_OBJECT3D_H
