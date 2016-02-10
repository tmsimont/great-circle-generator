#ifndef TREVOR_CALC3_H_
#define TREVOR_CALC3_H_

#include <vector>

const float PI = 3.1415926535897932384626433832795;
const float FAC1 = (180.0/PI);
const float FAC2 = (PI/180.0);
const float r = 1;

std::vector<float> crossProduct(std::vector<float> v1, std::vector<float> v2);
std::vector<float> unitVector(std::vector<float> v);

#endif // TREVOR_CALC3_H_