
#pragma once

#include "Common.h"
#include <btBulletDynamicsCommon.h>
#include <Bullet3Common/b3AlignedObjectArray.h>

struct GLInstanceVertex
{
    float xyzw[4];
    float normal[3];
    float uv[2];
};

struct GLInstanceGraphicsShape
{
    b3AlignedObjectArray<GLInstanceVertex>* m_vertices;
    int    m_numvertices;
    b3AlignedObjectArray<int>*   m_indices;
    int    m_numIndices;
    float   m_scaling[4];
};

#define VEC32BT(VEC3) (btVector3(VEC3->X, VEC3->Y, VEC3->Z))

inline btVector3 vec3tobt(const vec3 &position){
    return {position.x,position.y,position.z};
}

inline vec3 bttovec3(const btVector3 &position) {
    return {float(position.x()),float(position.y()),float(position.z())};
}

//TODO: vararg list
inline void print_btVector3(const btVector3 &vec) {
    cerr << "X:" << vec.x() << " Y:"<< vec.y() << " Z:"<< vec.z() << endl;
}

inline void print_vec3(const vec3 &vec) {
    cerr << "X:" << vec.x << "Y:"<< vec.y << "Z:"<< vec.z << endl;
}

inline vec3 btTransformToVec3(const btTransform &trans) {
    return {trans.getOrigin().getX(),
                trans.getOrigin().getY(),
                trans.getOrigin().getZ()};
}

inline quat btTransformToQuat(const btTransform &trans) {
    return {trans.getRotation().getX(),
                trans.getRotation().getY(),
                trans.getRotation().getZ(),
                trans.getRotation().getW()};
}

inline float clamp(const float &lo, const float &hi, const float &v)
{ return std::max(lo, std::min(hi, v)); }

inline float deg2rad(const float &deg)
{ return deg * M_PI / 180; }

inline vec3 mix(const vec3 &a, const vec3& b, const float &mixValue)
{ return a * (1 - mixValue) + b * mixValue; }

float quaternionToYaw(const btQuaternion &quat);

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

btVector3 quaternionToEulerXYZ(const btQuaternion &quat);
btVector3 quaternionToEuler(const btQuaternion &TQuat );

enum CollisionGroup {
    CG_NOTHING = 0,           // Nothing!
    CG_TERRAIN = BIT(0),      // The ground surface
    CG_WATER = BIT(1),        // Water terrain, or perhaps quicksand
    CG_DEBRIS = BIT(2),       // Debris, dead stuff, etc
    CG_AMMO = BIT(3),         // AmmoRound entities
    CG_WALL = BIT(4),         // Walls and other statics
    CG_OBJECT = BIT(5),       // Objects and other dynamics
    CG_UNIT = BIT(6),         // Units (btKinematicCharacterController)
    CG_VEHICLE = BIT(7),      // Vehicles (btRaycastVehicle)
    CG_PICKUP = BIT(8),       // Pickups, Powerups, etc.
};


