
#include "Application.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Lights/PointLight.h"

class PointLight;

Camera * staticCamera = nullptr;

void Application::createWorld(){
    engine->context->current_scene = std::make_unique<Scene>();
    auto sc = engine->context->current_scene.get();

    shared_ptr<Object> floor = make_shared <Object> (engine->context , MeshCube, TextureCube, FLOOR);
    floor->addPhysicalBody(1);
    engine->context->physical_system->createGround();
    floor->translate(50,20,20);
    floor->scale(0.1,100,100);
    floor->computeTransform();

    shared_ptr<Object> wall = make_shared <Object> (*floor.get());
    wall->name = WALL;
    wall->addPhysicalBody(1);
    wall->scale(100,1, 0.01);
    wall->computeTransform();

    shared_ptr<Object> wall1 = make_shared <Object> (*wall.get());
    wall1->name = WALL1;
    wall1->addPhysicalBody(1);
    wall1->rotateEuler(90,0,0);
    wall1->computeTransform();

    shared_ptr<Object> face = make_shared <Object> (engine->context , MeshCube, TextureCube, FACE);
    face->addPhysicalBody(1);

    shared_ptr<Object> face1 = make_shared <Object> (engine->context ,MeshCube , TextureCube,FACE1);
    face1->addPhysicalBody(1);

    sc->objects.insert(make_pair(FLOOR,floor));
    sc->objects.insert(make_pair(FACE,face));
    sc->objects.insert(make_pair(FACE1,face1));
    sc->objects.insert(make_pair(WALL,wall));
    sc->objects.insert(make_pair(WALL1,wall1));

    //shared_ptr <PointLight> plight ( new PointLight(Color(), Color(), Color(), 0, vec3(4,4,4), vec3(4,4,4)));

   // sc->pointLights.push_back(plight);
}


Application::Application()
{
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        //return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        //return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        //return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Hide the mouse and enable unlimited movement
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);


    // Define the viewport dimensions
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    engine = make_unique<Engine>(window);

    createWorld();

    staticCamera = engine->context->input_system->camera.get();

    initControls(); //needs staticCamera

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {

        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last printf() was more than 1 sec ago
            // printf("%f ms/frame\n", 1000.0/double(nbFrames));
            nbFrames = 0; //reset timer
            lastTime += 1.0;
        }
        engine->update(1);

    }
}



void Application::initControls(){
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static double lastX, lastY, firstMouse;

    lastX = xpos;
    lastY = ypos;
    firstMouse = false;


    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    if (staticCamera)
        staticCamera->processMouseMovement(xoffset, yoffset);
}


void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

    if (staticCamera)
        staticCamera->processMouseScroll(yoffset);
}


Application::~Application()
{
    glfwTerminate();
}

