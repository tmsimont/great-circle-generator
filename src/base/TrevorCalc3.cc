// copyright 2016 Trevor Simonton
#include "base/TrevorCalc3.h"
#include <cmath>

using std::vector;

/**
 * Return the cross product of 2 3x1 vectors
 */
vector<float> crossProduct(vector<float> v1, vector<float> v2) {
  vector<float> xproduct;
  xproduct.push_back((v1[1] * v2[2]) - (v2[1] * v1[2]));
  xproduct.push_back(-1 * ((v1[0] * v2[2]) - (v2[0] * v1[2])));
  xproduct.push_back((v1[0] * v2[1]) - (v2[0] * v1[1]));
  return xproduct;
}

/**
 * Get the unit vector of the given vector
 */
vector<float> unitVector(vector<float> v) {
  float l = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
  v[0] = v[0] / l;
  v[1] = v[1] / l;
  v[2] = v[2] / l;
  return v;
}

