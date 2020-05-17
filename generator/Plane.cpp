#include "Plane.h"
#include <GL/glew.h>
#include <IL/il.h>
#include <iostream>


using namespace std;

float getHeight(unsigned char * mat, int i, int j, unsigned int tw, float ceil) {
    return (float) (mat[tw*i+j]/255.0) * ceil ;
}

Plane::Plane(float width, float height) : Figure(1,1) {
    specs = glm::vec2(width, height);
    float half_width = specs.x / 2;
    float half_height = specs.y / 2;

    Figure::addVertice(-half_width, 0.0, -half_height);
    Figure::addVertice(half_width, 0.0, -half_height);
    Figure::addVertice(-half_width, 0.0, half_height);
    Figure::addVertice(half_width, 0.0, half_height);

    for(int i = 0; i < 4; i++)
        Figure::addNormal(0.0f, 1.0f, 0.0f);

    for(int i = 0; i <= 1; i++)
        for(int j = 0; j <= 1; j++)
            Figure::addTexCoord((float)i, (float) j);

    Figure::addIndex(0,2,3);
    Figure::addIndex(0,3,1);
}


Plane::Plane(float width, float height, float ceil, string height_map)
            : Figure(1, 1) {
    specs = glm::vec2(width, height);
    GLuint slices,          // stacks are width related.
            stacks;         // stacks are height related.

    unsigned int t, tw, th;
    unsigned char *imageData;
    float hstep, wstep;
    int k1, k2;

    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilGenImages(1,&t);
    ilBindImage(t);

    ilLoadImage((ILstring)height_map.c_str());

    ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

    tw = ilGetInteger(IL_IMAGE_WIDTH);
    // If width is 8, number of slices is 7;
    slices = tw - 1;

    th = ilGetInteger(IL_IMAGE_HEIGHT);
    // If height is 8, number of stacks is 7;
    stacks = th - 1;
    imageData = ilGetData();

    hstep = height / (float) stacks;
    wstep = width / (float) slices;
    glm::vec3 initialPoint = glm::vec3(-width/2,0,-height/2);

    for(int j = 0; j <= stacks; j++) {

        for(int i = 0; i <= slices; i++) {
            glm::vec3 p = initialPoint + glm::vec3(wstep * (float) i,
                    getHeight(imageData,i,j,tw,ceil) ,
                    hstep * (float) j);

            Figure::addVertice(p.x, p.y, p.z);

            Figure::addTexCoord((float) i, (float) j);
        }
    }

    for(int j = 0; j <= stacks; j++) {

        for(int i = 0; i <= slices; i++) {

            if( !j || !i || i == slices || j == stacks)
                Figure::addNormal(0.0f, 1.0f, 0.0f);
            else {
                glm::vec3 p1 = initialPoint + glm::vec3(wstep * (float) i,
                                                       getHeight(imageData,i,j-1,tw,ceil) ,
                                                       hstep * (float) (j-1));
                glm::vec3 p2 = initialPoint + glm::vec3(wstep * (float) i,
                                                       getHeight(imageData,i,j+1,tw,ceil) ,
                                                       hstep * (float) (j+1));
                glm::vec3 p3 = initialPoint + glm::vec3(wstep * (float) (i-1),
                                                       getHeight(imageData,i-1,j,tw,ceil) ,
                                                       hstep * (float) j);
                glm::vec3 p4 = initialPoint + glm::vec3(wstep * (float) (i+1),
                                                       getHeight(imageData,i+1,j,tw,ceil) ,
                                                       hstep * (float) j);
                glm::vec3 v1 = p2 - p1;
                glm::vec3 v2 = p4 - p3;
                glm::vec3 n = glm::normalize(glm::cross(v1,v2));
                Figure::addNormal(n.x, n.y, n.z);
            }
        }
    }

    // Colocar os respectivos indices.
    for(int i = 0; i < stacks; i++) {
        for(int j = 0; j < slices; j++) {
            k1 =  (stacks+1)*i + j;
            k2 = k1 + slices + 1;

            Figure::addIndex(k1, k2, k2 + 1);
            Figure::addIndex(k1+1,k1, k2 + 1);
        }
    }

}