//
// Created by syrayse on 15/05/20.
//

#include "ObjLoader.h"
#include "Common.h"

Object3d* ObjLoader::loadWVObj(string file_name, GLuint id_tex) {
    return nullptr;
}

Object3d* ObjLoader::load3DObj(string file_name, GLuint id_tex) {
    float val;
    Object3d* obj = new Object3d(id_tex);
    GLuint numVertices, tempI;

    ifstream inFile(file_name);
    inFile >> numVertices;

    for (int i=0; i < numVertices; i++)
    {
        for (int j = 0; j < 8; j++) {
            inFile >> val;
            obj->add_atomic(val);
        }
    }
    while (!inFile.eof())
    {
        inFile >> tempI;
        obj->add_index(tempI);
    }
    inFile.close();

    return obj;
}

Object3d* ObjLoader::loadFile(string file_name, GLuint id_tex) {
    string path = "../../samples/3D/";
    path.append(file_name);
    return isSuffixOf(file_name, ".obj") ? loadWVObj(path, id_tex) : load3DObj(path, id_tex);
}