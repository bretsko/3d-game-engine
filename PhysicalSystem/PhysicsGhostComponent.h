#pragma once

#include <btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include <entityx.h>

using namespace entityx;

struct PhysicsGhostComponent : Component<PhysicsGhostComponent>
{

    btGhostObject *ghost;
    btCollisionShape *ghostshape;
    btVector3 position;

    PhysicsGhostComponent(btCollisionShape *shape, btVector3 position) : ghostshape(shape), position(position)
    {
        ghost = new btGhostObject();
        ghost->setCollisionShape(ghostshape);
        btTransform trans;
        trans.setIdentity();
        trans.setOrigin(position);
        ghost->setWorldTransform(trans);
        ghost->setCollisionFlags( ghost->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
        //ghost->setActivationState(DISABLE_DEACTIVATION);
    }

    void setPosition(btVector3 const &btvector )
    {
        btTransform trans;
        trans.setIdentity();
        trans.setOrigin(btvector);
        ghost->setWorldTransform(trans);
    }

};
