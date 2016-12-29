#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <list>
#include "TypesNames.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <entityx/entityx.h>
namespace ex = entityx;

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::list;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using std::map;
using std::pair;
using std::make_pair;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;



//------------------ Constants ------------------------


static const float kInfinity = std::numeric_limits<float>::max();
static const float kEpsilon = 1e-8;

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define Print(x)  do { std::cerr << #x " = " << (x) << endl; } while(0)
//#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

//#define HAVE_M_PI
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#define DEG_TO_RAD(x) ((x) * (PI / 180.0f))
#define RAD_TO_DEG(x) ((x) * (180.0f / PI))

#define X_AXIS vec3(0.0f, 1.0f, 0.0f)
#define Y_AXIS vec3(1.0f, 0.0f, 0.0f) //or -1
#define Z_AXIS vec3(0.0f, 0.0f, 1.0f)
#define FRONT vec3(1.0f, 0.0f,0.0f) //(0.0f, 0.0f, 1.0f)
#define ORIGIN vec3(0.0f, 0.0f, 0.0f)

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)
#define BIT(x) (1<<(x))
