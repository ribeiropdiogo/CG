/**
 * Delivers all of the mechanism necessary to the definition
 * of groups.
 */
#ifndef GENERATOR_GROUP_H
#define GENERATOR_GROUP_H

#include <vector>
#include "TransformEvent.h"
#include "EngineMotion.h"
#include "Shader.h"
#include "Object3d.h"
#include <glm/glm.hpp>

using namespace std;

class Group {
private:
    /**
     * List of all of the transformations which must
     * be applied to this specific group. These events
     * must be processed in sequential order.
     */
    vector<TransformEvent> transformations;

    /**
     * Drawings required by the current group.
     */
    vector<Object3d> drawings;

    /**
     * Center position of the group, used in stencil.
     */
    GLfloat center[4];

    /**
     * Number of subgroups associated with the group.
     * This number of subgroups can be used in order to
     * preserver hierarchical manner of the mechanism.
     */
    int n_subgroups;

    /**
     * Defines the indexes of all the groups up in
     * hierarchy to the current group.
     */
    vector<int> n_upGroups;
public:
    /**
     * Initializes the group.
     */
    Group();

    /**
     * Announces that a new transformation
     * must be associated with the group.
     *
     * @param te New transformation event.
     */
    void pushTransformation(TransformEvent te);

    /**
     * Announces that a new object must be associated
     * with the group defined.
     *
     * @param de Newly loaded group.
     */
    void pushDraw(Object3d de);

    /**
     * Queries the group to render itself onto the
     * provided shader, including all the necarry transformations.
     *
     * @param shader Shader which must be used.
     * @param milis Time system used.
     *
     * @return The number of subgroups associated.
     */
    int publish(Shader shader, int milis);

    /**
     * Warns the group that it must accomodatte one more
     * subgroup.
     *
     * @return New number of subgroups.
     */
    int addSubgroup();

    /**
     * Indicates the list of all upper hierarchy groups.
     * @return All upper hierarchy groups.
     */
    vector<int> getUpgroup();

    /**
     * Indicates the upgroups of the current groups,
     * using, for this purpose, the all of the upper up group
     * parents. The up group parents are thus the group itself
     * and all of its group parents.
     *
     * @param upGroupIndex The up group.
     * @param upGroupParents All of the up groups for the up group used.
     */
    void setUpgroup(int upGroupIndex, vector<int> upGroupParents);

    /**
     * Adjusts the center position of the group in accordance to its
     * up groups, also using the time system.
     *
     * @param groups Set of all groups.
     * @param milis Time system in use.
     */
    void adjustCenter(vector<Group *> groups, int milis);

    /**
     * Calculates the center position of the group.
     * @return X center position.
     */
    float getCenterX();

    /**
     * Calculates the center position of the group.
     * @return Y center position.
     */
    float getCenterY();

    /**
     * Calculates the center position of the group.
     * @return Z center position.
     */
    float getCenterZ();
};

#endif
