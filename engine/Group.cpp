//
// Created by syrayse on 10/03/20.
//

#include <GL/glew.h>
#include "Group.h"

Group::Group() {
    n_subgroups = 0;
}

void Group::popDraw(int idx, GLuint * buffers, GLuint * indexes) {
    Object3d obj = drawings[idx].getObj();
    unsigned int i = drawings[idx].getBufferId();
    glBindBuffer(GL_ARRAY_BUFFER,buffers[i]);
    glVertexPointer(3,GL_FLOAT,0,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexes[i]);
    glDrawElements(GL_TRIANGLES,obj.getIndices().size(),GL_UNSIGNED_INT,nullptr);
}

void Group::pushTransformation(TransformEvent te) {
    transformations.push_back(te);
}

void Group::pushDraw(DrawEvent de) {
    drawings.push_back(de);
}

int Group::publish(GLuint * buffers, GLuint * indexes) {
    for (auto & transformation : transformations) {
        transformation.process();
    }

    for (int j = 0; j < drawings.size(); ++j) {
        popDraw(j, buffers, indexes);
    }

    return n_subgroups;
}

int Group::addSubgroup() {
    return ++n_subgroups;
}