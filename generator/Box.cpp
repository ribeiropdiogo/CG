#include <iostream>
#include "Box.h"

float guessY(int side, float inc);
float guessZ(int side, float inc);
void resetArr(float* vec, float v1, float v2, float v3);
void fillBands_fst(int* band1, int* band2, int divisions, int sp);
void fillBands_snd(int* band1, int* band2, int divisions, int sp);

Box::Box(float wx, float wy, float wz) : Box(wx,wy,wz,1) {}

Box::Box(float wx, float wy, float wz,
        int divisions) : Figure(divisions, divisions) {
    float incX = wx / divisions;
    float incY = wy / divisions;
    float incZ = wz / divisions;
    float actX, actY, actZ;
    actX = actY = actZ = 0;
    float currVertex[3] = {-wx/2, -wy/2, wz/2};
    int j,tmp,k1,k2;

    int count = 0;

    // Create biggest band
    genLateral(currVertex, divisions, wy, incY, incZ);
    indexLateralBack(divisions);

    resetArr(currVertex,-wx/2, -wy/2, wz/2);

    count = Figure::getVerticeSize() / 3;

    tmp = genStripes(currVertex, divisions, incX, incY, incZ);
    indexStripes(divisions, count);

    currVertex[2] = wz/2;

    genLateral(currVertex, divisions, wy, incY, incZ);
    indexLateralFront(divisions, count + tmp);

    if(divisions > 1) {
        indexGap(0, divisions, true);
        indexGap(count + tmp - 4 * divisions, divisions, false);
    }
    else {
        indexGapSpecial();
    }
}

int Box::genStripes(float * currVertex, int divisions, float incX, float incY, float incZ) {
    int count = 0;
    float actY, actZ;

    for(int i = 1; i < divisions; i++) {
        currVertex[0] += incX;

        for(int side = 0; side < 4; side++) {
            actY = guessY(side, incY);
            actZ = guessZ(side, incZ);

            for(int j = 0; j < divisions; j++) {
                Figure::addVertice(currVertex[0],currVertex[1],currVertex[2]);
                count++;
                currVertex[1] += actY;
                currVertex[2] += actZ;
            }

        }
    }

    currVertex[0] += incX;

    return count;
}

void Box::indexStripes(int divisions, int count) {
    int j, k1, k2, wrapcount = 4 * divisions;
    for(int i = 1, k = 0; i < divisions - 1; i++, k+=wrapcount) {
        for(j = 0; j < wrapcount - 1; j++) {
            k1 = count + k + j;
            k2 = count + k + j + wrapcount;

            Figure::addIndex(k1 +1, k1, k2);
            Figure::addIndex(k1 + 1, k2, k2+1);
        }

        k1 = count + k + j;
        k2 = count + k + j + wrapcount;

        Figure::addIndex(count + k, k1, k2);
        Figure::addIndex(count + k, k2, count + k + wrapcount);
    }
}

void Box::genLateral(float* currVertex, int divisions, float wy, float incY, float incZ) {
    for(int i = 0; i <= divisions; i++) {

        currVertex[1] = -wy/2;

        for(int j = 0; j <= divisions; j++) {
            Figure::addVertice(currVertex[0],currVertex[1],currVertex[2]);
            currVertex[1] += incY;
        }

        currVertex[2] -= incZ;
    }
}

void Box::indexLateralBack(int divisions) {
    int k1, k2;
    for(int i = 0; i < divisions; i++) {
        for(int j = 0; j < divisions; j++) {
            k1 = (divisions+1)*i + j;
            k2 = k1 + divisions + 1;

            Figure::addIndex(k1,k1+1,k2);
            Figure::addIndex(k2,k1+1,k2+1);
        }
    }
}

void Box::indexLateralFront(int divisions, int count) {
    int k1, k2;
    for(int i = 0; i < divisions; i++) {
        for(int j = 0; j < divisions; j++) {
            k1 = count + (divisions+1)*i + j;
            k2 = k1 + divisions + 1;

            Figure::addIndex(k1,k2,k1+1);
            Figure::addIndex(k1+1,k2,k2+1);
        }
    }
}

void Box::indexGap(int sp, int divisions, bool right) {
    int* band1 = (int*)malloc(sizeof(int)*divisions*4);
    int* band2 = (int*)malloc(sizeof(int)*divisions*4);
    int i, n, bs = (divisions + 1) * (divisions + 1);

    if(right)
        fillBands_fst(band1, band2, divisions, sp);
    else
        fillBands_snd(band1, band2, divisions, sp);

    bindBand(band1, band2, divisions*4 - 1);
}

void Box::indexGapSpecial() {
    int band1[4] = {0,1,3,2};
    int band2[4] = {4,5,7,6};
    bindBand(band1, band2, 3);
}

void Box::bindBand(int* band1, int* band2, int sz) {
    int i;
    for(i = 0; i < sz; i++) {
        Figure::addIndex(band1[i], band2[i], band1[i+1]);
        Figure::addIndex(band2[i], band2[i+1], band1[i+1]);
    }

    Figure::addIndex(band1[i],band2[i],band1[0]);
    Figure::addIndex(band2[i],band2[0],band1[0]);
}

float guessY(int side, float inc) {
    float r = 0;
    switch(side) {
        case 0:
            r = inc;
            break;
        case 2:
            r = -inc;
            break;
    }
    return r;
}

float guessZ(int side, float inc) {
    float r = 0;
    switch(side) {
        case 3:
            r = inc;
            break;
        case 1:
            r = -inc;
            break;
    }
    return r;
}

void resetArr(float* vec, float v1, float v2, float v3) {
    vec[0] = v1;
    vec[1] = v2;
    vec[2] = v3;
}

void fillBands_fst(int* band1, int* band2, int divisions, int sp) {
    int i, n, bs = (divisions + 1) * (divisions + 1);

    for(i = 0; i <= divisions; i++) {
        band1[i] = sp + i;
    }

    for(n = 2; n <= divisions; n++, i++) {
        band1[i] = sp + (divisions + 1)*n - 1;
    }

    for(n = bs - 1; n >= bs - divisions - 1; n--, i++) {
        band1[i] = sp + n;
    }

    for(n = divisions - 1; n >= 1; n--, i++) {
        band1[i] = sp + (divisions + 1)*n;
    }

    for(i = 0; i < divisions*4; i++) {
        band2[i] = sp + bs + i;
    }
}

void fillBands_snd(int* band1, int* band2, int divisions, int sp) {
    int i, n, bs = (divisions + 1) * (divisions + 1);

    for(i = 0; i < divisions*4; i++) {
        band1[i] = sp + i;
    }

    sp += i;

    for(i = 0; i <= divisions; i++) {
        band2[i] = sp + i;
    }

    for(n = 2; n <= divisions; n++, i++) {
        band2[i] = sp  + (divisions + 1)*n - 1;
    }

    for(n = bs - 1; n >= bs - divisions - 1; n--, i++) {
        band2[i] = sp + n;
    }

    for(n = divisions - 1; n >= 1; n--, i++) {
        band2[i] = sp + (divisions + 1)*n ;
    }
}