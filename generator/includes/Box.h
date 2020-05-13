//
// Created by Diogo Ribeiro on 21/02/2020.
//

#ifndef GENERATOR_BOX_H
#define GENERATOR_BOX_H


#include "Figure.h"
#include <glm/glm.hpp>

using namespace glm;

class Box : public Figure {
public:
    Box(float wx, float wy, float wz);
    Box(float wx, float wy, float wz,
            int divisions, bool uvMask);
private:
    Vec3 kv = * new Vec3({0.0f, 0.0f, 1.0f});
    Vec3 iv = * new Vec3({1.0f, 0.0f, 0.0f});
    /**
     * Gera a lateral com o respectivo vetor normal indicado.
     * @param normals Vetor normal a sup. da face.
     * @param steps Espaçamento entre cada um dos vertices.
     * @param divisions Numero de divisoes desejadas.
     * @param nVertices Numero de vertices já definidos
     * @return Número de vértices criados.
     */
    int genLateral(float *normals, float *steps,
                    int divisions, int nVertices, bool uvMask);

    /**
     * Indica em que indice se encontra a primeira casa não nula do array, ou 0 caso sejam todos nulos.
     * @param v Array considerado.
     * @return Indica da primeira casa nao nula.
     */
    int getIndex(float *v);
    void addPersonalUVMask(int divisions);
    void genUVMask(vec2 pos, vec2 du, vec2 dv, int divisions);
};

#endif //GENERATOR_BOX_H