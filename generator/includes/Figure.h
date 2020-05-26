/**
 * Encapsulates all of the necessary behaviour
 * of the figures deployed by the engine.
 *
 * All other figures of the generator
 * are thus subclasses of Figure.
 */
#ifndef GENERATOR_FIGURE_H
#define GENERATOR_FIGURE_H

#include <vector>

using namespace std;

class Figure {
private:
    /**
     * List of all the vertices used by the figure.
     */
    vector<float> m_vertices;

    /**
     * List of all indexes used by the figure,
     * indexes follow the order specified in vertices.
     */
    vector<int> m_indexes;

    /**
     * List of all of the normal used by the figure.
     */
    vector<float> m_normals;

    /**
     * List of all the texture coordinates used by the figure.
     */
    vector<float> m_tex;
public:
    /**
     * Base constructor of the figure.
     */
    Figure();

    /**
     * Performs a dump of the current figure in the manner
     * prefered by the engine, dumping that information into
     * the file.
     *
     * @param file File in which to dump.
     */
    void dump_new(char *file);

protected:
    /**
     * Inserts a vertice to the list of all  vertices.
     *
     * @param x X component of the vertex.
     * @param y Y component of the vertex.
     * @param z Z component of the vertex.
     */
    void addVertice(float x, float y, float z);

    /**
     * Add three simultaneous indexes, which makes up a triangle,
     * this assumes that the indexes specified in the order
     * in which they must be drawn.
     *
     * @param x 1st index of the triangle.
     * @param y 2nd index of the triangle.
     * @param z 3rd index of the triangle.
     */
    void addIndex(int x, int y, int z);

    /**
     * Adds a normal, associated with the corresponding
     * same index vertex of the vertex list.
     *
     * @param x X component of the normal.
     * @param y Y component of the normal.
     * @param z Z component of the normal.
     */
    void addNormal(float x, float y, float z);

    /**
     * Adds a texture coordinate associated with the corresponding
     * same index vertex in the vertex list.
     *
     * @param u U component of the texture coordinate.
     * @param v V component of the texture coordinate.
     */
    void addTexCoord(float u, float v);
};

#endif
