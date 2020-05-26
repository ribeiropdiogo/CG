/**
 * Main engine associated with the innerworkings of the
 * bezier surfaces.
 *
 * See phase 3 report for further information.
 */
#ifndef GENERATOR_BEZIERSUF_H
#define GENERATOR_BEZIERSUF_H

#include <vector>
#include <glm/glm.hpp>

using namespace std;

class BezierSuf {
private:
    /**
     * Coefficients of the bezier curve.
     */
    float Mat[16] = {
            -1.0f, 3.0f, -3.0f, 1.0f,
            3.0f, -6.0f, 3.0f, 0.0f,
            -3.0f, 3.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f
    };

    /**
     * Vector containing the indexes of the control
     * points associated with each of the bezier patches.
     * One object may have various patches.
     */
    vector<vector<int>> idx_patches;

    /**
     * All of the control points used, have into account
     * that data is all mixed in this vector, and indexes
     * of idx_patches are required to make sense out of this data.
     */
    vector<glm::vec3> controlps;

    /**
     * Performs a specialized matrix multiplication, efficiently
     * describring the necessary result intended through optimization.
     *
     * Reference to phase 3 report for further theorical analysis.
     *
     * @param alphas Alpha component of the multiplication.
     * @param betas Beta component of the multiplication.
     * @param idPatch Id of the patch being processed.
     *
     * @return Resulting multiplication.
     */
    glm::vec3 multMatrixes(const float *alphas, const float *betas, int idPatch);

    /**
     * Multiplies the specified line by its corresponding column
     * in the coeffient values.
     *
     * @param line Line used to multiply, will gain final result.
     */
    void multLineM(float *line);

    /**
     * Gets the du component of the bezier patch
     * at the specified patch and position.
     *
     * @param u U position in the current patch.
     * @param v V position in the current patch.
     * @param idPatch ID of the patch being processed.
     *
     * @return Partial derivative to U in that patch.
     */
    glm::vec3 getBezierU(float u, float v, int idPatch);

    /**
     * Gets the dv component of the bezier patch
     * at the specified patch and position.
     *
     * @param u U position in the current patch.
     * @param v V position in the current patch.
     * @param idPatch ID of the patch being processed.
     *
     * @return Partial derivative to V in that patch.
     */
    glm::vec3 getBezierV(float u, float v, int idPatch);

    /**
     * Creates a array with the third degree polynomial
     * generated for the specified input.
     *
     * @param n Input to generate polynomial.
     *
     * @return Third degree polynomial using n.
     */
    float *getCubic(float n);

    /**
     * Creates a array with the second degree polynomial
     * generate as the derivative of the function getCubic
     * for the specified input.
     *
     * @param n Input to generate polynomial.
     *
     * @return Derivative of polynomial.
     */
    float *getCubicDeriv(float n);

public:
    /**
     * Builds the bezier surface engine,
     * presenting all of the patches, control points
     * and necessary indxes.
     *
     * @param n_idx_patches All the necessary indexes of control points.
     * @param n_controlps All of the control points.
     */
    BezierSuf(vector<vector<int>> n_idx_patches,
              vector<glm::vec3> n_controlps);

    /**
     * Gets the surface position of the the specified bezier patch
     * in the corresponding position (u,v).
     *
     * @param u U position in the current patch.
     * @param v V position in the current patch.
     * @param idPatch ID of the patch being processed.
     *
     * @return Surface point calculated.
     */
    glm::vec3 getBezierSufPoint(float u, float v, int idPatch);

    /**
     * Gets the normal to surface vector of the the specified bezier patch
     * in the corresponding position (u,v).
     *
     * @param u U position in the current patch.
     * @param v V position in the current patch.
     * @param idPatch ID of the patch being processed.
     *
     * @return Normal to surface vector calculated.
     */
    glm::vec3 getBezierSufNorm(float u, float v, int idPatch);
};

#endif
