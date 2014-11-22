editable-bezier
===============
  Silver-Bezier repo contains the implementation of de Casteljau's algorithm to create a Bezier curve. The points are taken at run-time interactively from the user. To generate the curve, any of the top 50 pixels is selected. The curve can be intuitively editted, by selecting any of the control points (right-click) and then selecting a new point.
  
  The program also creates a surface of revolution generates an OFF file - "two.off" which can be viewed in 3-d in any of the mesh-viewers.
  
<h2>Source Files </h2>
  bezier1.cpp - contains the main code to run the program. It is an OpenGL implementation. It also contains the de Casteljau's algorithm to draw Bezier curve.
  
  MeshDS.cpp and MeshDS.h - These files create a class MeshDS to implement a mesh data structure to handle vertices and faces of the 3-d surface revolution.
  
  Point.cpp - It contains a class which represents a 3-d vertex.
