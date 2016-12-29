#pragma once

#include "Context.h"
#include "Camera.h"
//------------------ Properties ------------------------

//------------------ Utilities ------------------------
struct Context ;
class Camera;

class InputControlSystem {

    //TODO:only input must be here, move somewhere MVP

public:
    InputControlSystem(Context *context);

    mat4 calculateMVP();

    void quatCompute(double MouseX, double MouseY);
    //void quatCompute1(double MouseX, double MouseY);
    void computeMatricesFromInputs();
    //void computeMatricesFromInputs1();
    void computeMatricesFromInputs2();
    void computeMatricesFromInputs3();

    void moveCamera(float deltaTime);
    void rotateCamera(double angle, double x, double y, double z);
    void updateView();
    void mouseMove(int x, int y, int width, int height);
    void updateView_pitch();
    void mouseMove(double x, double y);
    void mouseMove();

    vec3 mouse_position;

    void updateViewEuler();

    //from camera position, target, up
    void calculate_view();

    mat4 rotateQuaternion(float deltaTime);
    mat4 rotateQuaternion();
    mat4 rotateQuaternion1();
    mat4 rotateCameraQuaternion(float deltaTime);
    mat4 rotateCameraQuaternion();
    mat4 rotateCameraQuaternion1();
    void ChangePitch(float degrees);
    void ChangeHeading(float degrees);
    void Move2D(int x, int y) ;

    void lookAt( vec3 target);
    void setLookAt(vec3 look_at);
    //mat4 getViewQuaternion(double rotationAngle, vec3 rotationAxis);
    mat4 getViewQuaternion();
    void setOrientation(float rightAngle, float upAngle);//in degrees
    void offsetOrientation(float rightAngle, float upAngle);//in degrees


    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 modelMatrix;

    glm::quat  viewQuat;

    shared_ptr<Camera> camera;

    float speed; // 3 units / second
    float mouseSpeed;

    //    int MouseX;
    //    int MouseY;

    vec2 mousePosition;
    // vector that describes mouseposition - center
    //vec3 MouseDirection;


    float max_pitch_rate;
    float max_heading_rate;
    bool move_camera;

    vec3 camera_up;

    float key_pitch;
    float key_yaw;
    float key_roll;

    Context *context;
};



