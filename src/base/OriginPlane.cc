// copyright 2016 Trevor Simonton
#include "base/OriginPlane.h"
#include "base/TrevorCalc3.h"

using std::endl;
using std::stringstream;

void OriginPlane::calculateNormal() {
  n = crossProduct(
    vector<float> {p2.x, p2.y, p2.z}, vector<float> {p3.x, p3.y, p3.z});
}

/**
 * Only 2 points are needed for construction. The 3rd point
 * is the (0,0,0) origin.
 */
OriginPlane::OriginPlane(Point p2, Point p3) {
  p1 = Point();
  this->p2 = p2;
  this->p3 = p3;
  calculateNormal();
}

/**
 * Return the vector of intersetion between this plane
 * and another plane.
 * @param p
 *  OriginPlane object with normal vector to compare for intersection vector
 */
vector<float> OriginPlane::intersectionVector(OriginPlane *p) {
  return crossProduct(p->n, n);
}

/**
 * Print to the given buffer the normal vector of this plane.
 */
void OriginPlane::printN(stringstream ss) {
  ss << printf("%f, %f, %f", n[0], n[1], n[2]) << endl;
}
