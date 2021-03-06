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
#include "DrawCircle.h"
#include "MeshDS.h"

#define mp make_pair
#define pb push_back
#define all(c) c.begin(),c.end()
#define rep(i,n) for(i=0; i<n; ++i)

using namespace std;

//Global Variables
int numPoints = 0;
bool draw = false; // if true, then only the curve is calculated
int click = 0; //to edit the curve
int newPoint;
static const double PI = 3.1416;

vector<Point> points; //to store the input points
vector<Point> samplePoints; // to store the sampled points of the curve


/* Initialize OpenGL Graphics */
void myInit()
{
  glClearColor(0.0,0.0,0.0,0.0);            // Set background color to black
  glColor3f(1.0,0.0,0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0,640.0,0.0,480.0);
}


/* drawDot draws a point of size 4 */
void drawDot(int x, int y)
{
  glColor3f(1.0,0.0,0.0);
  glBegin(GL_POINTS);
    glVertex2i(x,y);
  glEnd();
  glFlush();
}


/* drawLine draws a line between points p1 and p2 */
void drawLine(Point p1, Point p2)
{
  glColor3f(1.0,1.0,0.0);
  glLineWidth(1);
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
double factorial(int x)
{
  double fact = 1;
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
double binomial(int f, int n)
{
  double com = factorial(n)/(factorial(f)*factorial(n-f));
  return com;
}


/*curve in bernstein form---curve = (sum i=0ton)(control points*bernstein basis polynomial)*/
Point bernstein(double t, int num)
{
  Point n;
  double nx = 0;
  double ny = 0;

  // bernstein basis polynomial, b[i,n](t)= (nCi)*((1-t)^(n-i))*(t^i)
  for(int i=0; i<=num; i++)
  {
    nx += points[i].x*binomial(i,num)*pow(t,i)*pow((1-t),(num-i));
    ny += points[i].y*binomial(i,num)*pow(t,i)*pow((1-t),(num-i));
  }

  n.setxyz(nx,ny,0.0f);

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
      p.setxyz( (double)x, (double)(480-y), 0.0f );
      points.pb(p);
      numPoints++; //number of points selected so far

      DrawCircle dc = DrawCircle(mp(x,480-y), 10); //draws a circle of rad=10 around the point
      drawDot(x, 480 - y);
    }
    if(y<=50) //if any of the top 50 pixels are selected, then start drawing the curve
    {
      draw = true;
      glutPostRedisplay();
    }
  }
  else if(button == GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
  {
    if(click==0)
    {
      int sz = points.size();
      for(int i=0; i<sz; ++i)//check if the selected point already exists in the vector
      {
        int dx1 = points[i].x - x;
        int dy1 = points[i].y - (480-y);
        double rad = sqrt(dx1*dx1 + dy1*dy1);

        if(rad<=10) //mouse click corresponds to a particular point in the vector list
        {
          newPoint = i;
          click = 1;
          break;
        }
      }
    }
    else if(click==1)//select the new position of the point
    {
      points[newPoint].setxyz( (double)x, (double)(480-y), 0.0f );
      click = 0;
      draw = true;
      glutPostRedisplay();//new curve
    }
  }
  else if(button==GLUT_MIDDLE_BUTTON && state==GLUT_DOWN ) // parsing an input file
  {
    cout<<"parsing the OFF file\n";

    /* parser for off format files */
    MeshDS md, nd;
    md = MeshDS();
    nd = md.meshObject(); //nd is the mesh data structure

    cout<<"parsed the OFF input file\nvalues stored in object of MeshDS class\n";
  }
}


/*function creates a MeshDS object of the points entered by user and creates a surface of revolution*/
void convertToOFF()
{
  MeshDS mds = MeshDS();
  Point pds = Point();

  //to create the surface of revoltion about y-axis
  for(double theta = 0.0; theta<=2.0*PI; theta += PI/12.0)
  {
    for(int i=0; i<samplePoints.size(); ++i)
    {
      double newx = samplePoints[i].x * cos(theta);//value theta taken in radians
      double newy = samplePoints[i].y; //rotated about y-axis, so it doesn't change
      double newz = (-samplePoints[i].x) * sin(theta);

      pds.setxyz(newx,newy,newz);
      mds.initVertices(pds); //add the new point to vertices vector
    }
  }

  vector<int> vface; //temporary vector for each face
  for(int i=0; i<24; ++i)
  {
    for(int j=0; j<samplePoints.size()-1; ++j)
    {
      vface.clear(); //clear the vector for each iteration
      vface.pb(11*i + j); //each face is a triangle
      vface.pb(11*i + j+1);
      vface.pb(11*(i+1) + j);
      mds.initFaces(vface);
    }

    //four points- two triangle faces are created to represent them
    for(int j=0; j<samplePoints.size()-1; ++j)
    {
      vface.clear();
      vface.pb(11*(i+1) + j);
      vface.pb(11*i + j+1);
      vface.pb(11*(i+1) + j+1);
      mds.initFaces(vface);
    }
  }

  mds.makeOFFfile();
}


/*
  Handler for window-repaint event. Called back when the window first appears and
  whenever the window needs to be re-painted.
*/
void myDisplay()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1.0,1.0,1.0);

  if(draw==true)
  {
    samplePoints.clear();
    for(int i=0; i<numPoints-1;i++)
    {
      DrawCircle dc = DrawCircle(mp(points[i].x,points[i].y), 10);
      drawLine(points[i],points[i+1]);
    }
    DrawCircle dc1 = DrawCircle(mp(points[numPoints-1].x, points[numPoints-1].y), 10);

    int tempt = 0; //counter to sample the bezier curve at 0.1
    Point M, P = points[0];
    for(double t = 0.0;t <= 1.0; t += 0.001)
    {
      M = bernstein(t, numPoints-1);
      drawBezier(P,M);
      if(tempt%100==0)
      {
        samplePoints.pb(M); //curve sampled uniformly at 0.1
      }
      P=M;
      tempt++;
    }
    samplePoints.pb(points[numPoints-1]);
    draw = false;

    for(int i=0; i<samplePoints.size(); ++i)
    {
      DrawCircle dc2 = DrawCircle(mp(samplePoints[i].x, samplePoints[i].y), 5);
    }
    convertToOFF();
  }

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
