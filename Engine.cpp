
#include "Engine.h"
#include "Common.h"

#include "PhysicalSystem.h"

#include <chrono>
#include <stdio.h>
#include <stdlib.h>


Engine:: Engine(GLFWwindow* win){

    context = new Context();

    context->window = win;
    context->entityManager = &entities;
    createSystems();
};


void Engine::createSystems(){

    context->input_system =make_shared<InputControlSystem>(context);

    systems.add<RenderSystem>(context);
    systems.add<PhysicalSystem>(context);

    context->render_system = systems.system<RenderSystem>();
    context->physical_system = systems.system<PhysicalSystem>();

    systems.configure();
}

void  Engine::update(ex::TimeDelta dt) {

    systems.update_all(dt);

    //        do{

    //        } // Check if the ESC key was pressed or the window was closed
    //        while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
    //               glfwWindowShouldClose(window) == 0 );

    //      systems.update<DebugSystem>(dt);
    //      systems.update<MovementSystem>(dt);
    //      systems.update<CollisionSystem>(dt);
}





Engine::~Engine(){

    delete context;
};
