#include <iostream>
#include <memory.h>
#include <vector>
#include <fstream>
#include <Torus.h>
#include <MobiusStrip.h>
#include "PatchFig.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Cone.h"
#include "Torus.h"

using namespace std;

char * redir(char * file);

int main(int argc, char *argv[]) {

    float width, height, ceil;
    char *file;

    string figure = argv[1];

    if (figure == "plane") {
        //plane -> size & file
        width = atof(argv[2]);
        height = atof(argv[3]);
        Plane *p = nullptr;

        if(argc == 5) {
            file = redir(argv[4]);
            p = new Plane(width, height);

        } else if(argc == 7) {
            ceil = atof(argv[4]);
            string h = argv[5];
            file = redir(argv[6]);
            p = new Plane(width, height, ceil, h);
        }
        else {
            cout << "Wrong number of arguments!" << endl;
            return 1;
        }

        p->dump_new(file);
    } else if (figure == "box") {

        //box -> X,Y,Z dimensions & file
        float X = atof(argv[2]);
        float Y = atof(argv[3]);
        float Z = atof(argv[4]);
        char *file = redir(argv[5]);

        if (argc == 6) {
            Box box(X, Y, Z);
            box.dump_new(file);
        } else if (argc == 7) {
            Box box2(X, Y, Z, atoi(argv[6]), false);
            box2.dump_new(file);
        } else if (argc == 8) {
            Box box3(X, Y, Z, atoi(argv[6]), true);
            box3.dump_new(file);
        } else {
            cout << "Wrong number of arguments!" << endl;
        }

    } else if (figure == "sphere") {
        if (argc != 6) {
            cout << "Wrong number of arguments!" << endl;
        } else {
            Sphere sp(atof(argv[2]), atoi(argv[3]), atoi(argv[4]));
            sp.dump_new(redir(argv[5]));
        }

    } else if (figure == "cone") {
        if (argc != 7) {
            cout << "Wrong number of arguments!" << endl;
        } else {
            Cone cone(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]));
            cone.dump_new(redir(argv[6]));
        }
    } else if (figure == "patch") {
        if(argc != 5) {
            cout << "Wrong number of arguments!" << endl;
        } else {
            PatchFig pfig(argv[2], atoi(argv[3]));

            pfig.dump_new(redir(argv[4]));
        }
    } else if (figure == "torus") {
        if (argc != 7) {
            cout << "Wrong number of arguments!" << endl;
        } else {
            Torus tor(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]));
            tor.dump_new(redir(argv[6]));
        }
    } else if (figure == "mobius") {
        if(argc != 5) {
            cout << "Wrong number of arguments!" << endl;
        } else {
            MobiusStrip mob(atoi(argv[2]), atoi(argv[3]));
            mob.dump_new(redir(argv[4]));
        }
    } else {
        cout<<"Invalid figure!"<<endl;
    }

    return 0;
}

char * redir(char * file) {
    char *workdir, *dir;
    file = strdup(file);
    workdir = strdup("../../samples/3D/");
    workdir = (char*) realloc(workdir, strlen(workdir) + strlen(file) + 1);
    dir = strcat(workdir, file);
    return dir;
}