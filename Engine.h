#pragma once

#include "Common.h"
#include "Context.h"
#include "InputControlSystem/InputControlSystem.h"
#include "RenderSystem/RenderSystem.h"

#include <entityx/entityx.h>
namespace ex = entityx;

class Engine : public ex::EntityX
{
public:

    ~Engine();
    Engine(GLFWwindow* win);

    void update(ex::TimeDelta dt);

    void createSystems();

    Context *context;
};



