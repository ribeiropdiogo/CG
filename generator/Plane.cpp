#include <iostream>
#include <fstream>
#include "Plane.h"

using namespace std;

void plane::generateModel(int size, char *file) {
    cout<<"Generating model for the plane!"<<endl;

    double halfsize = size/2;
    double nhalfsize = size/2 * -1;

    std::ofstream outfile(file);

    //Sendo que o plano é constituído por 2 triângulos, vamos ter sempre no total 6 vértices.
    outfile << "6" << std::endl;

    //Triangle A
    outfile << halfsize << ", 0.0" << ", " << nhalfsize  << std::endl;
    outfile << nhalfsize << ", 0.0" << ", " << nhalfsize  << std::endl;
    outfile << halfsize << ", 0.0" << ", " << halfsize  << std::endl;

    //Triangle B
    outfile << nhalfsize << ", 0.0" << ", " << nhalfsize  << std::endl;
    outfile << nhalfsize << ", 0.0" << ", " << halfsize  << std::endl;
    outfile << halfsize << ", 0.0" << ", " << halfsize  << std::endl;

    outfile.close();

    cout<<"Done!"<<endl;
}
