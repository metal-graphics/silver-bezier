#include <iostream>
#include <cstdlib>
#include <GL/glut.h>
#include <time.h>
#include <utility>
#include "DrawCircle.h"

pii origin;

void DrawCircle::plotPixel( int x, int y )
{
  glColor3f(1.0,0.0,0.0);
  
  glBegin(GL_POINTS);

  glPointSize(10.0);

  glVertex2i( x, y );

  glEnd();

  glFlush();
}


//constructor
DrawCircle::DrawCircle( pii a )
{
  p = a;

  midpointCircle();
}


void DrawCircle::midpointCircle()
{
  int x = 0;
  int radius = 10;
  int y = radius;
  int d = 1- radius;

  origin = make_pair( x, y );
  circlePoints( );

  while( y>x )
  {
    if( d<0 )
    {
      d+= (2*x+3);
    }
    else
    {
      d+=(2*(x-y)+5);

      y--;
    }
    x++;

    origin = make_pair(x,y);
    circlePoints();
  }
}


void DrawCircle::circlePoints()
{
  plotPixel( origin.first+ p.first, origin.second+p.second );

  plotPixel( origin.second+p.first, origin.first+p.second );

  plotPixel( origin.second+p.first, -origin.first+p.second );

  plotPixel( origin.first+p.first, -origin.second+p.second );

  plotPixel( -origin.first+p.first, -origin.second+p.second );

  plotPixel( -origin.second+p.first, -origin.first+p.second );

  plotPixel( -origin.second+p.first, origin.first+p.second );

  plotPixel( -origin.first+p.first, origin.second+p.second );
}





