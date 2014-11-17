/*----------------------------------------------------------------------------*/
/*                                                                            */
/*   Copyright (c) 2014 Metal-Graphics                                        */
/*   All rights reserved.                                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#include <iostream>
#include <cstdlib>
#include <GL/glut.h>
#include <windows.h>
#include <cmath>
#include <vector>
#include <utility>

#define mp make_pair
#define pb push_back
#define all(c) c.begin(),c.end()
#define rep(i,n) for(i=0; i<n; ++i)

using namespace std;

//Global Variables
int numPoints = 0;


class Point
{
  public:
    float x, y, z;

    void setxy(float x2, float y2)
    {
      x = x2;
      y = y2;
    }
/*
    const Point & operator=(const Point &rPoint)
    {
      x = rPoint.x;
      y = rPoint.y;
      z = rPoint.z;

      return *this;
    }*/
};

vector<Point> points;


/* Initialize OpenGL Graphics */
void myInit()
{
  glClearColor(0.0,0.0,0.0,0.0);            // Set background color to black
  glColor3f(1.0,0.0,0.0);
  glPointSize(4.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0,640.0,0.0,480.0);
}


/* drawDot draws a point of size 4 */
void drawDot(int x, int y)
{
  glBegin(GL_POINTS);
    glVertex2i(x,y);
  glEnd();
  glFlush();
}


/* drawLine draws a line between points p1 and p2 */
void drawLine(Point p1, Point p2)
{
  glBegin(GL_LINES);
    glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p2.x, p2.y, p2.z);
  glEnd();
  glFlush();
}


/* this function draws the bezier curve between points a and d, b,c are the control points */
Point drawBezier(Point A, Point B, Point C, Point D, double t)
{
  Point P;

  P.x = pow((1 - t), 3) * A.x + 3 * t * pow((1 -t), 2) * B.x + 3 * (1-t) * pow(t, 2)* C.x + pow (t, 3)* D.x;
  P.y = pow((1 - t), 3) * A.y + 3 * t * pow((1 -t), 2) * B.y + 3 * (1-t) * pow(t, 2)* C.y + pow (t, 3)* D.y;
  P.z = pow((1 - t), 3) * A.z + 3 * t * pow((1 -t), 2) * B.z + 3 * (1-t) * pow(t, 2)* C.z + pow (t, 3)* D.z;

  return P;
}


void myMouse(int button, int state, int x, int y)
{
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    Point p = Point();
    p.setxy( (float)x, (float)(480-y) );
    points.pb(p);
    numPoints++;

    drawDot(x, 480 - y);

    if(numPoints == 4)
    {
      int size= points.size();
      glColor3f(1.0,1.0,1.0);

      drawLine(points[size-4], points[size-3]);
      drawLine(points[size-3], points[size-2]);
      drawLine(points[size-2], points[size-1]);

      Point POld = points[size-4];
      for(double t = 0.0;t <= 1.0; t += 0.01)
      {
        Point P = drawBezier(points[size-4], points[size-3], points[size-2], points[size-1],  t);
        drawLine(POld, P);
        POld = P;
      }
      glColor3f(1.0,0.0,0.0);
      numPoints = 0;
    }
  }
}


/*
  Handler for window-repaint event. Called back when the window first appears and
  whenever the window needs to be re-painted.
*/
void myDisplay()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);                      // Initialize GLUT
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowSize(640,480);                // Set the window's initial width & height
  glutInitWindowPosition(100,150);            // Position the window's initial top-left corner
  glutCreateWindow("Bezier Curve");           // Create window with the given title
  glutMouseFunc(myMouse);                     // Register callback handler for mouse interaction
  glutDisplayFunc(myDisplay);                 // Register callback handler for window re-paint event
  myInit();                                   // Our own OpenGL initialization
  glutMainLoop();                             // Enter the infinite event-processing loop

  return 0;
}
