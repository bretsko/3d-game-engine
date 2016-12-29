//#include "VBOindexer.hpp"

#include "Common.h"
#include <map>
#include <string.h> // for memcmp


// Returns true if v1 can be considered equal to v2
bool is_near(float v1, float v2){
 return fabs( v1-v2 ) < 0.01f;
}

// Searches through all already-exported vertices
// for a similar one.
// Similar = same position + same UVs + same normal
bool getSimilarVertexIndex( 
 vec3 & in_vertex, 
 vec2 & in_uv,
 vec3 & in_normal, 
 vector<vec3> & out_vertices,
 vector<vec2> & out_uvs,
 vector<vec3> & out_normals,
    unsigned int & result
){
 // Lame linear search
 for ( unsigned int i=0; i<out_vertices.size(); i++ ){
  if (
   is_near( in_vertex.x , out_vertices[i].x ) &&
   is_near( in_vertex.y , out_vertices[i].y ) &&
   is_near( in_vertex.z , out_vertices[i].z ) &&
   is_near( in_uv.x     , out_uvs     [i].x ) &&
   is_near( in_uv.y     , out_uvs     [i].y ) &&
   is_near( in_normal.x , out_normals [i].x ) &&
   is_near( in_normal.y , out_normals [i].y ) &&
   is_near( in_normal.z , out_normals [i].z )
  ){
   result = i;
   return true;
  }
 }
 // No other vertex could be used instead.
 // Looks like we'll have to add it to the VBO.
 return false;
}

void indexVBO_slow(
 vector<vec3> & in_vertices,
 vector<vec2> & in_uvs,
 vector<vec3> & in_normals,

    vector<unsigned int> & out_indices,
 vector<vec3> & out_vertices,
 vector<vec2> & out_uvs,
 vector<vec3> & out_normals
){
 // For each input vertex
 for ( unsigned int i=0; i<in_vertices.size(); i++ ){

  // Try to find a similar vertex in out_XXXX
        unsigned int index;
  bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i],     out_vertices, out_uvs, out_normals, index);

  if ( found ){ // A similar vertex is already in the VBO, use it instead !
   out_indices.push_back( index );
  }else{ // If not, it needs to be added in the output data.
   out_vertices.push_back( in_vertices[i]);
   out_uvs     .push_back( in_uvs[i]);
   out_normals .push_back( in_normals[i]);
            out_indices .push_back( (unsigned int)out_vertices.size() - 1 );
  }
 }
}

struct PackedVertex{
 vec3 position;
 vec2 uv;
 vec3 normal;
 bool operator<(const PackedVertex that) const{
  return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
 };
};

struct PackedVertexf{
    float position;
    float uv;
    float normal;
    bool operator<(const PackedVertex that) const{
        return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
    };
};


bool getSimilarVertexIndex_fast( 
    PackedVertex & packed,
    map<PackedVertex,unsigned int> & VertexToOutIndex,
    unsigned int & result
){
    map<PackedVertex,unsigned int>::iterator it = VertexToOutIndex.find(packed);
 if ( it == VertexToOutIndex.end() ){
  return false;
 }else{
  result = it->second;
  return true;
 }
}

//void indexVBO(
//    vector<float> & in_vertices,
//    vector<float> & in_uvs,
//    vector<float> & in_normals,

//    vector<unsigned int> & out_indices,
//    vector<vec3> & out_vertices,
//    vector<vec2> & out_uvs,
//    vector<vec3> & out_normals
//){
//    map<PackedVertex,unsigned int> VertexToOutIndex;

//    // For each input vertex
//    for ( unsigned int i=0; i<in_vertices.size(); i++ ){

//        PackedVertexf packed = {in_vertices[i], in_uvs[i], in_normals[i]};


//        // Try to find a similar vertex in out_XXXX
//        unsigned int index;
//        bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);

//        if ( found ){ // A similar vertex is already in the VBO, use it instead !
//            out_indices.push_back( index );
//        }else{ // If not, it needs to be added in the output data.
//            out_vertices.push_back( in_vertices[i]);
//            out_uvs     .push_back( in_uvs[i]);
//            out_normals .push_back( in_normals[i]);
//            unsigned int newindex = (unsigned int)out_vertices.size() - 1;
//            out_indices .push_back( newindex );
//            VertexToOutIndex[ packed ] = newindex;
//        }
//    }
//}


void indexVBO(
    vector<vec3> & in_vertices,
 vector<vec2> & in_uvs,
 vector<vec3> & in_normals,

    vector<unsigned int> & out_indices,
 vector<vec3> & out_vertices,
 vector<vec2> & out_uvs,
 vector<vec3> & out_normals
){
    map<PackedVertex,unsigned int> VertexToOutIndex;

 // For each input vertex
 for ( unsigned int i=0; i<in_vertices.size(); i++ ){

  PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};
  

  // Try to find a similar vertex in out_XXXX
        unsigned int index;
  bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);

  if ( found ){ // A similar vertex is already in the VBO, use it instead !
   out_indices.push_back( index );
  }else{ // If not, it needs to be added in the output data.
   out_vertices.push_back( in_vertices[i]);
   out_uvs     .push_back( in_uvs[i]);
   out_normals .push_back( in_normals[i]);
            unsigned int newindex = (unsigned int)out_vertices.size() - 1;
   out_indices .push_back( newindex );
   VertexToOutIndex[ packed ] = newindex;
  }
 }
}


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
){

 // For each input vertex
 for ( unsigned int i=0; i<in_vertices.size(); i++ ){

  // Try to find a similar vertex in out_XXXX
        unsigned int index;
  bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i],     out_vertices, out_uvs, out_normals, index);

  if ( found ){ // A similar vertex is already in the VBO, use it instead !
   out_indices.push_back( index );

   // Average the tangents and the bitangents
   out_tangents[index] += in_tangents[i];
   out_bitangents[index] += in_bitangents[i];
  }else{ // If not, it needs to be added in the output data.
   out_vertices.push_back( in_vertices[i]);
   out_uvs     .push_back( in_uvs[i]);
   out_normals .push_back( in_normals[i]);
   out_tangents .push_back( in_tangents[i]);
   out_bitangents .push_back( in_bitangents[i]);
            out_indices .push_back( (unsigned int)out_vertices.size() - 1 );
  }
 }
}
