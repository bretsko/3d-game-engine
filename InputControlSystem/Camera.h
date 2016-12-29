#pragma once

#include "Common.h"
#include "Scene/Object.h"
#include "btutilities.h"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW        = 180.0f; //0.0f;
const float PITCH      =  0.0f; //180.0f;
const float SPEED      =  1.0f;
const float SENSITIVTY = 0.01;
const float ZOOM       =  45.0f;


class Context;

//TODO: class Camera : public Object
class Camera
{
public:

    Camera(Context * ctx, vec3 position = vec3(ORIGIN), vec3 target = vec3(vec3(ORIGIN) + FRONT));

    // Cotr with vectors
    Camera(Context * ctx,vec3 position, vec3 up , float yaw, float pitch);

    // Cotr with scalar values
    Camera(Context * ctx,float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);


    //------------------ Transforms ------------------------


    void setOrientation(float rightAngle, float upAngle);
    void offsetOrientation(float rightAngle, float upAngle);
    mat4 getOrientation() const;

    void scale(float zoomFactor);

    //Camera will be rotated to look at current look at point.
    void moveTo(vec3& point);
    void lookAt(vec3& point);
    vec3 windowToWorld(vec3 winCoord, glm::vec4 viewPort);

    //move to distance of object from center, rotate, move back
    void rotateAbout(vec3 rotations, vec3 point);

    //Only rotates position,
    void rotate(vec3& rotations);
    void translate(vec3& offset);

    // Returns the view matrix calculated using Eular Angles and the lookAt Matrix
    mat4 calcViewMatrix();

    //private:

    void pitch(float pitchRadians) {
        rotate(pitchRadians, Y_AXIS);
    }

    void yaw(float yawRadians) {
        rotate(yawRadians, X_AXIS);
    }

    void roll(float rollRadians) {
        rotate(rollRadians, Z_AXIS);
    }

    void turn(float turnRadians) {
        glm::quat q = glm::angleAxis(turnRadians, orientation * X_AXIS);
        rotate(q);
    }

    void rotate(float angleRadians, const vec3& axis) {
        glm::quat q = glm::angleAxis(angleRadians, axis);
        rotate(q);
    }

    void rotate(const glm::quat& rotation) {
        orientation = rotation * orientation;
    }

    vec3 getForward() const {
        return glm::conjugate(orientation) * FRONT;
    }

    vec3 getLeft() const {
        return glm::conjugate(orientation) * vec3(-1.0, 0.0f, 0.0f);
    }

    vec3 getUp() const {
        return glm::conjugate(orientation) * X_AXIS;
    }

    void moveForward(float movement) {
        position += getForward() * movement;
    }

    void moveLeft(float movement) {
        position += getLeft() * movement;
    }

    void moveUp(float movement) {
        position += getUp() * movement;
    }

    mat4 getViewMatrix() const {
        mat4 viewMatrix = mat4_cast(orientation);
        viewMatrix = glm::translate(viewMatrix, -position);
        return viewMatrix;
    }

    //------------------ Collision ------------------------

    void init();
    void moveTo_phys(vec3 position);
    void addCollision();
    void checkCollision(vec3 target);
    void addGhostCollisionObject();
    void updateGhostCollisions();


    //------------------ Controls ------------------------
    void processMouseScroll(float yoffset);
    void processKeyboard(Camera_Movement direction, GLfloat deltaTime);

    void processMouseMovement(GLfloat xoffset, GLfloat yoffset);


    //------------------ Properties ------------------------

    vec3 position;
    quat orientation;

    vec3 up;
    vec3 right;
    vec3 worldUp;
    vec3 front;
    vec3 target;

    mat4 rotation;

    mat4 view;
    mat4 projection;


    // Eular Angles
    float yaw_angle;
    float pitch_angle;
    float roll_angle;

    // Camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
    float initialFoV;
    float aspect_ratio;
    float fov;
    float near_plane;
    float far_plane;

    vec3 position_delta;
    vec3 direction;

    //Physics
    btCollisionObject * collisionObject;

    string name;
    Context * context;
};

