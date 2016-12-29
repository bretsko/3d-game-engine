
#include "InputControlSystem.h"
#include "GeometryUtils/Quaternion_utils.h"

InputControlSystem::InputControlSystem (Context *ctx):context(ctx) {

    camera = make_shared<Camera>(ctx,vec3(0.0f, 0.0f, 3.0f));

    camera->position = vec3( 80, 50, 0 );

    camera->pitch_angle = -3.14f / 4 ;
    camera->yaw_angle = 3.14f;

    camera->initialFoV = 45.0f;
    camera->fov = camera->initialFoV;

    speed = 100.0f;
    camera->movementSpeed = speed;
    mouseSpeed = 0.5f;

    camera->view = mat4(1.0f);
    modelMatrix =  camera->view;

    max_pitch_rate = 5;
    max_heading_rate = 5;

    vec3 cameraPos   = vec3(0.0f, 0.0f,  3.0f);
    vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
    vec3 cameraUp    = vec3(0.0f, 1.0f,  0.0f);

    mousePosition = vec2(float(WINDOW_WIDTH/2), float(WINDOW_HEIGHT/2));

    projectionMatrix = glm::perspective(camera->fov, 4.0f / 3.0f, 0.1f, 1000.0f);
}


void InputControlSystem::moveCamera(float deltaTime){
    if (glfwGetKey( context->window, GLFW_KEY_E ) == GLFW_PRESS){

        shared_ptr<Object> obj = context->current_scene->objects["Face"];

        obj->translate(0.1,0.1,0.1);
    }

    if (glfwGetKey( context->window, GLFW_KEY_Q ) == GLFW_PRESS){
        camera->processKeyboard(UP ,deltaTime);
    }
    if (glfwGetKey( context->window, GLFW_KEY_Z ) == GLFW_PRESS){
        camera->processKeyboard(DOWN,deltaTime);
    }
    if (glfwGetKey( context->window, GLFW_KEY_W ) == GLFW_PRESS){
        camera->processKeyboard(FORWARD,deltaTime);
    }
    if (glfwGetKey( context->window, GLFW_KEY_S ) == GLFW_PRESS){
        camera->processKeyboard(BACKWARD  ,deltaTime);
    }
    if (glfwGetKey( context->window, GLFW_KEY_D ) == GLFW_PRESS){
        camera->processKeyboard(RIGHT,deltaTime);
    }
    if (glfwGetKey( context->window, GLFW_KEY_A ) == GLFW_PRESS){
        camera->processKeyboard(LEFT,deltaTime);
    }
}



//TODO: EULER WORKING
void InputControlSystem::computeMatricesFromInputs(){

    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    moveCamera(deltaTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(context->window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(context->window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    vec2 mouse_delta = vec2(xpos, ypos) - mousePosition;

    camera->yaw_angle += deltaTime*mouseSpeed * float(WINDOW_WIDTH/2 - xpos );
    camera->pitch_angle  += deltaTime*mouseSpeed * float( WINDOW_HEIGHT/2 - ypos );

    camera->front = vec3(
                sin(camera->pitch_angle),
                cos(camera->yaw_angle) * cos(camera->pitch_angle),
                sin(camera->yaw_angle) * cos(camera->pitch_angle)
                );

    camera->front = glm::normalize( camera->front);

    camera->right = glm::normalize(glm::cross( camera->front, camera->worldUp));

    camera->right = glm::normalize( camera->right);

    camera->up = glm::normalize(glm::cross(  camera->right,  camera->front));

    float FoV = camera->initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    projectionMatrix = glm::perspective(camera->zoom, (GLfloat)WINDOW_WIDTH  / (GLfloat)WINDOW_HEIGHT, 0.1f, 1000.0f);


    camera->view = glm::lookAt(camera->position, camera->position + camera->front, camera->up);

    viewMatrix = camera->view ;

    lastTime = currentTime;
}


mat4  InputControlSystem::getViewQuaternion(){

    quat MyQuaternion  = quat(vec3(camera->pitch_angle,camera->yaw_angle,0));

    glm::quat quat1 = quat(vec3(camera->pitch_angle,0,0));
    glm::quat quat2 = quat(vec3(0,camera->yaw_angle,0));

    glm::quat quat3 = glm::cross(quat1, quat2);

    quat3 = glm::normalize(quat3);

    return glm::toMat4(quat3);
}


void InputControlSystem::computeMatricesFromInputs3(){
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(context->window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(context->window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    //camera->processKeyboard();

    camera->processMouseMovement(xpos,ypos);

    // camera->updateCameraVectors(); //is called in processMouseMovement

    viewMatrix = camera->calcViewMatrix();
    lastTime = currentTime;
}





void InputControlSystem::Move2D(int x, int y) {
    //compute the mouse delta from the previous mouse position
    vec3 mouse_delta = mouse_position - vec3(x, y, 0);
    //if the camera is moving, meaning that the mouse was clicked and dragged, change the pitch and heading
    // if (move_camera) {
    ChangeHeading(.08f * mouse_delta.x);
    ChangePitch(.08f * mouse_delta.y);
    // }
    mouse_position = vec3(x, y, 0);
}

void InputControlSystem::ChangePitch(float degrees) {
    //Check bounds with the max pitch rate so that we aren't moving too fast
    if (degrees < -max_pitch_rate) {
        degrees = -max_pitch_rate;
    } else if (degrees > max_pitch_rate) {
        degrees = max_pitch_rate;
    }
    camera->pitch_angle += degrees;

    //Check bounds for the camera pitch
    if (camera->pitch_angle > 360.0f) {
        camera->pitch_angle -= 360.0f;
    } else if (camera->pitch_angle < -360.0f) {
        camera->pitch_angle += 360.0f;
    }
}


void InputControlSystem::ChangeHeading(float degrees) {
    //Check bounds with the max heading rate so that we aren't moving too fast
    if (degrees < -max_heading_rate) {
        degrees = -max_heading_rate;
    } else if (degrees > max_heading_rate) {
        degrees = max_heading_rate;
    }
    //This controls how the heading is changed if the camera is pointed straight up or down
    //The heading delta direction changes
    if (camera->pitch_angle > 90 && camera->pitch_angle < 270 || (camera->pitch_angle < -90 && camera->pitch_angle > -270)) {
        camera->yaw_angle -= degrees;
    } else {
        camera->yaw_angle += degrees;
    }
    //Check bounds for the camera heading
    if (camera->yaw_angle > 360.0f) {
        camera->yaw_angle -= 360.0f;
    } else if (camera->yaw_angle < -360.0f) {
        camera->yaw_angle += 360.0f;
    }
}




//need target
void InputControlSystem::quatCompute(double MouseX, double MouseY){

    static double currentRotationX = 0.0;

    // The maximum angle we can look up or down, in radians
    double maxAngle = 1;

    currentRotationX += camera->front.x;

    // We don't want to rotate up more than one radian, so we cap it.
    if(currentRotationX > 1)
    {
        currentRotationX = 1;
        return;
    }
    // We don't want to rotate down more than one radian, so we cap it.
    if(currentRotationX < -1)
    {
        currentRotationX = -1;
        return;
    }

    else
    {
        // get the axis to rotate around the x-axis.

        vec3 axis = glm::cross(camera->target - camera->position, camera->up);

        // To be able to use the quaternion conjugate, the axis to
        // rotate around must be normalized.

        axis =  glm::normalize(axis);

        // Rotate around the y axis
        rotateCamera(camera->target.y, axis.x, axis.y, axis.z);

        // Rotate around the x axis
        rotateCamera(camera->target.x, 0, 1, 0);
    }

    calculate_view(); // from camera view, position, up
}




//using quaternion - NOT WORKING
void InputControlSystem::rotateCamera(double Angle, double x, double y, double z)
{
    quaternion temp, quat_view, result;

    temp.x = x * sin(Angle/2);
    temp.y = y * sin(Angle/2);
    temp.z = z * sin(Angle/2);
    temp.w = cos(Angle/2);

    quat_view.x = camera->target.x;
    quat_view.y = camera->target.y;
    quat_view.z = camera->target.z;
    quat_view.w = 0;

    result = mult(mult(temp, quat_view), conjugate(temp));

    camera->target.x = result.x;
    camera->target.y = result.y;
    camera->target.z = result.z;


    // calculate_view();

    //at the end call
    //gluLookAt(camera->position.x, camera->position.y, camera->position.z,
    //camera->target.x, camera->target.y, camera->target.z, camera->up.x, camera->up.y, camera->up.z).

}


//TODO: from camera view, position, up
void InputControlSystem::calculate_view()  {
    //    vec4 forward = vec4(0.0f, 0.0f, -1.0f, 0.0f);
    //    vec4 forward_rotated = camera->view * forward; //mat4 x vec4

    //    camera->target = camera->position += forward_rotated;

    //    camera->view = glm::lookAt(v4tov3(camera->position), v4tov3(camera->target), v4tov3(camera->up));

    viewMatrix = glm::lookAt( camera->position, camera->position+camera->target,
                              // and looks here : at the same position, plus "direction"
                              camera->up // Head is up (set to 0,-1,0 to look upside-down)
                              );

}





void InputControlSystem::mouseMove(){

    //  vec2 mouse_delta = vec2(x, y) - mousePosition;

    //  viewMatrix = mat4_cast(camera->orientation);

    //mat4 rotate = mat4_cast(camera->orientation);

    // mat4 translate = mat4(1.0f);
    // translate = glm::translate(translate, -camera->position);
    //    //WAS translate = glm::translate(translate, -eyeVector);

    //    // camera->view = rotate * translate;

    // viewMatrix = rotate * translate;

    //    camera->right  = vec3(
    //                sin(camera->yaw_angle - 3.14f/2.0f),
    //                0,
    //                cos(camera->yaw_angle - 3.14f/2.0f)
    //                );

    //    camera->up = glm::normalize(glm::cross(  camera->right,  camera->front));


    vec3 axis = glm::cross(camera->front, camera->up);

    //compute  for pitch based on the camera pitch angle
    glm::quat pitch_quat = glm::angleAxis(camera->pitch_angle, axis);

    //determine heading quaternion from the camera up vector and the heading angle
    glm::quat heading_quat = glm::angleAxis(camera->yaw_angle, camera->up);

    glm::quat temp = glm::cross(pitch_quat, heading_quat);

    temp = glm::normalize(temp);

    camera->front = glm::rotate(temp, camera->front);



    //camera->position += camera->position_delta;
    camera->target = camera->position + camera->front * 1.0f;

    //damping for smooth camera
    //camera->yaw_angle *= .5;
    // camera->pitch_angle *= .5;
    // camera->position_delta = camera->position_delta * .8f;

    viewMatrix = glm::lookAt(camera->position, camera->target, camera->up);
}



//FPS, no roll
void InputControlSystem::mouseMove(double x, double y)
{
    //  if (isMousePressed == false)
    //    return;

    //always compute delta
    //mousePosition is the last mouse position
    vec2 mouse_delta = vec2(x, y) - mousePosition;

    //  const float mouseX_Sensitivity = 0.25f;
    //  const float mouseY_Sensitivity = 0.25f;
    //  //note that yaw and pitch must be converted to radians.
    //  //this is done in UpdateView() by glm::rotate
    //  camera->yaw  += mouseX_Sensitivity * mouse_delta.x;
    //  camera->pitch += mouseY_Sensitivity * mouse_delta.y;

    camera->rotate(glm::radians(-camera->pitch_angle),  X_AXIS );
    camera->rotate(glm::radians(camera->yaw_angle),Y_AXIS );

    //  camera->orientation = glm::normalize(camera->orientation);

    viewMatrix = camera->getViewMatrix();

    //  viewMatrix = mat4_cast(camera->orientation);

    //mat4 rotate = mat4_cast(camera->orientation);

    // mat4 translate = mat4(1.0f);
    // translate = glm::translate(translate, -camera->position);
    //    //WAS translate = glm::translate(translate, -eyeVector);

    //    // camera->view = rotate * translate;
    // viewMatrix = rotate * translate;

    mousePosition = vec2(x, y);
    //  updateViewEuler();
}





mat4 InputControlSystem::rotateQuaternion(float deltaTime){

    vec3 gPosition1 = camera->position;
    vec3 gOrientation1;

    vec3 gPosition2 = camera->front;
    quat gOrientation2;

    gOrientation1.x += glm::radians(camera->pitch_angle);
    gOrientation1.y += glm::radians(camera->yaw_angle);
    gOrientation1.z += glm::radians(camera->roll_angle);

    vec3 desiredDir = gPosition1-gPosition2;
    vec3 desiredUp = vec3(0.0f, 1.0f, 0.0f); // +Y

    quat targetOrientation = normalize(mylookAt(desiredDir, desiredUp));

    // And interpolate
    gOrientation2 = rotateTowards(gOrientation2, targetOrientation, 100.0f*deltaTime);


    return mat4_cast(gOrientation2);

}





mat4 InputControlSystem::rotateQuaternion1(){

    vec3 gPosition1 = camera->position;
    vec3 gOrientation1;

    vec3 gPosition2  = camera->target;

    // vec3 gPosition2( 1.5f, 0.0f, 0.0f);
    quat gOrientation2;


    // As an example, rotate arount the vertical axis at 180°/sec
    gOrientation1.x += glm::radians(camera->pitch_angle);
    gOrientation1.y += glm::radians(camera->yaw_angle);
    gOrientation1.z += glm::radians(camera->roll_angle);


    vec3 desiredDir = gPosition1-gPosition2;
    vec3 desiredUp = Y_AXIS; // +Y

    // Compute the desired orientation
    quat targetOrientation = normalize(mylookAt(desiredDir, desiredUp));

    // And interpolate
    gOrientation2 = rotateTowards(gOrientation2, targetOrientation, 1.0f);

    return mat4_cast(gOrientation2);

}



mat4 InputControlSystem::rotateQuaternion(){

    vec3 gPosition1(0.0f, 0.0f, 0.0f);
    vec3 gOrientation1;

    vec3 gPosition2( 1.5f, 0.0f, 0.0f);
    quat gOrientation2;

    bool gLookAtOther = true;

    // As an example, rotate arount the vertical axis at 180°/sec
    gOrientation1.x += glm::radians(camera->pitch_angle);
    gOrientation1.y += glm::radians(camera->yaw_angle);
    gOrientation1.z += glm::radians(camera->roll_angle);

    if (gLookAtOther){
        vec3 desiredDir = gPosition1-gPosition2;
        vec3 desiredUp = vec3(0.0f, 1.0f, 0.0f); // +Y

        // Compute the desired orientation
        quat targetOrientation = normalize(mylookAt(desiredDir, desiredUp));

        // And interpolate
        gOrientation2 = rotateTowards(gOrientation2, targetOrientation, 1.0f);
    }

    return mat4_cast(gOrientation2);
}








mat4 InputControlSystem::rotateCameraQuaternion(float deltaTime){

    vec3 gOrientation1;
    quat gOrientation2;

    // As an example, rotate around the vertical axis at 180°/sec
    gOrientation1.x += glm::radians(camera->pitch_angle);
    gOrientation1.y += glm::radians(camera->yaw_angle);
    gOrientation1.z += glm::radians(camera->roll_angle);

    vec3 desiredDir = camera->target - camera->position;

    // Compute the desired orientation
    quat targetOrientation = normalize(mylookAt(desiredDir, Y_AXIS));

    // And interpolate
    gOrientation2 = rotateTowards(gOrientation2, targetOrientation, 1.0f*deltaTime);

    return mat4_cast(gOrientation2);

}



void InputControlSystem::lookAt( vec3 target){
    camera->target = camera->position + camera->front * 1.0f;

    viewMatrix = glm::lookAt(camera->position, camera->target, camera->up);
}




void InputControlSystem::updateView()
{
    //temporary frame quaternion from pitch,yaw,roll
    //here roll is not used

    glm::quat key_quat = glm::quat(vec3(camera->pitch_angle, camera->yaw_angle, camera->roll_angle));
    //reset values
    // camera->roll_angle = camera->pitch_angle = camera->pitch_angle = 0;


    //order matters,update camera->quat
    camera->orientation = key_quat * camera->orientation;
    camera->orientation = glm::normalize(camera->orientation);

    viewMatrix = camera->getViewMatrix();
}


void InputControlSystem::updateView_pitch()
{
    //FPS camera:  RotationX(pitch) * RotationY(yaw)
    glm::quat qPitch = glm::angleAxis(camera->pitch_angle, Y_AXIS);
    glm::quat qYaw = glm::angleAxis(camera->yaw_angle, X_AXIS);
    glm::quat qRoll = glm::angleAxis(camera->roll_angle,Z_AXIS);

    //For a FPS camera we can omit roll
    glm::quat orientation = qPitch * qYaw;
    orientation = glm::normalize(orientation);
    mat4 rotate = mat4_cast(orientation);

    mat4 translate = mat4(1.0f);

    translate = glm::translate(translate, -camera->position);

    //WAS translate = glm::translate(translate, - eye);

    camera->view = rotate * translate;
}




void  InputControlSystem::computeMatricesFromInputs2(){

    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    double xpos, ypos;
    glfwGetCursorPos(context->window, &xpos, &ypos);

    glfwSetCursorPos(context->window, float(WINDOW_WIDTH/2), float(WINDOW_HEIGHT/2));

    // Compute new orientation
    camera->yaw_angle += mouseSpeed * float(WINDOW_WIDTH/2 - xpos );
    camera->pitch_angle   += mouseSpeed * float( WINDOW_HEIGHT/2 - ypos );
    clamp_angle(camera->yaw_angle);
    clamp_angle(camera->pitch_angle);
    camera->yaw_angle = camera->pitch_angle;
    camera->pitch_angle = camera->pitch_angle;

    camera->view= glm::rotate(
                camera->view,
                (float)glm::radians(mouseSpeed * ypos ), //angle_in_degrees
                Y_AXIS //rotate y
                );

    camera->view  = glm::rotate(
                camera->view,
                (float)glm::radians(mouseSpeed * ypos ),
                X_AXIS //rotate x
                );

    vec3 target(
                cos( camera->pitch_angle) * sin( camera->yaw_angle),
                sin( camera->pitch_angle),
                cos( camera->pitch_angle) * cos( camera->yaw_angle)
                );


    camera->target = target;

    viewMatrix = camera->view;

    lastTime = currentTime;
}





void InputControlSystem::setLookAt(vec3 look_at)
{
    auto  m_camera_look_at = look_at;
    auto m_camera_direction = glm::normalize(m_camera_look_at - camera->position);
    vec3 right_vector = glm::cross(m_camera_direction, vec3(0, 1, 0));
    auto m_camera_up = glm::cross(right_vector, m_camera_direction);
    return;
}


mat4 InputControlSystem::calculateMVP(){

    computeMatricesFromInputs(); //TODO: camera

    mat4 MVP_matrix = projectionMatrix * viewMatrix * modelMatrix;

    return MVP_matrix;
}



void InputControlSystem::updateViewEuler()
{
    //roll can be removed from here. because is not actually used in FPS camera
    //mat4 matRoll  = mat4(1.0f);//identity matrix;
    mat4 matPitch = mat4(1.0f);//identity matrix
    mat4 matYaw   = mat4(1.0f);//identity matrix

    //roll, pitch and yaw are used to store our angles in our class
    //matRoll = glm::rotate(matRoll, camera->roll, Z_AXIS);
    matPitch = glm::rotate(matPitch, camera->pitch_angle, Y_AXIS);
    matYaw = glm::rotate(matYaw, camera->yaw_angle, X_AXIS);

    //order matters
    // mat4 rotate = mattRoll * matPitch * matYaw;

    mat4 rotate = matPitch * matYaw;

    mat4 translate = mat4(1.0f);

    //was eyevector
    translate = glm::translate(translate, -camera->position);

    viewMatrix = rotate * translate;
}
