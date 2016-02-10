#include "Point.h"
#include <cmath>

/**
* Given spherical coordinates, build point and populate Cartesian
*/
Point::Point(float theta, float phi) {
	this->theta = fmod(theta, 2 * PI);
	this->phi = fmod(phi, 2 * PI);
	x = r * sin(this->phi) * cos(this->theta);
	y = r * sin(this->phi) * sin(this->theta);
	z = r * cos(this->phi);
}

/**
* Given Cartesian coordinates, build point and populate spherical
*/
Point::Point(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	phi = acos(z);
	theta = atan2(y, x);
}

/**
 * Get the haversine distance to another point (distance between 
 * two points on a sphere)
 */
float Point::distanceTo(std::shared_ptr<Point> other) {
	float dlon = other->theta - theta;
	float dlat = other->phi - phi;
	float a = pow(sin(dlat / 2), 2) + cos(phi) * cos(other->phi) * pow(sin(dlon / 2), 2);
	float c = 2 * atan2(sqrt(a), sqrt(1 - a));
	return c;
}

void Point::print(std::stringstream *ss) {
	*ss << printf("x: %f, y: %f, z: %f", x, y, z) << std::endl;
	*ss << printf("theta: %f, phi: %f", theta, phi) << std::endl;
}

/**
 * The JSON output here is designed to work with the coordinate 
 * system used by d3.js
 */
void Point::printJSON(std::stringstream *ss) {
	*ss << "[" << theta*FAC1 - 90 << ", " << 90 - phi * FAC1 << "]";
}
