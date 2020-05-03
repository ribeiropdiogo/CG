//
// Created by syrayse on 03/05/20.
//

#ifndef GENERATOR_MOBIUSSTRIP_H
#define GENERATOR_MOBIUSSTRIP_H

#include "Figure.h"
#include <Vec3.h>

class MobiusStrip : public Figure {

public:
    /**
     * Constroi uma mobius strip
     * @param stacks desloca de edge para edge.
     * @param slices desloca a volta da fita.
     */
    MobiusStrip(int stacks, int slices);

private:
    Vec3 getGradient(float u, float v);
    Vec3 getPoint(float u, float v);
};

#endif //GENERATOR_MOBIUSSTRIP_H
