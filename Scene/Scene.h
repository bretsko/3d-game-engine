#pragma once

#include "Common.h"
#include "Scene/Object.h"
#include "PhysicalSystem/PhysicalBody.h"
#include "Lights/PointLight.h"

class Object;

struct Scene
{
    vector <shared_ptr<PointLight>> pointLights;
    map<string,shared_ptr<Object>> objects;
};
