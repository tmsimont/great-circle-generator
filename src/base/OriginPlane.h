// copyright 2016 Trevor Simonton
#ifndef SRC_BASE_ORIGINPLANE_H_
#define SRC_BASE_ORIGINPLANE_H_

#include "TrevorCalc3.h"
#include <vector>
#include "Point.h"
#include <sstream>

using std::vector;
using std::stringstream;

// This represents a plane in 3d space passing through
// the (0,0,0) origin
class OriginPlane {
 private:
  void calculateNormal();

 public:
  // any 3 points make a plane
  Point p1;
  Point p2;
  Point p3;

  // n is the normal vector
  vector<float> n;

  // Only 2 points are needed for construction. The 3rd point
  // is the (0,0,0) origin.
  OriginPlane(Point p2, Point p3);

  // Return the vector of intersetion between this plane
  // and another plane.
  // Pass in the OriginPlane object with normal vector
  // to compare for intersection vector
  vector<float> intersectionVector(OriginPlane *p);

  // Print to the given buffer the normal vector of this plane.
  void printN(stringstream ss);
};

#endif  // SRC_BASE_ORIGINPLANE_H_

