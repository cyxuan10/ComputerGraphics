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
#define WindowWidth 1920
#define WindowHeight 1080

struct object {
    vector <float> pointList;
    vector <int> faceList;
    int textureID;
};

vector <object> objectList;

//globals
char ch = '1';
static float g[3];
static int f[3];
static float i1, i2, i3; // three faces.
static unsigned int texture[10]; // Array of texture ids.
static int id = 0; // Currently displayed texture id.
static float centerX = WindowWidth / 2 - 200;
static float centerY = WindowHeight / 2 - 145;
static float xEye = 90.0, yEye = 0.0, zEye = 0.0; // Camera's position. Values are default direction.
static float xMove = 8.0, yMove = 0.0, zMove = 0.0; // Camera's direction. Values are default direction.
static bool lighting = 1; // Day or Night?
static float spotDirection[] = { xEye, -yEye, -zEye }; // Spotlight direction.
static float lightPos[] = { xMove, yMove, zMove, 1 };
static int color = 0;

// Draw faces for the vertices indices.
void DrawFace(vector<float> pointList, vector<int> faceList, int id)
{
    glBindTexture(GL_TEXTURE_2D, texture[id]);
    for (int i = 0; i < faceList.size(); i += 3)
    {
        glBegin(GL_POLYGON);
        i1 = faceList[i] - 1; //need to -1 because index of vector starts from 0
        glTexCoord2f(1.0, 1.0);
        glVertex3f(pointList[i1 * 3], pointList[i1 * 3 + 1], pointList[i1 * 3 + 2]);

        i2 = faceList[i + 1] - 1;
        glTexCoord2f(0.6, 1.0);
        glVertex3f(pointList[i2 * 3], pointList[i2 * 3 + 1], pointList[i2 * 3 + 2]);

        i3 = faceList[i + 2] - 1;
        glTexCoord2f(0.0, 0.0);
        glVertex3f(pointList[i3 * 3], pointList[i3 * 3 + 1], pointList[i3 * 3 + 2]);
        glEnd();
    }
}

void loadOBJ(std::string fileName, int txtID)
{
    vector <float> pointList_tmp;
    vector <int> faceList_tmp;

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
            for (int i = 0; i < 3; i++)
            {
                pointList_tmp.push_back(g[i]);
            }
        }
        if (line[0] == 'f') {
            lineStream >> prefix >> f[0] >> f[1] >> f[2];
            for (int i = 0; i < 3; i++)
            {
                faceList_tmp.push_back(f[i]);
            }
        }
    }

    object obj_tmp = {
        pointList_tmp,
        faceList_tmp,
        txtID
    };

    objectList.push_back(obj_tmp);
}

// Load external textures.
void loadExternalTextures()
{
    // Local storage for bmp image data.
    BitMapFile* image[10];

    // Load the image.
    image[0] = getbmp("white.bmp");

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

    image[1] = getbmp("purple.bmp");

    // Create texture object texture[1]. 
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    image[2] = getbmp("yellow.bmp");

    // Create texture object texture[2]. 
    glBindTexture(GL_TEXTURE_2D, texture[2]);

    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[2]->sizeX, image[2]->sizeY, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image[2]->data);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    image[3] = getbmp("beige.bmp");

    // Create texture object texture[3]. 
    glBindTexture(GL_TEXTURE_2D, texture[3]);

    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[3]->sizeX, image[3]->sizeY, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image[3]->data);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    image[4] = getbmp("red.bmp");

    // Create texture object texture[4]. 
    glBindTexture(GL_TEXTURE_2D, texture[4]);

    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[4]->sizeX, image[4]->sizeY, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image[4]->data);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    image[5] = getbmp("wood.bmp");

    // Create texture object texture[5]. 
    glBindTexture(GL_TEXTURE_2D, texture[5]);

    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[5]->sizeX, image[5]->sizeY, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image[5]->data);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    image[6] = getbmp("blue.bmp");

    // Create texture object texture[6]. 
    glBindTexture(GL_TEXTURE_2D, texture[6]);

    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[6]->sizeX, image[6]->sizeY, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image[6]->data);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    image[7] = getbmp("leaf.bmp");

    // Create texture object texture[7]. 
    glBindTexture(GL_TEXTURE_2D, texture[7]);

    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[7]->sizeX, image[7]->sizeY, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image[7]->data);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    image[8] = getbmp("brown.bmp");

    // Create texture object texture[8]. 
    glBindTexture(GL_TEXTURE_2D, texture[8]);

    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[8]->sizeX, image[8]->sizeY, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image[8]->data);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    image[9] = getbmp("navy.bmp");

    // Create texture object texture[9]. 
    glBindTexture(GL_TEXTURE_2D, texture[9]);

    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[9]->sizeX, image[9]->sizeY, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image[9]->data);

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

    glShadeModel(GL_FLAT);

    float globAmb[] = { 0.1, 0.1, 0.1, 1.0 };

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

    // Day light

    // Light property vectors.
    float light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    float light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    float light_specular[] = { 1.0, 0.27, 0.0, 1.0 };
    float light_position[] = { -160.0, -55.0, 200.0, 1.0 };

    // Light properties.
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Night light

    // Light property vectors.
    float lightAmb[] = { 1.0, 1.0, 1.0, 1.0 };
    float lightSpec[] = { 1.0, 1.0, 1.0, 1.0 };
    float lightDif[] = { 1.0, 1.0, 1.0, 1.0 };

    if (color == 0)
    {

    }
    else if (color == 1)
    {
        lightAmb[1] = 0.0; lightAmb[2] = 0.0;
        lightDif[1] = 0.0; lightDif[2] = 0.0;
        lightSpec[1] = 0.0; lightSpec[2] = 0.0;
    }
    else if (color == 2)
    {
        lightAmb[0] = 0.0; lightAmb[2] = 0.0;
        lightDif[0] = 0.0; lightDif[2] = 0.0;
        lightSpec[0] = 0.0; lightSpec[2] = 0.0;
    }
    else if (color == 3)
    {
        lightAmb[0] = 0.0; lightAmb[1] = 0.0;
        lightDif[0] = 0.0; lightDif[1] = 0.0;
        lightSpec[0] = 0.0; lightSpec[1] = 0.0;
    }

    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 7.5);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDif);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpec);

    if (lighting)
    {
        glDisable(GL_LIGHT1); // Disable Night light
        glEnable(GL_LIGHT0); // Enable Day light
    }
    else
    {
        glDisable(GL_LIGHT0); // Disable Day light
        glEnable(GL_LIGHT1); // Enable Night light 
    }

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.
    // Enable two-sided lighting.

    glutPostRedisplay();
}

void init(void)
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100 };
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
}

// Track mouse motion
void mouseMotion(int x, int y)
{
    int deltaX, deltaY;
    float alphaAux, betaAux;
    float r = sqrt(pow(xEye, 2) + pow(yEye, 2) + pow(zEye, 2)); // r = sqrt(xEye^2 + yEye^2 + zEye^2)

    deltaX = x - centerX;       // deltaX = 0 initially
    deltaY = y - centerY;       // deltaY = 0 initially

    // left mouse button: move camera
    alphaAux = deltaX * 0.2;
    betaAux = deltaY * 0.2;

    xEye = cos((0 + alphaAux) * PI / 180) * cos((0 + 1 * betaAux) * PI / 180);
    yEye = sin(betaAux * PI / 180);
    zEye = sin(alphaAux * PI / 180) * cos(betaAux * PI / 180);

    spotDirection[0] = -xEye;
    spotDirection[1] = -yEye;
    spotDirection[2] = -zEye;

    glutPostRedisplay();
}

// Callback routine for non-ASCII key entry.
void keyInput(unsigned char key, int x, int y)
{
    if (key == 'r')
    {
        xMove = 8.0, yMove = 0.0, zMove = 0.0; // Recenter model.
        xEye = 90.0, yEye = 0.0, zEye = 0.0; // Recenter model.

        spotDirection[0] = xEye;
        spotDirection[1] = -yEye;
        spotDirection[2] = -zEye;

        lightPos[0] = xMove;
        lightPos[1] = yMove;
        lightPos[2] = zMove;
    }

    glutPostRedisplay();
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
    // if (xEye < -75)      : go back

    // if (xEye < 45)       : either left or right
    //      if (zEye < 0)   : left
    //      if (zEye > 0)   : right

    // else                 : default

    switch (key) {
        // UP ARROW KEY
    case GLUT_KEY_UP:
        if (xEye < -0.75) {
            if (xMove < 20.5) xMove += 0.5;      // BACK
        }
        else if (xEye < 0.45) {
            if (zEye < 0) {
                if (zMove < 20.5) zMove += 0.5;  // LEFT
            }
            else {
                if (zMove > -6.5) zMove -= 0.5; // RIGHT
            }
        }
        else {
            if (xMove > -6.5) xMove -= 0.5;     // DEFAULT: FRONT
        }
        break;
        // DOWN ARROW KEY
    case GLUT_KEY_DOWN:
        if (xEye < -0.75) {
            if (xMove > -6.5) xMove -= 0.5;     // FRONT
        }
        else if (xEye < 0.45) {
            if (zEye < 0) {
                if (zMove > -6.5) zMove -= 0.5; // RIGHT
            }
            else {
                if (zMove < 20.5) zMove += 0.5;  // LEFT
            }
        }
        else {
            if (xMove < 20.5) xMove += 0.5;      // DEFAULT: BACK
        }
        break;
        // RIGHT ARROW KEY
    case GLUT_KEY_RIGHT:
        if (xEye < -0.75) {
            if (zMove < 20.5) zMove += 0.5;      // LEFT
        }
        else if (xEye < 0.45) {
            if (zEye < 0) {
                if (xMove > -6.5) xMove -= 0.5; // FRONT
            }
            else {
                if (xMove < 20.5) xMove += 0.5;  // BACK
            }
        }
        else {
            if (zMove > -6.5) zMove -= 0.5;     // DEFAULT: RIGHT
        }
        break;
        // LEFT ARROW KEY
    case GLUT_KEY_LEFT:
        if (xEye < -0.75) {
            if (zMove > -6.5) zMove -= 0.5;     // RIGHT
        }
        else if (xEye < 0.45) {
            if (zEye < 0) {
                if (xMove < 20.5) xMove += 0.5;  // BACK
            }
            else {
                if (xMove > -6.5) xMove -= 0.5; // FRONT
            }
        }
        else {
            if (zMove < 20.5) zMove += 0.5;      // DEFAULT: LEFT
        }
        break;
        // PAGE UP KEY
    case GLUT_KEY_PAGE_UP:
        if (yMove < 1.0) yMove += 1.0;
        break;
        // PAGE DOWN KEY
    case GLUT_KEY_PAGE_DOWN:
        if (yMove > -1.0) yMove -= 1.0;
        break;
    }
    lightPos[0] = xMove;
    lightPos[1] = yMove;
    lightPos[2] = zMove;

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

//////////////////////////////////////////////////////////////////////     
// The right click functions.
////////////////////////////////////////////////////////////////////// 


// The right button menu callback function.
void rightMenu(int id)
{
    if (id == 15) exit(0); // last option = exit the canvas
}

// The light sub-menu callback function.
void light_menu(int id)
{
    if (id == 0)
    {
        lighting = 1;
    }
    if (id == 1)
    {
        lighting = 0;
    }

    glutPostRedisplay();
}

// The light sub-menu callback function.
void color_menu(int id)
{
    if (id == 2)
    {
        color = 0;
    }
    if (id == 3)
    {
        color = 1;
    }
    if (id == 4)
    {
        color = 2;
    }
    if (id == 5)
    {
        color = 3;
    }

    glutPostRedisplay();
}

// Function to create menu.
void makeMenu(void)
{
    int light_sub_menu;
    int color_sub_menu;

    light_sub_menu = glutCreateMenu(light_menu);
    glutAddMenuEntry("Day mode", 0);
    glutAddMenuEntry("Night mode", 1);

    color_sub_menu = glutCreateMenu(color_menu);
    glutAddMenuEntry("White", 2);
    glutAddMenuEntry("Red", 3);
    glutAddMenuEntry("Green", 4);
    glutAddMenuEntry("Blue", 5);

    glutCreateMenu(rightMenu);
    glutAddSubMenu("Lighting", light_sub_menu); // Add first option and "Day mode" "Night mode" submenus.
    glutAddSubMenu("Night mode color", color_sub_menu); // Add Second option and color submenus.
    glutAddMenuEntry("Quit", 15);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void drawScene()
{
    gluLookAt(xMove, yMove, zMove, -xEye + xMove, -yEye + yMove, -zEye + zMove, 0.0, 1.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    init();
    light();
    for (int i = 0; i < objectList.size(); i++)
    {
        object obj_tmp = objectList[i];
        DrawFace(obj_tmp.pointList, obj_tmp.faceList, obj_tmp.textureID);
    }
    glDisable(GL_TEXTURE_2D);
}

void translatedObj()
{
    for (float x = -1.5; x < 2; x += 0.5)
    {
        glPushMatrix();
        glTranslatef(5.5 + x, abs(x), 20.25);
        glutWireCube(0.5);
        glPopMatrix();
    }

    for (float x = 0; x < 3.5; x += 1.5)
    {
        glPushMatrix();
        glTranslatef(7 - x, 2, 20.25);
        glutWireCube(0.5);
        glPopMatrix();
    }

    for (float x = 0; x < 1; x += 0.5)
    {
        glPushMatrix();
        glTranslatef(6 + x, 2.5, 20.25);
        glutWireCube(0.5);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(4.5 + x, 2.5, 20.25);
        glutWireCube(0.5);
        glPopMatrix();
    }
}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFlush();
    glLoadIdentity();
    drawScene();
    translatedObj();
    glutFullScreen();
    glutSwapBuffers(); //swap the buffers
}

int main(int argc, char** argv)
{
    //lighting = 0;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WindowWidth, WindowHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("ObjLoader");
    glutWarpPointer(centerX, centerY); // Cursor location

    // Create texture ids.
    glGenTextures(10, texture);

    // Load external texture. 
    loadExternalTextures();

    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    // 0 - white , 1 - purple , 2 - yellow , 3 - beige , 4 - red 
    // 5 - wood , 6 - blue, 7 - leaf, 8 - brown, 9 - navy

    loadOBJ("./plane.obj", 2); // 0
    loadOBJ("./ceiling.obj", 2); // 1
    loadOBJ("./plane2.obj", 3); // 2
    loadOBJ("./plane3.obj", 3); // 3
    loadOBJ("./fridge.obj", 4); // 4
    loadOBJ("./fridge2.obj", 0); // 5
    loadOBJ("./sphere.obj", 3); // 6
    loadOBJ("./cup.obj", 0); // 7
    loadOBJ("./drawer.obj", 1); // 8
    loadOBJ("./table.obj", 5); // 9
    loadOBJ("./window.obj", 0); // 10
    loadOBJ("./humanoid.obj", 0); // 11
    loadOBJ("./pot.obj", 4); // 12
    loadOBJ("./leaves.obj", 7); // 13
    loadOBJ("./branch.obj", 8); // 14
    loadOBJ("./board.obj", 8); // 15
    loadOBJ("./tap.obj", 0); // 16
    loadOBJ("./sink.obj", 5); // 17
    loadOBJ("./cabinet.obj", 5); // 18
    loadOBJ("./book1.obj", 6); // 19
    loadOBJ("./book2.obj", 4); // 20
    loadOBJ("./sky.obj", 6); // 21

    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);

    // Register the mouse motion callback function.
    glutPassiveMotionFunc(mouseMotion);

    makeMenu();
    glutMainLoop();

    return 0;
}