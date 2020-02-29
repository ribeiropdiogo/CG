//
// Created by angelo on 28/02/20.
//
#include <iostream>
#include <fstream>
#include <math.h>
#include "cone.h"

using namespace std;

void cone::generateModel(float radius, float height, int slices, int stacks, char *file) {

    cout<<"Generating model for the cone!"<<endl;

    int vertices = (3 * slices) + (6 * slices * stacks);

    double oneSlice = (M_PI * 2.0f) / slices;
    double oneStack = height / (float) stacks;
    double oneRadius = radius / (float) stacks;
    double currentRadius = radius;
    std::ofstream outfile(file);

    //vertices
    outfile << vertices << std::endl;

    // base
    for(int i = 0; i < slices; i++){
        outfile << 0 << " " << 0 << " " << 0 << std::endl;
        outfile << radius * sin(oneSlice * (i + 1)) << " " << 0 << " " << (radius * cos(oneSlice * (i + 1))) << std::endl;
        outfile << radius * sin(oneSlice * i) << " " << 0 << " " << radius * cos(oneSlice * i)  << std::endl;
    }


    // superficie lateral
    for(int i = 0; i < stacks; i++){
        double smallRadius = currentRadius - oneRadius;

        for(int j = 0; j < slices; j++){

            double currentHeight = oneStack * i;

            //primeiro triangulo
            outfile << currentRadius * sin(oneSlice * j) << " " << currentHeight << " " << currentRadius * cos(oneSlice * j) << std::endl;
            outfile << currentRadius * sin(oneSlice * (j + 1)) << " " << currentHeight << " " << currentRadius * cos(oneSlice * (j + 1)) << std::endl;
            outfile << smallRadius * sin(oneSlice * j) << " " << currentHeight + oneStack << " " << smallRadius * cos(oneSlice * j)  << std::endl;

            //segundo triangulo
            outfile << currentRadius * sin(oneSlice * (j + 1)) << " " << currentHeight << " " << currentRadius * cos(oneSlice * (j + 1)) << std::endl;
            outfile << smallRadius * sin(oneSlice * (j + 1)) << " " << currentHeight + oneStack << " " << smallRadius * cos(oneSlice * (j + 1)) << std::endl;
            outfile << smallRadius * sin(oneSlice * (j)) << " " << currentHeight + oneStack << " " << smallRadius * cos(oneSlice * (j))  << std::endl;
        }
        currentRadius = smallRadius;
    }


    outfile.close();

    cout<<"Done!"<<endl;
}
