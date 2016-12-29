#include "PhysicalBody.h"

//TODO: make factory

//log body->getCenterOfMassPosition()


PhysicalBody::PhysicalBody(Object *obj, Context *ctx, PhysicalBody_t physBodyType, btScalar mass_, btScalar friction_ = 1000, btCollisionShape *colShape = new btBoxShape({20,20,20})):obj(obj), mass(mass_),friction (friction_),collisionShape(colShape),context(ctx){

    //btCollisionShape * mPlayerBox = new btBoxShape(btVector3(1,3,1));

    auto dynWorld = ctx->physical_system->dynamicsWorld;
    //TODO: use decltype or typeid to make a copy of abstract class
    //collisionShape = new btCollisionShape (*colShape);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    if (isDynamic)
        collisionShape->calculateLocalInertia(mass, localInertia);
    btDefaultMotionState* fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), vec3tobt(obj->position) )); //orientation and position

    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, collisionShape, localInertia);

    rigidBody = new btRigidBody(fallRigidBodyCI);
    rigidBody->setCollisionShape(collisionShape);

    //     static int i = 1;
    //     moveToConstantly_phys({i,i,i});
    //     i*=i;

    rigidBody->setRollingFriction(0.03);
    rigidBody->setFriction(1);//10000
    rigidBody->setAnisotropicFriction(colShape->getAnisotropicRollingFrictionDirection(),btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

    rigidBody->setDamping(0.1,0.1);

    rigidBody->setUserPointer((void*)obj);
    //ctx->current_scene->rigidBodies.push_back(shared_ptr<PhysicalBody>(this));
    dynWorld->addRigidBody(rigidBody);


    //  btTransform playerWorld;
    //  playerWorld.setIdentity();

    //playerPos is a VEC3 that holds the camera position.
    // playerWorld.setOrigin({position.x, position.y,position.z});
    // collisionObject->setWorldTransform(playerWorld);
    // collisionObject->forceActivationState(DISABLE_DEACTIVATION);//maybe not needed

    // dynWorld->addCollisionObject(collisionObject);
}


//TODO: make sphere
PhysicalBody::PhysicalBody(Object *obj, Context *ctx,PhysicalBody_t physBodyType):PhysicalBody(obj, ctx,physBodyType,1, 1, new btSphereShape(10)){

}

//TODO: make sphere
PhysicalBody::PhysicalBody(Object *obj, Context *ctx, PhysicalBody_t physBodyType, btScalar mass):PhysicalBody(obj, ctx, physBodyType, 1,  1, new btSphereShape(10)){

}

// Copy constructor TODO
PhysicalBody::PhysicalBody(const PhysicalBody & rhs) :
    collisionShape(rhs.collisionShape),
    context(rhs.context),
    mass(rhs.mass){
    cerr << "PhysicalBody copy constructor called" << endl;
    // rhs.rigidBody

    auto dynWorld = context->physical_system->dynamicsWorld;

    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    if (isDynamic)
        collisionShape->calculateLocalInertia(mass, localInertia);
    btDefaultMotionState* fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0))); //orientation and position

    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, collisionShape, localInertia);

    rigidBody = new btRigidBody(fallRigidBodyCI);
    rigidBody->setCollisionShape(collisionShape);

    //     static int i = 1;
    //     moveToConstantly_phys({i,i,i});
    //     i*=i;

    rigidBody->setRollingFriction(0.03);
    rigidBody->setFriction(1);//10000
    rigidBody->setAnisotropicFriction(collisionShape->getAnisotropicRollingFrictionDirection(),btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

    //rigidBody->setDamping(0.2,0.2);

    //TODO: fix when creating rigidBody->setUserPointer((void*)obj);
    //ctx->current_scene->rigidBodies.push_back(shared_ptr<PhysicalBody>(this));
    dynWorld->addRigidBody(rigidBody);
}


//PhysicalBody::PhysicalBody(Object *obj, Context *ctx, btScalar mass_ = 1, btScalar friction_ = 1000):position({0,0,0}), mass(mass_),friction (friction_)
//{
//    // A box of 2m*2m*2m (1.0 is the half-extent !)
//    //  collisionShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));

//    collisionShape = new btSphereShape(1);
//    mass = mass_;
//    btVector3 fallInertia(0, 0, 0);
//    collisionShape->calculateLocalInertia(mass, fallInertia);
//    btDefaultMotionState* fallMotionState =
//            new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0))); //orientation and position


//    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, collisionShape, fallInertia);

//    rigidBody = new btRigidBody(fallRigidBodyCI);
//    rigidBody->setUserPointer((void*)obj);
//    rigidBody->setFriction(10000);
//    //rigidBody->setDamping(0.2,0.2);
//    //ctx->current_scene->rigidBodies.push_back(shared_ptr<PhysicalBody>(this));
//    ctx->physical_system->dynamicsWorld->addRigidBody(rigidBody);
//}


//------------------ Functions ------------------------


inline void PhysicalBody::moveToConstantly_phys(vec3 position){
    rigidBody->getWorldTransform().setOrigin(vec3tobt(position));

    //OR   btTransform trans;
    //   trans.setIdentity();
    //   trans.setOrigin(vec3tobt(position));
    //   rigidBody->setWorldTransform(trans);
}


void PhysicalBody::push(float x, float y, float z)
{
    rigidBody->applyCentralImpulse(btVector3(x, y, z));
}

void PhysicalBody::getTransform(float* matrix) const
{
    if(rigidBody != NULL && matrix != NULL)
    {
        rigidBody->getWorldTransform().getOpenGLMatrix(matrix);
    }
}

void PhysicalBody::setTransform(const float* matrix) const
{
    if(rigidBody != NULL && matrix != NULL)
    {
        btTransform t;
        t.setFromOpenGLMatrix(matrix);
        rigidBody->setWorldTransform(t);
    }
}


void PhysicalBody::setPos(float x, float y, float z)
{
    btTransform t = rigidBody->getWorldTransform();
    t.setOrigin(btVector3(x, y, z));
    rigidBody->setWorldTransform(t);
}

void PhysicalBody::setRotation(float x, float y, float z)
{
    btTransform t = rigidBody->getWorldTransform();
    btQuaternion q;
    q.setEulerZYX(z, y, x);
    t.setRotation(q);
    rigidBody->setWorldTransform(t);
}

void PhysicalBody::stop()
{
    rigidBody->setLinearVelocity(btVector3(0, 0, 0));
    rigidBody->setAngularVelocity(btVector3(0, 0, 0));
    rigidBody->clearForces();
}

btTransform PhysicalBody::getRealTransform()const
{
    return rigidBody->getWorldTransform();
}


void PhysicalBody::applyCentralForce(btVector3& force)
{
    rigidBody->applyCentralForce(force);
}


void PhysicalBody::setBounciness(float restitution, float friction)
{
    rigidBody->setFriction(friction);
    rigidBody->setRestitution(restitution);
}


vec3 PhysicalBody::getPosition()const
{
    vec3 ret;
    ret.x = rigidBody->getWorldTransform().getOrigin().getX();
    ret.y = rigidBody->getWorldTransform().getOrigin().getY();
    ret.z = rigidBody->getWorldTransform().getOrigin().getZ();
    return ret;
}


//TODO:
void PhysicalBody::reset()
{
    //    isFractured= false;

    //    for( u32 f=0; f<fractureArray.size(); f++)
    //    {
    //        k.physics->removeRigidBody( fractureArray[f]->rb );
    //        fractureArray[f]->remove();
    //    }
    //    fractureArray.clear();

    //    btTransform t;
    //    t.setIdentity();
    //    t.setOrigin( btVector3( 0,0,0 ));
    //    rb->setWorldTransform( t );
    //    //rb->getBroadphaseHandle()->m_collisionFilterGroup= ECG_STATIC | ECG_DYNAMIC | ECG_FRACTURE_CUBE;
    //    rb->getCollisionShape()->setLocalScaling( btVector3( m_size.X, m_size.Y, m_size.Z ));
    //    //rb->setDamping( 0.5f,0.5f );
    //    rb->setLinearVelocity( btVector3(0,0,0));
    //    rb->setAngularVelocity( btVector3(0,0,0));
    //    rb->clearForces();
    //    rb->updateInertiaTensor();

    //    setPosition( btVector3(0,0,0) );
    //    setPosition( vec3df(0,0,0) );
    //    setVisible( true );
}





void PhysicalBody::setPosition( const vec3& pos )
{
    //rigidBody->getWorldTransform().setOrigin( btVector3(vec3tobt(pos) ));

    auto btVec =  btVector3( vec3tobt (pos) );
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVec);

    rigidBody->setWorldTransform(trans );
     // Object::setPosition( pos );
}

void PhysicalBody::setRotation( const quat& rot )
{
//TODO:
}

void PhysicalBody::setRotation( const vec3& rot )
{
    //TODO: the conversion to quat is not accurate by 4 degrees!
    //cout<< "vec3df rotation:"<< rot.Y << rot.X << rot.Z <<endl;
    const btQuaternion q( rot.y, rot.x, rot.z );
    //cout<< "btQuat rotation:"<< q.y() << q.x() << q.z() <<endl;
    rigidBody->getWorldTransform().setRotation( q );

   // Object::setRotation( rot );
}


//void PhysicalBody::setSize( const vec3& newSize )
//{
//    rigidBody->getCollisionShape()->setLocalScaling( btVector3( newSize.X, newSize.Y, newSize.Z ));
//    btVector3 localInertia;
//    btScalar newMass( (newSize.X+newSize.Y+newSize.Z)/3 );
//    rigidBody->getCollisionShape()->calculateLocalInertia( newMass, localInertia );
//    rigidBody->setMassProps( newMass, localInertia );
//    rigidBody->updateInertiaTensor();

//   // Object::setSize( newSize );
//}


PhysicalBody::~PhysicalBody(){
    delete rigidBody;
    delete collisionShape;
}

//------------------ Utilities ------------------------
