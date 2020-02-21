#include <iostream>
#include "plane.h"
#include "box.h"

using namespace std;

int main(int argc, char *argv[]) {

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

        if (argc == 6)
            b.generateModel();
        else
            cout<<"Wrong number of arguments!"<<endl;
    } else if (figure == "sphere"){
        cout<<"You want a sphere!"<<endl;
    } else if (figure == "cone"){
        cout<<"You want a cone!"<<endl;
    } else {
        cout<<"Invalid figure!"<<endl;
    }

    return 0;
}