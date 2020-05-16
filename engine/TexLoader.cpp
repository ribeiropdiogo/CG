//
// Created by syrayse on 16/05/20.
//

#include "TexLoader.h"
#include <IL/il.h>
#include <iostream>
#include <unordered_map>
#include "Common.h"

unordered_map<string, GLuint> loadedTextures;

GLuint TexLoader::loadDDSTexture(const char* texture) {
    unsigned char* header;

    unsigned int width;
    unsigned int height;
    unsigned int mipMapCount;

    unsigned int blockSize;
    unsigned int format;

    unsigned int w;
    unsigned int h;

    unsigned char* buffer = 0;

    GLuint tid = 0;

    // open the DDS file for binary reading and get file size
    FILE* f;
    if((f = fopen(texture, "rb")) == 0)
        return 0;
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // allocate new unsigned char space with 4 (file code) + 124 (header size) bytes
    // read in 128 bytes from the file
    header = (unsigned char*)malloc(128);
    fread(header, 1, 128, f);

    // compare the `DDS ` signature
    if(memcmp(header, "DDS ", 4) != 0)
        return 0;

    // extract height, width, and amount of mipmaps - yes it is stored height then width
    height = (header[12]) | (header[13] << 8) | (header[14] << 16) | (header[15] << 24);
    width = (header[16]) | (header[17] << 8) | (header[18] << 16) | (header[19] << 24);
    mipMapCount = (header[28]) | (header[29] << 8) | (header[30] << 16) | (header[31] << 24);

    // figure out what format to use for what fourCC file type it is
    // block size is about physical chunk storage of compressed data in file (important)
    if(header[84] == 'D') {
        switch(header[87]) {
            case '1': // DXT1
                format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                blockSize = 8;
                break;
            case '3': // DXT3
                format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                blockSize = 16;
                break;
            case '5': // DXT5
                format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                blockSize = 16;
                break;
            case '0': // DX10
                // unsupported, else will error
                // as it adds sizeof(struct DDS_HEADER_DXT10) between pixels
                // so, buffer = malloc((file_size - 128) - sizeof(struct DDS_HEADER_DXT10));
            default: return 0;
        }
    } else // BC4U/BC4S/ATI2/BC55/R8G8_B8G8/G8R8_G8B8/UYVY-packed/YUY2-packed unsupported
        return 0;

    // allocate new unsigned char space with file_size - (file_code + header_size) magnitude
    // read rest of file
    buffer = (unsigned char*)malloc(file_size - 128);
    if(buffer == 0)
        return 0;
    fread(buffer, 1, file_size, f);

    // prepare new incomplete texture
    glGenTextures(1, &tid);
    if(tid == 0)
        return 0;

    // bind the texture
    // make it complete by specifying all needed parameters and ensuring all mipmaps are filled
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount-1); // opengl likes array length of mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // don't forget to enable mipmaping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // prepare some variables
    unsigned int offset = 0;
    unsigned int size = 0;
    w = width;
    h = height;

    // loop through sending block at a time with the magic formula
    // upload to opengl properly, note the offset transverses the pointer
    // assumes each mipmap is 1/2 the size of the previous mipmap
    for (unsigned int i=0; i<mipMapCount; i++) {
        if(w == 0 || h == 0) { // discard any odd mipmaps 0x1 0x2 resolutions
            mipMapCount--;
            continue;
        }
        size = ((w+3)/4) * ((h+3)/4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, i, format, w, h, 0, size, buffer + offset);
        offset += size;
        w /= 2;
        h /= 2;
    }
    // discard any odd mipmaps, ensure a complete texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount-1);
    // unbind
    glBindTexture(GL_TEXTURE_2D, 0);

    // easy macro to get out quick and uniform (minus like 15 lines of bulk)

    free(buffer);
    free(header);
    fclose(f);
    return tid;
}

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

    return isSuffixOf(file_name, ".dds") ? loadDDSTexture(file_name.c_str()) : loadNormalTexture(file_name.c_str());
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