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

namespace TexLoader {
    GLuint loadNormalTexture(const string texture);

    GLuint loadTexture(const string texture);

    GLuint loadCubemapTexture(vector<std::string> faces);

    void bindSkybox();
    void renderSkybox();
    void addSkyboxFace(string face);
}

#endif //GENERATOR_TEXLOADER_H
