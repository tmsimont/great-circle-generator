// copyright 2016 Trevor Simonton
#ifndef SRC_BASE_POINT_H_
#define SRC_BASE_POINT_H_

#include <memory>
#include <vector>
#include <sstream>
#include "base/TrevorCalc3.h"

using std::vector;
using std::endl;
using std::stringstream;
using std::shared_ptr;
using std::weak_ptr;

// Point represents a set of coordinates in Cartesian and Polar
// coordinates that describes a point on the surface of a sphere.
// This point supports conversion from Cartesian to Polar, and
// can be output as d3.js globe friendly JSON format.
class Point {
 public:
  int id;
  float theta;
  float phi;
  float x;
  float y;
  float z;
  vector<weak_ptr<Point> > neighbors;

  // Point is at origin by default
  Point() : theta(0), phi(0), x(0), y(0), z(0) {}

  // Given spherical coordinates, build point and populate Cartesian
  Point(float theta, float phi);

  // Given Cartesian coordinates, build point and populate spherical
  Point(float x, float y, float z);

  // Get the haversine distance to another point
  float distanceTo(shared_ptr<Point> other);

  void print(stringstream *ss);

  // The JSON output here is designed to work with the coordinate
  // system used by d3.js
  void printJSON(stringstream *ss);
};

#endif  // SRC_BASE_POINT_H_
