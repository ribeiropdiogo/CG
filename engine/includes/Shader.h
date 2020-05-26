/**
 * Methods regarding the working of shaders
 * each different shader represent as instance of the
 * shader class.
 */
#ifndef GENERATOR_SHADER_H
#define GENERATOR_SHADER_H

#include <string>
#include <GL/glew.h>

using namespace std;

class Shader {
private:
    /**
     * ID of said shader.
     */
    unsigned int ID;

    /**
     * Loads the source code associated with
     * the corresponding shader.
     *
     * @param path Name of the shader.
     *
     * @return The source code.
     */
    static string loadCodeFile(const char *path);

    /**
     * Compiles the code associated with the the specified shaders
     * performing all the necessary linking and error reporting.
     *
     * @param source Source code to compile.
     * @param shaderc Type of shader, can now vary from GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
     *
     * @return ID of the newly compiled shader.
     */
    static unsigned int compileSource(const char *source, GLuint shaderc);

public:
    /**
     * Setups the shader in memory, indicating the vertex shader
     * and its fragment shader counter part.
     *
     * @param vertexPath Vertex shader in use.
     * @param fragmentPath Fragment shader in use.
     */
    Shader(const char *vertexPath, const char *fragmentPath);

    /**
     * Gets the ID of the current shader.
     * @return ID of the shader.
     */
    unsigned int getID();

    /**
     * Announce that you seek to take use of the corresponding shader
     * this method will call upon glUseProgram to perform utilize
     * the corrresponding shader.
     */
    void use();
};

#endif
