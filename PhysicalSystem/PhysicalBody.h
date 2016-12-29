#pragma once

#include "Context.h"
#include "Scene/Object.h"
#include "btutilities.h"

class Object;
class Context;

typedef enum PhysicalBodyType{DYNAMIC_BODY, STATIC_BODY }PhysicalBody_t;

//-------------------------------Collisions--------------------------------

enum collisiontypes {
    COL_NOTHING = 0, //<Collide with nothing
    COL_PLAYER = BIT(1), //<Collide with ships
    COL_WALL = BIT(2), //<Collide with walls
    COL_FACE = BIT(3) //<Collide with powerups
};

//int playerCollidesWith = COL_WALL;
//int wallCollidesWith = COL_NOTHING;
//int faceCollidesWith = COL_PLAYER | COL_WALL;

//After setting these up, simply add your body objects to the world as normal, except as the second and third parameters pass your collision type for that body, and the collision mask.

//If you are not using a recent enough version of Bullet, you may find this overload of addRigidBody does not exist. In this case, use btDynamicsWorld::addCollisionObject instead of the more usual btDynamicsWorld::addRigidBody. You may have to set the gravity on the rigid body manually in this case.

//btRigidBody ship; // Set up the other ship stuff
//btRigidBody wall; // Set up the other wall stuff
//btRigidBody powerup; // Set up the other powerup stuff

//mWorld->addRigidBody(ship, COL_SHIP, shipCollidesWith);
//mWorld->addRigidBody(wall, COL_WALL, wallCollidesWith);
//mWorld->addRigidBody(powerup, COL_POWERUP, powerupCollidesWith);

class PhysicalBody
{
public:
    PhysicalBody() = default; //TODO:
    ~PhysicalBody();
    PhysicalBody(Object *obj, Context *ctx, PhysicalBody_t physBodyType);
    PhysicalBody(Object *obj, Context *ctx, PhysicalBody_t physBodyType, btScalar mass);
    //PhysicalBody(Object *obj, Context *ctx, btScalar mass_, btScalar friction_);
    PhysicalBody(Object *obj, Context *ctx, PhysicalBody_t physBodyType, btScalar mass_, btScalar friction_, btCollisionShape *colShape);

    //TODO:
    // PhysicalBody(PhysicalBody *obj, Context *ctx);

    friend void swap(PhysicalBody& first, PhysicalBody& second) // nothrow
    {
        using std::swap;

        swap(first.rigidBody, second.rigidBody); //FIXIT: or new
        swap(first.collisionShape, second.collisionShape);  //FIXIT: or new
    }


    PhysicalBody& operator=(PhysicalBody other) // not const Object& other
    {
        using std::swap;
        swap(*this, other);
        // or      swap(x, rhs.x);  //FIXIT: or new
        return *this;
    }


    PhysicalBody(PhysicalBody&& other)  // move constructor
        : PhysicalBody() // initialize via default constructor, C++11 only
    {
        cerr << "PhysicalBody move constructor called" << endl;
        swap(*this, other);
    }

    // PhysicalBody(PhysicalBody && rhs) : x(std::move(rhs.x)) {}

    //PhysicalBody& PhysicalBody=(const PhysicalBody&) & = default;
    // PhysicalBody& PhysicalBody=(PhysicalBody&&) & = default;

    // Copy constructor TODO
    PhysicalBody(const PhysicalBody & rhs);


    //------------------ Functions ------------------------


    inline void moveToConstantly_phys(vec3 position);


    //----------------------------------------------------------------

    void setRotation( const vec3& rot );
    void setRotation( const quat& rot ); //TODO:

    void setPosition( const vec3& pos );
    void reset();

    void push(float x, float y, float z);
    void getTransform(float* matrix) const;
    void setTransform(const float* matrix) const;
    void setPos(float x, float y, float z);
    void setRotation(float x, float y, float z);
    void stop();
    btTransform getRealTransform()const;
    void applyCentralForce(btVector3& force);
    void setBounciness(float restitution, float friction);
    vec3 getPosition()const;


    //------------------ Properties ------------------------

    Object *obj;
    btRigidBody* rigidBody;

    btCollisionShape* collisionShape;
    btTransform transform;

    btScalar friction;
    btScalar mass;

    Context *context;

};

