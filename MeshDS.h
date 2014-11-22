/*----------------------------------------------------------------------------*/
/*                                                                            */
/*   Copyright (c) 2014 Metal-Graphics                                        */
/*   All rights reserved.                                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#include <iostream>
#include <vector>
#include <cstdlib>
#include <utility>
#include <fstream>
#include "Point.cpp"

using namespace std;

class MeshDS
{ 
  public:
    vector< Point > vertices;
    vector< pair<int, int> > edges;
    vector< vector<int> > faces;
    MeshDS();
    void initVertices(Point);
    void initEdges(int, int);
    void initFaces(vector<int>);
    MeshDS meshObject();
    void makeOFFfile();
};


