/**
 * Defines all of the mechanisms associated with the engine.
 */
#ifndef GENERATOR_ENGINE_H
#define GENERATOR_ENGINE_H

#include <unordered_map>
#include <vector>
#include "Group.h"
#include "EngineMotion.h"
#include "IL/il.h"
#include "Shader.h"

class Engine {
private:
    /**
     * Specifies the name of the window associated
     * with the engine.
     */
    const string WIN_NAME = "CG@DI-UM";

    /**
     * Set of all loaded objects in the system,
     * may have repeating elements, since it is directly
     * used by the rendering function. However, using the map
     * declared below, memory space can be saved storing the
     * previously loaded objects, and thus avoid uncessary
     * reloading.
     */
    vector<Object3d> loadedEvents;

    /**
     * Using as a cache mechanism in order to avoid reloading
     * of repeated objects. The object may not change, but in turn
     * the texture used might. Thus the key used in the map is
     * a tuple corresponding to the name of the file and the
     * texture being used.
     */
    unordered_map<string, Object3d> loadedObj;

    /**
     * Specifies all of the groups contained in the engine
     * this vector is directly used in order to simulate a
     * hierarchical representation associated with the groups.
     * Each groups knows the number of subgroups it has, and
     * thus, if knowing this, groups can be storing sequentially,
     * as long as the number of subgroups is always into account.
     */
    static vector<Group*> groups;

    /**
     * Many type of objects have to be accounted for,
     * thus this method serves as purpose to bind them all
     * into OpenGL and GPU, so they intrinsic value can further
     * be used in the renderization.
     */
    void bindAllObjects();

    /**
     * Method responsible for the renderization of all of the
     * groups, this function is naturally recursive, in order
     * to follow up with the hierarchical nature of the engine.
     *
     * @param idx Index of the groups in group vector.
     * @param milis Time system at the of renderization.
     *
     * @return The number of groups processed.
     */
    static int runGroups(int idx, int milis);

    /**
     * Method associated with all the renderization
     * required by the mechanisms, includes setting up
     * the camera, potential skyboxes, lights
     * and any other system component.
     */
    static void renderScene();

    /**
     * Function to be used when the system is idle,
     * mainly used to keep track of the FPS score and
     * and allow for the maintenance of the focus feature
     * while the object it is associated with is moving.
     */
    static void idleFunc();

    /**
     * Used to process the mouse events incoming,
     * mostly used to keep track of the moused events in order
     * to associate them with focus feature, and thus allow for
     * focusing specific elements.
     *
     * @param button Mouse button associated.
     * @param state Mouse state associated.
     * @param xx Position in the x axis associated with the event.
     * @param yy Position in the y axis associated with the event.
     */
    static void processMouseButtons(int button, int state, int xx, int yy);

public:
    /**
     * Setups the engine and starts the OpenGL components necessary
     * for the fidelization of the rendering. Allows the building
     * of all the necessary key mappings.
     *
     * @param eargc Number of elements that might be passed to the engnie.
     * @param argv Aliens arguments the engine might make use of.
     */
    void start(int *eargc, char **argv);

    /**
     * Method responsible for entering the glut mainloop,
     * but first, it setups the necessary shaders and applies
     * all the necessary binding to the system.
     */
    void loop();

    /**
     * Warns the system that a new group has just been
     * announced, with this information the engine can know
     * provide the necessary methods to organize itself
     * and accomodate for future mechanisms, which might
     * be associated with this newly announced group.
     */
    static void newGroup();

    /**
     * Announces that a new transformation was
     * announced, and will be associated with the most
     * recently created group.
     *
     * @param te Transformation Event recently announced.
     */
    static void newTransform(TransformEvent te);

    /**
     * Announces that the most recently announced group
     * is subgroup of the group indicated as argument.
     *
     * @param group Supergroup of the recently announced group.
     */
    static void addSubgroup(int group);

    /**
     * Associates a group with its upgroup, calculating
     * all of the corresponding groups associated.
     *
     * @param upGroup Supergroup associated.
     * @param group Subgroup of the upGroup.
     *
     * @return The vector of all upgroups associated.
     */
    static vector<int> addUpgroup(int upGroup, int group);

    /**
     * Appends, onto the cube map, the necessary faces in the
     * order necessary throught the algorithm.
     *
     * @param face Texture of the cube map face.
     */
    static void appendCubeMapFace(string face);

    /**
     * Setups the initial position for the camera
     * position.
     *
     * @param x x axis position of the camera.
     * @param y y axis position of the camera.
     * @param z z axis position of the camera.
     */
    static void initialCamera(float x, float y, float z);

    /**
     * Loads an object onto the engine, taking use of
     * its defined texture and predifined base material.
     *
     * @param file Name of the model to be loaded.
     * @param texture Texture which must be used.
     * @param material Base material for the model.
     */
    void newObj(const string &file, string texture, Material material);

    /**
     * Takes cares of the innerworkings associated in
     * setting up the necassary mechanisms of the engine,
     * reponsible for the main chunk in loading the model.
     *
     * @param file Name of the model to be loaded.
     * @param texture Texture which must be used.
     * @param material Base material for the model.
     *
     * @return Address the corresponding loaded model.
     */
    Object3d *newDrawing(const string &file, const string &texture, Material material);

    /**
     * Sets up the necessary background color the engine system.
     *
     * @param r Red component of the background.
     * @param g Green component of the background.
     * @param b Blue component of the background.
     */
    static void setBackgroundColor(float r, float g, float b);
};

#endif
