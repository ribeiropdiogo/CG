//
// Created by syrayse on 08/05/20.
//

#ifndef GENERATOR_SHADER_H
#define GENERATOR_SHADER_H

#include <string>

using namespace std;

class Shader {

private:
    unsigned int ID;
    static const char* loadCodeFile(const char *path);
    static unsigned int compileSource(const char * source);

public:
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;
    void setFloat(const string &name, float value) const;
};

#endif //GENERATOR_SHADER_H
