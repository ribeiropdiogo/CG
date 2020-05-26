#include <cmath>
#include <glm/glm.hpp>
#include "MobiusStrip.h"

MobiusStrip::MobiusStrip(int stacks, int slices)
        : Figure() {
    float u, v;
    float du = 2.0f * M_PI / (float) slices;
    float dv = 2.0f / (float) stacks;
    int k1 = 0, k2 = 0, nV = 0, tmp = 0;

    // Adiciona todos os vertices necessarios.
    for (int k = 1; k >= -1; k -= 2) {
        for (int j = 0; j <= stacks; j++) {

            for (int i = 0; i <= slices; i++) {
                u = du * (float) i;
                v = -1.0f + dv * (float) j;

                glm::vec3 point = getPoint(u, v);
                glm::vec3 norm = glm::normalize(getGradient(u, v));
                norm = norm * (float) k;

                Figure::addVertice(point.x, point.y, point.z);
                Figure::addNormal(norm.x, norm.y, norm.z);
                Figure::addTexCoord((float) i / (float) slices,
                                    (float) j / (float) stacks);
                nV++;
            }
        }
        tmp = nV;
    }
    tmp /= 2;

    // Indexa tudo o que for necessario.
    for(int j = 0; j <= stacks; j++) {

        for(int i = 0; i < slices ; i++) {
            k1 = j*slices + i;
            k2 = k1 + slices;

            Figure::addIndex(k1, k2 + 1, k2);
            Figure::addIndex(k1, k1 + 1, k2 + 1);
        }

        for(int i = 0; i < slices; i++) {
            k1 = j*slices + i + tmp;
            k2 = k1 + slices;
            Figure::addIndex(k1, k2, k2 + 1);
            Figure::addIndex(k1, k2 + 1, k1 + 1);
        }
    }
}

glm::vec3 MobiusStrip::getGradient(float u, float v) {
    glm::vec3 du = glm::vec3(
            -((v / 2) * cos(u / 2) + 1) * sin(u) - (v * sin(u / 2) * cos(u)) / 4,
            v * cos(u / 2) / 4,
            ((v / 2) * cos(u / 2) + 1) * cos(u) - (v * sin(u / 2) * sin(u)) / 4
    );

    glm::vec3 dv = glm::vec3(
            cos(u / 2) * cos(u) / 2,
            sin(u / 2) / 2,
            cos(u / 2) * sin(u) / 2
    );

    return glm::normalize(glm::cross(du, dv));
}

glm::vec3 MobiusStrip::getPoint(float u, float v) {
    return glm::vec3(
            (1 + (v / 2) * cos(u / 2)) * cos(u),
            (v / 2) * sin(u / 2),
            (1 + (v / 2) * cos(u / 2)) * sin(u)
    );
}