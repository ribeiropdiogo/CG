#ifndef GENERATOR_FIGURE_H
#define GENERATOR_FIGURE_H

#include <vector>

using namespace std;

class Figure {
private:
    int m_stacks;
    int m_slices;
    vector<float> m_vertices;
    vector<int> m_indexes;
    vector<float> m_normals;
    vector<float> m_tex;
public:
    Figure(int stacks, int slices);

    void dump_new(char *file);

protected:
    void addVertice(float x, float y, float z);

    void addIndex(int x, int y, int z);

    void addNormal(float x, float y, float z);

    void addTexCoord(float u, float v);
};

#endif
