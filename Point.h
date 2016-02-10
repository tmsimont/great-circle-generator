#ifndef POINT_H_
#define POINT_H_

#include "TrevorCalc2.h"
#include <memory>
#include <sstream>
#include <vector>


/**
 * Point represents a set of coordinates in Cartesian and Polar
 * coordinates that describes a point on the surface of a sphere.
 * This point supports conversion from Cartesian to Polar, and
 * can be output as d3.js globe fristd::endly JSON format.
 */
class Point {
public:
	int id;
	float theta;
	float phi;
	float x;
	float y;
	float z;
	std::vector<std::weak_ptr<Point> > neighbors;

	/**
	* Point is at origin by default
	*/
	Point() : theta(0),phi(0),x(0),y(0),z(0) {};

	/**
	* Given spherical coordinates, build point and populate Cartesian
	*/
	Point(float theta, float phi);

	/**
	* Given Cartesian coordinates, build point and populate spherical
	*/
	Point(float x, float y, float z);
	
	/**
	 * Get the haversine distance to another point (distance between 
	 * two points on a sphere)
	 */
	float distanceTo(std::shared_ptr<Point> other);
	void print(std::stringstream *ss);
	void printJSON(std::stringstream *ss);
};

#endif // POINT_H_