#ifndef GENERATOR_SHADER_H
#define GENERATOR_SHADER_H

#include <string>
#include <GL/glew.h>

using namespace std;

class Shader {

private:
    unsigned int ID;
    static string loadCodeFile(const char *path);
    static unsigned int compileSource(const char * source, GLuint shaderc);

public:
    Shader(const char* vertexPath, const char* fragmentPath);
    unsigned int getID();
    void use();
};

#endif
