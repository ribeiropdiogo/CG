#include <iostream>
#include "includes/tinyxml.h"
#include "includes/Object3d.h"
#include "includes/tinystr.h"
#include "includes/tinyxml.h"
#include <GL/glut.h>
#include <vector>

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

void drawCylinder(float radius, float height, int slices)
{
    float currx, futx, currz, futz;
    double y = height / 2;
    double rotation = 2 * M_PI / slices;
    glBegin(GL_TRIANGLES);
    // Topo
    for (int i = 0; i < slices; i++) {
        currx = radius * cos(rotation * i);
        futx = radius * cos(rotation * (i + 1));
        currz = radius * sin(rotation * i);;
        futz = radius * sin(rotation * (i + 1));
        // Topo
        glVertex3f(currx, y, currz);
        glVertex3f(0.0f, y, 0.0f);
        glVertex3f(futx, y, futz);

        // Lateral 1
        glVertex3f(currx, y, currz);
        glVertex3f(futx, y, futz);
        glVertex3f(currx, -y, currz);

        // Lateral 2
        glVertex3f(futx, y, futz);
        glVertex3f(futx, -y, futz);
        glVertex3f(currx, -y, currz);

        // Base
        glVertex3f(futx, -y, futz);
        glVertex3f(0.0f, -y, 0.0f);
        glVertex3f(currx, -y, currz);
    }
    // Lateral

    // Base
    glEnd();
}

void handle_rotation(unsigned char key, int x, int y) {
    switch (key) {
        case 'b':
            angle -= 10.0f;
            break;
        case 'n':
            angle += 10.0f;
            break;
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
        case 'x':
            basescale += 0.1f;
            height += 0.1f;
            break;
        case 'c':
            basescale -= 0.1f;
            height -= 0.1f;
            break;
        case 'q':
            scale += 0.1f;
            break;
        case 'e':
            scale -= 0.1f;
            break;
        case 'g':
            globeAngle += 10.0f;
            break;
        default:;
    }

    glutPostRedisplay();
}

void handle_keys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            height += 0.05f;
            break;
        case GLUT_KEY_DOWN:
            if(height > 0.0f)
                height -= 0.05f;
            break;
        case GLUT_KEY_RIGHT:
            dispx += 0.2f;
            break;
        case GLUT_KEY_LEFT:
            dispx -= 0.2f;
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

    drawAxes();
// put the geometric transformations here

// put drawing instructions here
    //drawCylinder(1, 1, 20);
/* São percorridos todos os objetos presentes no vector para
que estes sejam desenhados*/
for (Object3d obj : objs)
    obj.drawObject(mode);
    // End of frame
    glutSwapBuffers();

}

int main(int argc, char **argv) {
    char * file = strdup(argv[2]);
    char * workdir = strdup("../../samples/XML/");
    workdir = (char*) realloc(workdir, strlen(workdir) + strlen(file) + 1);
    char * dir = strcat(workdir, file);
    TiXmlDocument doc(dir);
    mode = argv[1][0];
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
    glutKeyboardFunc(handle_rotation);
    glutSpecialFunc(handle_keys);
//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

// Read Objects
// enter GLUT's main cycle
    glutMainLoop();

    return 1;
}

