#include "TangentSpace.h"

void computeTangentBasis(
 // inputs
 vector<vec3> & vertices,
 vector<vec2> & uvs,
 vector<vec3> & normals,
 // outputs
 vector<vec3> & tangents,
 vector<vec3> & bitangents
){

 for (unsigned int i=0; i<vertices.size(); i+=3 ){

  // Shortcuts for vertices
  vec3 & v0 = vertices[i+0];
  vec3 & v1 = vertices[i+1];
  vec3 & v2 = vertices[i+2];

  // Shortcuts for UVs
  vec2 & uv0 = uvs[i+0];
  vec2 & uv1 = uvs[i+1];
  vec2 & uv2 = uvs[i+2];

  // Edges of the triangle : postion delta
  vec3 deltaPos1 = v1-v0;
  vec3 deltaPos2 = v2-v0;

  // UV delta
  vec2 deltaUV1 = uv1-uv0;
  vec2 deltaUV2 = uv2-uv0;

  float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
  vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
  vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;

  // Set the same tangent for all three vertices of the triangle.
  // They will be merged later, in vboindexer.cpp
  tangents.push_back(tangent);
  tangents.push_back(tangent);
  tangents.push_back(tangent);

  // Same thing for binormals
  bitangents.push_back(bitangent);
  bitangents.push_back(bitangent);
  bitangents.push_back(bitangent);

 }

 // See "Going Further"
 for (unsigned int i=0; i<vertices.size(); i+=1 )
 {
  vec3 & n = normals[i];
  vec3 & t = tangents[i];
  vec3 & b = bitangents[i];
  
  // Gram-Schmidt orthogonalize
  t = glm::normalize(t - n * glm::dot(n, t));
  
  // Calculate handedness
  if (glm::dot(glm::cross(n, t), b) < 0.0f){
   t = t * -1.0f;
  }

 }


}


