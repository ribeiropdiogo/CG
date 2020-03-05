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
public:
    Figure(int stacks, int slices);
    int getStacks();
    int getSlices();
    int getVerticeSize();
    int getIndexSize();
    vector<float> getVertices();
    vector<int> getIndexes();
    void dump_old(char * file);
    void dump_new(char * file);
protected:
    void addVertice(float x, float y, float z);
    void addIndex(int x, int y, int z);
};

#endif //GENERATOR_FIGURE_H
