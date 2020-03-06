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
#include <vector>

using namespace std;

class Object3d {
private:
    int numVertices;
    vector<GLfloat> points;
    vector<GLuint> index;
    vector<GLfloat> * pontos = &points;
    vector<GLuint> * indices = &index;
public:
    /* A função loadObject começa por ir buscar
    o ficheiro que contem a informação dos vertices
    (é utilizada a concatenação com a string ../ para que se possam
    por os ficheiros dentro da pasta parser e não dentro da pasta cmake-build-debug*/
    void loadObject(char * filePath)
    {
        char * string = strdup(filePath);
        char * workdir = strdup("../../samples/3D/");
        workdir = (char*) realloc(workdir, strlen(workdir) + strlen(string) + 1);
        char * useThis = strcat(workdir,string);
        float x,y,z;
        unsigned int tempI;
        ifstream inFile(useThis);
        inFile >> numVertices;
        /* alocado estado para um array de floats grande o suficiente para gerir todos os vertices do objeto */
        for (int i=0;i<numVertices;i++)
        {
            inFile >> x >> y >> z;
            pontos->push_back(x);
            pontos->push_back(y);
            pontos->push_back(z);
        }
        while (!inFile.eof())
        {
            inFile >> tempI;
            indices->push_back(tempI);
        }
        inFile.close();
    }

    // Função simples para eliminar o objeto
    void destroyObject()
    {
        numVertices=0;
        pontos->clear();
        indices->clear();
    }

    vector<GLfloat> * getPontos()
    {
        return pontos;
    }
    vector<GLuint> * getIndices()
    {
        return indices;
    }
};


#endif //TP_OBJECT3D_H
