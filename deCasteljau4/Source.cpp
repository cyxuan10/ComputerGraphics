//////////////////////////////////////////////////////////////////////////
// deCasteljau4.cpp
//
// This program illustrates de Casteljau's algorithm to create a quadratic
// Bezier curve approximating 4 control points.
//
// Interaction: 
// Press the left/right/up/down arrows to move the control point.
// Press space bar to switch the control point to be moved.
// Press 'a' to toggle between antialiasing on and off.
// Press 'u' / 'i' to decrease/increase the curve parameter u.
// Press 'd' / 's' to decrease/increase the speed of point movement.
// 
// Sumanta Guha
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#  include <GL/glut.h>

using namespace std;

// Begin globals.
static float t = 0.5; // Curve parameter.
static float speed = 0.4; // Speed of control point movement.
static int space = 0; // No. of points.
static int isAntialiased = 0; // Is antialiasing on?
static char theStringBuffer[10]; // String buffer.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Control points.
float ctrlpoints[4][3] = { { -25.0, -30.0, 0.0 },
                           { 25.0, 10.0, 0.0 },
                           { 10.0, 40.0, 0.0 },
                           { -10.0, 30.0, 0.0 } };
// End globals.

// Routine to draw a bitmap character string.
void writeBitmapString(void* font, char* string)
{
    char* c;

    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Routine to convert floating point to char string.
void floatToString(char* destStr, int precision, float val)
{
    snprintf(destStr, sizeof(destStr), "%.2f", val);
    destStr[precision] = '\0';
}

// Initialization routine.
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

// Drawing routine.
void drawScene(void)
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT);

    glColor4f(0.0, 0.0, 0.0, 1.0);
    glLoadIdentity();

    // Write u value.
    glColor3f(0.0, 0.0, 0.0);
    floatToString(theStringBuffer, 4, t);
    glRasterPos3f(-40.0, 40.0, 0.0);
    writeBitmapString((void*)font, (char*)"t = ");
    writeBitmapString((void*)font, theStringBuffer);

    // Write s value.
    glColor3f(0.0, 0.0, 0.0);
    floatToString(theStringBuffer, 4, speed);
    glRasterPos3f(-40.0, 37.0, 0.0);
    writeBitmapString((void*)font, (char*)"speed = ");
    writeBitmapString((void*)font, theStringBuffer);

    glRasterPos3f(-40.0, 34.0, 0.0);
    // Antialiasing control.
    if (isAntialiased)
    {
        glEnable(GL_BLEND); // Enable blending.
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.

        glEnable(GL_LINE_SMOOTH); // Enable line antialiasing.
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // Ask for best line antialiasing.
        glEnable(GL_POINT_SMOOTH);  // Enable point antialiasing.
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Ask for best point antialiasing.
        writeBitmapString((void*)font, (char*)"Antialiasing on!");
    }
    else
    {
        glDisable(GL_BLEND);
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_POINT_SMOOTH);
        writeBitmapString((void*)font, (char*)"Antialiasing off!");
    }

    /* ------------ First lines ------------*/
    // The lines joining the control points.
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i < 4; i++)
        glVertex3fv(&ctrlpoints[i][0]);
    glEnd();

    // The control points as dots.
    glPointSize(8.0);
    glColor3f(0.15, 0.55, 1.0);
    glBegin(GL_POINTS);
    for (i = 0; i < 4; i++)
        glVertex3fv(&ctrlpoints[i][0]);
    glEnd();

    glPointSize(4.0);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    for (i = 0; i < 4; i++)
        glVertex3fv(&ctrlpoints[i][0]);
    glEnd();

    /* ------------ Second lines ------------*/

    // Point size and color.
    glPointSize(5.0);
    glColor3f(0.47, 0.24, 1.0);

    // The point interpolating between the first two control points.
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 2, &ctrlpoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_POINTS);
    glEvalCoord1f(t);
    glEnd();

    // The point interpolating between the second two control points.
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 2, &ctrlpoints[1][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_POINTS);
    glEvalCoord1f(t);
    glEnd();

    // The point interpolating between the last two control points.
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 2, &ctrlpoints[2][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_POINTS);
    glEvalCoord1f(t);
    glEnd();

    float PointOne[3] = { (1 - t) * ctrlpoints[0][0] + t * ctrlpoints[1][0],
                          (1 - t) * ctrlpoints[0][1] + t * ctrlpoints[1][1],
                          0.0 };
    float PointTwo[3] = { (1 - t) * ctrlpoints[1][0] + t * ctrlpoints[2][0],
                          (1 - t) * ctrlpoints[1][1] + t * ctrlpoints[2][1],
                          0.0 };
    float PointThree[3] = { (1 - t) * ctrlpoints[2][0] + t * ctrlpoints[3][0],
                            (1 - t) * ctrlpoints[2][1] + t * ctrlpoints[3][1],
                            0.0 };
    float PointFour[3] = { ((1 - t) * PointOne[0] + t * PointTwo[0]),
                           ((1 - t) * PointOne[1] + t * PointTwo[1]),
                           0.0 };
    float PointFive[3] = { ((1 - t) * PointTwo[0] + t * PointThree[0]),
                           ((1 - t) * PointTwo[1] + t * PointThree[1]),
                           0.0 };

    // The line joining the first two points drawn above.
    glBegin(GL_LINES);
    glVertex3f(PointOne[0], PointOne[1], PointOne[2]);
    glVertex3f(PointTwo[0], PointTwo[1], PointTwo[2]);
    glEnd();

    // The line joining the second two points drawn above.
    glBegin(GL_LINES);
    glVertex3f(PointTwo[0], PointTwo[1], PointTwo[2]);
    glVertex3f(PointThree[0], PointThree[1], PointThree[2]);
    glEnd();

    /* ------------ Third line ------------*/

    // Color.
    glColor3f(0.0, 0.5, 0.0);

    // The point interpolating between the first two points drawn above.
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrlpoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_POINTS);
    glEvalCoord1f(t);
    glEnd();

    // The point interpolating between the last two points drawn above.
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrlpoints[1][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_POINTS);
    glEvalCoord1f(t);
    glEnd();

    // The line joining the two points drawn above.
    glBegin(GL_LINES);
    glVertex3f(PointFour[0], PointFour[1], PointFour[2]);
    glVertex3f(PointFive[0], PointFive[1], PointFive[2]);
    glEnd();

    // The point interpolating between the two points drawn above.
    glColor3f(1.0, 0.0, 0.0);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    glEvalCoord1f(t);
    glEnd();

    // The Bezier curve is drawn from 0 to parameter value.
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(2.0);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glMapGrid1f(100, 0.0, 1.0);
    glEvalMesh1(GL_LINE, 0, (int)(t * 100));
    glLineWidth(1.0);

    // Show point number.
    glColor3f(0.0, 0.0, 0.0);

    glRasterPos3f(ctrlpoints[0][0], ctrlpoints[0][1] - 3, 0.0);
    writeBitmapString((void*)font, (char*)"P0");

    glRasterPos3f(ctrlpoints[1][0], ctrlpoints[1][1] - 3, 0.0);
    writeBitmapString((void*)font, (char*)"P1");

    glRasterPos3f(ctrlpoints[2][0], ctrlpoints[2][1] - 3, 0.0);
    writeBitmapString((void*)font, (char*)"P2");

    glRasterPos3f(ctrlpoints[3][0], ctrlpoints[3][1] - 3, 0.0);
    writeBitmapString((void*)font, (char*)"P3");

    glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a':
        ++isAntialiased %= 2;
        break;
    case 's':
        speed += 0.2; // Increase speed of control point movement.
        break;
    case 'd':
        if (speed > 0.2)
            speed -= 0.2; // Decrease speed of control point movement.
        break;
    case 't':
        if (t > 0.0) t -= 0.02;
        break;
    case 'y':
        if (t < 0.98) t += 0.02;
        break;
    case 'r':
        ctrlpoints[0][0] = -25.0;
        ctrlpoints[0][1] = -30.0;
        ctrlpoints[1][0] = 25.0;
        ctrlpoints[1][1] = 10.0;
        ctrlpoints[2][0] = 10.0;
        ctrlpoints[2][1] = 40.0;
        ctrlpoints[3][0] = -10.0;
        ctrlpoints[3][1] = 30.0;
        t = 0.5;
        speed = 0.4;
        break;
    case ' ':
        space++;
        if (space == 4)
            space = 0;
        break;
    default:
        break;
    }
    //printf("im at %d, x = %.3f\n", space, ctrlpoints[space][0]);
    glutPostRedisplay();
}

void PovSpace(int c)
{
    switch (space)
    {
    case 0:
        ctrlpoints[0][c] += speed;
        break;
    case 1:
        ctrlpoints[1][c] += speed;
        break;
    case 2:
        ctrlpoints[2][c] += speed;
        break;
    case 3:
        ctrlpoints[3][c] += speed;
        break;
    default:
        break;
    }
}

void NegSpace(int c)
{
    switch (space)
    {
    case 0:
        ctrlpoints[0][c] -= speed;
        break;
    case 1:
        ctrlpoints[1][c] -= speed;
        break;
    case 2:
        ctrlpoints[2][c] -= speed;
        break;
    case 3:
        ctrlpoints[3][c] -= speed;
        break;
    default:
        break;
    }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
        NegSpace(0);
    if (key == GLUT_KEY_RIGHT)
        PovSpace(0);
    if (key == GLUT_KEY_DOWN)
        NegSpace(1);
    if (key == GLUT_KEY_UP)
        PovSpace(1);

    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Press the left/right/up/down arrows to move the control point." << endl
        << "Press space bar to switch the control point to be moved." << endl
        << "Press 'a' to toggle between antialiasing on and off." << endl
        << "Press 'u' / 'i' to decrease/increase the curve parameter u." << endl
        << "Press 'd' / 's' to decrease/increase the speed of point movement." << endl;
}

// Main routine.
int main(int argc, char** argv)
{
    printInteraction();
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("deCasteljau4.cpp");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);

    setup();

    glutMainLoop();
}
