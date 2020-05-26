/**
 * Defines necessary mechanisms for loaded objects.
 */
#ifndef TP_OBJECT3D_H
#define TP_OBJECT3D_H


#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#endif
#include <vector>
#include <glm/glm.hpp>
#include <tuple>

using namespace std;

/**
 * Structure which encapsulates the necessary behaviours necessary
 * to the inner workings of materials. This structure is used in order
 * to pass it directly as a uniform block to the shaders.
 */
typedef struct material {
    glm::vec4 ambient;          /* Ambient component of a material */
    glm::vec4 diffuse;          /* Diffuse component of a material */
    glm::vec4 specular;         /* Specular component of a material */
    float shininess;            /* Shininess of said material */
    GLuint idTexture = 0;       /* ID of the texture it is masked with */
} Material;

class Object3d {
private:
    /**
     * Indicates the number of vertices that make up the said object.
     */
    int used = 0;

    /**
     * Buffer of all of the materials used by the current object,
     * assuming that the indicated materials have been previously
     * Buffered into OpenGL.
     */
    vector<GLuint> *material_bufs = nullptr;

    /**
     * Buffer ID of all of the VAOs which make up the object,
     * also assuming prior buffering.
     */
    vector<GLuint> *obj_vaos = nullptr;

    /**
     * All of the texture ids which make up the components
     * of the object.
     */
    vector<GLuint> *texture_ids = nullptr;

    /**
     * Every object posses a vector of all the faces of vector associated with
     * it, to be able to generate vertex array buffers. There is always atleast one material, the
     * default material, all others must be announced using announce_material.
     * Besides the information of the vertex, indexes must also be added.
     */
    vector<tuple<Material, tuple<vector<GLfloat> *, vector<GLuint> *> > > raw_obj_info;

    /**
     * Pointers to the most recently in use vectors,
     * in order to avoid acessing the vector to get its last position.
     * Thus, raw_obj_info serves only as a information holder.
     */
    vector<GLfloat> *iu_info = nullptr;
    vector<GLuint> *iu_index = nullptr;
public:
    /**
     * Constructor of Object3D, which requires
     * the main texture in use and also the default
     * material it should be associated with.
     *
     * @param id_tex Base texture.
     * @param material Base material in use.
     */
    Object3d(GLuint id_tex, Material material);

    /*
     * An announce must be made for each new material being processed,
     * from now on, other faces defined will be stored in the iu_info.
     *
     * When an announcement is made, the object now knows
     * that it has to store new incoming information as associated
     * with the newly announced material.
     *
     * @param new_mat Newly announced material.
     */
    void announce_material(Material new_mat);

    /**
     * Adds a face to the objects, append to the last material.
     *
     * A face point assumes that the ordering is being done sequentially,
     * if you want to use specific indexes, you must add_point and then add_index.
     *
     * @param position Position of the point.
     * @param normal Normal of the point to be added.
     * @param texcoord Texture coordinate associated.
     */
    void add_face_point(glm::vec3 position, glm::vec3 normal, glm::vec2 texcoord);

    /**
     * Adds a point and assumes the point will be later indexed using add_index.
     *
     * @param position Position of the point.
     * @param normal Normal of the point to be added.
     * @param texcoord Texture coordinate associated.
     */
    void add_point(glm::vec3 position, glm::vec3 normal, glm::vec2 texcoord);

    /**
     * Adds a single atomic values, it is the user's responsability
     * to keep track of the meaning of the atom indicated. It can
     * either refer to position, normal, or texture coordinates.
     * Special care must taken into account.
     *
     * @param atomic Atom we seek to insert.
     */
    void add_atomic(float atomic);

    /**
     * Allows the definition of indexes associated with
     * each of the vertexs, must be used when we want to
     * indexes incoming information.
     *
     * Indexes are all placed into the same buffer, and it is
     * assumed that the used knows the order in which they
     * must place the indexes.
     *
     * @param new_index The new indexes placed into the buffer.
     */
    void add_index(GLuint new_index);

    /**
     * Pushes into the info buffer all of the data associated with
     * the position, normals, and texture coordinates, in this exact
     * order.
     *
     * @param position Position of the vertex.
     * @param normal Normal of the vertex.
     * @param texcoords Texture coordinates of the vertex.
     */
    void push_data_info(glm::vec3 position, glm::vec3 normal, glm::vec2 texcoords);

    /**
     * Binds all of the materials and texture together.
     */
    void bind();

    /**
     * Draws the specified object and all of its sub-materials
     * using the specified shader.
     *
     * @param id_shader THe shader in use.
     */
    void draw(GLuint id_shader);
};

#endif
