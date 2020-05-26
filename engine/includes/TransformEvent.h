/**
 * Responsible for taking care of of the necessary transformations
 * which might be applied to the system.
 */
#ifndef GENERATOR_TRANSFORMEVENT_H
#define GENERATOR_TRANSFORMEVENT_H

#include "CRSpline.h"

/**
 * All the types of transformations present in the system.
 */
enum TType {ROTATE, TRANSLATE, SCALE, CATMULLROM};

class TransformEvent {
private:
    /**
     * Type of transformation.
     */
    TType m_type;

    /**
     * Indicates whether the transformation is dependent,
     * or not, in time.
     */
    bool m_timeDep;

    /**
     * Indicate the previously registered time of the system,
     * together with lap_time, this field can be used to calculate
     * the ratio corresponding to the time passed since the last
     * call of the function to apply transformations.
     *
     * Only necessary if transformation is time dependent.
     */
    int m_oldtime;

    /**
     * If the transformation is time dependent, it indicates
     * the time in which a full transformation should be performed.
     * However, if we're leading with a repeating transformation, then
     * lap time will be achieved many times.
     */
    int m_laptime;

    /**
     * X component of the transformation, this value is used when
     * dealing with the following type of transformations:
     * ROTATE, TRANSLATE, SCALE.
     */
    float m_x;

    /**
     * Y component of the transformation, this value is used when
     * dealing with the following type of transformations:
     * ROTATE, TRANSLATE, SCALE.
     */
    float m_y;

    /**
     * Z component of the transformation, this value is used when
     * dealing with the following type of transformations:
     * ROTATE, TRANSLATE, SCALE.
     */
    float m_z;

    /**
     * Angle to be applied to the transformation, this value
     * is used only for rotation transformations.
     */
    float m_angle;

    /**
     * Total time passed since the beginning of the system.
     */
    float m_totaltime;

    /**
     * Corresponds to previous up vector of the system,
     * this is used by CATMULLROM in order to preserver
     * orientation of the object.
     */
    glm::vec3 Yii;

    /**
     * Indicates a pointer to the CATMULLROM classes dedicated to
     * the current transformations.
     */
    CRSpline *spline = nullptr;

    /**
     * Builds a base transformation with the values indicated,
     * this specific constructor is mostly used to specify transformations
     * where rotations are used. But not reserved to.
     *
     * @param type Type of the transformation.
     * @param x X component of the transformation.
     * @param y Y component of the transformation.
     * @param z Z component of the transformation.
     * @param angle Angle associated with the transformation.
     * @param laptime Laptime that should be used by the system.
     */
    TransformEvent(TType type, float x, float y, float z,
                   float angle, int laptime);

    /**
     * Performs all of the necessary transformations in the CATMULLROM
     * environment, in order to move the object along the curve, according
     * to the time milis, and preserve its orientation tangent to the curve.
     *
     * @param milis Time of the system.
     */
    void dealWithCatmullR(float milis);

public:
    /**
     * Constructor mostly used to specify translate operations, but
     * not reserved to.
     *
     * @param type Type of the transformation.
     * @param x X component of the transformation.
     * @param y Y component of the transformation.
     * @param z Z component of the transformation.
     */
    TransformEvent(TType type, float x, float y, float z);

    /**
     * Constructor used to specify a CATMULLROM curve transformation.
     *
     * @param points Points in which the CATMULLROM curve occurs.
     * @param laptime Time in use the curve should be transversed.
     * @param loop Indicates wether the curve is looping or not.
     */
    TransformEvent(vector<glm::vec3> points, int laptime, bool loop);

    /**
     * Constructor for time dependent transformations, mostly
     * used in terms of TRANSLATE and SCALE.
     *
     * @param type Type of the transformation.
     * @param x X component of the transformation.
     * @param y Y component of the transformation.
     * @param z Z component of the transformation.
     * @param laptime Laptime that should be used by the system.
     */
    TransformEvent(TType type, float x, float y, float z, int laptime);

    /**
     * Constructor to specify ROTATIONS, the corresponding
     * axis of rotation and the angle of rotation.
     *
     * @param x X component of the transformation.
     * @param y Y component of the transformation.
     * @param z Z component of the transformation.
     * @param angle Angle of rotation.
     */
    TransformEvent(float x, float y, float z, float angle);

    /**
     * Constructor used to specify ROTATIONS, the corresponding axis
     * of rotation and the laptime, since no angle ise indicated,
     * it is assumed that laptime corresponds to the time necessary
     * to perform a full rotation around said axis.
     *
     * @param x X component of the transformation.
     * @param y Y component of the transformation.
     * @param z Z component of the transformation.
     * @param laptime Laptime that should be used by the system.
     */
    TransformEvent(float x, float y, float z, int laptime);

    /**
     * Applies the necessary transformation, and all of the necessary
     * processing, depending on the type of transformation and the current
     * system time.
     *
     * @param milis Current system time being used.
     */
    void process(int milis);
};

#endif
