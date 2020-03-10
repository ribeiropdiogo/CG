//
// Created by syrayse on 10/03/20.
//

#include "Group.h"

void Group::popDraw(GLuint * buffers, GLuint * indexes) {
    int i = drawings.front().getBufferId();
    glBindBuffer(GL_ARRAY_BUFFER,buffers[i]);
    glVertexPointer(3,GL_FLOAT,0,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexes[i]);
    glDrawElements(GL_TRIANGLES,obj.getIndices().size(),GL_UNSIGNED_INT,NULL);
    drawings.pop();
}

void Group::pushTransformation(TransformEvent te) {
    transformations.push(te);
}

void Group::pushDraw(DrawEvent de) {
    drawings.push(de);
}

int Group::publish(GLuint * buffers, GLuint * indexes) {
    for (int i = 0; i < transformations.size(); ++i) {
        transformations.front().process();
        transformations.pop();
    }

    for (int j = 0; j < drawings.size(); ++j) {
        popDraw(buffers, indexes);
    }

    return n_subgroups;
}