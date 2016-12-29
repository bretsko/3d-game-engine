
#include "Object.h"
#include "MeshFactory.h"
#include "TextureFactory.h"
#include "GeometryUtils/Quaternion_utils.h"

Object::Object(Context *ctx, MeshType meshType, TextureType textureType, string name):
    context(ctx),
    name(name),
    position({0,0,0}),
    translationMatrix(glm::translate(mat4(), {0,0,0})),
    rotationMatrix(glm::eulerAngleYXZ(0, 0, 0)),
    scalingMatrix( glm::scale(mat4(), vec3(1, 1, 1)))
{

    cerr << "Created "<<  name << endl;

    meshes = MeshFactory::createMesh(meshType);
    texture = TextureFactory::createTexture(textureType);
}

Object::Object(Context *ctx, MeshType meshType, string name):context(ctx),name(name),position({0,0,0}), translationMatrix(glm::translate(mat4(), {0,0,0})), rotationMatrix(glm::eulerAngleYXZ(0, 0, 0)),scalingMatrix( glm::scale(mat4(), vec3(1, 1, 1)))
{
    cerr << "Created "<<  name << endl;

    meshes = MeshFactory::createMesh(meshType);
}


//------------------ Physics ------------------------


void Object::stepPhysicSimulation()
{
}


//update physics, position, modelMatrix,
void Object::updatePhysics(){      //not working

    btTransform trans;
    physBody->rigidBody->applyGravity();

    physBody->rigidBody->getMotionState()->getWorldTransform(trans);

    position = btTransformToVec3(trans);

    //TODO:
    btVector3 rotationEuler = quaternionToEuler(trans.getRotation());

    mat4 TranslationMatrix = glm::translate(mat4(), position);

    translationMatrix = TranslationMatrix;
}



//TODO: check
mat4 Object::getModelMatrix() const
{
    return mat4();
}


void Object::addPhysicalBody(btScalar mass ){

    physBody = make_shared<PhysicalBody>(this ,context , STATIC_BODY, mass);
}

//------------------ Transforms ------------------------


void Object::translate(float dist_x, float dist_y,float dist_z){

    vec3 translation(dist_x, dist_y, dist_z);


    position += translation;

    if (physBody)
        physBody->setPosition(position);

    mat4 TranslationMatrix = glm::translate(mat4(), translation);

    translationMatrix *= TranslationMatrix;
}

//--------------------------TODO:Rotate Around pivot-------------------------

void Object::rotateEuler(float angle_x, float angle_y,float angle_z){

    vec3 orientation;

    orientation.x += glm::radians(angle_x);
    orientation.y += glm::radians(angle_y);
    orientation.z += glm::radians(angle_z);

    mat4 RotationMatrix = glm::eulerAngleYXZ(orientation.y, orientation.x, orientation.z);

    rotationMatrix *= RotationMatrix;
}


void Object::scale(float times_x, float times_y,float times_z){

    mat4 ScalingMatrix = glm::scale(mat4(), vec3(times_x, times_y, times_z));
    scalingMatrix *= ScalingMatrix;
}



void Object::rotateQuaternion(float angle_x, float angle_y,float angle_z){

    vec3 gPosition1(0.0f, 0.0f, 0.0f);
    vec3 gOrientation1;

    vec3 gPosition2( 1.5f, 0.0f, 0.0f);
    quat gOrientation2;

    bool gLookAtOther = true;

    gOrientation1.x += glm::radians(angle_x);
    gOrientation1.y += glm::radians(angle_y);
    gOrientation1.z += glm::radians(angle_z);

    if (gLookAtOther){
        vec3 desiredDir = gPosition1-gPosition2;
        vec3 desiredUp = vec3(0.0f, 1.0f, 0.0f); // +Y

        // Compute the desired orientation
        quat targetOrientation = normalize(mylookAt(desiredDir, desiredUp));

        // And interpolate
        gOrientation2 = rotateTowards(gOrientation2, targetOrientation, 1.0f);
    }

    mat4 RotationMatrix = mat4_cast(gOrientation2);

    rotationMatrix *= RotationMatrix;
}


void Object::setPosition(float coord_x, float coord_y,float coord_z,float){
    position.x = coord_x;
    position.y = coord_y;
    position.z = coord_z;
}



//------------------ computeTransform ------------------------

void Object::computeTransform(){
    computeScaling();
    computeRotation();
    computeTranslation();
    transform = translationMatrix * rotationMatrix * scalingMatrix;
}

void Object::computeTranslation(){

    translationMatrix = glm::translate(mat4(), position); //or -position

}

void Object::computeRotation(){  //TODO:
    //
}

void Object::computeScaling(){  //TODO:
    //
}


//------------------ utilities ------------------------

void Object::setName(MeshType meshType){
    switch (meshType) {
    case MilitiaBlend:
        name = "MilitiaBlend";
        break;
    case MeshKidsRoom:
        name = "MeshKidsRoom";
        break;
    case FaceFBX:
        name = "FaceFBX";
        break;
    case Mesh1:
        name = "Mesh1";
        break;
    case Mesh2: name = "Mesh2"; break;
    case Mesh3: name = "Mesh3"; break;
    case Mesh4: name = "Mesh4"; break;
    case Mesh5: name = "Mesh5"; break;
    case Mesh6: name = "Mesh6"; break;
    case MeshSkeletons: name = "MeshSkeletons"; break;
    case MeshCube: name = "MeshCube"; break;
    case MeshSuzanne: name = "MeshSuzanne"; break;
    case MeshBench: name = "MeshBench"; break;
    case MeshRoom: name = "MeshRoom"; break;
    case MeshRoomThickWalls: name = "MeshRoomThickWalls"; break;
    }
}




//--------------------Transform Animations-----------------------------


void Object::rotateEuler(float angle_x, float angle_y,float angle_z,float deltaTime){

    vec3 orientation;

    orientation.x += glm::radians(angle_x)* deltaTime;
    orientation.y += glm::radians(angle_y)* deltaTime;
    orientation.z += glm::radians(angle_z)* deltaTime;

    mat4 RotationMatrix = glm::eulerAngleYXZ(orientation.y, orientation.x, orientation.z);

    rotationMatrix *= RotationMatrix;
}
void Object::transformQuaternion( float deltaTime){

    vec3 gPosition1(-1.5f, 0.0f, 0.0f);
    vec3 gOrientation1;

    vec3 gPosition2( 1.5f, 0.0f, 0.0f);
    quat gOrientation2;

    bool gLookAtOther = true;

    gOrientation1.y += 3.14159f/2.0f * deltaTime;


    if (gLookAtOther){
        vec3 desiredDir = gPosition1-gPosition2;
        vec3 desiredUp = vec3(0.0f, 1.0f, 0.0f); // +Y

        quat targetOrientation = normalize(mylookAt(desiredDir, desiredUp));

        gOrientation2 = rotateTowards(gOrientation2, targetOrientation, 1.0f*deltaTime);
    }

    mat4 RotationMatrix = mat4_cast(gOrientation2);

    rotationMatrix *=  RotationMatrix;
}
void Object::translate(float dist_x, float dist_y,float dist_z,float deltaTime){

    vec3 translation(dist_x* deltaTime, dist_y* deltaTime, dist_z* deltaTime);


    position+=translation;

    if (physBody)
        physBody->setPosition(position);

    mat4 TranslationMatrix = glm::translate(mat4(), translation);

    translationMatrix *= TranslationMatrix;
}



void Object::rotateQuaternion(float angle_x, float angle_y,float angle_z,float deltaTime){

    vec3 gPosition1(0.0f, 0.0f, 0.0f);
    vec3 gOrientation1;

    vec3 gPosition2( 1.5f, 0.0f, 0.0f);
    quat gOrientation2;

    bool gLookAtOther = true;

    gOrientation1.x += glm::radians(angle_x)* deltaTime;
    gOrientation1.y += glm::radians(angle_y)* deltaTime;
    gOrientation1.z += glm::radians(angle_z)* deltaTime;

    if (gLookAtOther){
        vec3 desiredDir = gPosition1-gPosition2;
        vec3 desiredUp = vec3(0.0f, 1.0f, 0.0f); // +Y

        // Compute the desired orientation
        quat targetOrientation = normalize(mylookAt(desiredDir, desiredUp));

        // And interpolate
        gOrientation2 = rotateTowards(gOrientation2, targetOrientation, 1.0f*deltaTime);
    }

    mat4 RotationMatrix = mat4_cast(gOrientation2);

    rotationMatrix *= RotationMatrix;
}


//TODO:
void Object::rotateTowardsQuaternion(float angle_x, float angle_y,float angle_z,float deltaTime){

}


void Object::scale(float times_x, float times_y,float times_z,float deltaTime){

    mat4 ScalingMatrix = glm::scale(mat4(), vec3(times_x* deltaTime, times_y* deltaTime, times_z* deltaTime));
    scalingMatrix *= ScalingMatrix;
}


