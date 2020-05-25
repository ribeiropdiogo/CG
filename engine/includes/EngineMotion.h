/**
 * Deliveres the methods required to deal with the motion
 * inerent to the engine.
 */
#ifndef GENERATOR_ENGINEMOTION_H
#define GENERATOR_ENGINEMOTION_H

namespace EngineMotion {
    /**
     * Responsible for the processing of the events associated
     * with *ASCII key stokes*.
     *
     * @param key ASCII code associated with the pressed key.
     * @param x Position in axis X of the mouse, when key is pressed.
     * @param y Position in axis Y of the mouse, when key is pressed.
     */
    void handle_ascii(unsigned char key, int x, int y);

    /**
     * Responsible for the processing of the events associated
     * with *non-ASCII key stokes*.
     *
     * @param key Code associated with the pressed key.
     * @param x Position in axis X of the mouse, when key is pressed.
     * @param y Position in axis Y of the mouse, when key is pressed.
     */
    void handle_special(int key, int x, int y);

    /**
     * Defines the necessary behaviour which must happen
     * when a certain ASCII key is pressed.
     *
     * @param key Code of the key recently unpressed.
     * @param xn Axis X position when key is unpressed.
     * @param yn Axis Y position when key is unpressed.
     */
    void up_ascii(unsigned char key, int xn, int yn);

    /**
     * Defines the necessary behaviour which must happen
     * when a certain special key is pressed.
     *
     * @param key Code of the key recently unpressed.
     * @param xn Axis X position when key is unpressed.
     * @param yn Axis Y position when key is unpressed.
     */
    void up_special(int key, int xn, int yn);

    /**
     * Places the camere onto the scene, taking use
     * of the line of sight indicated and wether it is focusing
     * in some object or not, thus the value focused can be used
     * for special camera placing.
     *
     * @param focused Indicates if the camera is focusing on any object.
     * @param lookX Line of sight in the X axis.
     * @param lookY Line of sight in the Y axis.
     * @param lookZ Line of sight in the Z axis.
     */
    void place_camera(bool focused,float lookX,float lookY,float lookZ);

    /**
     * Defines the projection in which the engine
     * is set uppon.
     *
     * @param w Width of the viewport.
     * @param h Height of the viewport.
     */
    void projection_size(int w, int h);

    /**
     * Allows the client to specify that it seeks
     * to use the previously predifined ASCII key
     * mappers built onto the EngineMotion system.
     */
    void build_key_mappers();

    /**
     * Allows the client to specify that it seeks
     * to use the previously predifined special key
     * mappers built onto the EngineMotion system.
     */
    void build_special_mappers();

    /**
     * Asks the EngineMotion mechanism to confirm
     * the current system time, this can be used in
     * order to specify pause 'n' play mechanisms.
     *
     * @param milis Current system time.
     *
     * @return Confirmed system time.
     */
    int checkSysTime(int milis);

    /**
     * Sets up the camera position at the specified
     * location.
     *
     * @param x X axis Postion of the camera.
     * @param y Y axis Postion of the camera.
     * @param z Z axis Postion of the camera.
     */
    void setCamera(float x, float y, float z);
};

#endif
