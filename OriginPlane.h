#ifndef ORIGIN_PLANE_H_
#define ORIGIN_PLANE_H_

#include "TrevorCalc2.h"
#include <vector>
#include "Point.h"
#include <sstream>

/**
 * This represents a plane in 3d space passing through 
 * the (0,0,0) origin
 */
class OriginPlane {
private:
	/**
	 * calculate and set the normal std::vector of this plane.
	 */
	void calculateNormal();
public:
	// any 3 points make a plane
	Point p1;
	Point p2;
	Point p3;
	
	// n is the normal std::vector
	std::vector<float> n;
	
	/**
	 * Only 2 points are needed for construction. The 3rd point
	 * is the (0,0,0) origin.
	 */
	OriginPlane(Point p2, Point p3);
	
	/**
	 * Return the std::vector of intersetion between this plane
	 * and another plane.
	 * @param p
	 *  OriginPlane object with normal std::vector to compare for intersection std::vector
	 */
	std::vector<float> intersectionVector(OriginPlane *p);
	
	/**
	 * Print to the given buffer the normal std::vector of this plane.
	 */
	void printN(std::stringstream ss);
};

#endif // ORIGIN_PLANE_H_