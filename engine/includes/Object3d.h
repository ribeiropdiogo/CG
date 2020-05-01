//
// Created by ruimendes on 26/02/20.
//

#ifndef TP_OBJECT3D_H
#define TP_OBJECT3D_H


#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>

using namespace std;

class Object3d {
private:
    int numVertices;
    vector<GLfloat> points;
    vector<GLuint> index;
    vector<GLfloat> normals;
    vector<GLfloat> texCoord;

public:
    Object3d() {
        numVertices = 0;
    }
    /* A função loadObject começa por ir buscar
    o ficheiro que contem a informação dos vertices
    (é utilizada a concatenação com a string ../ para que se possam
    por os ficheiros dentro da pasta parser e não dentro da pasta cmake-build-debug*/
    void loadObject(string filePath)
    {
        //destroyObject();
        char * string = strdup(filePath.c_str());
        char * workdir = strdup("../../samples/3D/");
        workdir = (char*) realloc(workdir, strlen(workdir) + strlen(string) + 1);
        char * useThis = strcat(workdir,string);
        float x,y,z,nx,ny,nz,uv,vv;
        unsigned int tempI;
        ifstream inFile(useThis);
        inFile >> numVertices;

        /* alocado estado para um array de floats grande o suficiente para gerir todos os vertices do objeto */
        for (int i=0;i<numVertices;i++)
        {
            inFile >> x >> y >> z >> nx >> ny >> nz >> uv >> vv;

            points.push_back(x);
            points.push_back(y);
            points.push_back(z);

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            texCoord.push_back(uv);
            texCoord.push_back(vv);
        }
        while (!inFile.eof())
        {
            inFile >> tempI;
            index.push_back(tempI);
        }
        inFile.close();
    }

    // Função simples para eliminar o objeto
    void destroyObject()
    {
        numVertices=0;
        points.clear();
        index.clear();
    }

    int getNumVertices() {
        return numVertices;
    }

    vector<GLfloat> getPontos()
    {
        //cout << "Points size:" << points.size() << endl;
        return points;
    }

    vector<GLuint> getIndices()
    {
        return index;
    }

    vector<GLfloat> getNormals() {
        return normals;
    }

    vector<GLfloat> getTexCoords() {
        return texCoord;
    }
};


#endif //TP_OBJECT3D_H
