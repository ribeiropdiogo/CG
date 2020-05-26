/**
 * Necessary methods to work with bezier patches.
 *
 * Bezier patch is a subclass of Figure.
 */
#ifndef GENERATOR_PATCHFIG_H
#define GENERATOR_PATCHFIG_H

#include "Figure.h"
#include <glm/glm.hpp>
#include "BezierSuf.h"

class PatchFig : public Figure {
private:
    /**
     * Number of patches initially defined.
     */
    int n_patches{};

    /**
     * Base engine which encapsulates all of the behaviours
     * associated with bezier surfaces.
     */
    BezierSuf *bs = nullptr;

    /**
     * Loads a specified file, parsing it onto
     * a viable bezier figure.
     *
     * @param file Name of the file to load.
     */
    void loadFile(char *file);

    /**
     * Order the PatchFig to assemble the previously
     * loaded bezier patch file, with the required tessellation
     * level.
     *
     * @param tessellation Tessellation level of the figure.
     */
    void buildPatches(int tessellation);

public:
    /**
     * Builds the equivalent bezier figure, using
     * the data specified in the file and its tessellation level.
     *
     * @param file Name of the file to load.
     * @param tessellation Tessellation level of the figure.
     */
    PatchFig(char *file, int tessellation);
};

#endif
