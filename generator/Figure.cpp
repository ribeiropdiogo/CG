//
// Created by syrayse on 01/03/20.
//

#include "includes/Figure.h"

Figure::Figure(int stacks, int slices) {
    m_stacks = stacks;
    m_slices = slices;
}

int Figure::getStacks() {
    return m_stacks;
}

int Figure::getSlices() {
    return m_slices;
}

int Figure::getVerticeSize() {
    return m_vertices.size();
}

int Figure::getIndexSize() {
    return m_indexes.size();
}

vector<float> Figure::getVertices() {
    return m_vertices;
}

vector<int> Figure::getIndexes() {
    return m_indexes;
}

void Figure::addVertice(float x, float y, float z) {
    m_vertices.push_back(x);
    m_vertices.push_back(y);
    m_vertices.push_back(z);
}

void Figure::addIndex(int x, int y, int z) {
    m_indexes.push_back(x);
    m_indexes.push_back(y);
    m_indexes.push_back(z);
}
