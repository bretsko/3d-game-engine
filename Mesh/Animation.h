#pragma once

#include <glm/glm.hpp>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"

#ifndef PI
#define PI  3.14159265
#endif
//------------------ Properties ------------------------

//------------------ Utilities ------------------------
//Client program creates these and passes them to mesh.
//Right when mesh finds one of these, it starts the update method.
class Animation
{
public:
 enum AnimationType {ROTATE, SCALE, TRANSLATE, POSITION, TRANSPARENCY};
 enum EasingType {LINEAR, ELASTIC_OUT, QUAD_OUT, SINE_OUT, NONE};
 Animation(AnimationType type);

 //Pass in the initial values, read from somewhere.
 //Typically 0,0,0 if object is untranslated.
 //1.0, 1.0f, 1.0f if object is unscaled.
 //0, 0, 0 if object is unrotated.
 //Pass in current center if moving to an absolute position.
 void setStart(Mesh* mesh, vec3& start);
 //Enter final scale, rotation (degrees), or position
 void setGoal(vec3& goal, float duration, EasingType easing = LINEAR);
 //Calls the appropriate transformation function in the mesh.
 void update(int time);

 void chain(Animation& animation);

 bool hasEnded() {return ended;}
 bool containsLink() {return hasLink;}
 Animation* getLink(){ return nextLink;}

 ~Animation();

private:
 vec3 calculateStep(float t);
 AnimationType animationType;
 EasingType easingType;
 vec3 start;
 vec3 changeInValue;
 vec3 goal;
 vec3 currentTransformation; // a portion of the goal trans
 int startTime;
 int timeLeft;
 float duration;
 bool started;
 bool hasLink;
 bool ended;
 Mesh* mesh;
 Animation* nextLink;
};
