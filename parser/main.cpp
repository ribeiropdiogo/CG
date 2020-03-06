#include <iostream>
#include "includes/tinyxml.h"
#include "includes/tinystr.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include "includes/Object3d.h"

#define _USE_MATH_DEFINES
#include <math.h>

GLfloat angle = 0.0f;
GLfloat mode = GL_FILL; // GL_FILL, GL_LINE; GL_POINT
GLfloat dispx = 0.0f;
GLfloat dispy = 0.0f;
GLfloat dispz = 0.0f;
GLfloat height = 2.0f;
GLfloat basescale = 2.0f;

GLfloat beta = 0.0f;
GLfloat alpha = 0.0f;

GLfloat scale = 1.0f;

GLfloat globeAngle = 0.0f;

int n = 0;
GLuint * buffers;
GLuint * indexes;

std::vector<Object3d> objs;

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void handle_rotation(unsigned char key, int x, int y) {
    switch (key) {
        case '1':
            mode = GL_FILL;
            break;
        case '2':
            mode = GL_LINE;
            break;
        case '3':
            mode = GL_POINT;
            break;
        case 'w':
            beta += M_PI / 100;
            if (beta > M_PI / 2)
                beta -= M_PI / 100;
            break;
        case 's':
            beta -= M_PI / 100;
            if (beta < -M_PI / 2)
                beta += M_PI / 100;
            break;
        case 'd':
            alpha += M_PI / 100;
            break;
        case 'a':
            alpha -= M_PI / 100;
            break;
        default:;
    }

    glutPostRedisplay();
}

void drawAxes(void)
{
    glBegin(GL_LINES);
// X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f( 100.0f, 0.0f, 0.0f);
// Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
// Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
}

void renderObject(int i,Object3d obj)
{
    glBindBuffer(GL_ARRAY_BUFFER,buffers[i]);
    glVertexPointer(3,GL_FLOAT,0,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexes[i]);
    glDrawElements(GL_TRIANGLES,obj.getIndices().size(),GL_UNSIGNED_INT,NULL);
}

void renderScene(void) {
    glClearColor(1,1,1,1);
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();

    gluLookAt(5.0*sin(alpha)*cos(beta), 5.0 * sin(beta), 5.0*cos(alpha)*cos(beta),
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

    glPolygonMode(GL_FRONT_AND_BACK, mode);

// put the geometric transformations here

// put drawing instructions here
 drawAxes();

/* São percorridos todos os buffers para
que estes sejam desenhados*/
for (int i=0;i<n;i++)
    renderObject(i,objs[i]);
glutSwapBuffers();
}

int main(int argc, char **argv) {
    char * file = strdup(argv[1]);
    char * workdir = strdup("../../samples/XML/");
    workdir = (char*) realloc(workdir, strlen(workdir) + strlen(file) + 1);
    char * dir = strcat(workdir, file);
    //objs.reserve(1);

    TiXmlDocument doc(dir);
    if (!doc.LoadFile()) return 1;
    TiXmlHandle hDoc(&doc);
    TiXmlElement *pElem;
    TiXmlHandle hRoot(0);
    pElem = hDoc.FirstChildElement().Element();
    if (!pElem) return 1;
    const char *string = pElem->Value();
    hRoot = TiXmlHandle(pElem);

// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("CG@DI-UM");

// Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(handle_rotation);

    glewInit();

//  OpenGL settings
   glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_CULL_FACE);

// Read Objects
    if (strcmp(string, "scene") == 0) {
        pElem = hRoot.FirstChildElement().Element();
        for (pElem; pElem; pElem = pElem->NextSiblingElement()) {
            objs.resize(n+1);
            objs[n].loadObject((char *) pElem->Attribute("file"));
            n++;
        }
        //printf("%p %p",objs[0].getIndices().data(),objs[1].getIndices().data());
        buffers = (GLuint *) malloc(sizeof(GLuint) * n);
        indexes = (GLuint *) malloc(sizeof(GLuint) * n);
        glGenBuffers(n,buffers);
        glGenBuffers(n,indexes);
        for (int i=0;i<n;i++) {
            glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
            glBufferData(GL_ARRAY_BUFFER, objs[i].getPontos().size() * sizeof(GLfloat),
                         objs[i].getPontos().data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, objs[i].getIndices().size() * sizeof(GLuint),
                         objs[i].getIndices().data(), GL_STATIC_DRAW);
        }
    }

// enter GLUT's main cycle
    glutMainLoop();
        return 1;

    }