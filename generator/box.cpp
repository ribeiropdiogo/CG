#include <iostream>
#include <fstream>
#include "box.h"

using namespace std;

void box::generateModel(int x, int y, int z, char *file) {
    cout<<"Generating model for the box!"<<endl;

    double halfx = x/2;
    double nhalfx = x/2 * -1;

    double halfy = y/2;
    double nhalfy = y/2 * -1;

    double halfz = z/2;
    double nhalfz = z/2 * -1;

    std::ofstream outfile(file);

    outfile << "36" << std::endl;

    //Face Superior
    outfile << halfx << ", " << halfy << ", " << nhalfx  << std::endl;
    outfile << nhalfx << ", " << halfy << ", " << nhalfx  << std::endl;
    outfile << halfx << ", " << halfy << ", " << halfx  << std::endl;

    outfile << nhalfx << ", " << halfy << ", " << nhalfx  << std::endl;
    outfile << nhalfx << ", " << halfy << ", " << halfx  << std::endl;
    outfile << halfx << ", " << halfy << ", " << halfx  << std::endl;

    //Face Inferior
    outfile << halfx << ", " << nhalfy << ", " << nhalfx  << std::endl;
    outfile << halfx << ", " << nhalfy << ", " << halfx  << std::endl;
    outfile << nhalfx << ", " << nhalfy << ", " << nhalfx  << std::endl;

    outfile << nhalfx << ", " << nhalfy << ", " << nhalfx  << std::endl;
    outfile << halfx << ", " << nhalfy << ", " << halfx  << std::endl;
    outfile << nhalfx << ", " << nhalfy << ", " << halfx  << std::endl;

    //Faces Laterais
    //Face 1
    outfile << halfx << ", " << nhalfy << ", " << halfz  << std::endl;
    outfile << halfx << ", " << halfy << ", " << halfz  << std::endl;
    outfile << nhalfx << ", " << halfy << ", " << halfz  << std::endl;

    outfile << nhalfx << ", " << nhalfy << ", " << halfz  << std::endl;
    outfile << halfx << ", " << nhalfy << ", " << halfz  << std::endl;
    outfile << nhalfx << ", " << halfy << ", " << halfz  << std::endl;

    //Face 2
    outfile << halfx << ", " << nhalfy << ", " << nhalfz  << std::endl;
    outfile << nhalfx << ", " << halfy << ", " << nhalfz  << std::endl;
    outfile << halfx << ", " << halfy << ", " << nhalfz  << std::endl;

    outfile << nhalfx << ", " << halfy << ", " << nhalfz  << std::endl;
    outfile << halfx << ", " << nhalfy << ", " << nhalfz  << std::endl;
    outfile << nhalfx << ", " << nhalfy << ", " << nhalfz  << std::endl;

    //Face 3
    outfile << halfx << ", " << halfy << ", " << halfz  << std::endl;
    outfile << halfx << ", " << nhalfy << ", " << halfz  << std::endl;
    outfile << halfx << ", " << halfy << ", " << nhalfz  << std::endl;

    outfile << halfx << ", " << halfy << ", " << nhalfz  << std::endl;
    outfile << halfx << ", " << nhalfy << ", " << halfz  << std::endl;
    outfile << halfx << ", " << nhalfy << ", " << nhalfz  << std::endl;

    //Face 4
    outfile << nhalfx << ", " << halfy << ", " << halfz  << std::endl;
    outfile << nhalfx << ", " << halfy << ", " << nhalfz  << std::endl;
    outfile << nhalfx << ", " << nhalfy << ", " << halfz  << std::endl;

    outfile << nhalfx << ", " << halfy << ", " << nhalfz  << std::endl;
    outfile << nhalfx << ", " << nhalfy << ", " << nhalfz  << std::endl;
    outfile << nhalfx << ", " << nhalfy << ", " << halfz  << std::endl;


    outfile.close();

    cout<<"Done!"<<endl;

}