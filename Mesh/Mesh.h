#pragma once

#include "Common.h"

#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>

//#include "tiny_obj_loader.h"

//#include "Geometry.h"
//#include "Triangle.h"
//#include "Box.h"

#include "RenderSystem/GLVAO.h"
#include "RenderSystem/GLDebugDefs.h"

#include <fstream>
#include <queue>
#include <sstream>


class AssimpBone;
class AssimpNode;
class AssimpFace;
class AssimpMaterial;
class AssimpAnimation;


//------------------ Utilities ------------------------
struct Mesh {
    //class Mesh : public geometry {

    //friend class RenderSystem;
    //friend class AnimPlay;

    //TODO: Mesh(const char *filename, bool yes);

    Mesh();
    ~Mesh()= default; //TODO:

    //TODO:

    //    friend void swap(Mesh& first, Mesh& second) // nothrow
    //    {
    //        using std::swap;

    //        swap(first.parent, second.parent);
    //        swap(first.children, second.children);
    //        swap(first.mesh_type, second.mesh_type);
    //        swap(first.name, second.name);
    //        swap(first.indices, second.indices);
    //        swap(first.indexed_vertices, second.indexed_vertices);
    //        swap(first.indexed_uvs, second.indexed_uvs);
    //        swap(first.indexed_normals, second.indexed_normals);
    //        swap(first.vao, second.vao);   //FIXIT: or new
    //    }

    //    Mesh& operator=(Mesh other) // not const Object& other
    //    {
    //        using std::swap;
    //        swap(*this, other);
    //        // or      swap(x, rhs.x);  //FIXIT: or new
    //        return *this;
    //    }


    //    Mesh(Mesh&& other)  // move constructor
    //        : Mesh() // initialize via default constructor, C++11 only
    //    {
    //        swap(*this, other);
    //    }

    // Mesh(Mesh && rhs) : x(std::move(rhs.x)) {}


    //Mesh& operator=(const Mesh&) & = default;
    // Mesh& operator=(Mesh&&) & = default;       // Move assignment operator

    // Copy constructor TODO
    //    Mesh(const Mesh & rhs) :
    //        parent( rhs.parent), //FIXIT: or new
    //        children( rhs.children),  //FIXIT: or new
    //        mesh_type(rhs.mesh_type),
    //        name( rhs.name),
    //        indices(rhs.indices),
    //        indexed_vertices(rhs.indexed_vertices),
    //        indexed_uvs(rhs.indexed_uvs),
    //        indexed_normals(rhs.indexed_normals),
    //        vao (rhs.vao) {}


    //------------------ Functions ------------------------


    // virtual ray_path intersect(ray ray) const override;
    //    virtual num_t get_color(const vec3 &point) const override;
    //    virtual vec3 get_normal(const vec3 &point) const override;
    //    virtual material get_material() const override;


    void addChild(shared_ptr<Mesh> child) {
        this->children.push_back(child);
        child->parent = this;
    }


    //------------------ Properties ------------------------


    MeshType mesh_type;

    //std::weak_ptr
    Mesh* parent;
    // AssimpNode* nd;

    vector<shared_ptr<Mesh>> children;
    string name;
    mat4 transform;

    vector<unsigned int> indices;
    vector<vec3> indexed_vertices;
    vector<vec2> indexed_uvs;
    vector<vec3> indexed_normals;

    //????GLVAO *vao;
    shared_ptr<GLVAO> vao;
    int numFaces;
    int materialIndex;
    // vector<AssimpBone*> bones;


    // For the physics bits
    // vector<AssimpFace>* faces;
    // vector<glm::vec4> vertices;


    //    int* boneIds;
    //    float* boneWeights;
    //    btVector3 boundingSize;
    //    btCollisionShape *shape;
    //    bool recenter;



    //tinyobj:
    //    vector<tinyobj::shape_t> shapes;
    //    vector<tinyobj::material_t> materials;
    //    vector<vector<triangle> > _triangles;
    //    vector<vector<material> > _materials;
    //    vector<box> _boxes;
    //    mutable material _first_material;
    //    mutable vec3 _first_normal;
};



