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

void Figure::dump_new(char * file) {
    unsigned int t, nvertices = m_indexes.size();

    std::ofstream outfile(file);

    outfile << m_vertices.size() / 3  << endl;

    for(unsigned int i = 0; i < m_vertices.size() / 3; i++) {
        t = 3*i;
        outfile << m_vertices[t] << " " << m_vertices[t+1] << " " << m_vertices[t+2] << " "
                << m_normals[t] << " " << m_normals[t+1] << " " << m_normals[t+2] << " "
                << m_tex[2*i] << " " << m_tex[2*i+1] << endl;
    }

    for(unsigned int i = 0; i < nvertices; i++) {
        outfile << m_indexes[i] << endl;
    }

    outfile.close();
}

void Figure::addNormal(float x, float y, float z) {
    m_normals.push_back(x);
    m_normals.push_back(y);
    m_normals.push_back(z);
}

void Figure::addTexCoord(float u, float v) {
    m_tex.push_back(u);
    m_tex.push_back(v);
}