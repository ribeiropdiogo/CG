//
// Created by syrayse on 08/05/20.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include "Shader.h"

using namespace std;

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    int sucess;
    unsigned int vertex, fragment;
    char infoLog[512];

    string vShaderCode = Shader::loadCodeFile(vertexPath);
    string fShaderCode = Shader::loadCodeFile(fragmentPath);

    vertex = Shader::compileSource(vShaderCode.c_str(), GL_VERTEX_SHADER);
    fragment = Shader::compileSource(fShaderCode.c_str(), GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &sucess);

    if(!sucess) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int Shader::getID() {
    return ID;
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

string Shader::loadCodeFile(const char *path) {
    string code;
    ifstream file;

    file.exceptions(ifstream::failbit | ifstream::badbit);

    try {
        file.open(path);

        stringstream fStream;

        fStream << file.rdbuf();

        file.close();

        code = fStream.str();
    } catch(ifstream::failure e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
    }

    return code;
}

unsigned int Shader::compileSource(const char * source, GLuint shaderc) {
    unsigned int v;
    int sucess;
    char infoLog[512];

    v = glCreateShader(shaderc);
    glShaderSource(v, 1, &source, NULL);
    glCompileShader(v);

    glGetShaderiv(v, GL_COMPILE_STATUS, &sucess);
    if(!sucess) {
        glGetShaderInfoLog(v, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    };

    return v;
}