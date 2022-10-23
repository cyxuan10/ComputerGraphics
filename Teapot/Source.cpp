//header
#include <GL/freeglut.h>
#include <iostream>    
#include <fstream>      
#include <string>
#include <sstream>
#include <vector>

#include "getbmp.h"
using namespace std;

#define PI 3.14159265
#define ANGLE 0.05

//globals
char ch = '1';
vector <float> pointList;
vector <int> faceList;
static float g[3];
static float tmpmax[3], tmpmin[3]; // tmpmax stores the maximum point while tmpmin stores the minimum point
static int f[3];
static int cnt = 0;
static unsigned int texture[1]; // Array of texture ids.
static int id = 0; // Currently displayed texture id.
static bool flat = 0; // Surface is flat or smooth.
static bool teapot = 0; // Change model.
static float center[3]; // Center of model.
static float i1, i2, i3; // three faces.
static float xMove = 0.0, yMove = 600.0, zMove = 600.0; // Movement components. Values are default location.
static float xLight = 0.0, yLight = 1500.0, zLight = 1500.0; // Light components. Values are default location.
static float light_position[] = { xLight, yLight, zLight, 1.0 }; // Spotlight position.

float diagonal()
{
    float l = sqrt(pow(tmpmax[0] - tmpmin[0], 2) + pow(tmpmax[1] - tmpmin[1], 2) + pow(tmpmax[2] - tmpmin[2], 2) * 1.0); // Diagonal line length.

    for (int i = 0; i < 3; i++)
        center[i] = (tmpmax[i] + tmpmin[i]) / 2; // Center.


    printf(" ---------------------------------------\n");

    if (teapot == 0)
        printf("| Teapot Model\t\t\t\t|\n");
    else if (teapot == 1)
        printf("| Mushroom Model\t\t\t|\n");

    printf("|---------------------------------------|\n");
    cout << "| Diagonal Line Length: " << l << "\t\t|" << endl;
    cout << "| Center: " << center[0] << ", " << center[1] << ", " << center[2] << "\t|" << endl;
    printf(" ---------------------------------------\n");

    return 0;
}

void output()
{
    printf(" --------------------------------------------------------------------------------\n");
    printf("| Press 's' to switch between flat shading and smooth shading. \t\t\t |\n");
    printf("| Press 'x', 'y', and 'z' to rotate the light source about the x, y, and z axis. |\n");
    printf("| Press 'r' to reset the camera to its initial position. \t\t\t |\n");
    printf("| Press 't' to swap the teapot model with a mushroom model. \t\t\t |\n");
    printf(" --------------------------------------------------------------------------------\n");
}

class faceNormal
{
public:
    float vr[3], area;

    faceNormal(float* coord, float A)
    {
        for (int i = 0; i < 3; i++)
            vr[i] = coord[i];
        area = A;
    }
};

// Vector of face normals.
vector<faceNormal> faceNormals;

void ComputeFaceNormals(float* coord1, float* coord2, float* coord3)
{
    float va[3], vb[3], vr[3], val;
    float norm[3];
    float area;

    /* Calculate Vector1 and Vector2 */
    for (int i = 0; i < 3; i++)
    {
        va[i] = coord2[i] - coord1[i];
        vb[i] = coord3[i] - coord1[i];
    }

    /* cross product */
    vr[0] = va[1] * vb[2] - vb[1] * va[2];
    vr[1] = vb[0] * va[2] - va[0] * vb[2];
    vr[2] = va[0] * vb[1] - vb[0] * va[1];

    /* normalization factor */
    val = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);

    for (int i = 0; i < 3; i++)
        norm[i] = vr[i] / val;

    area = 0.5 * (sqrt(pow(vr[0], 2) + pow(vr[1], 2) + pow(vr[2], 2)));

    //printf("Face normal: %f, %f, %f\n", norm[0], norm[1], norm[2]);

    faceNormals.push_back(faceNormal(norm, area));
}

class vertexNormal
{
public:
    float vn[3];

    vertexNormal(float* coord)
    {
        for (int i = 0; i < 3; i++)
            vn[i] = coord[i];
    }
};

// Vector of vertex normals.
vector<vertexNormal> vertexNormals;

void ComputeVertexNormals()
{
    float coord[3] = { 0,0,0 };

    for (int i = 1; i <= pointList.size(); i++)
    {
        float A = 0, vnr[3], Vnorm;

        for (int j = 0; j < faceList.size(); j++)
        {
            if (faceList[j] == i)
            {
                int face;
                if (j % 3 == 0)
                    face = (j + 3) / 3; // face vertex one
                else if (j % 3 == 1)
                    face = (j + 2) / 3; // face vertex two
                else if (j % 3 == 2)
                    face = (j + 1) / 3; // face vertex three

                coord[0] += faceNormals[face - 1].area * faceNormals[face - 1].vr[0];
                coord[1] += faceNormals[face - 1].area * faceNormals[face - 1].vr[1];
                coord[2] += faceNormals[face - 1].area * faceNormals[face - 1].vr[2];
                A += faceNormals[face - 1].area; // Sum of area
            }
        }

        for (int i = 0; i < 3; i++)
            coord[i] /= A;

        /* normalization factor */
        Vnorm = sqrt(coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2]);

        for (int i = 0; i < 3; i++)
            vnr[i] = coord[i] / Vnorm;

        vertexNormals.push_back(vertexNormal(vnr));
    }
}

// Read the vertices indices.
void ParseFace(float* arr, float* max, float* min)
{
    for (int i = 0; i < 3; i++)
    {
        // compare and store the max and min of x, y, z vertices
        if (arr[i] > max[i])
            max[i] = arr[i];
        else if (arr[i] < min[i])
            min[i] = arr[i];
        else {}

        pointList.push_back(arr[i]);
        cnt++;
    }
}

// Draw faces for the vertices indices.
void DrawFace()
{
    glBindTexture(GL_TEXTURE_2D, texture[id]);
    for (int i = 0; i < faceList.size(); i += 3)
    {
        glBegin(GL_POLYGON);
        i1 = faceList[i] - 1; //need to -1 because index of vector starts from 0
        glTexCoord2f(1.0, 1.0);
        if (flat == 0)
            glNormal3f(vertexNormals[i1].vn[0], vertexNormals[i1].vn[1], vertexNormals[i1].vn[2]);
        else
            glNormal3f(faceNormals[i / 3].vr[0], faceNormals[i / 3].vr[1], faceNormals[i / 3].vr[2]);
        glVertex3f(pointList[i1 * 3] - center[0], pointList[i1 * 3 + 1] - center[1], pointList[i1 * 3 + 2] - center[2]);

        i2 = faceList[i + 1] - 1;
        glTexCoord2f(0.5, 1.0);
        if (flat == 0)
            glNormal3f(vertexNormals[i2].vn[0], vertexNormals[i2].vn[1], vertexNormals[i2].vn[2]);
        else
            glNormal3f(faceNormals[i / 3].vr[0], faceNormals[i / 3].vr[1], faceNormals[i / 3].vr[2]);
        glVertex3f(pointList[i2 * 3] - center[0], pointList[i2 * 3 + 1] - center[1], pointList[i2 * 3 + 2] - center[2]);

        i3 = faceList[i + 2] - 1;
        glTexCoord2f(0.0, 0.0);
        if (flat == 0)
            glNormal3f(vertexNormals[i3].vn[0], vertexNormals[i3].vn[1], vertexNormals[i3].vn[2]);
        else
            glNormal3f(faceNormals[i / 3].vr[0], faceNormals[i / 3].vr[1], faceNormals[i / 3].vr[2]);
        glVertex3f(pointList[i3 * 3] - center[0], pointList[i3 * 3 + 1] - center[1], pointList[i3 * 3 + 2] - center[2]);
        glEnd();
    }
}

void loadOBJ(std::string fileName)
{
    ifstream inFile(fileName);
    if (!inFile.is_open()) {
        cout << "Error while opening file.";
        return;
    }

    string line;
    int gh = 0;
    while (getline(inFile, line)) {
        stringstream lineStream;
        string prefix;

        lineStream << line;

        if (line[0] == 'v') {
            lineStream >> prefix >> g[0] >> g[1] >> g[2];
            ParseFace(g, tmpmax, tmpmin);
        }
        if (line[0] == 'f') {
            lineStream >> prefix >> f[0] >> f[1] >> f[2];
            for (int i = 0; i < 3; i++)
            {
                faceList.push_back(f[i]);
            }

            float arr1[3], arr2[3], arr3[3];
            for (int i = 0; i < 3; i++)
            {
                arr1[i] = pointList[f[0] * 3 - (i + 1)];
                arr2[i] = pointList[f[1] * 3 - (i + 1)];
                arr3[i] = pointList[f[2] * 3 - (i + 1)];
            }
            ComputeFaceNormals(arr1, arr2, arr3);
        }
    }

    ComputeVertexNormals();
}

void computeBoundingBox() {
    glColor3f(1.0, 1.0, 1.0); // White bounding box
    float fmax[3];
    float fmin[3];
    for (int u = 0; u < 3; u++)
    {
        fmax[u] = tmpmax[u] - center[u];
        fmin[u] = tmpmin[u] - center[u];
    }
    // FRONT
    glBegin(GL_LINE_LOOP);
    glVertex3f(fmin[0], fmin[1], fmin[2]);
    glVertex3f(fmin[0], fmax[1], fmin[2]);
    glVertex3f(fmax[0], fmax[1], fmin[2]);
    glVertex3f(fmax[0], fmin[1], fmin[2]);
    glEnd();

    // Back
    glBegin(GL_LINE_LOOP);
    glVertex3f(fmax[0], fmin[1], fmax[2]);
    glVertex3f(fmax[0], fmax[1], fmax[2]);
    glVertex3f(fmin[0], fmax[1], fmax[2]);
    glVertex3f(fmin[0], fmin[1], fmax[2]);
    glEnd();

    // Right
    glBegin(GL_LINE_LOOP);
    glVertex3f(fmax[0], fmin[1], fmin[2]);
    glVertex3f(fmax[0], fmax[1], fmin[2]);
    glVertex3f(fmax[0], fmax[1], fmax[2]);
    glVertex3f(fmax[0], fmin[1], fmax[2]);
    glEnd();

    // LEFT
    glBegin(GL_LINE_LOOP);
    glVertex3f(fmin[0], fmin[1], fmax[2]);
    glVertex3f(fmin[0], fmax[1], fmax[2]);
    glVertex3f(fmin[0], fmax[1], fmin[2]);
    glVertex3f(fmin[0], fmin[1], fmin[2]);
    glEnd();

    // TOP
    glBegin(GL_LINE_LOOP);
    glVertex3f(fmax[0], fmax[1], fmax[2]);
    glVertex3f(fmax[0], fmax[1], fmin[2]);
    glVertex3f(fmin[0], fmax[1], fmin[2]);
    glVertex3f(fmin[0], fmax[1], fmax[2]);
    glEnd();

    // BOTTOM
    glBegin(GL_LINE_LOOP);
    glVertex3f(fmax[0], fmin[1], fmin[2]);
    glVertex3f(fmax[0], fmin[1], fmax[2]);
    glVertex3f(fmin[0], fmin[1], fmax[2]);
    glVertex3f(fmin[0], fmin[1], fmin[2]);
    glEnd();

    glFlush();
}

// Load external textures.
void loadExternalTextures()
{
    // Local storage for bmp image data.
    BitMapFile* image[1];

    // Load the image.
    image[0] = getbmp("one.bmp");

    // Create texture object texture[0]. 
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void light()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glEnable(GL_DEPTH_TEST); // Enable depth testing.

    // Turn on OpenGL lighting.
    glEnable(GL_LIGHTING);

    // Light property vectors.
    float light_ambient[] = { 0.41, 0.43, 0.58, 1.0 };
    float light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
    float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    // Light properties.
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHT0); // Enable particular light source.

    // Enable local viewpoint.
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    // Enable two-sided lighting.
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glPushMatrix();

}

void init(void)
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };
    GLfloat mat_ambient[] = { 1.0, 0.5, 0.31, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 0.5, 0.31, 1.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    gluLookAt(xMove, yMove, zMove, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

//Mouse functions
static GLint button, tempState, tempX, tempY;
static float alpha = 0, beta = 0;
static float r = sqrt(pow(xMove, 2) + pow(yMove, 2) + pow(zMove, 2));

// Mouse wheel callback routine.
void mouseWheel(int wheel, int state, int x, int y)
{
    if (wheel == 3)
    {
        xMove *= 0.97; yMove *= 0.97; zMove *= 0.97;
        r = sqrt(pow(xMove, 2) + pow(yMove, 2) + pow(zMove, 2));
    }
    else if (wheel == 4)
    {
        xMove *= 1.03; yMove *= 1.03; zMove *= 1.03;
        r = sqrt(pow(xMove, 2) + pow(yMove, 2) + pow(zMove, 2));
    }

    if (state == GLUT_DOWN)
    {
        tempX = x;
        tempY = y;
    }
    //stop tracking the mouse
    else if (state == GLUT_UP) {
        alpha += (tempX - x);
        beta += (y - tempY);
    }
    glutPostRedisplay();
}

// Orbit camera
// Track mouse motion while buttons are pressed
void mouseMotion(int x, int y)
{
    int deltaX, deltaY;
    float alphaAux, betaAux;
    float rAux = 1;

    deltaX = tempX - x;
    deltaY = y - tempY;

    // left mouse button: move camera
    alphaAux = alpha + deltaX;
    betaAux = beta + deltaY;

    // To let the mouse drag the model in 360 degrees only.
    if (betaAux > 90)
        betaAux = 90;
    else if (betaAux < -90)
        betaAux = -90;

    rAux = r;

    xMove = rAux * cos(betaAux * PI / 180) * sin(alphaAux * PI / 180);
    yMove = rAux * sin(betaAux * PI / 180);
    zMove = rAux * cos(betaAux * PI / 180) * cos(alphaAux * PI / 180);

    glutPostRedisplay();
}

// Callback routine for non-ASCII key entry.
void keyInput(unsigned char key, int x, int y)
{
    if (key == 's')
    {
        if (flat == 0)
            flat += 1;
        else if (flat == 1)
            flat -= 1;
    }
    if (key == 'r')
    {
        xMove = 0.0, yMove = 600.0, zMove = 600.0; // Recenter model.
    }
    if (key == 't')
    {
        for (int i = 0; i < 3; i++)
        {
            tmpmax[i] = 0;
            tmpmin[i] = 0; // To reset the value of bounding box.
        }
        pointList.clear();
        faceList.clear();
        faceNormals.clear();
        vertexNormals.clear(); // To clear the value of stored vectors.

        if (teapot == 0)
        {
            loadOBJ("./Mush.obj");
            teapot = 1;
            diagonal();

            DrawFace();
        }

        else if (teapot == 1)
        {
            loadOBJ("./Teapot.obj");
            teapot = 0;
            diagonal();

            DrawFace();
        }
    }
    if (key == 'x')
    {
        float tmpYlight = yLight;
        float tmpZlight = zLight;

        yLight = tmpYlight * cos(ANGLE) - tmpZlight * sin(ANGLE);
        zLight = tmpZlight * cos(ANGLE) + tmpYlight * sin(ANGLE);

        light_position[1] = yLight;// -center[1];
        light_position[2] = zLight;// -center[2];

        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }
    if (key == 'y')
    {
        float tmpXlight = xLight;
        float tmpZlight = zLight;

        xLight = tmpXlight * cos(ANGLE) + tmpZlight * sin(ANGLE);
        zLight = tmpZlight * cos(ANGLE) - tmpXlight * sin(ANGLE);

        light_position[0] = xLight;
        light_position[2] = zLight;

        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }
    if (key == 'z')
    {
        float tmpXlight = xLight;
        float tmpYlight = yLight;

        xLight = tmpXlight * cos(ANGLE) - tmpYlight * sin(ANGLE);
        yLight = tmpXlight * sin(ANGLE) + tmpYlight * cos(ANGLE);

        light_position[0] = xLight;
        light_position[1] = yLight;

        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }

    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, (float)w / (float)h, 0.001, 5000.0);
    glMatrixMode(GL_MODELVIEW);
}

void drawScene()
{
    glEnable(GL_TEXTURE_2D);
    init();
    light();
    DrawFace();
    glDisable(GL_TEXTURE_2D);
    computeBoundingBox();
}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFlush();
    glLoadIdentity();
    drawScene();
    glutSwapBuffers(); //swap the buffers
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("ObjLoader");

    output();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    loadOBJ("./teapot.obj");

    glutKeyboardFunc(keyInput);

    // Register the mouse wheel callback function.
    glutMouseFunc(mouseWheel);

    // Register the mouse motion callback function.
    glutMotionFunc(mouseMotion);


    // Enable color material mode:
    // The ambient and diffuse color of the front faces will track the color set by glColor().
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Create texture ids.
    glGenTextures(1, texture);

    // Load external texture. 
    loadExternalTextures();

    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    diagonal();

    glutMainLoop();
    return 0;
}