/**
 * API of Figure
 *
 * Figure is intended to be used as  superclass
 * to all other figures defined and used in the
 * generator.
 */
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
    vector<float> m_normals; // normal coords.
    vector<float> m_tex;     // tex coords.
public:
    Figure(int stacks, int slices);
    int getStacks();
    int getSlices();
    int getVerticeSize();
    int getIndexSize();
    vector<float> getVertices();
    vector<int> getIndexes();
    vector<float> getNormals();
    vector<float> getTexCoords();
    void dump_new(char * file);
protected:
    void addVertice(float x, float y, float z);
    void addIndex(int x, int y, int z);
    void addNormal(float x, float y, float z);
    void addTexCoord(float u, float v);
};

#endif //GENERATOR_FIGURE_H
