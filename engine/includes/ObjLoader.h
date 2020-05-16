//
// Created by syrayse on 15/05/20.
//

#ifndef GENERATOR_OBJLOADER_H
#define GENERATOR_OBJLOADER_H

#include "Object3d.h"

class ObjLoader {
private:
    static Object3d* loadWVObj(string file_name, GLuint id_tex);
    static Object3d* load3DObj(string file_name, GLuint id_tex);
public:
    static Object3d* loadFile(string file_name, GLuint id_tex);
};

#endif //GENERATOR_OBJLOADER_H
