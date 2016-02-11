// copyright 2016 Trevor Simonton
#include "base/GreatCircle.h"
#include "base/TrevorCalc3.h"

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::stringstream;

vector<shared_ptr<Point> >
GreatCircle::twoCircleIntersections(GreatCircle *c2) {
  // use the line intersection of the circle's planes
  vector<float> v = plane->intersectionVector(c2->plane);
  v = unitVector(v);
  // use the unit vector that is the intersection vector to generate 1 point
  shared_ptr<Point> i1 = make_shared<Point>(v[0], v[1], v[2]);
  // rotate to opposite side of the circle for other point
  shared_ptr<Point> i2 = make_shared<Point>(
    i1->theta + PI, 2 * PI - (i1->phi + PI));
  return vector<shared_ptr<Point> > {i1, i2};
}

void GreatCircle::printFivePoints(stringstream *ss) {
  // a vector that is cross product of p1's position vector
  // and the plane's normal is 90 degrees rotated along great circle from p1
  // (it is orthogonal to p1 and to the circle's plane's normal...)
  vector<float> doublyOrthoVector = crossProduct(
    vector<float>{p1.x, p1.y, p1.z}, plane->n);
  doublyOrthoVector = unitVector(doublyOrthoVector);

  // build a point from that doubly orthognal cross product vector
  Point pd = Point(
    doublyOrthoVector[0], doublyOrthoVector[1], doublyOrthoVector[2]);

  // build points opposite to our magic cross product and the given p1 point
  Point oppositeP1 = Point(p1.theta + PI, 2 * PI - (p1.phi + PI));
  Point oppositeP2 = Point(pd.theta + PI, 2 * PI - (pd.phi + PI));

  // we now print out 4 nicely spaced out points for d3.js
  p1.printJSON(ss);
  *ss << ",";
  pd.printJSON(ss);
  *ss << ",";
  oppositeP1.printJSON(ss);
  *ss << ",";
  oppositeP2.printJSON(ss);
  *ss << ",";
  p1.printJSON(ss);
}
