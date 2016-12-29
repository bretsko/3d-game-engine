#pragma once

#include "Common.h"
#include "Scene/Scene.h"
#include "InputControlSystem.h"
#include "RenderSystem/RenderSystem.h"
#include "PhysicalSystem/PhysicalSystem.h"

class InputControlSystem;
class Scene;
class RenderSystem;
class PhysicalSystem;

struct Context
{

    shared_ptr<InputControlSystem> input_system;
    shared_ptr<RenderSystem> render_system;
    shared_ptr<PhysicalSystem> physical_system;

    ex::EventManager * eventManager;
    ex::EntityManager * entityManager;
    ex::SystemManager * systemManager;

    unique_ptr<Scene> current_scene;
    GLFWwindow* window;
};

