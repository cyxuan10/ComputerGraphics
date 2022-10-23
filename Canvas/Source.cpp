////////////////////////////////////////////////////////////////////////////////////        
// canvas.cpp
//
// This program allows the user to draw simple shapes on a canvas.
//
// Interaction:
// Left click on a box on the left to select a primitive.
// Then left click on the drawing area: 
// Once for point or stamp.
// Twice for line, rectangle, circle or hexagon.
// Thrice for triangle.
// Infinite left clicks until a middle click stops the polyline.
// Right click for menu options.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////// 

#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath>

#include <GL/freeglut.h>

using namespace std;

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define POLYLINE 4
#define TRIANGLE 5
#define CIRCLE 6
#define HEXAGON 7
#define STAMP 8
#define PI 3.14159265358979324
#define NUMBERPRIMITIVES 8

// Use the STL extension of C++.
using namespace std;

// Globals.
static int width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point.
static int lineWidth = 1; // Default line-width.
static int color; // Color of point.
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int tempX, tempY; // Co-ordinates of clicked point.
static int R; //Radius of circle.
static int numVertices = 2000; // Number of vertices on circle.
static int tempX2, tempY2; // Co-ordinates of second clicked point.
static int isGrid = 1; // Is there grid?
static float r, g, b; // Red, Green, Blue of color.
static bool fll; // Fill in color


//////////////////////////////////////////////////////////////////////     
// The classes
//////////////////////////////////////////////////////////////////////  


/* ------------------------------Point class---------------------------------- */

class Point
{
public:
	Point(int xVal, int yVal, float pointSize, float red, float green, float blue)
	{
		x = xVal; y = yVal; size = pointSize;
		r = red; g = green; b = blue;
	}
	void drawPoint(); // Function to draw a point.
private:
	int x, y; // x and y co-ordinates of point.
	float size; // Size of point.
	float r, g, b; // Red, Blue, Green of color of point.
};

// Function to draw a point.
void Point::drawPoint()
{
	glPointSize(size);
	glColor3f(r, g, b);
	glBegin(GL_POINTS);
	glVertex3f(x, y, 0.0);
	glEnd();
}

// Vector of points.
vector<Point> points;

// Iterator to traverse a Point array.
vector<Point>::iterator pointsIterator;

/* ------------------------------Line class---------------------------------- */

class Line
{
public:
	Line(int x1Val, int y1Val, int x2Val, int y2Val, int line_width, float red, float green, float blue)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val; lw = line_width;
		r = red; g = green; b = blue;
	}
	void drawLine();
private:
	int x1, y1, x2, y2, lw; // x and y co-ordinates of endpoints.
							// lw = linewidth
	float r, g, b; // Red, Blue, Green of color of line.
};

// Function to draw a line.
void Line::drawLine()
{
	glLineWidth(lw);
	glColor3f(r, g, b);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0.0);
	glVertex3f(x2, y2, 0.0);
	glEnd();
}

// Vector of lines.
vector<Line> lines;

// Iterator to traverse a Line array.
vector<Line>::iterator linesIterator;

/* ------------------------------Rectangle class---------------------------------- */

class Rect
{
public:
	Rect(int x1Val, int y1Val, int x2Val, int y2Val, int lineWidth,
		float red, float green, float blue, bool fillColour)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val; lw = lineWidth;
		r = red; g = green; b = blue; fill = fillColour;
	}
	void drawRectangle();
private:
	int x1, y1, x2, y2, lw; // x and y co-ordinates of diagonally opposite vertices.
							// lw = linewidth
	float r, g, b; // Red, Blue, Green of color of rectangle.
	bool fill; // Fill color into the primitive.
};

// Function to draw a rectangle.
void Rect::drawRectangle()
{
	glLineWidth(lw);
	if (fill == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (fill == 1) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(r, g, b);
	glRectf(x1, y1, x2, y2);
}

// Vector of rectangles.
vector<Rect> rectangles;

// Iterator to traverse a Rect array.
vector<Rect>::iterator rectanglesIterator;

/* ------------------------------Polyline class---------------------------------- */

class PolyLine
{
public:
	PolyLine(int x1Val, int y1Val, int x2Val, int y2Val, int lineWidth, float red, float green, float blue)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val; lw = lineWidth;
		r = red; g = green; b = blue;
	}
	void drawPolyLine();
private:
	int x1, y1, x2, y2, lw; // x and y co-ordinates of two points of polyline.
							// lw = linewidth
	float r, g, b; // Red, Blue, Green of color of polyline.
};

// Vector of polyline.
vector <PolyLine> polyLines;

// Iterator to traverse a Polyline array.
vector<PolyLine>::iterator polyLinesIterator;

void PolyLine::drawPolyLine()
{
	glLineWidth(lw);
	glColor3f(r, g, b);
	glBegin(GL_LINE_STRIP);
	glVertex3f(x1, y1, 0.0);
	glVertex3f(x2, y2, 0.0);
	glEnd();
}

/* ------------------------------Triangle class---------------------------------- */

class Triangle
{
public:
	Triangle(int x1Val, int y1Val, int x2Val, int y2Val, int x3Val,
		int y3Val, int lineWidth, float red, float green, float blue,
		bool fillColor)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val; x3 = x3Val; y3 = y3Val; lw = lineWidth;
		r = red; g = green; b = blue; fill = fillColor;
	}
	void drawTriangle();
private:
	int x1, y1, x2, y2, x3, y3, lw; // x and y co-ordinates of 3 points of triangle.
									// lw = linewidth
	float r, g, b; // Red, Blue, Green of color of triangle.
	bool fill; // Fill color into the primitive.
};

// Function to draw a triangle.
void Triangle::drawTriangle()
{
	glLineWidth(lineWidth);
	if (fill == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (fill == 1) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLES);
	glVertex3f(x1, y1, 0);
	glVertex3f(x2, y2, 0);
	glVertex3f(x3, y3, 0);
	glEnd();
}

// Vector of triangle.
vector<Triangle> triangles;

// Iterator to traverse a Triangle array.
vector<Triangle>::iterator trianglesIterator;

/* ------------------------------Circle class---------------------------------- */

class Circle
{
public:
	Circle(int x1Val, int y1Val, int x2Val, int y2Val, int line_width,
		float red, float green, float blue, bool fillColor)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val; lw = line_width;
		r = red; g = green; b = blue; fill = fillColor;
	}
	void drawCircle();
private:
	int x1, y1, x2, y2, lw; // x and y co-ordinates of 2 points of circle.
							// lw = linewidth
	float r, g, b; // Red, Blue, Green of color of circle.
	bool fill; // Fill color into the primitive.
};

// Function to calculate the radius from x1, y1 to x2, y2 (FOR CIRCLE AND HEXAGON)
float distance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

// Function to draw a circle.
void Circle::drawCircle()
{
	float t = 0; // Angle parameter.

	R = distance(x1, y1, x2, y2);

	glLineWidth(lw);

	if (fill == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(r, g, b);

		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < numVertices; ++i)
		{
			glVertex3f(x1 + R * cos(t), y1 + R * sin(t), 0.0);
			t += 2 * PI / numVertices;
		}
		glEnd();
	}
	else if (fill == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(r, g, b);

		glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i < numVertices; ++i)
		{
			glVertex3f(x1 + R * cos(t), y1 + R * sin(t), 0.0);
			t += 2 * PI / numVertices;
		}
		glEnd();
	}

	glFlush();
}

// Vector of circle.
vector<Circle> circles;

// Iterator to traverse a Circle array.
vector<Circle>::iterator circlesIterator;

/* ------------------------------Hexagon class---------------------------------- */
class Hexagon
{
public:
	Hexagon(int x1Val, int y1Val, int x2Val, int y2Val, int line_width,
		float red, float green, float blue, bool fillColor)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val; lw = line_width;
		r = red; g = green; b = blue; fill = fillColor;
	}
	void drawHexagon();
private:
	int x1, y1, x2, y2, lw; // x and y co-ordinates of 2 points of hexagon.
							// lw = linewidth
	float r, g, b; // Red, Blue, Green of color of hexagon.
	bool fill; // Fill color into the primitive.
};

// Function to draw a hexagon.
void Hexagon::drawHexagon()
{
	float t = 0; // Angle parameter.

	R = distance(x1, y1, x2, y2);

	glLineWidth(lw);

	if (fill == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(r, g, b);

		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < 6; ++i)
		{
			glVertex3f(x1 + R * cos(t), y1 + R * sin(t), 0.0);
			t += 2 * PI / 6;

		}
		glEnd();
	}
	else if (fill == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(r, g, b);

		glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i < 6; ++i)
		{
			glVertex3f(x1 + R * cos(t), y1 + R * sin(t), 0.0);
			t += 2 * PI / 6;
		}
		glEnd();
	}

	glFlush();
}

// Vector of hexagon.
vector<Hexagon> hexagons;

// Iterator to traverse a Hexagon array.
vector<Hexagon>::iterator hexagonsIterator;

/* ------------------------------Stamp class---------------------------------- */
class Stamp
{
public:
	Stamp(int x1Val, int y1Val, int faceType, float red, float green, float blue, bool fillColor)
	{
		x1 = x1Val; y1 = y1Val; facetype = faceType;
		r = red; g = green; b = blue; fill = fillColor;
	}
	void drawStamp();
private:
	int x1, y1, facetype; // x and y co-ordinates of stamp.
	float r, g, b; // Red, Blue, Green of color of stamp.
	bool fill; // Fill color into the primitive.
};

// Function to draw a stamp.
void Stamp::drawStamp()
{
	float t = 0; // Angle parameter.

	// Outline of Stamp.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0, 0, 0);
	glLineWidth(1);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < numVertices; ++i)
	{
		glVertex3f(x1 + 30 * cos(t), y1 + 30 * sin(t), 0.0);
		t += 2 * PI / numVertices;
	}
	glEnd();
	
	// Color of Stamp.
	if (fill == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(r, g, b);

		glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i < numVertices; ++i)
		{
			glVertex3f(x1 + 29.5 * cos(t), y1 + 29.5 * sin(t), 0.0);
			t += 2 * PI / numVertices;
		}
		glEnd();
	}

	// Eyes
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	glVertex3f(x1 - 12.5, y1 + 8, 0.0);
	glVertex3f(x1 + 12.5, y1 + 8, 0.0);
	glEnd();

	glColor3f(0, 0, 0);
	switch (facetype) {
		// Smile face
	case 0:
		glBegin(GL_LINE_STRIP);
		glVertex3f(x1 - 12.5, y1 - 5, 0.0);
		glVertex3f(x1, y1 - 15, 0.0);
		glVertex3f(x1 + 12.5, y1 - 5, 0.0);
		glEnd();
		break;

		// Poker face
	case 1:
		glBegin(GL_LINE_STRIP);
		glVertex3f(x1 - 12.5, y1 - 10, 0.0);
		glVertex3f(x1, y1 - 10, 0.0);
		glVertex3f(x1 + 12.5, y1 - 10, 0.0);
		glEnd();
		break;

		// Sad face
	case 2:
		glBegin(GL_LINE_STRIP);
		glVertex3f(x1 - 12.5, y1 - 15, 0.0);
		glVertex3f(x1, y1 - 5, 0.0);
		glVertex3f(x1 + 12.5, y1 - 15, 0.0);
		glEnd();
		break;
	}


	glFlush();
}

// Vector of stamp.
vector<Stamp> stamps;

// Iterator to traverse a Stamp array.
vector<Stamp>::iterator stampsIterator;

vector<int> primitiveArray; // An array to store the order of drew primitive.
vector<int>::iterator primitiveArrayIterator;

// Function to let latest primitive be on the top.
void draw() {
	primitiveArrayIterator = primitiveArray.begin();
	pointsIterator = points.begin();
	linesIterator = lines.begin();
	rectanglesIterator = rectangles.begin();
	polyLinesIterator = polyLines.begin();
	trianglesIterator = triangles.begin();
	circlesIterator = circles.begin();
	hexagonsIterator = hexagons.begin();
	stampsIterator = stamps.begin();

	int i = 0;
	int tmp_prim;

	while (primitiveArrayIterator != primitiveArray.end()) {
		tmp_prim = primitiveArray.at(i);
		switch (tmp_prim) {
		case 0:
			pointsIterator->drawPoint();
			pointsIterator++;
			break;
		case 1:
			linesIterator->drawLine();
			linesIterator++;
			break;
		case 2:
			rectanglesIterator->drawRectangle();
			rectanglesIterator++;
			break;
		case 3:
			polyLinesIterator->drawPolyLine();
			polyLinesIterator++;
			break;
		case 4:
			trianglesIterator->drawTriangle();
			trianglesIterator++;
			break;
		case 5:
			circlesIterator->drawCircle();
			circlesIterator++;
			break;
		case 6:
			hexagonsIterator->drawHexagon();
			hexagonsIterator++;
			break;
		case 7:
			stampsIterator->drawStamp();
			stampsIterator++;
			break;
		}
		i++;
		primitiveArrayIterator++;
	}
}

//////////////////////////////////////////////////////////////////////     
// The functions to draw selection box in left selection area.
//////////////////////////////////////////////////////////////////////  


/* ------------------------------Point selection box---------------------------------- */

void drawPointSelectionBox(void)
{
	if (primitive == POINT) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.9 * height, 0.1 * width, height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.9 * height, 0.1 * width, height);

	// Draw point icon.
	glPointSize(3.0);
	glColor3f(r, g, b);
	glBegin(GL_POINTS);
	glVertex3f(0.05 * width, 0.95 * height, 0.0);
	glEnd();
}

/* ------------------------------Line selection box---------------------------------- */
void drawLineSelectionBox(void)
{
	if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.8 * height, 0.1 * width, 0.9 * height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.8 * height, 0.1 * width, 0.9 * height);

	// Draw line icon.
	glColor3f(r, g, b);
	glBegin(GL_LINES);
	glVertex3f(0.025 * width, 0.825 * height, 0.0);
	glVertex3f(0.075 * width, 0.875 * height, 0.0);
	glEnd();
}

/* ------------------------------Rectangle selection box---------------------------------- */
void drawRectangleSelectionBox(void)
{
	if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);

	// Draw rectangle icon.
	glColor3f(r, g, b);
	if (fll == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (fll == 1) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.025 * width, 0.725 * height, 0.075 * width, 0.775 * height);

}

/* ------------------------------Polyline selection box---------------------------------- */
void drawPolyLineSelectionBox(void)
{
	if (primitive == POLYLINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.6 * height, 0.1 * width, 0.7 * height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.6 * height, 0.1 * width, 0.7 * height);

	// Draw polyline icon.
	glColor3f(r, g, b);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.02 * width, 0.625 * height, 0.0);
	glVertex3f(0.04 * width, 0.675 * height, 0.0);
	glVertex3f(0.06 * width, 0.625 * height, 0.0);
	glVertex3f(0.08 * width, 0.675 * height, 0.0);
	glEnd();

}

/* ------------------------------Triangle selection box---------------------------------- */
void drawTriangleSelectionBox(void)
{
	if (primitive == TRIANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.5 * height, 0.1 * width, 0.6 * height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.5 * height, 0.1 * width, 0.6 * height);

	// Draw triangle icon.
	glColor3f(r, g, b);
	if (fll == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (fll == 1) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.05 * width, 0.575 * height, 0.0);
	glVertex3f(0.025 * width, 0.525 * height, 0.0);
	glVertex3f(0.075 * width, 0.525 * height, 0.0);
	glEnd();

}

/* ------------------------------Circle selection box---------------------------------- */
void drawCircleSelectionBox(void)
{
	if (primitive == CIRCLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.4 * height, 0.1 * width, 0.5 * height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.4 * height, 0.1 * width, 0.5 * height);

	// Draw circle icon.
	float t = 0; // Angle parameter.

	if (fll == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(r, g, b);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < numVertices; ++i)
		{
			glVertex3f(0.05 * width + 15 * cos(t), 0.45 * height + 15 * sin(t), 0.0);
			t += 2 * PI / numVertices;
		}
	}
	else if (fll == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(r, g, b);
		glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i < numVertices; ++i)
		{
			glVertex3f(0.05 * width + 15 * cos(t), 0.45 * height + 15 * sin(t), 0.0);
			t += 2 * PI / numVertices;
		}
	}
	glEnd();

	glFlush();
}

/* ------------------------------Hexagon selection box---------------------------------- */
void drawHexagonSelectionBox(void)
{
	if (primitive == HEXAGON) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.3 * height, 0.1 * width, 0.4 * height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.3 * height, 0.1 * width, 0.4 * height);

	// Draw hexagon icon.
	float t = 0; // Angle parameter.

	if (fll == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(r, g, b);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < 6; ++i)
		{
			glVertex3f(0.05 * width + 15 * cos(t), 0.35 * height + 15 * sin(t), 0.0);
			t += 2 * PI / 6;
		}
	}
	else if (fll == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(r, g, b);
		glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i < 6; ++i)
		{
			glVertex3f(0.05 * width + 15 * cos(t), 0.35 * height + 15 * sin(t), 0.0);
			t += 2 * PI / 6;
		}
	}
	glEnd();

	glFlush();
}

/* ------------------------------ Random smiley stamp selection box---------------------------------- */
void drawStampSelectionBox(void)
{
	if (primitive == STAMP) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.2 * height, 0.1 * width, 0.3 * height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.2 * height, 0.1 * width, 0.3 * height);

	// Draw stamp icon.
	float t = 0; // Angle parameter.

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < numVertices; ++i)
	{
		glVertex3f(0.05 * width + 15 * cos(t), 0.25 * height + 15 * sin(t), 0.0);
		t += 2 * PI / numVertices;
	}
	glEnd();

	glPointSize(3.0);

	// Eyes
	glBegin(GL_POINTS);
	glVertex3f(0.05 * width - 6.25, 0.25 * height + 4, 0.0);
	glVertex3f(0.05 * width + 6.25, 0.25 * height + 4, 0.0);
	glEnd();

	// Mouth
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.05 * width - 6.25, 0.25 * height - 4, 0.0);
	glVertex3f(0.05 * width, 0.25 * height - 8, 0.0);
	glVertex3f(0.05 * width + 6.25, 0.25 * height - 4, 0.0);
	glEnd();

	glFlush();
}

/* ------------------------------Unused part---------------------------------- */
void drawInactiveArea(void)
{
	glColor3f(0.6, 0.6, 0.6);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.0, 0.1 * width, (1 - NUMBERPRIMITIVES * 0.1) * height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.0, 0.1 * width, (1 - NUMBERPRIMITIVES * 0.1) * height);
}


//////////////////////////////////////////////////////////////////////     
// Other functions.
////////////////////////////////////////////////////////////////////// 


// Function to draw temporary point.
void drawTempPoint(void)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glVertex3f(tempX, tempY, 0.0);
	glEnd();
}

// Function to draw second temporary point.
void drawSecTempPoint(void)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glVertex3f(tempX2, tempY2, 0.0);
	glEnd();
}

// Function to draw a grid.
void drawGrid(void)
{
	int i;

	glLineWidth(1);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x5555);
	glColor3f(0.75, 0.75, 0.75);

	glBegin(GL_LINES);
	for (i = 2; i <= 9; i++)
	{
		glVertex3f(i * 0.1 * width, 0.0, 0.0);
		glVertex3f(i * 0.1 * width, height, 0.0);
	}
	for (i = 1; i <= 9; i++)
	{
		glVertex3f(0.1 * width, i * 0.1 * height, 0.0);
		glVertex3f(width, i * 0.1 * height, 0.0);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	draw();

	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(1);

	drawPointSelectionBox();
	drawLineSelectionBox();
	drawRectangleSelectionBox();
	drawPolyLineSelectionBox();
	drawTriangleSelectionBox();
	drawCircleSelectionBox();
	drawHexagonSelectionBox();
	drawStampSelectionBox();
	drawInactiveArea();

	if (((primitive == LINE) || (primitive == RECTANGLE) || (primitive == POLYLINE) || (primitive == TRIANGLE) || (primitive == CIRCLE) 
		|| (primitive == HEXAGON) || (primitive == STAMP)) && (pointCount == 1)) drawTempPoint();
	if (((primitive == TRIANGLE)) && (pointCount == 2)) {
		drawTempPoint();
		drawSecTempPoint();
	}

	if (isGrid) drawGrid();

	glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
	if (y < (1 - NUMBERPRIMITIVES * 0.1) * height) primitive = INACTIVE;
	else if (y < (1 - 7 * 0.1) * height) primitive = STAMP;
	else if (y < (1 - 6 * 0.1) * height) primitive = HEXAGON;
	else if (y < (1 - 5 * 0.1) * height) primitive = CIRCLE;
	else if (y < (1 - 4 * 0.1) * height) primitive = TRIANGLE;
	else if (y < (1 - 3 * 0.1) * height) primitive = POLYLINE;
	else if (y < (1 - 2 * 0.1) * height) primitive = RECTANGLE;
	else if (y < (1 - 1 * 0.1) * height) primitive = LINE;
	else primitive = POINT;
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		y = height - y; // Correct from mouse to OpenGL co-ordinates.

		// Click outside canvas - do nothing.
		if (x < 0 || x > width || y < 0 || y > height);

		// Click in left selection area.
		else if (x < 0.1 * width)
		{
			pickPrimitive(y);
			pointCount = 0;
		}

		// Click in canvas.
		else
		{
			if (primitive == POINT) {
				points.push_back(Point(x, y, pointSize, r, g, b));
				primitiveArray.push_back(0);
			}
			/* In the clicks of canvas, the xand y refer to the latest clicks,
			   and every tempX and tempY will temporary store the coordinates of x and y,
			   making them to be the location of first click.*/

			else if (primitive == LINE)
			{
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else
				{
					lines.push_back(Line(tempX, tempY, x, y, lineWidth, r, g, b));
					primitiveArray.push_back(1); // To push the primitive into the array, which able to let the latest primitive stay at front.
					pointCount = 0;
				}
			}
			else if (primitive == RECTANGLE)
			{
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else
				{
					rectangles.push_back(Rect(tempX, tempY, x, y, lineWidth, r, g, b, fll));
					primitiveArray.push_back(2); // To push the primitive into the array, which able to let the latest primitive stay at front.
					pointCount = 0;
				}
			}
			else if (primitive == POLYLINE)
			{
				if (pointCount == 0)
				{
					tempX = x; tempY = y;

				}
				else
				{
					tempX2 = tempX; tempY2 = tempY;
					tempX = x; tempY = y;
					polyLines.push_back(PolyLine(tempX2, tempY2, tempX, tempY, lineWidth, r, g, b));
					primitiveArray.push_back(3); // To push the primitive into the array, which able to let the latest primitive stay at front.
				}

				pointCount++;
			}

			else if (primitive == TRIANGLE)
			{
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else if (pointCount == 1)
				{
					tempX2 = x; tempY2 = y;
					pointCount++;
				}
				else
				{
					triangles.push_back(Triangle(tempX, tempY, tempX2, tempY2, x, y, lineWidth, r, g, b, fll));
					primitiveArray.push_back(4); // To push the primitive into the array, which able to let the latest primitive stay at front.
					pointCount = 0;
				}
			}
			else if (primitive == CIRCLE)
			{
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else
				{
					circles.push_back(Circle(tempX, tempY, x, y, lineWidth, r, g, b, fll));
					primitiveArray.push_back(5); // To push the primitive into the array, which able to let the latest primitive stay at front.
					pointCount = 0;
				}
			}
			else if (primitive == HEXAGON)
			{
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else
				{
					hexagons.push_back(Hexagon(tempX, tempY, x, y, lineWidth, r, g, b, fll));
					primitiveArray.push_back(6); // To push the primitive into the array, which able to let the latest primitive stay at front.
					pointCount = 0;
				}
			}
			else if (primitive == STAMP)
			{
			    // Generate random float numbers in order to make random colors.
				float n1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); 
				float n2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float n3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

				stamps.push_back(Stamp(x, y, rand() % 3, n1, n2, n2, fll));
				primitiveArray.push_back(7); // To push the primitive into the array, which able to let the latest primitive stay at front.
			}
		}
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		if (primitive == POLYLINE) pointCount = 0; // If middle button is clicked, the polyline ends.
	}

	glutPostRedisplay();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set viewing box dimensions equal to window dimensions.
	glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

	// Pass the size of the OpenGL window to globals.
	width = w;
	height = h;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
	points.clear();
	lines.clear();
	rectangles.clear();
	polyLines.clear();
	triangles.clear();
	circles.clear();
	hexagons.clear();
	primitiveArray.clear();
	primitive = INACTIVE;
	pointCount = 0;
}


//////////////////////////////////////////////////////////////////////     
// The right click functions.
////////////////////////////////////////////////////////////////////// 


// The right button menu callback function.
void rightMenu(int id)
{
	if (id == 1) // second option = clear all shapes
	{
		clearAll();
		glutPostRedisplay();
	}
	if (id == 15) exit(0); // last option = exit the canvas
}

// The grid sub-menu callback function.
void grid_menu(int id)
{
	if (id == 2) isGrid = 1;
	if (id == 3) isGrid = 0;
	glutPostRedisplay();
}

// The line-width sub-menu callback function.
void line_width_menu(int id)
{
	if (id == 4)
	{
		lineWidth = 1;
		pointSize = 3.0;
	}
	if (id == 5)
	{
		lineWidth = 2;
		pointSize = 4.0;
	}
	if (id == 6)
	{
		lineWidth = 3;
		pointSize = 5.0;
	}
	if (id == 7)
	{
		lineWidth = 4;
		pointSize = 6.0;
	}

	glutPostRedisplay();
}

void color_menu(int id)
{
	if (id == 8) {
		r = 0;
		g = 0;
		b = 0; // Black
	}
	if (id == 9) {
		r = 255;
		g = 129;
		b = 0; // Orange
	}
	if (id == 10) {
		r = 212;
		g = 175;
		b = 55; // Gold
	}
	if (id == 11) {
		r = 9;
		g = 195;
		b = 219; // Ocean
	}
	r = r / 255;
	g = g / 255;
	b = b / 255;
	glutPostRedisplay();
}

void filled_menu(int id)
{
	if (id == 12) fll = 0;
	if (id == 13) fll = 1;
	glutPostRedisplay();
}


// Function to create menu.
void makeMenu(void)
{
	int grid_sub_menu;
	int line_width_sub_menu;
	int color_sub_menu;
	int filled_sub_menu;

	grid_sub_menu = glutCreateMenu(grid_menu);
	glutAddMenuEntry("On", 2);
	glutAddMenuEntry("Off", 3);

	line_width_sub_menu = glutCreateMenu(line_width_menu);
	glutAddMenuEntry("Size = 1", 4);
	glutAddMenuEntry("Size = 2", 5);
	glutAddMenuEntry("Size = 3", 6);
	glutAddMenuEntry("Size = 4", 7);

	color_sub_menu = glutCreateMenu(color_menu);
	glutAddMenuEntry("Black", 8);
	glutAddMenuEntry("Orange", 9);
	glutAddMenuEntry("Gold", 10);
	glutAddMenuEntry("Ocean", 11);

	filled_sub_menu = glutCreateMenu(filled_menu);
	glutAddMenuEntry("Outlined", 12);
	glutAddMenuEntry("Filled", 13);

	glutCreateMenu(rightMenu);
	glutAddSubMenu("Grid", grid_sub_menu); // Add first option and "ON" "OFF" submenus.
	glutAddMenuEntry("Clear", 1);
	glutAddSubMenu("Line-width", line_width_sub_menu); // Add third option and four types of font size submenu.
	glutAddSubMenu("Line-color", color_sub_menu); // Add forth option and four types of color submenu.
	glutAddSubMenu("Fill color", filled_sub_menu); // Add fifth option and outlined/filled option submenu.
	glutAddMenuEntry("Quit", 15);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	makeMenu(); // Create menu.
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	cout << "Interaction:" << endl;
	cout << "Left click on a box on the left to select a primitive." << endl
		<< "Then left click on the drawing area: " << endl
		<< "\tOnce for point or stamp." << endl
		<< "\tTwice for line, rectangle, circle or hexagon. " << endl
		<< "\tThrice for triangle." << endl
		<< "\tInfinite left clicks until a middle click stops the polyline." << endl
		<< "Right click for menu options." << endl;
}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("canvas_Chu You Xuan_CST2002030.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouseControl);

	setup();

	glutMainLoop();
}