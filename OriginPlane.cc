#include "OriginPlane.h"
#include "TrevorCalc3.h"

void OriginPlane::calculateNormal() {
	n = crossProduct(std::vector<float> {p2.x, p2.y, p2.z}, std::vector<float> {p3.x, p3.y, p3.z});
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
 * Return the std::vector of intersetion between this plane
 * and another plane.
 * @param p
 *  OriginPlane object with normal std::vector to compare for intersection std::vector
 */
std::vector<float> OriginPlane::intersectionVector(OriginPlane *p) {
	return crossProduct(p->n, n);
}

/**
 * Print to the given buffer the normal std::vector of this plane.
 */
void OriginPlane::printN(std::stringstream ss) {
	ss << printf("%f, %f, %f", n[0], n[1], n[2]) << std::endl;
}
