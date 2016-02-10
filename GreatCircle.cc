#include <iostream>
#include "GreatCircle.h"
#include "TrevorCalc2.h"

/**
 * Each pair of great circles has exactly 2 intersections.
 * Use this function to fine the 2 intersections with this circle
 * and another.
 * @param c2
 *   The other circle to use to find 2 intersections
 */
std::vector<std::shared_ptr<Point> > GreatCircle::twoCircleIntersections(GreatCircle *c2) {
	// use the line intersection of the circle's planes
	std::vector<float> v = plane->intersectionVector(c2->plane);
	v = unitVector(v);
	// use the unit std::vector that is the intersection std::vector to generate 1 point
	std::shared_ptr<Point> i1 = std::make_shared<Point>(v[0], v[1], v[2]);
	// rotate to opposite side of the circle for other point
	std::shared_ptr<Point> i2 = std::make_shared<Point>(i1->theta + PI, 2 * PI - (i1->phi + PI));
	return std::vector<std::shared_ptr<Point> > {i1, i2};
}


/**
 * To draw this circle on a sphere, we need 5 nicely spaced points.
 * The first and fifth points are the same point.
 * This helps us draw 4 segments around a circle, which is what we need
 * when drawing the circle in d3.js
 */
void GreatCircle::printFivePoints(std::stringstream *ss) {

	// a std::vector that is cross product of p1's position std::vector 
	// and the plane's normal is 90 degrees rotated along great circle from p1
	// (it is orthogonal to p1 and to the circle's plane's normal...)
	std::vector<float> doublyOrthoVector = crossProduct(std::vector<float>{p1.x, p1.y, p1.z}, plane->n);
	doublyOrthoVector = unitVector(doublyOrthoVector);
	
	// build a point from that doubly orthognal cross product std::vector
	Point pd = Point(doublyOrthoVector[0], doublyOrthoVector[1], doublyOrthoVector[2]);

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

