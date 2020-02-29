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
    /* A função loadObject começa por ir buscar
    o ficheiro que contem a informação dos vertices
    (é utilizada a concatenação com a string ../ para que se possam
    por os ficheiros dentro da pasta parser e não dentro da pasta cmake-build-debug*/
    void loadObject(char * filePath)
    {
        char * string = strdup(filePath);
        char * useThis = strcat(strdup("../"),string);
        ifstream inFile(useThis);
        inFile >> numVertices;
        /* alocado estado para um array de floats grande o suficiente para gerir todos os vertices do objeto */
        pontos = (float *) malloc(sizeof(float)*3*numVertices);
        for (int i=0;i<numVertices*3;i+=3)
            inFile >> pontos[i] >> pontos[i+1] >> pontos[i+2];
        inFile.close();
    }

    // Função simples para eliminar o objeto
    void destroyObject()
    {
        numVertices=0;
        free(pontos);
    }

    // Função que desenha o objeto baseado nos vértices presentes no array pontos
    void drawObject() {
        glPolygonMode(GL_FRONT,GL_FILL);
        glBegin(GL_TRIANGLES);
        for (int i=0;i<numVertices*3;i+=3)
            glVertex3f(pontos[i],pontos[i+1],pontos[i+2]);
        glEnd();
    }
};


#endif //TP_OBJECT3D_H
