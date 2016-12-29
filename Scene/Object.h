#pragma once

#include "Common.h"
#include "Mesh/Mesh.h"
#include "Texture/Texture.h"
#include "Context.h"
#include "PhysicalSystem/PhysicalBody.h"
#include "RenderSystem/GLDebugDefs.h"
#include "RenderSystem/GLVAO.h"

#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>


class AssimpBone;
class AssimpNode;
class AssimpFace;
class AssimpMaterial;
class AssimpAnimation;

class PhysicalBody;
class Context;

class Object
{

public:
    Object() = default;
    ~Object() = default;

    Object(Context *ctx, MeshType meshType, TextureType textureType, string name);
    Object(Context *ctx, MeshType meshType, string name);

    friend void swap(Object& first, Object& second) // nothrow
    {
        using std::swap;
        swap(first.name, second.name);
        swap(first.translationMatrix, second.translationMatrix);
        swap(first.rotationMatrix, second.rotationMatrix);
        swap(first.scalingMatrix, second.scalingMatrix);
        swap(first.transform, second.transform);
        swap(first.position, second.position);
        swap(first.physBody, second.physBody);
        swap(first.texture, second.texture);
        swap(first.meshes, second.meshes);
    }


    Object& operator=(Object other) // not const Object& other
    {
        swap(*this, other);
        // or      swap(x, rhs.x);
        return *this;
    }


    Object(Object&& other)  // move constructor
        : Object() // initialize via default constructor, C++11 only
    {
        swap(*this, other);
    }

    // Copy constructor TODO
    Object(const Object & rhs) :
        context(rhs.context),
        name(rhs.name),  //TODO: change to some defaul tname
        texture(rhs.texture),
        meshes(rhs.meshes),

        position(rhs.position),
        translationMatrix(rhs.translationMatrix),
        rotationMatrix(rhs.rotationMatrix),
        scalingMatrix(rhs.scalingMatrix),
        transform(rhs.transform) {
    }


    //------------------ Transforms ------------------------


    void setPosition(float coord_x, float coord_y,float coord_z,float);

    void translate(float dist_x, float dist_y,float dist_z,float deltaTime);
    void translate(float dist_x, float dist_y,float dist_z);

    void scale(float times_x, float times_y,float times_z,float deltaTime);
    void scale(float times_x, float times_y,float times_z);

    void rotateEuler(float angle_x, float angle_y,float angle_z,float deltaTime);
    void rotateEuler(float angle_x, float angle_y,float angle_z);
    void rotateQuaternion(float angle_x, float angle_y,float angle_z,float deltaTime);
    void rotateQuaternion(float angle_x, float angle_y,float angle_z);

    void rotateTowardsQuaternion(float angle_x, float angle_y,float angle_z,float deltaTime);


    void computeTransform(); //matrix calculaitions by multiplication
    void computeTranslation();  //TODO:
    void computeRotation();  //TODO:
    void computeScaling();

    void transformQuaternion( float deltaTime);  //example
    void transformEuler(float deltaTime);        //example

    mat4 getModelMatrix() const; //from position, rotation, scaling

    //------------------ Properties ------------------------

    string name;
    ex::Entity entity;

    shared_ptr<Texture> texture;
    shared_ptr<PhysicalBody> physBody;
    shared_ptr<vector<Mesh>> meshes;

    vec3 position;
    vec3 rotation;
    float scaling;

    mat4 transform;
    mat4 translationMatrix;
    mat4 rotationMatrix;
    mat4 scalingMatrix;

    Context *context;


    //------------------ Utilities ------------------------

    void setName(MeshType meshType); // instead use enumToString(a)


    //------------------ Physics ------------------------

    void addPhysicalBody(float mass );
    void updatePhysics();

    void stepPhysicSimulation();
};

