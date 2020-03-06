#include <iostream>
#include <memory.h>
#include <vector>
#include <fstream>
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Cone.h"

using namespace std;

int main(int argc, char *argv[]) {
    char * file = strdup(argv[1]);
    char * workdir = strdup("../../samples/3D/");
    workdir = (char*) realloc(workdir, strlen(workdir) + strlen(file) + 1);
    char * dir = strcat(workdir, file);


    Cone plane(1,2,500,500);
    plane.dump_old(dir);



    /*
    string figure = argv[1];

    if (figure == "plane"){
        plane p;

        //plane -> size & file
        int size = atoi(argv[2]);
        char *file = argv[3];

        if (argc == 4)
            p.generateModel(size,file);
        else
            cout<<"Wrong number of arguments!"<<endl;

    } else if (figure == "box"){
        box b;

        //box -> X,Y,Z dimensions & file
        int X = atoi(argv[2]);
        int Y = atoi(argv[3]);
        int Z = atoi(argv[4]);
        char *file = argv[5];

        if (argc == 6)
            b.generateModel(X,Y,Z,file);
        else
            cout<<"Wrong number of arguments!"<<endl;
    } else if (figure == "sphere"){
        cout<<"You want a sphere!"<<endl;

    } else if (figure == "Cone"){
        cout<<"You want a Cone!"<<endl;

        cone c;
        float radius = atof(argv[2]);
        float height = atof(argv[3]);
        int slices = atoi(argv[4]);
        int stacks = atoi(argv[5]);
        char* file = argv[6];

        if(argc == 7){
            c.generateModel(radius,height,slices,stacks,file);
        }else{
            cout << "Wrong number of arguments!" << endl;
        }
    } else {
        cout<<"Invalid figure!"<<endl;
    }

    return 0;
     */
}