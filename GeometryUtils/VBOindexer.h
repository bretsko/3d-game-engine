#pragma once

void indexVBO(
 vector<vec3> & in_vertices,
 vector<vec2> & in_uvs,
 vector<vec3> & in_normals,

    vector<unsigned int> & out_indices,
 vector<vec3> & out_vertices,
 vector<vec2> & out_uvs,
 vector<vec3> & out_normals
);


void indexVBO_TBN(
 vector<vec3> & in_vertices,
 vector<vec2> & in_uvs,
 vector<vec3> & in_normals,
 vector<vec3> & in_tangents,
 vector<vec3> & in_bitangents,

    vector<unsigned int> & out_indices,
 vector<vec3> & out_vertices,
 vector<vec2> & out_uvs,
 vector<vec3> & out_normals,
 vector<vec3> & out_tangents,
 vector<vec3> & out_bitangents
);

