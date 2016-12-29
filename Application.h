#pragma once

#include "Common.h"

struct GLFWwindow;
class Camera;
class Engine;


class Application {
public:

    Application();
    ~Application();

    void createWorld();
    void initControls();

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

private:

    unique_ptr<Engine>engine;
    GLFWwindow* window;
};



