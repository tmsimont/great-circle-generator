// copyright 2016 Trevor Simonton
#ifndef SRC_BASE_TREVORCALC3_H_
#define SRC_BASE_TREVORCALC3_H_

#include <vector>

using std::vector;

const float PI = 3.1415926535897932384626433832795;
const float FAC1 = (180.0/PI);
const float FAC2 = (PI/180.0);
const float r = 1;

vector<float> crossProduct(vector<float> v1, vector<float> v2);
vector<float> unitVector(vector<float> v);

#endif  // SRC_BASE_TREVORCALC3_H_
