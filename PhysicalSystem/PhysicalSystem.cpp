
#include "PhysicalSystem.h"

#include "Scene/Object.h"
#include "RenderSystem/Shader.h"
#include "Texture/Texture.h"


struct rCallBack : public btCollisionWorld::ContactResultCallback
{
    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObject*
                             colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1,
                             int index1)
    {
        btVector3 ptA = cp.getPositionWorldOnA();
        btVector3 ptB = cp.getPositionWorldOnB();
        return 0;
    }
};

PhysicalSystem::PhysicalSystem(Context *ctx):context(ctx){


    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0,-9.81f,0));

    //TODO:
    //------------------ DebugDrawer -----------------------------

    //  debugDrawer = new DebugDrawer();
    //debugDrawer->setDebugMode(btIDebugDraw.DebugDrawModes.DBG_MAX_DEBUG_DRAW_MODE);
    //  debugDrawer->setDebugMode( btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE
    //                             );
    //dynamicsWorld->setDebugDrawer(debugDrawer);
}


//------------------ Functions ------------------------

void PhysicalSystem::update(ex::EntityManager &entities, ex::EventManager &events, ex::TimeDelta dt) {

    dynamicsWorld->debugDrawWorld();

    if(context->current_scene ){
        dynamicsWorld->stepSimulation(1 / 60.f, 10);

        for (auto obj : context->current_scene->objects){

            shared_ptr<Object> object = obj.second;

            if(!object->physBody)
                continue;

            object->updatePhysics(); //not working

            auto physBody = object->physBody;


        }

    }

}


void PhysicalSystem::createEmptyDynamicsWorld()
{
    ///collision configuration contains default setup for memory, collision setup
    collisionConfiguration = new btDefaultCollisionConfiguration();
    //collisionConfiguration->setConvexConvexMultipointIterations();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    broadphase = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
    solver = sol;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void PhysicalSystem::createGround(){
    groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

    btDefaultMotionState* groundMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));

    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody);
}

void PhysicalSystem::stepSimulation(float deltaTime)
{
    if (dynamicsWorld)
    {
        dynamicsWorld->stepSimulation(deltaTime);
    }
}



//TODO:
void PhysicalSystem::physicsDebugDraw(int debugFlags)
{
    if (dynamicsWorld && dynamicsWorld->getDebugDrawer())
    {
        dynamicsWorld->getDebugDrawer()->setDebugMode(debugFlags);
        dynamicsWorld->debugDrawWorld();
    }
}

//TODO:
void PhysicalSystem::debugDraw(int debugDrawFlags)
{
    if (dynamicsWorld)
    {
        if (dynamicsWorld->getDebugDrawer())
        {
            dynamicsWorld->getDebugDrawer()->setDebugMode(debugDrawFlags);
        }
        dynamicsWorld->debugDrawWorld();
    }

}

btRigidBody* PhysicalSystem::createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape,  const btVector4& color = btVector4(1, 0, 0, 1))
{
    btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        shape->calculateLocalInertia(mass, localInertia);

    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

    btRigidBody* body = new btRigidBody(cInfo);

    body->setUserIndex(-1);
    dynamicsWorld->addRigidBody(body);
    return body;
}


/**
* Add an action
**/
void PhysicalSystem::addAction(btActionInterface* action)
{
    dynamicsWorld->addAction(action);
}


/**
* Add a vehicle
**/
void PhysicalSystem::addVehicle(btRaycastVehicle* vehicle)
{
    dynamicsWorld->addAction(vehicle);
}


/**
* Remove a btCollisionObject from the game world, but keep the object around for adding later.
**/
void PhysicalSystem::remRigidBody(btCollisionObject* body)
{
    dynamicsWorld->removeCollisionObject(body);
}


/**
* Delete a rigid body, and remove it from the game world too
**/
void PhysicalSystem::delRigidBody(btRigidBody* body)
{
    if (!body) return;

    if (body->getMotionState()) {
        delete body->getMotionState();
    }

    dynamicsWorld->removeCollisionObject(body);

    delete body;
}


/**
* Delete a btCollisionObject, and remove it from the game world too
**/
void PhysicalSystem::delCollisionObject(btCollisionObject* body)
{
    dynamicsWorld->removeCollisionObject(body);
    delete body;
}


/**
* Delete an action
**/
void PhysicalSystem::delAction(btActionInterface* action)
{
    dynamicsWorld->removeAction(action);
    delete action;
}



/**
* Step the physics forward by the given amount of time
*
* Very useful docs:
*   http://bulletphysics.org/mediawiki-1.5.8/index.php/Stepping_The_World
*
* Currently running at 60fps with up to 3 substeps
* Which means below 20fps the simulation will "lose" time
**/
void PhysicalSystem::stepTime(int ms)
{
    dynamicsWorld->stepSimulation(
                static_cast<float>(ms) / 1000.0f,
                3,
                1.0f/60.0f
                );
}




/**
* Returns the world
**/
btDiscreteDynamicsWorld* PhysicalSystem::getWorld()
{
    return dynamicsWorld;
}


/**
* Create and add a rigid body
*
* colShape = The collission shape of the body
* m = mass
* x,y,z = origin position
**/
btRigidBody* PhysicalSystem::addRigidBody(btCollisionShape* colShape, float m, float x, float y, float z, CollisionGroup group)
{
    btDefaultMotionState* myMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(x,y,z)));

    btScalar mass(m);
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    if (isDynamic)
        colShape->calculateLocalInertia(mass,localInertia);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(
                mass,
                myMotionState,
                colShape,
                localInertia
                );
    btRigidBody* body = new btRigidBody(rbInfo);

    dynamicsWorld->addRigidBody(body, group, this->masks[group]);

    return body;
}


/**
* Create and add a rigid body
*
* colShape = The collission shape of the body
* m = mass
* motionState = origin motion state (position and rotation)
**/
btRigidBody* PhysicalSystem::addRigidBody(btCollisionShape* colShape, float m, btDefaultMotionState* motionState, CollisionGroup group)
{
    btScalar mass(m);
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    if (isDynamic)
        colShape->calculateLocalInertia(mass,localInertia);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(
                mass,
                motionState,
                colShape,
                localInertia
                );
    btRigidBody* body = new btRigidBody(rbInfo);

    dynamicsWorld->addRigidBody(body, group, this->masks[group]);

    return body;
}


/**
* Add a rigid body which already exists
**/
void PhysicalSystem::addRigidBody(btRigidBody* body, CollisionGroup group)
{
    dynamicsWorld->addRigidBody(body, group, this->masks[group]);
}


/**
* Add a collision object
**/
void PhysicalSystem::addCollisionObject(btCollisionObject* obj, CollisionGroup group)
{
    dynamicsWorld->addCollisionObject(obj, group, this->masks[group]);
}



void PhysicalSystem::checkGhostCollision()
{
    int i;

    for (i=0; i<dynamicsWorld->getCollisionObjectArray().size(); i++)
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);

        btAlignedObjectArray < btCollisionObject* > objsInsidePairCachingGhostObject;
        btAlignedObjectArray < btCollisionObject* >* pObjsInsideGhostObject = NULL;
        btGhostObject* ghost = btGhostObject::upcast(obj);

        if(ghost)
        {
            objsInsidePairCachingGhostObject.resize(0);
            pObjsInsideGhostObject = &ghost->getOverlappingPairs();
#ifdef DEBUG
            cout << ghost->getNumOverlappingObjects() << endl;
#endif // DEBUG

            //processGhostCollisions(*pObjsInsideGhostObject, ghost);
        }
    }
}


bool PhysicalSystem::pickBody(const btVector3& rayFromWorld, const btVector3& rayToWorld)
{
    if (dynamicsWorld==0)
        return false;

    btCollisionWorld::ClosestRayResultCallback rayCallback(rayFromWorld, rayToWorld);

    dynamicsWorld->rayTest(rayFromWorld, rayToWorld, rayCallback);

    return false;
}

PhysicalSystem::~PhysicalSystem(){
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

