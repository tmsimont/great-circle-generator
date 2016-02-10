#include "TrevorCalc3.h"
#include <cmath>

/**
 * Return the cross product of 2 3x1 std::vectors
 */
std::vector<float> crossProduct(std::vector<float> v1, std::vector<float> v2) {
	std::vector<float> xproduct;
	xproduct.push_back((v1[1] * v2[2]) - (v2[1] * v1[2]));
	xproduct.push_back(-1 * ((v1[0] * v2[2]) - (v2[0] * v1[2])));
	xproduct.push_back((v1[0] * v2[1]) - (v2[0] * v1[1]));
	return xproduct;
}

/**
 * Get the unit std::vector of the given std::vector
 */
std::vector<float> unitVector(std::vector<float> v) {
	float l = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
	v[0] = v[0] / l;
	v[1] = v[1] / l;
	v[2] = v[2] / l;
	return v;
}

