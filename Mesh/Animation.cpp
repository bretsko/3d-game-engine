#include "Animation.h" 

Animation::Animation(AnimationType type):animationType(type), started(false), ended(false),hasLink(false){}
//------------------ Properties ------------------------

//------------------ Utilities ------------------------

void Animation::setStart(Mesh* mesh, vec3& start){
 this->mesh = mesh;
 this->start = start;
}
void Animation::setGoal(vec3& goal, float duration, EasingType easing){
 this->goal = goal;
 this->duration = duration;
 changeInValue = goal - start;
 easingType = easing;
}

//Linear or now, try ease.
vec3 Animation::calculateStep(float t){
 switch(easingType)
 {
  case LINEAR:
   return changeInValue * (t/duration) + start;
  case ELASTIC_OUT:
   {//From Robert Penning's easing functions
    if (t == 0.0f) return start;
    if ((t /= duration) == 1.0f) return start + changeInValue;  
    float p = duration * .3f;
    vec3 a = changeInValue; 
    float s = p / 4;
    return (a * (float)pow(2, -10 * t) * (float)sin( (t * duration - s) * (2 * PI)/ p ) + changeInValue + start);
   }
  case QUAD_OUT:
   return (-changeInValue) * (t - 2) * (t /= duration)  + start;
  case SINE_OUT:
   return changeInValue * (float) sin(t/duration * (PI/2.0f)) + start;
  case NONE:
   return goal;
 }
 //linear interpolation (y = mx + b)
 /*
 y is the new position, m is change in value over duration, x is time, b is starting value
 */
 //return (1.0f - t) * start + t* goal;
}

//Implement these, decide whether to make calculations separately or not.
void Animation::update(int time){
 if(!started){
  startTime = time;
  started = true;
 }
 float difference = time - startTime;
 
 if(difference > duration){
  ended = true;
  //take parameters from chained one to this one
  //or let client handle traversing chain
  //or have an animation manager class that can start animations. That way mesh is only passed once.
  //bad thins is that you can't use the same animation on multiple variables witout perhaps some ugliness.
  return;
 }

// switch(animationType){
//  case TRANSLATE:
//   mesh->translate(calculateStep(difference));
//   break;
//  case ROTATE:
//   mesh->rotateCenteredTo(calculateStep(difference));
//   break;
//  case SCALE:
//   mesh->scaleCenter(calculateStep(difference));
//   break;
//  case POSITION:
//   mesh->moveTo(calculateStep(difference));
//   break;
//  case TRANSPARENCY: //only the first value is taken into account
//   mesh->setAlpha(calculateStep(difference).x);
//   break;
// }
}

void Animation::chain(Animation& animation){
 //copy to heap
 hasLink = true;
 nextLink = new Animation(animation);
}

Animation::~Animation(){
 //delete nextLink;
}
