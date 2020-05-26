#include "Box.h"
#include <cmath>
#include <glm/glm.hpp>

using namespace glm;

#define DIM 3

Box::Box(float wx, float wy, float wz) : Box(wx,wy,wz,1, false) {}

Box::Box(float wx, float wy, float wz,
         int divisions, bool uvMask) : Figure() {

    int nVertices = 0;
    float normals[] = {0.0f, 0.0f, 0.0f};
    float weights[] = {wx, wy, wz};
    float steps[DIM];

    // Constroi os steps necessarios.
    for (int i = 0; i < DIM; i++)
        steps[i] = weights[i] / (float) divisions;
    
    // Constroi todas as laterais.
    for(float & normal : normals) {
        normal = 1.0f;
        nVertices += genLateral(normals,steps,divisions,nVertices, uvMask);

        normal = -1.0f;
        nVertices += genLateral(normals,steps,divisions,nVertices, uvMask);

        normal = 0.0f;
    }

    if(uvMask)
        Box::addPersonalUVMask(divisions);
}


int Box::genLateral(float *normals, float *steps,
               int divisions, int nVertices, bool uvMask) {
    int k1, k2, nNew = 0;

    // Reconstroi os pesos
    float wts[] = {steps[0] * (float) divisions,
                   steps[1] * (float) divisions,
                   steps[2] * (float) divisions};

    // Cria vetor de steps
    glm::vec3 vstep = glm::vec3(steps[0], steps[1], steps[2]);

    // Importa normals para forma de Vec3.
    glm::vec3 norm = glm::vec3(normals[0], normals[1], normals[2]);

    // Calcula o vetor delta.
    float nx = normals[0];
    glm::vec3 dlt = kv * (-nx) + iv * (1 - abs(nx));

    // Calcula o vetor s.
    glm::vec3 s = glm::normalize(glm::cross(dlt, norm));

    // Define o ponto inicial.
    float *dltv = &dlt[0];
    float *sv = &s[0];
    float *nv = &norm[0];
    int in = getIndex(nv), idlt = getIndex(dltv), is = getIndex(sv);
    glm::vec3 P0 = norm * (wts[in] / 2) - dlt * (wts[idlt] / 2) - s * (wts[is] / 2);

    // Colocar todos os vertices.
    for (int j = 0; j <= divisions; j++) {

        for (int i = 0; i <= divisions; i++) {
            glm::vec3 Pi = vstep * ((s * (float) i) + (dlt * (float) j)) + P0;
            Figure::addVertice(Pi.x, Pi.y, Pi.z);
            Figure::addNormal(normals[0], normals[1], normals[2]);
            if (!uvMask)
                Figure::addTexCoord((float) i / (float) divisions,
                                    (float) j / (float) divisions);
            nNew++;
        }

    }

    // Colocar os respectivos indices.
    for(int i = 0; i < divisions; i++) {
        for(int j = 0; j < divisions; j++) {
            k1 = nVertices + (divisions+1)*i + j;
            k2 = k1 + divisions + 1;

            Figure::addIndex(k1,k2 + 1,k2);
            Figure::addIndex(k1+1,k2 + 1,k1);
        }
    }

    return nNew;
}

int Box::getIndex(float *v) {
    int i;

    for(i = 0; i < DIM && abs(v[i]) < 0.1f; i++)
        ;

    return i % DIM;
}

void Box::addPersonalUVMask(int divisions) {
    float v = 0.3333, u = 0.25;
    float vv = v / (float) divisions;
    float vu = u / (float) divisions;

    // Positive X
    genUVMask(vec2(3*u,v), vec2(vu,0), vec2(0,vv), divisions);

    // Negative X
    genUVMask(vec2(u,v), vec2(vu,0), vec2(0,vv), divisions);

    // Positive Y
    genUVMask(vec2(u,v), vec2(0,-vv), vec2(vu,0), divisions);

    // Negative Y
    genUVMask(vec2(2*u,2*v), vec2(0,vv), vec2(-vu,0), divisions);

    // Positive Z
    genUVMask(vec2(2*u,v), vec2(vu,0), vec2(0,vv), divisions);

    // Negative Z
    genUVMask(vec2(0,2*v), vec2(vu,0), vec2(0,-vv), divisions);
}

void Box::genUVMask(vec2 pos, vec2 du, vec2 dv, int divisions) {
    for(int i = 0; i <= divisions; i++) {
        vec2 p = pos + du * (float) i;

        for(int j = 0; j <= divisions; j++) {
            vec2 l = p + dv * (float) j;

            Figure::addTexCoord(l.x, l.y);
        }
    }
}