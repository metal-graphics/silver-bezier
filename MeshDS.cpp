/*----------------------------------------------------------------------------*/
/*                                                                            */
/*   Copyright (c) 2014 Metal-Graphics                                        */
/*   All rights reserved.                                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#include "MeshDS.h"

char fileName[] = "one.off";
char outputFile[] = "two.off";
/* MeshDS is parser for OFF format files */
/* MeshDS takes input from OFF format files and returns a MeshDS object */

MeshDS::MeshDS() //constructor
{

}

void MeshDS::initVertices( Point p ) //initializes vertices and forms a vector
{
  vertices.push_back(p);
}

//forms a data structure for vertices... a and b are vertex numbers(in vertices vector)
void MeshDS::initEdges(int a, int b )
{
  edges.push_back(make_pair(a,b));
}

//it forms a ds for faces. vector size at each vector index denotes number of vertices for each face
void MeshDS::initFaces(vector<int> f)
{
  faces.push_back(f);
}

MeshDS MeshDS::meshObject()
{
  MeshDS obj = MeshDS();

  ifstream input( fileName ); // off file input

  if(input.is_open())
  {
    string format;
    getline(input, format);

    /*if(format.size()>3 || format[0]!='O' || format[1]!='F' || format[2]!='F')
    {
      cout<<"file is not in OFF format\n";
      return NULL;
    }
    */

    int numberVertices, numberEdges, numberFaces;

    input>> numberVertices >> numberFaces >> numberEdges;

    double tempx, tempy, tempz;
    for(int i=0; i < numberVertices; ++i)
    {
      input>> tempx >> tempy >> tempz;
      Point p = Point();
      p.setxyz(tempx,tempy,tempz);
      obj.initVertices(p);
    }

    int data, n; //n is teh number of vertices for each face, data is teh vertex number
    for(int i=0; i< numberFaces; ++i)
    {
      input >> n;
      vector<int> face; // size of teh this vector gives the number of vertices forthat face
      for(int j=0; j<n; ++j)
      {
        input >> data;
        face.push_back(data); //the vector face contains the vertex numbers reqd to make that face
      }
      obj.initFaces(face);
    }

    input.close();
  }

  return obj;
}


void MeshDS::makeOFFfile()
{
  ofstream output(outputFile);

  output << "OFF\n";

  int numberVertices = vertices.size();
  int numberFaces = faces.size();

  output << numberVertices << " " << numberFaces << " " << 0 << "\n";

  for(int i=0; i<numberVertices; ++i)
  {
    output << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << "\n";
  }

  int faceSize;

  for(int i=0; i<numberFaces; ++i)
  {
    faceSize = faces[i].size();

    output << faceSize << " ";
    for(int j=0; j<faceSize; ++j)
    {
      output << faces[i][j] << " ";
    }
    output << "\n";
  }

  cout<<"OFF file generated\n";
}
