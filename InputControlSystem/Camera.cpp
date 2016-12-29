#include "Camera.h"

Camera::Camera(Context * ctx,float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
    context(ctx),
    front(vec3(0.0f, 0.0f, 1.0f)),
    movementSpeed(SPEED),
    mouseSensitivity(SENSITIVTY),
    zoom(ZOOM),
    name("camera"),
    yaw_angle (yaw),
    pitch_angle(pitch),
    position (vec3(posX, posY, posZ))
{
    init();
}


Camera::Camera(Context * ctx, vec3 position = ORIGIN, vec3 up = X_AXIS, float yaw = YAW, float pitch = PITCH) :
    context(ctx),
    front(FRONT),
    position (position),
    movementSpeed(SPEED),
    mouseSensitivity(SENSITIVTY),
    zoom(ZOOM),
    name("camera"),
    yaw_angle (yaw),
    pitch_angle(pitch)
{
    init();
}

Camera::Camera(Context * ctx, vec3 position, vec3 target) :
    context(ctx),
    position (position),
    front(FRONT),
    movementSpeed(SPEED),
    mouseSensitivity(SENSITIVTY),
    zoom(ZOOM),
    yaw_angle (YAW),
    pitch_angle(PITCH),
    name("camera")
{
    init();
}

//Common init
void Camera::init(){
    worldUp   = Y_AXIS;
    up = worldUp;

    rotation = mat4(1.0f);
    target = position + FRONT; //front

    aspect_ratio = 1024.0f / 768.0f;
    fov = 45.0f;
    near_plane = 0.1f;
    far_plane = 1000.0f;

    view = glm::lookAt(position, target, up);
    projection = glm::perspective(zoom, aspect_ratio, near_plane, far_plane);

}


//------------------ Physics ------------------------

void Camera::addGhostCollisionObject(){
//TODO:
}


//or world->contactTest(player, callback);
void Camera::updateGhostCollisions(){
    btManifoldArray manifoldArray;

    btBroadphasePairArray& pairArray =
            ((btPairCachingGhostObject*)collisionObject)->getOverlappingPairCache()->getOverlappingPairArray();
    int numPairs = pairArray.size();

    for (int i = 0; i < numPairs; ++i)
    {
        manifoldArray.clear();

        const btBroadphasePair& pair = pairArray[i];

        btBroadphasePair* collisionPair =
                context->physical_system->dynamicsWorld->getPairCache()->findPair(
                    pair.m_pProxy0,pair.m_pProxy1);

        if (!collisionPair) continue;

        if (collisionPair->m_algorithm)
            collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

        for (int j=0;j<manifoldArray.size();j++)
        {
            btPersistentManifold* manifold = manifoldArray[j];

            bool isFirstBody = manifold->getBody0() == collisionObject;

            btScalar direction = isFirstBody ? btScalar(-1.0) : btScalar(1.0);

            for (int p = 0; p < manifold->getNumContacts(); ++p)
            {
                const btManifoldPoint&pt = manifold->getContactPoint(p);

                if (pt.getDistance() < 0.f)
                {
                    const btVector3& ptA = pt.getPositionWorldOnA();
                    const btVector3& ptB = pt.getPositionWorldOnB();
                    const btVector3& normalOnB = pt.m_normalWorldOnB;

                    // handle collisions here
                }
            }
        }
    }
}

//TODO:
void Camera::checkCollision(vec3 target){
    btVector3 btFrom(position.x, position.y, position.z);
    btVector3 btTo(target.x, target.y, target.z);
    btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

    context->physical_system->dynamicsWorld->rayTest(btFrom, btTo, res);

    if(res.hasHit()){
        cerr << "Collision at: " << res.m_hitPointWorld.getX() << res.m_hitPointWorld.getY()<< res.m_hitPointWorld.getZ() << endl;
    }
}


//TODO:
void Camera::addCollision(){
    btCollisionShape * sphereShape = new btSphereShape(1);
    collisionObject = new btCollisionObject();
    collisionObject->setCollisionShape(sphereShape);
    btTransform playerWorld;
    playerWorld.setIdentity();

    playerWorld.setOrigin(btVector3({position.x,position.y,position.z}));

    collisionObject->setWorldTransform(playerWorld);

    context->physical_system->dynamicsWorld->addCollisionObject(collisionObject);
}

void Camera::moveTo_phys(vec3 position){
    collisionObject->getWorldTransform().setOrigin(vec3tobt(position));
}


//------------------ Transforms ------------------------


void Camera::setOrientation(float rightAngle, float upAngle)
{
    yaw_angle  = rightAngle;
    pitch_angle = upAngle;
}

void Camera::offsetOrientation(float rightAngle, float upAngle)
{
    yaw_angle += rightAngle;
    pitch_angle += upAngle;
}

mat4 Camera::getOrientation() const
{
    glm::quat q = glm::angleAxis(glm::radians(pitch_angle), Y_AXIS);
    q*= glm::angleAxis(glm::radians( yaw_angle), X_AXIS);
    return  mat4_cast(q);
}


mat4 Camera::calcViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}



//------------------ Controls ------------------------


void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw_angle   += xoffset;
    pitch_angle += yoffset;

}



void Camera::processMouseScroll(GLfloat yoffset)
{
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= mouseSensitivity*2 * yoffset;
    if (zoom <= 1.0f)
        zoom = 1.0f;
    if (zoom >= 45.0f)
        zoom = 45.0f;
}


void Camera::processKeyboard(Camera_Movement direction, GLfloat deltaTime)
{

    float velocity = movementSpeed * deltaTime;
    if(direction == BACKWARD)
        position -= front * velocity;

    if(direction == FORWARD )
        position += front * velocity;

    if(direction == LEFT)
        position -= right * velocity;

    if(direction == RIGHT)
        position += right * velocity;

    if(direction == UP)
        position += up * velocity;

    if(direction == DOWN)
        position -= up * velocity;
}


