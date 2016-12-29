#pragma once

#include <btBulletDynamicsCommon.h>

#include <entityx.h>

using namespace entityx;

struct PhysicsComponent : Component<PhysicsComponent>
{

    btCollisionShape *shape;
    btScalar mass;
    btRigidBody *rigidBody;
    btMotionState *motionState;
    btVector3 fallInertia;
    int collisionFlags;


    PhysicsComponent(btCollisionShape *shape, btMotionState *motionState, btScalar mass, btVector3 fallInertia) : shape(shape), motionState(motionState), mass(mass), fallInertia(fallInertia)
    {
        shape->calculateLocalInertia(mass, fallInertia);
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,motionState,shape,fallInertia);
        rigidBody = new btRigidBody(rigidBodyCI);
        rigidBody->setActivationState(DISABLE_DEACTIVATION);
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

    }
    void setCollisionFlags(int colflags)
    {
        collisionFlags = collisionFlags | colflags;
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | collisionFlags);

    }
    int getCollisionFlags()
    {
        return rigidBody->getCollisionFlags();
    }

};

