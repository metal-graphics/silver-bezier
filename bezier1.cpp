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

    const Point & operator=(const Point &rPoint)
    {
      x = rPoint.x;
      y = rPoint.y;
      z = rPoint.z;

      return *this;
    }
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


/* draws the bezier curve given by initial, final and control points */
void drawBezier(Point p1, Point p2)
{
  glColor3f(1,1,1);
  glLineWidth(2.5);
  glBegin(GL_LINES);
    glVertex2f(p1.x,p1.y);
    glVertex2f(p2.x,p2.y);
  glEnd();

  glFlush();
}


/* calculates the factorial of a number x */
float factorial(int x)
{
  float fact = 1;
  int p=1;
  if(x==0 || x==1)
    return 1;
  else
  {
    while(p<=x)
    {
      fact*=p;
      p++;
    }
  }

  return fact;
}


/* finds the binomial coefficient nCf */
float binomial(int f, int n)
{
  float com = factorial(n)/(factorial(f)*factorial(n-f));
  return com;
}


Point bernstein(float t, int num)
{
  Point n;
  float nx = 0;
  float ny = 0;

  for(int i=0; i<=num; i++)
  {
    nx += points[i].x*binomial(i,num)*pow(t,i)*pow((1-t),(num-i));
    ny += points[i].y*binomial(i,num)*pow(t,i)*pow((1-t),(num-i));
  }

  n.setxy(nx,ny);

  return n;
}


/* Handler function to take inputs interactively */
void myMouse(int button, int state, int x, int y)
{
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    if(y>50)
    {
      Point p = Point();
      p.setxy( (float)x, (float)(480-y) );
      points.pb(p);
      numPoints++;

      drawDot(x, 480 - y);
    }
    if(y<=50)
    {
      glColor3f(1.0,1.0,1.0);

      for(int i=0; i<numPoints-1;i++)
      {
        drawLine(points[i],points[i+1]);
      }

      Point M, P = points[0];
      for(float t = 0.0;t <= 1.0; t += 0.001)
      {
        M = bernstein(t, numPoints-1);
        drawBezier(P,M);
        P=M;
      }

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
