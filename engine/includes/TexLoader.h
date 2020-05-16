//
// Created by syrayse on 16/05/20.
//

#ifndef GENERATOR_TEXLOADER_H
#define GENERATOR_TEXLOADER_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>

using namespace std;

class TexLoader {
private:
    static GLuint loadDDSTexture(const char* texture);
    static GLuint loadNormalTexture(const char* texture);
public:
    static GLuint loadTexture(const char* texture);
    static GLuint loadCubemapTexture(vector<string> faces);
};

#endif //GENERATOR_TEXLOADER_H
