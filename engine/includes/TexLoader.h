/**
 * Responsible for the manangement of different textures
 * including the mechanism necessary to avoid
 * reloading the same texture twice. Keeping track of the
 * necessary texture ids.
 */
#ifndef GENERATOR_TEXLOADER_H
#define GENERATOR_TEXLOADER_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>

using namespace std;

namespace TexLoader {
    /**
     * Loads a normal texture onto the system.
     *
     * @param texture Texture to be loaded.
     *
     * @return ID of the texture loaded.
     */
    GLuint loadNormalTexture(const string texture);

    /**
     * Loads a texture onto the system, but firstly
     * it checks if that same texture as ever been loaded before
     * and if it as, the user is gifted with its previously defined
     * id. So for this reason, id textures may not be continuous.
     *
     * @param texture Texture to be loaded.
     *
     * @return ID of the texture loaded.
     */
    GLuint loadTexture(const string texture);

    /**
     * Provided with a set of all cube maps faces, it can
     * sucessly generate the ID corresponding to the cubemap
     * textures as a whole.
     *
     * @param faces Faces to be defined.
     *
     * @return ID of the cubemap loaded.
     */
    GLuint loadCubemapTexture(vector<std::string> faces);

    /**
     * Binds the skybox to OpenGL if and only
     * the provided texture where enough to
     * fill all of the cube's 6 faces.
     */
    void bindSkybox();

    /**
     * Renders a skybox onto the system.
     */
    void renderSkybox();

    /**
     * Inserts another cubemap face onto the system,
     * following a specific order.
     * @param face
     */
    void addSkyboxFace(string face);
}

#endif
