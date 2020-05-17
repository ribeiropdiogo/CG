//
// Created by syrayse on 16/05/20.
//

#include "TexLoader.h"
#include <IL/il.h>
#include <iostream>
#include <unordered_map>
#include "Common.h"

#define TEX_DIR_LOADER "../../resources/textures/"

unordered_map<string, GLuint> loadedTextures;

GLuint TexLoader::loadNormalTexture(const char* texture) {
    unsigned int textureID;

    if(loadedTextures.find(texture) == loadedTextures.end()) {
        // Doesnt exist any texture by that name.
        unsigned int t, tw, th;
        unsigned char *texData;
        ilGenImages(1, &t);
        ilBindImage(t);
        glGenTextures(1, &textureID);

        ilLoadImage((ILstring) texture);
        tw = ilGetInteger(IL_IMAGE_WIDTH);
        th = ilGetInteger(IL_IMAGE_HEIGHT);
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        texData = ilGetData();

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);
        loadedTextures[texture] = textureID;

    }
    else {
        // If a texture with that name already exists.
        textureID = loadedTextures[texture];
    }

    return textureID;
}

GLuint TexLoader::loadTexture(const char* texture) {
    string file_name = "../../resources/textures/";
    file_name.append(texture);

    return loadNormalTexture(file_name.c_str());
}

GLuint TexLoader::loadCubemapTexture(vector<string> faces) {
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