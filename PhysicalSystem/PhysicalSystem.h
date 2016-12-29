#pragma once

#include "Common.h"
#include "btutilities.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

#include <entityx/entityx.h>
namespace ex = entityx;

#include <Mesh/Mesh.h>
#include "CommonRenderInterface.h"
#include "DebugDrawer.h"

class Context;


struct PhysicalSystem : public ex::System <PhysicalSystem> {

    PhysicalSystem(Context *ctx);
    ~PhysicalSystem();

    //------------------ Functions ------------------------

    void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override ;


    btRigidBody* addRigidBody(btCollisionShape* colShape, float mass, float x, float y, float z, CollisionGroup group);
    btRigidBody* addRigidBody(btCollisionShape* colShape, float m, btDefaultMotionState* motionState, CollisionGroup group);
    void addRigidBody(btRigidBody* body, CollisionGroup group);
    void addCollisionObject(btCollisionObject* obj, CollisionGroup group);
    btDiscreteDynamicsWorld* getWorld();
    void addAction(btActionInterface* action);
    void addVehicle(btRaycastVehicle* vehicle);
    void remRigidBody(btCollisionObject* body);
    void delRigidBody(btRigidBody* body);
    void delCollisionObject(btCollisionObject* body);
    void delAction(btActionInterface* action);
    void stepTime(int ms);
    void  createGround();

    bool contactCallbackFunction(btManifoldPoint& cp,const btCollisionObjectWrapper* obj1,int id0,int index0,const btCollisionObjectWrapper* obj2,int id1,int index1);

    void checkGhostCollision();
    void processGhostCollisions(btAlignedObjectArray<btCollisionObject*>& obj, btGhostObject *ghost);

    void createEmptyDynamicsWorld();

    void stepSimulation(float deltaTime);

    void physicsDebugDraw(int debugFlags);

    void exitPhysics();

    void debugDraw(int debugDrawFlags);

    bool pickBody(const btVector3& rayFromWorld, const btVector3& rayToWorld);

    bool movePickedBody(const btVector3& rayFromWorld, const btVector3& rayToWorld);

    btBoxShape* createBoxShape(const btVector3& halfExtents)
    {
        btBoxShape* box = new btBoxShape(halfExtents);
        return box;
    }

    btRigidBody* createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape,  const btVector4& color);


    void renderScene()
    {
    }


    //------------------ Properties ------------------------


    Context *context;

    map<int, int> masks;
    static const int mask_entities = (CG_WALL | CG_OBJECT | CG_UNIT | CG_VEHICLE);

    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    btCollisionShape* groundShape;

    vector<btCollisionShape*> collisionShapes;
   // DebugDrawer * debugDrawer; //TODO:
};





