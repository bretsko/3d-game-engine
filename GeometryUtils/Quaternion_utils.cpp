#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include "GeometryUtils/Quaternion_utils.h"

// Returns a quaternion such that q*start = dest
quat rotationBetweenVectors(vec3 start, vec3 dest){
    start =  glm::normalize(start);
    dest = normalize(dest);

    float cosTheta = glm::dot(start, dest);
    vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f){
        // special case when vectors in opposite directions :
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        // This implementation favors a rotation around the Up axis,
        // since it's often what you want to do.
        rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
        if (length2(rotationAxis) < 0.01 ) // bad luck, they were parallel, try again!
            rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

        rotationAxis = normalize(rotationAxis);
        return angleAxis(180.0f, rotationAxis);
    }

    // Implementation from Stan Melax's Game Programming Gems 1 article
    rotationAxis = cross(start, dest);

    float s = sqrt( (1+cosTheta)*2 );
    float invs = 1 / s;

    return quat(
        s * 0.5f,
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
    );


}


// Returns a quaternion that will make your object looking towards 'direction'.
// Similar to rotationBetweenVectors, but also controls the vertical orientation.
// This assumes that at rest, the object faces +Z.
// Beware, the first parameter is a direction, not the target point !
quat mylookAt(vec3 direction, vec3 desiredUp){

    if (length2(direction) < 0.0001f )
        return quat();

    // Recompute desiredUp so that it's perpendicular to the direction
    // You can skip that part if you really want to force desiredUp
    vec3 right = cross(direction, desiredUp);
    desiredUp = cross(right, direction);

    // Find the rotation between the front of the object (that we assume towards +Z,
    // but this depends on your model) and the desired direction
    quat rot1 = rotationBetweenVectors(vec3(0.0f, 0.0f, 1.0f), direction);
    // Because of the 1rst rotation, the up is probably completely screwed up.
    // Find the rotation between the "up" of the rotated object, and the desired up
    vec3 newUp = rot1 * vec3(0.0f, 1.0f, 0.0f);
    quat rot2 = rotationBetweenVectors(newUp, desiredUp);

    // Apply them
    return rot2 * rot1; // remember, in reverse order.
}



// Like SLERP, but forbids rotation greater than maxAngle (in radians)
// In conjunction to lookAt, can make your characters
quat rotateTowards(quat q1, quat q2, float maxAngle){

    if( maxAngle < 0.001f ){
        // No rotation allowed. Prevent dividing by 0 later.
        return q1;
    }

    float cosTheta = dot(q1, q2);

    // q1 and q2 are already equal.
    // Force q2 just to be sure
    if(cosTheta > 0.9999f){
        return q2;
    }

    // Avoid taking the long path around the sphere
    if (cosTheta < 0){
        q1 = q1*-1.0f;
        cosTheta *= -1.0f;
    }

    float angle = acos(cosTheta);

    // If there is only a 2° difference, and we are allowed 5°,
    // then we arrived.
    if (angle < maxAngle){
        return q2;
    }

    // This is just like slerp(), but with a custom t
    float t = maxAngle / angle;
    angle = maxAngle;

    quat res = (sin((1.0f - t) * angle) * q1 + sin(t * angle) * q2) / sin(angle);
    res = normalize(res);
    return res;

}

















void tests(){

    glm::vec3 Xpos(+1.0f,  0.0f,  0.0f);
    glm::vec3 Ypos( 0.0f, +1.0f,  0.0f);
    glm::vec3 Zpos( 0.0f,  0.0f, +1.0f);
    glm::vec3 Xneg(-1.0f,  0.0f,  0.0f);
    glm::vec3 Yneg( 0.0f, -1.0f,  0.0f);
    glm::vec3 Zneg( 0.0f,  0.0f, -1.0f);

    // Testing standard, easy case
    // Must be 90° rotation on X : 0.7 0 0 0.7
    quat X90rot = rotationBetweenVectors(Ypos, Zpos);

    // Testing with v1 = v2
    // Must be identity : 0 0 0 1
    quat id = rotationBetweenVectors(Xpos, Xpos);

    // Testing with v1 = -v2
    // Must be 180° on +/-Y axis : 0 +/-1 0 0
    quat Y180rot = rotationBetweenVectors(Xpos, Xneg);

    // Testing with v1 = -v2, but with a "bad first guess"
    // Must be 180° on +/-Y axis : 0 +/-1 0 0
    quat X180rot = rotationBetweenVectors(Zpos, Zneg);


}




double length(quaternion quat)
{
    return sqrt(quat.x * quat.x + quat.y * quat.y +
                quat.z * quat.z + quat.w * quat.w);
}

quaternion normalize(quaternion quat)
{
    double L = length(quat);

    quat.x /= L;
    quat.y /= L;
    quat.z /= L;
    quat.w /= L;

    return quat;
}

quaternion conjugate(quaternion quat)
{
    quat.x = -quat.x;
    quat.y = -quat.y;
    quat.z = -quat.z;
    return quat;
}

quaternion mult(quaternion A, quaternion B)
{
    quaternion C;

    C.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
    C.y = A.w*B.y - A.x*B.z + A.y*B.w + A.z*B.x;
    C.z = A.w*B.z + A.x*B.y - A.y*B.x + A.z*B.w;
    C.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;
    return C;
}



void clamp_angle(float&angle){
    while (angle < 0.0f) angle += 360.0f;
    while (angle > 360.0f) angle -= 360.0f;
}


vec3 v4tov3(vec4 v1)  {
    return vec3(v1.x, v1.y, v1.z);
}


void printMatrix(const float m[16], const string &str)
{
    printf("%s:\n", str.c_str());

    for (int i=0; i<4; i++)
    {
        printf("[");
        //for (int j=i*4+0; j<i*4+4; j++)   // row major, 0, 1, 2, 3
        for (int j=i+0; j<16; j+=4) // OpenGL is column major by default, 0, 4, 8, 12
        {
            //printf("%d, ", j);            // print matrix index
            printf("%.2f, ", m[j]);

        }
        printf("]\n");
    }
    printf("\n");
}




// Returns a quaternion such that q*start = dest
quat RotationBetweenVectors(vec3 start, vec3 dest){
 start = normalize(start);
 dest = normalize(dest);

 float cosTheta = dot(start, dest);
 vec3 rotationAxis;

 if (cosTheta < -1 + 0.001f){
  // special case when vectors in opposite directions :
  // there is no "ideal" rotation axis
  // So guess one; any will do as long as it's perpendicular to start
  // This implementation favors a rotation around the Up axis,
  // since it's often what you want to do.
  rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
  if (length2(rotationAxis) < 0.01 ) // bad luck, they were parallel, try again!
   rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

  rotationAxis = normalize(rotationAxis);
  return angleAxis(180.0f, rotationAxis);
 }

 // Implementation from Stan Melax's Game Programming Gems 1 article
 rotationAxis = cross(start, dest);

 float s = sqrt( (1+cosTheta)*2 );
 float invs = 1 / s;

 return quat(
  s * 0.5f,
  rotationAxis.x * invs,
  rotationAxis.y * invs,
  rotationAxis.z * invs
 );


}



// Returns a quaternion that will make your object looking towards 'direction'.
// Similar to RotationBetweenVectors, but also controls the vertical orientation.
// This assumes that at rest, the object faces +Z.
// Beware, the first parameter is a direction, not the target point !
quat LookAt(vec3 direction, vec3 desiredUp){

 if (length2(direction) < 0.0001f )
  return quat();

 // Recompute desiredUp so that it's perpendicular to the direction
 // You can skip that part if you really want to force desiredUp
 vec3 right = cross(direction, desiredUp);
 desiredUp = cross(right, direction);

 // Find the rotation between the front of the object (that we assume towards +Z,
 // but this depends on your model) and the desired direction
 quat rot1 = RotationBetweenVectors(vec3(0.0f, 0.0f, 1.0f), direction);
 // Because of the 1rst rotation, the up is probably completely screwed up.
 // Find the rotation between the "up" of the rotated object, and the desired up
 vec3 newUp = rot1 * vec3(0.0f, 1.0f, 0.0f);
 quat rot2 = RotationBetweenVectors(newUp, desiredUp);

 // Apply them
 return rot2 * rot1; // remember, in reverse order.
}



// Like SLERP, but forbids rotation greater than maxAngle (in radians)
// In conjunction to LookAt, can make your characters 
quat RotateTowards(quat q1, quat q2, float maxAngle){

 if( maxAngle < 0.001f ){
  // No rotation allowed. Prevent dividing by 0 later.
  return q1;
 }

 float cosTheta = dot(q1, q2);

 // q1 and q2 are already equal.
 // Force q2 just to be sure
 if(cosTheta > 0.9999f){
  return q2;
 }

 // Avoid taking the long path around the sphere
 if (cosTheta < 0){
  q1 = q1*-1.0f;
  cosTheta *= -1.0f;
 }

 float angle = acos(cosTheta);

 // If there is only a 2° difference, and we are allowed 5°,
 // then we arrived.
 if (angle < maxAngle){
  return q2;
 }

 // This is just like slerp(), but with a custom t
 float t = maxAngle / angle;
 angle = maxAngle;

 quat res = (sin((1.0f - t) * angle) * q1 + sin(t * angle) * q2) / sin(angle);
 res = normalize(res);
 return res;

}
