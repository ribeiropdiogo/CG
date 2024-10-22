#include "Object3d.h"

using namespace std;

Object3d::Object3d(GLuint id_tex, Material material) {
    material_bufs = new vector<GLuint>();
    obj_vaos = new vector<GLuint>();
    texture_ids = new vector<GLuint>();
    material.idTexture = id_tex;
    announce_material(material);
}

void Object3d::announce_material(Material new_mat) {
    iu_info = new vector<GLfloat>();
    iu_index = new vector<GLuint>();

    texture_ids->push_back(new_mat.idTexture);

    raw_obj_info.push_back(
            make_tuple(new_mat, make_tuple(iu_info, iu_index) ) );
}

void Object3d::add_face_point(glm::vec3 position, glm::vec3 normal, glm::vec2 texcoord) {
    push_data_info(position, normal, texcoord);
    iu_index->push_back(used++);
}

void Object3d::add_point(glm::vec3 position, glm::vec3 normal, glm::vec2 texcoord) {
    push_data_info(position, normal, texcoord);
}

void Object3d::add_atomic(float atomic) {
    iu_info->push_back(atomic);
}

void Object3d::add_index(GLuint new_index) {
    iu_index->push_back(new_index);
}

void Object3d::push_data_info(glm::vec3 position, glm::vec3 normal, glm::vec2 texcoords) {
    iu_info->push_back(position.x);
    iu_info->push_back(position.y);
    iu_info->push_back(position.z);

    iu_info->push_back(normal.x);
    iu_info->push_back(normal.y);
    iu_info->push_back(normal.z);

    iu_info->push_back(texcoords.x);
    iu_info->push_back(texcoords.y);
}

void Object3d::bind() {
    GLuint N = raw_obj_info.size();
    GLuint* VAOs = (GLuint *) malloc(sizeof(GLuint)*N);
    GLuint* VBOs = (GLuint *) malloc(sizeof(GLuint)*N);
    GLuint* indexes = (GLuint *) malloc(sizeof(GLuint)*N);
    GLuint* materials = (GLuint *) malloc(sizeof(GLuint)*N);

    glGenVertexArrays(N, VAOs);
    glGenBuffers(N, VBOs);
    glGenBuffers(N, indexes);
    glGenBuffers(N, materials);

    for(int i = 0; i < N; i++) {
        vector<GLfloat> tmp = *std::get<0>(std::get<1>(raw_obj_info[i]));
        vector<GLuint> indices = *std::get<1>(std::get<1>(raw_obj_info[i]));

        material_bufs->push_back(materials[i]);
        obj_vaos->push_back(VAOs[i]);

        glBindBuffer(GL_UNIFORM_BUFFER, materials[i]);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(Material),&std::get<0>(raw_obj_info[i]), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, tmp.size() * sizeof(GLfloat), tmp.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                     indices.data(), GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
}

void Object3d::draw(GLuint id_shader) {
    GLuint blockIndex;
    GLuint N = raw_obj_info.size();

    for(int i = 0; i < N; i++) {
        glUniform1i(glGetUniformLocation(id_shader, "ourTexture"), 0);
        blockIndex = glGetUniformBlockIndex(id_shader, "Materials");
        glUniformBlockBinding(id_shader, blockIndex, 1);

        vector<GLuint> indices = *std::get<1>(std::get<1>(raw_obj_info[i]));

        glBindBufferBase(GL_UNIFORM_BUFFER, 1, (*material_bufs)[i]);

        glBindTexture(GL_TEXTURE_2D, (*texture_ids)[i]);

        glBindVertexArray((*obj_vaos)[i]);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

    }
}