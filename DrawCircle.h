#include <iostream>
#include <utility>

using namespace std;

typedef pair<int, int> pii;

class DrawCircle
{
  public:
    void plotPixel( int, int );
    DrawCircle( pii );

  private:
    pii p;
    void circlePoints( void );
    void midpointCircle( void );
};
