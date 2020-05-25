#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include "Torus.h"

Torus::Torus(float R, float r, int stacks, int slices) : Figure(stacks, slices) {
    int k1, k2;
    float thetainc = 2.0f*M_PI / slices;
    float phiinc = 2*M_PI / stacks;
    float theta = 0.0, phi;

    float x,y,z;


    for (int i = 0; i <= slices; i++) {
        phi = 0.0;

        for (int j = 0; j <= stacks; j++) {
            x = (R + r * cos(phi)) * cos(theta);
            y = r * sin(phi);
            z = (R + r * cos(phi)) * sin(theta);
            Torus::addVertice((float) x, (float) y, (float) z);
            glm::vec3 p = glm::normalize(Torus::getGradient(phi, theta, r, R));
            Figure::addNormal(p.x, p.y, p.z);
            Figure::addTexCoord((float) i / (float) slices,
                                (float) j / (float) stacks);
            phi += phiinc;
        }

        theta += thetainc;
    }

    for(int i = 0; i < slices; i++) {

        k1 = i * (stacks + 1);
        k2 = k1 + stacks + 1;

        for(int j = 0; j < stacks; j++, k1++, k2++) {
            Figure::addIndex(k2 + 1, k1, k1 + 1);
            Figure::addIndex(k2, k1, k2 + 1);
        }

    }
}

glm::vec3 Torus::getGradient(float u, float v, float r, float R) {
    // Calcular du.
    glm::vec3 du = glm::vec3(
            -r * cos(v) * sin(u),
            r * cos(u),
            -r * sin(v) * sin(u));

    // Calcular dv.
    glm::vec3 dv = glm::vec3(
            -R * sin(v) - r * sin(v) * cos(u),
            0.0f,
            R * cos(v) + r * cos(v) * cos(u));

    return glm::normalize(glm::cross(du, dv));
}
