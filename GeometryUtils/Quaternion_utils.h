#pragma once

#pragma once

#include "Common.h"

#include <fstream>
#include <queue>
#include <sstream>

void printMatrix(const float m[16], const string &str);

vec3 v4tov3(vec4 v1);

void clamp_angle(float&angle);

quat rotationBetweenVectors(vec3 start, vec3 dest);

quat mylookAt(vec3 direction, vec3 desiredUp);

quat rotateTowards(quat q1, quat q2, float maxAngle);

struct quaternion
{
    double x, y, z, w;
};

quaternion mult(quaternion A, quaternion B);
quaternion conjugate(quaternion quat);
quaternion normalize(quaternion quat);
double length(quaternion quat);
void printMatrix(const float m[16], const string &str);
void clamp_angle(float&angle);

inline bool comparevec3(vec3 &a, vec3 &b){
    return a.x == b.x && a.y == b.y && a.z == b.z;
}


template <class T>
bool contains(const vector<T> &vec, const T &value)
{
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

quat RotationBetweenVectors(vec3 start, vec3 dest);

quat LookAt(vec3 direction, vec3 desiredUp);

quat RotateTowards(quat q1, quat q2, float maxAngle);
