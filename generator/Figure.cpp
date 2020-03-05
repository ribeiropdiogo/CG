//
// Created by syrayse on 01/03/20.
//
#include <fstream>
#include <iostream>
#include "Figure.h"

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

void Figure::dump_old(char * file) {
    vector<float> vertices = getVertices();
    vector<int> indexes = getIndexes();
    unsigned int t, nvertices = indexes.size();

    std::ofstream outfile(file);

    outfile << nvertices << endl;

    for(unsigned int i = 0; i < nvertices; i++) {
        t = 3*indexes[i];
        outfile << vertices[t] << " " << vertices[t+1] << " " << vertices[t+2] << endl;
    }

    outfile.close();
}

void Figure::dump_new(char * file) {
    vector<float> vertices = getVertices();
    vector<int> indexes = getIndexes();
    unsigned int t, nvertices = indexes.size();

    std::ofstream outfile(file);

    outfile << nvertices << endl;

    for(unsigned int i = 0; i < vertices.size() / 3; i++) {
        t = 3*i;
        outfile << vertices[t] << " " << vertices[t+1] << " " << vertices[t+2] << endl;
    }

    for(unsigned int i = 0; i < nvertices; i++) {
        outfile << indexes[i] << endl;
    }

    outfile.close();
}