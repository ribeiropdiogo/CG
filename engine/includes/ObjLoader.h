/**
 * Mechanisms in charge of the loading of the different
 * types of object models available to the system.
 */
#ifndef GENERATOR_OBJLOADER_H
#define GENERATOR_OBJLOADER_H

#include "Object3d.h"

class ObjLoader {
private:
    /**
     * Performs the loading of a wavefront object model
     * in the system.
     *
     * @param file_name File to be loaded.
     * @param id_tex Base texture being used.
     * @param material Base material being used.
     *
     * @return The newly created object.
     */
    static Object3d *loadWVObj(string file_name, GLuint id_tex, Material material);

    /**
     * Performs the loading of a 3D object model, defined
     * by the makers of this engine.
     *
     * @param file_name File to be loaded.
     * @param id_tex Base texture being used.
     * @param material Base material being used.
     *
     * @return The newly created object.
     */
    static Object3d *load3DObj(string file_name, GLuint id_tex, Material material);

public:
    /**
     * Abstracts for the necessary method to load each of the models,
     * this method is responsible for decising which of models to use
     * in accordance to the file name.
     *
     * @param file_name File to be loaded.
     * @param id_tex Base texture being used.
     * @param material Base material being used.
     *
     * @return The newly created object.
     */
    static Object3d *loadFile(string file_name, GLuint id_tex, Material material);
};

#endif
