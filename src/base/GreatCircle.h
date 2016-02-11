// copyright 2016 Trevor Simonton
#ifndef SRC_BASE_GREATCIRCLE_H_
#define SRC_BASE_GREATCIRCLE_H_

#include "TrevorCalc3.h"
#include <vector>
#include <memory>
#include <sstream>
#include "Point.h"
#include "OriginPlane.h"

using std::vector;
using std::stringstream;
using std::shared_ptr;


// Representation of a single great circle
class GreatCircle {
 public:
  // list of intersections with other great circles
  vector<shared_ptr<Point> > intersections;

  // each great circle corresponds to an origin plane
  OriginPlane *plane;

  // we only need 2 points to define our circle
  Point p1;
  Point p2;

  // We need 2 points on the circle for construction.
  GreatCircle(Point a, Point b):
    plane(new OriginPlane(a, b)), p1(a), p2(b) {}

  ~GreatCircle() { delete plane; }

  // Each pair of great circles has exactly 2 intersections.
  // Use this function to fine the 2 intersections with this circle
  // and another.
  vector<shared_ptr<Point> > twoCircleIntersections(GreatCircle *c2);

  // To draw this circle on a sphere, we need 5 nicely spaced points.
  // The first and fifth points are the same point.
  // This helps us draw 4 segments around a circle, which is what we need
  // when drawing the circle in d3.js
  void printFivePoints(stringstream *ss);
};

#endif  // SRC_BASE_GREATCIRCLE_H_
