#include <iostream>
#include "tinyxml.h"
#include "Object3d.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <GL/glut.h>
#include <vector>

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

void drawAxes(void)
{
    glBegin(GL_LINES);
// X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f( 100.0f, 0.0f, 0.0f);
// Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
// Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
}

void renderScene(void) {
    glClearColor(1,1,1,1);
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(5.0,5.0,5.0,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);
    drawAxes();
// put the geometric transformations here

// put drawing instructions here

/* São percorridos todos os objetos presentes no vector para
que estes sejam desenhados*/
for (Object3d obj : objs)
    obj.drawObject();
    // End of frame
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    TiXmlDocument doc(argv[1]);
    if (!doc.LoadFile()) return 1;
    TiXmlHandle hDoc (&doc);
    TiXmlElement* pElem;
    TiXmlHandle hRoot(0);
    Object3d obj;
    pElem=hDoc.FirstChildElement().Element();
    if (!pElem) return 1;
    const char * string = pElem->Value();
    hRoot=TiXmlHandle(pElem);
    if (strcmp(string,"scene")==0)
    {
        pElem=hRoot.FirstChildElement().Element();
        for (pElem;pElem;pElem=pElem->NextSiblingElement())
        {
            /* É utilizado tinyXML para ir buscar o atributo file 
            ao model para desta forma saber qual o ficheiro que contem
            os vertices necessarios para montar o modelo */
            obj.loadObject((char *) pElem->Attribute("file"));
            /* Depois do objeto ser carregado é colocado num vector nativo de C++
            onde será guardado para ser desenhado posteriormente */
            objs.push_back(obj);
        }

    }
// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("CG@DI-UM");

// Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

// Read Objects
    obj.loadObject("example.3d");
// enter GLUT's main cycle
    glutMainLoop();

    return 1;
}

