class Point
{
  public:
    double x, y, z;
    void setxyz(double x2, double y2, double z2)
    {
      x = x2;
      y = y2;
      z = z2;
    }

    const Point & operator=(const Point &newPoint)
    {
      x = newPoint.x;
      y = newPoint.y;
      z = newPoint.z;

      return *this;
    }
};
