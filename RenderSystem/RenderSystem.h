#pragma once

#include "Common.h"
#include <entityx/entityx.h>

#include "Scene/Object.h"
#include "Scene/Scene.h"
#include "Context.h"
#include "Lights/PointLight.h"


class Object;
class Scene;
namespace ex = entityx;

struct RenderSystem : public ex::System <RenderSystem> {

    RenderSystem(Context *ctx);
    ~RenderSystem();

    void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override ;

    //-----------------RENDERING-----------------------------

    void renderTexture(const Texture *tex);
    void renderObject(const Object & obj);
    void renderObjectWithShadows(const Object & obj);
    void renderLight(const PointLight & light);


    void renderScene (const Scene *sc);
    void renderObject (const Object *obj);

    void renderMesh(  const Mesh &mesh);

    void renderObjectRecursive (const Object *obj);

    void screenPosToWorldRay(
            int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
            int screenWidth, int screenHeight,  // Window size, in pixels
            mat4 ViewMatrix,               // Camera position and orientation
            mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
            vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
            vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
            );

    void picking_bullet();


    void bindMVP();

    void normalInit();
    void normalLoop();
    void normalLoop_old();

    void depthInit();
    void depthLoop();

    void stupidInit();
    void stupidLoop();

    void draw();
    void draw_vertices();

    void renderShadowMap();


    //-----------------TRANSFORMS-----------------------------

    void updateMVP(const Object *object);

    void transformEuler(const Object *obj);
    void transformQuaternion(const Object *obj);

    //-----------------VARS-----------------------------

    size_t num_vertices;
    GLuint shaderProgram;
    GLuint lightShaderProgram;
    vector<unsigned int> indices;

    GLuint VAO;
    GLuint LightVAO;

    GLuint normalbuffer;
    GLuint elementbuffer;
    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint texture;

    GLuint TextureID;
    GLuint LightID;
    GLuint MatrixID;
    GLuint ViewMatrixID ;
    GLuint ModelMatrixID;

    GLuint depthProgramID;
    GLuint depthMatrixID;
    GLuint ShadowMapID;
    GLuint DepthBiasID;
    GLuint depthTexture;
    GLuint MyFramebuffer;
    GLuint lightInvDirID;

    GLuint quad_vertexbuffer;
    GLuint quad_programID;
    GLuint texID;
    Context *context;

    float deltaTime;
    double lastFrameTime;
};
