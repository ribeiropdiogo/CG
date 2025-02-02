#include "ObjLoader.h"
#include "Common.h"
#include <unordered_map>
#include <sstream>
#include <TexLoader.h>

#define OBJ_FOLDER "../../samples/3D/"

using namespace std;

void parse_point_wvobj(string name, int *indexes, int* sizes) {
    int num, i = 0;
    string item;
    stringstream ss(name);

    while( getline(ss,item,'/') ) {
        if(!item.empty()) {
            num = stoi(item);

            if(num < 0) {
                num = sizes[i] + num;
            } else
                num--;

            indexes[i++] = num;
        } else {
            indexes[i++] = 1;
        }
    }
}

// Reads all of the materials associated with the specified name.
// used must provide the necessary mtlib
unordered_map<string,Material> parse_materials(string mtlib) {
    string line, name;
    ifstream file(OBJ_FOLDER + mtlib);
    bool first_found = true;
    Material* mat = new Material();
    unordered_map<string,Material>* materials = new unordered_map<string,Material>();

    if(file.is_open()) {

        while ( getline(file,line) ) {

            if(!line.empty()) {

                string str;
                vector<string> tokens;
                stringstream ss(line);

                while(getline(ss, str,' ')) {
                    tokens.push_back(str);
                }

                if(!tokens[0].compare("newmtl")) {
                    // new material found.
                    name = tokens[1];
                    if(!first_found) {
                        materials->insert({name, *mat});
                        mat = new Material();
                    } else
                        first_found = false;
                }
                else if(!tokens[0].compare("Ka")) {
                    // specifications of ambient.
                    mat->ambient = glm::vec4(stof(tokens[1]),stof(tokens[2]),stof(tokens[3]),1);
                }
                else if(!tokens[0].compare("Kd")) {
                    // specifications of diffuse.
                    mat->diffuse = glm::vec4(stof(tokens[1]),stof(tokens[2]),stof(tokens[3]),1);
                }
                else if(!tokens[0].compare("Ks")) {
                    // specifications of specular.
                    mat->specular = glm::vec4(stof(tokens[1]),stof(tokens[2]),stof(tokens[3]),1);
                }
                else if(!tokens[0].compare("Ns")) {
                    // specifications of shininess.
                    mat->shininess = stof(tokens[1]);
                }
                else if(!tokens[0].compare("map_Kd")) {
                    // new texture found.
                    mat->idTexture = TexLoader::loadTexture(tokens[1].c_str());
                }

            }

        }

        materials->insert({name, *mat});

    }

    return *materials;
}

Object3d* ObjLoader::loadWVObj(string file_name, GLuint id_tex, Material material) {
    string line, name;
    ifstream file(OBJ_FOLDER + file_name);
    vector<glm::vec3> points;
    vector<glm::vec3> normals;
    vector<glm::vec2> texcoord;
    unordered_map<string,Material> materials;

    Object3d* obj = new Object3d(id_tex, material);

    if(file.is_open()) {

        while ( getline(file,line) ) {

            if(!line.empty()) {

                string str;
                vector<string> tokens;
                stringstream ss(line);

                while(getline(ss, str,' ')) {
                    tokens.push_back(str);
                }

                if(!tokens[0].compare("v")) {
                    // Found new vertice.
                    points.push_back(glm::vec3( stof(tokens[1]),stof(tokens[2]),stof(tokens[3]) ));
                }
                else if(!tokens[0].compare("vn")) {
                    // found new vertice normal
                    normals.push_back(glm::vec3( stof(tokens[1]),stof(tokens[2]),stof(tokens[3]) ));
                }
                else if(!tokens[0].compare("vt")) {
                    // found new tex coord.
                    texcoord.push_back(glm::vec2( stof(tokens[1]),stof(tokens[2]) ));
                }
                else if(!tokens[0].compare("mtllib")) {
                    // found new material lib, must learn and conquest.
                    auto newmats = parse_materials(tokens[1]);
                    materials.insert(newmats.begin(), newmats.end());

                }
                else if(!tokens[0].compare("usemtl")) {
                    // if the material was previously observed.
                    if( materials.find(tokens[1]) != materials.end() ) {
                        // then, announce the new material.
                        obj->announce_material(materials[tokens[1]]);
                    }

                }
                else if(!tokens[0].compare("f")) {
                    int indexes[3];
                    int sizes[3] = {(int)points.size(), (int)texcoord.size(), (int)normals.size()};
                    int N = tokens.size() - 3;

                    for(int i = 0; i < N; i++) {
                        for(int j = 0; j < 3; j++ ) {
                            glm::vec2 tc = glm::vec2(0);
                            parse_point_wvobj(tokens[i+j+1],indexes,sizes);

                            if(texcoord.size() != 0)
                                tc = glm::vec2(texcoord[indexes[1]]);

                            obj->add_face_point(points[indexes[0]],
                                    normals[indexes[2]],
                                    tc);
                        }
                    }

                }

            }

        }

    }

    return obj;
}

Object3d* ObjLoader::load3DObj(string file_name, GLuint id_tex, Material material) {
    float val = 0.0f;
    Object3d *obj = new Object3d(id_tex, material);
    GLuint numVertices = 0, tempI = 0;

    ifstream inFile(file_name);
    inFile >> numVertices;

    for (int i=0; i < numVertices; i++)
    {
        for (int j = 0; j < 8; j++) {
            inFile >> val;
            obj->add_atomic(val);
        }
    }
    while (!inFile.eof())
    {
        inFile >> tempI;
        obj->add_index(tempI);
    }
    inFile.close();

    return obj;
}

Object3d* ObjLoader::loadFile(string file_name, GLuint id_tex, Material material) {
    string path = OBJ_FOLDER;
    path.append(file_name);
    return isSuffixOf(file_name, ".obj") ? loadWVObj(path, id_tex, material) : load3DObj(path, id_tex, material);
}