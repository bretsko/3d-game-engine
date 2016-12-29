#pragma once

#include "Common.h"

void computeTangentBasis(
 // inputs
 vector<vec3> & vertices,
 vector<vec2> & uvs,
 vector<vec3> & normals,
 // outputs
 vector<vec3> & tangents,
 vector<vec3> & bitangents
);
