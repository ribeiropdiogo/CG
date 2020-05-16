//
// Created by ruimendes on 26/02/20.
//

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

typedef struct material {
    glm::vec4 ambient = glm::vec4(0.2, 0.2, 0.2, 1);
    glm::vec4 diffuse = glm::vec4(0.8, 0.8, 0.8, 1);
    glm::vec4 specular = glm::vec4(1, 1, 1, 1);
    float shininess = 100.0f;
    GLuint idTexture = 0;
} Material;

class Object3d {
private:
    vector< GLuint >* material_bufs = nullptr;
    vector< GLuint >* obj_vaos = nullptr;
    vector< GLuint >* texture_ids = nullptr;
    // Every object posses a vector of all the faces of vector associated with
    // it, to be able to generate vertex array buffers. There is always atleast one material, the
    // default material, all others must be announced using announce_material.
    // Besides the information of the vertex, indexes must also be added.
    vector< tuple< Material,tuple< vector<GLfloat>*,vector<GLuint>* > > > raw_obj_info;

    // We do not need to know the number of vertexes, it is necessarly implicit
    // to each of the materials.

    // Cache each of the vector, to avoid uncessary tuple acces in raw_obj_info.
    vector<GLfloat> * iu_info = nullptr;
    vector<GLuint> * iu_index = nullptr;
public:
    Object3d(GLuint id_tex);

    /*
     * An announce must be made for each new material being processed,
     * from now on, other faced defined will be stored in the iu_info.
     */
    void announce_material(Material new_mat);

    /**
     * Adds a face to the objects, append to the last material.
     *
     * A face point assumes that the ordering is being done sequentially,
     * if you want to use specific indexes, you must add_point and then add index.
     *
     * @param position Position of the point.
     * @param normal Normal of the point to be added.
     * @param texcoord Texture coordinate associated.
     */
    void add_face_point(glm::vec3 position, glm::vec3 normal, glm::vec2 texcoord);

    /**
     * Adds a point and assumes the point will be later indexed using add_index.
     * @param position Position of the point.
     * @param normal Normal of the point to be added.
     * @param texcoord Texture coordinate associated.
     */
    void add_point(glm::vec3 position, glm::vec3 normal, glm::vec2 texcoord) ;

    void add_atomic(float atomic) ;

    void add_index(GLuint new_index) ;

    void push_data_info(glm::vec3 position, glm::vec3 normal, glm::vec2 texcoords) ;

    void bind();

    void draw(GLuint id_shader);
    void set_texture_id(GLuint id_tex);
};


#endif //TP_OBJECT3D_H
