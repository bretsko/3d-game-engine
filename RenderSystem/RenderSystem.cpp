#include "RenderSystem.h"
#include  "GeometryUtils/Quaternion_utils.h"

//#include "tiny_obj_loader.h"   // EITHER
#include "AssimpLoader.h"     // OR

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Scene/Object.h"
#include "RenderSystem/Shader.h"
#include "Texture/Texture.h"


RenderSystem::RenderSystem(Context *ctx):context(ctx){
    // depthInit();
    normalInit();
}


void RenderSystem::normalInit(){
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //TODO: put into Shader contructor
    auto shader = Shader("shaders/indexed.vert", "shaders/indexed.frag" );
    shaderProgram = shader.program;
    // shaderProgram = loadShaders( "shaders/indexed.vert", "shaders/indexed.frag" );

    //   lightShaderProgram = LoadShaders( "shaders/materials.vert", "shaders/materials.frag" );

    //TODO: put into Shader class
    auto lightShader = Shader( "shaders/light_casters.vert", "shaders/light_casters.frag");
    lightShaderProgram = lightShader.program;
    // lightShaderProgram = loadShaders( "shaders/light_casters.vert", "shaders/light_casters.frag");
    glUseProgram(lightShaderProgram);
    glUniform1i(glGetUniformLocation(lightShaderProgram, "material.diffuse"),  0);
    glUniform1i(glGetUniformLocation(lightShaderProgram, "material.specular"), 1);


    // Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);

    // shaderProgram = LoadShaders( "shaders/normal.vert", "shaders/normal.frag" );

    //   shaderProgram = LoadShaders( "shaders/StandardShading.vert", "shaders/StandardShading.frag" );

    glUseProgram(shaderProgram);
    TextureID  = glGetUniformLocation(shaderProgram, "myTextureSampler");
    LightID = glGetUniformLocation(shaderProgram, "LightPosition_worldspace");

    MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    ViewMatrixID = glGetUniformLocation(shaderProgram, "V");
    ModelMatrixID = glGetUniformLocation(shaderProgram, "M");

    //current_scene = context->current_scene;

    glGenTextures(1, &texture);
}

void RenderSystem::normalLoop(){

    renderScene(context->current_scene.get());

}



void RenderSystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {

    glfwPollEvents();

    lastFrameTime = glfwGetTime();
    static double currentTime = glfwGetTime();
    deltaTime = (float)(currentTime - lastFrameTime);
    lastFrameTime = currentTime;

    // Clear the colorbuffer
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // normalLoop_old();
    normalLoop();
    // depthLoop();
    // stupidLoop();

    glfwSwapBuffers(context->window);

    glBindVertexArray(0);
}



//TODO: recursive subdivide objects into meshes

void RenderSystem::renderScene (const Scene *sc){

    auto camera = context->input_system->camera;

    assert(sc != NULL);

    if(sc->objects.size() > 0){
        //        static bool once;

        //        if(!once){
        //            once =true;
        //            bufferVertices(context->current_scene->objects[0]);
        //        }

        //        bindMVP();
        //        draw_vertices();

        static bool first_done;
        int count = 0;

        auto num_objects = sc->objects.size();

        glUseProgram(shaderProgram);
        TextureID  = glGetUniformLocation(shaderProgram, "myTextureSampler");
        LightID = glGetUniformLocation(shaderProgram, "LightPosition_worldspace");

        MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        ViewMatrixID = glGetUniformLocation(shaderProgram, "V");
        ModelMatrixID = glGetUniformLocation(shaderProgram, "M");

        glBindVertexArray(VAO); //objects

        for (auto const& x : sc->objects)
        {
            shared_ptr<Object> obj = x.second;

            //  cout << x.first  // string (key)
            //  << ':'
            //  << x.second // shared_ptr<Object>
            //  << endl ;


            //obj->updatePhysics(); //physics, MVP

            //obj->translate(0.1,0.1,0.1,deltaTime);
            //obj->transformQuaternion(deltaTime);
            // obj->transformEuler(deltaTime);
            //3.14159f/2.0f

            //  obj->rotateEuler(90,0,0,deltaTime);
            // obj->rotateQuaternion(90,0,0,deltaTime);

            // obj->scale(1.01,1.01,1.01,deltaTime);

            if(obj->physBody)
                obj->updatePhysics();

            //picking_bullet();  //TODO: not working

            obj->computeTransform();

            updateMVP(obj.get());  //camera update

            renderObject(obj.get());

        }

        //        for (auto const& x : sc->objects)
        //        {
        //            shared_ptr<Object> obj = x.second;
        //            renderObject(obj.get());
        //        }



        glBindVertexArray(0); //objects


        //-------------------------LAMP------------------------


        glUseProgram(lightShaderProgram);

        MatrixID = glGetUniformLocation(lightShaderProgram, "projection"); //MVP
        ViewMatrixID = glGetUniformLocation(lightShaderProgram, "view"); //V
        ModelMatrixID = glGetUniformLocation(lightShaderProgram, "model"); //M

        mat4 projectionMatrix = context->input_system->projectionMatrix;
        mat4 viewMatrix = context->input_system->viewMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, glm::value_ptr(viewMatrix));


        //TODO: put into separate function, or class
        //-------------------------SPOTLIGHT


        GLint lightPosLoc             = glGetUniformLocation(lightShaderProgram, "light.position");
        GLint lightSpotdirLoc         = glGetUniformLocation(lightShaderProgram, "light.direction");
        GLint lightSpotCutOffLoc      = glGetUniformLocation(lightShaderProgram, "light.cutOff");
        GLint lightSpotOuterCutOffLoc = glGetUniformLocation(lightShaderProgram, "light.outerCutOff");
        GLint viewPosLoc              = glGetUniformLocation(lightShaderProgram, "viewPos");

        glUniform3f(lightPosLoc,             camera->position.x, camera->position.y, camera->position.z);
        glUniform3f(lightSpotdirLoc,         camera->front.x, camera->front.y, camera->front.z);
        glUniform1f(lightSpotCutOffLoc,      glm::cos(glm::radians(12.5f)));
        glUniform1f(lightSpotOuterCutOffLoc, glm::cos(glm::radians(17.5f)));
        glUniform3f(viewPosLoc,              camera->position.x, camera->position.y, camera->position.z);

        // Set lights properties
        glUniform3f(glGetUniformLocation(lightShaderProgram, "light.ambient"),   1.1f, 1.1f, 1.1f);
        // We set the diffuse intensity a bit higher; note that the right lighting conditions differ with each lighting method and environment.
        // Each environment and lighting type requires some tweaking of these variables to get the best out of your environment.
        glUniform3f(glGetUniformLocation(lightShaderProgram, "light.diffuse"),   1.8f, 1.8f, 1.8f);
        glUniform3f(glGetUniformLocation(lightShaderProgram, "light.specular"),  2.0f, 2.0f, 2.0f);
        glUniform1f(glGetUniformLocation(lightShaderProgram, "light.constant"),  2.0f);
        glUniform1f(glGetUniformLocation(lightShaderProgram, "light.linear"),    0.09);
        glUniform1f(glGetUniformLocation(lightShaderProgram, "light.quadratic"), 0.032);//0.032
        // Set material properties
        glUniform1f(glGetUniformLocation(lightShaderProgram, "material.shininess"), 32.0f);



        //TODO: put into separate function, or class

        //-------------------------MATERIALS

        // Set lights properties
        //        vec3 lightColor;
        //        lightColor.x = sin(glfwGetTime() * 2.0f);
        //        lightColor.y = sin(glfwGetTime() * 0.7f);
        //        lightColor.z = sin(glfwGetTime() * 1.3f);
        //        vec3 diffuseColor = lightColor * vec3(1.5f); // Decrease the influence
        //        vec3 ambientColor = diffuseColor * vec3(0.2f); // Low influence
        //        glUniform3f(glGetUniformLocation(lightShaderProgram, "light.ambient"),  ambientColor.x, ambientColor.y, ambientColor.z);
        //        glUniform3f(glGetUniformLocation(lightShaderProgram, "light.diffuse"),  diffuseColor.x, diffuseColor.y, diffuseColor.z);
        //        glUniform3f(glGetUniformLocation(lightShaderProgram, "light.specular"), 1.0f, 1.0f, 1.0f);
        //        // Set material properties
        //        glUniform3f(glGetUniformLocation(lightShaderProgram, "material.ambient"),   1.0f, 0.5f, 0.31f);
        //        glUniform3f(glGetUniformLocation(lightShaderProgram, "material.diffuse"),   1.0f, 0.5f, 0.31f);
        //        glUniform3f(glGetUniformLocation(lightShaderProgram, "material.specular"),  0.5f, 0.5f, 0.5f); // Specular doesn't have full effect on this object's material
        //        glUniform1f(glGetUniformLocation(lightShaderProgram, "material.shininess"), 32.0f);


        for (auto const& x : sc->objects)
        {
            shared_ptr<Object> obj = x.second;

            //            if(obj->texture!=nullptr){
            //                auto tex = obj->texture.get();

            //                assert(tex!=nullptr);

            //                glActiveTexture(GL_TEXTURE0);

            //                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            //                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            //                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            //                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //                glBindTexture(GL_TEXTURE_2D, texture);
            //                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->width, tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->image);
            //                glBindTexture(GL_TEXTURE_2D, texture);

            //               // glUniform1i(TextureID, 0);
            //                // Bind our texture in Texture Unit 0
            //                //  glActiveTexture(GL_TEXTURE0);
            //                //  glBindTexture(GL_TEXTURE_2D, Texture);
            //                //  // Set our "myTextureSampler" sampler to user Texture Unit 0
            //                //  glUniform1i(TextureID, 0);
            //            }


            for( auto mesh : *obj->meshes){
                mesh.vao->bind();

                mat4 model= obj->transform;

                model = glm::translate(model, vec3(0.2,0.2,0.0));

                // model = glm::scale(model, vec3(1.1f));

                glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, glm::value_ptr(model));


                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, mesh.vao->position);
                glVertexAttribPointer(
                            0,                  // attribute
                            3,                  // size
                            GL_FLOAT,           // type
                            GL_FALSE,           // normalized?
                            0,                  // stride
                            (void*)0            // array buffer offset
                            );


                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, mesh.vao->normal);
                glVertexAttribPointer(
                            2,                                // attribute
                            3,                                // size
                            GL_FLOAT,                         // type
                            GL_FALSE,                         // normalized?
                            0,                                // stride
                            (void*)0                          // array buffer offset
                            );


                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao->index);

                glDrawElements(
                            GL_TRIANGLES,      // mode
                            mesh.numFaces*3,    // count
                            GL_UNSIGNED_INT,   // type
                            (void*)0           // element array buffer offset
                            );

                glDisableVertexAttribArray(0);

                glDisableVertexAttribArray(1);



                //----------------------SIMPLE DRAW---------------------------


                //                glEnableVertexAttribArray(0);
                //                glBindBuffer(GL_ARRAY_BUFFER, mesh.vao->position);

                //                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
                //                // Note that we skip over the normal vectors

                //                glDrawArrays(GL_TRIANGLES, 0, mesh.numFaces );

                //                glDisableVertexAttribArray(0);


                mesh.vao->unbind();
            }

        }

        glBindVertexArray(0);  //light

    }
}


//TODO: not working
void RenderSystem::picking_bullet(){

    // (Instead of picking each frame if the mouse button is down,
    // you should probably only check if the mouse button was just released)
    if (glfwGetMouseButton(context->window, GLFW_MOUSE_BUTTON_LEFT)){

        vec3 out_origin;
        vec3 out_direction;

        screenPosToWorldRay(
                    WINDOW_WIDTH/2, WINDOW_HEIGHT/2,
                    WINDOW_WIDTH, WINDOW_HEIGHT,
                    context->input_system->viewMatrix,
                    context->input_system->projectionMatrix,
                    out_origin,
                    out_direction
                    );

        vec3 out_end = out_origin + out_direction*1000.0f;

        //        btVector3 Start(-1, -1,-1);
        //        btVector3 End(1,1,1);
        //        rayTest(Start, End, RayCallback);

        //        if(RayCallback.hasHit()) {
        //            btVector3 End = RayCallback.m_hitPointWorld;
        //            btVector3 Normal = RayCallback.m_hitNormalWorld;

        //            // Do some clever stuff here
        //        }

        btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(out_origin.x, out_origin.y, out_origin.z), btVector3(out_end.x, out_end.y, out_end.z));

        context->physical_system->dynamicsWorld->rayTest(btVector3(out_origin.x, out_origin.y, out_origin.z), btVector3(out_end.x, out_end.y, out_end.z), RayCallback);

        if(RayCallback.hasHit()) {

            void * ptr = RayCallback.m_collisionObject->getUserPointer();

            Object* gameObj = static_cast<Object*>(ptr);

            if(gameObj){

                //                btVector3 relativeForce = btVector3(100,100,100);
                //                btmat3x3 boxRot =  gameObj->physBody->rigidBody->getWorldTransform().getBasis();
                //                btVector3 correctedForce = boxRot * relativeForce;
                //               gameObj->physBody->rigidBody->applyCentralForce(correctedForce);

                gameObj->physBody->rigidBody->applyImpulse(btVector3(0.01,0.01,0.01),btVector3(context->input_system->camera->position.x,context->input_system->camera->position.y,context->input_system->camera->position.z));

                //  gameObj->physBody->rigidBody->applyForce(

                //                        btQuaternion worldQuat(body_->getOrientation());
                //                        fvec = worldQuat * fvec;

                cerr  << "Hit: " << gameObj->name << endl;
            }
            // cerr << "Hit: " <<  RayCallback.m_hitPointWorld.x()  << ", "  <<  RayCallback.m_hitPointWorld.y()  << ", " <<  RayCallback.m_hitPointWorld.z()  << ", " <<  RayCallback.m_hitNormalWorld.x() << endl;

        }
    }
}

void RenderSystem::screenPosToWorldRay(
        int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
        int screenWidth, int screenHeight,  // Window size, in pixels
        mat4 ViewMatrix,               // Camera position and orientation
        mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
        vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
        vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
        ){

    // The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
    glm::vec4 lRayStart_NDC(
                ((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
                ((float)mouseY/(float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
                -1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
                1.0f
                );
    glm::vec4 lRayEnd_NDC(
                ((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f,
                ((float)mouseY/(float)screenHeight - 0.5f) * 2.0f,
                0.0,
                1.0f
                );


    // The Projection matrix goes from Camera Space to NDC.
    // So inverse(ProjectionMatrix) goes from NDC to Camera Space.
    mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);

    // The View Matrix goes from World Space to Camera Space.
    // So inverse(ViewMatrix) goes from Camera Space to World Space.
    mat4 InverseViewMatrix = glm::inverse(ViewMatrix);

    glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera/=lRayStart_camera.w;
    glm::vec4 lRayStart_world  = InverseViewMatrix       * lRayStart_camera; lRayStart_world /=lRayStart_world .w;
    glm::vec4 lRayEnd_camera   = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera  /=lRayEnd_camera  .w;
    glm::vec4 lRayEnd_world    = InverseViewMatrix       * lRayEnd_camera;   lRayEnd_world   /=lRayEnd_world   .w;


    // Faster way (just one inverse)
    //mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
    //glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
    //glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


    vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
    lRayDir_world = glm::normalize(lRayDir_world);


    out_origin = vec3(lRayStart_world);
    out_direction = glm::normalize(lRayDir_world);
}



void RenderSystem::renderObject (const Object *obj){
    assert(obj != NULL);
    assert(obj->meshes->size() != 0);

    static bool first_done;
    int count = 0;

    bool render_texture;
    Texture* tex ;

    if(obj->texture!=nullptr){
        tex = obj->texture.get();
        render_texture = true;
    }

    //auto num_meshes = obj->mesh->model->meshes.size();
    // cerr  <<  "\n\n Number of meshes: " << num_meshes << endl;


    // TODO: colours and light position, colour

    //GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    //GLint lightColorLoc  = glGetUniformLocation(shaderProgram, "lightColor");
    //GLint lightPosLoc    = glGetUniformLocation(shaderProgram, "lightPos");
    //GLint viewPosLoc     = glGetUniformLocation(shaderProgram, "viewPos");
    //glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
    //glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);
    //glUniform3f(lightPosLoc,    lightPos.x, lightPos.y, lightPos.z);
    //glUniform3f(viewPosLoc,     camera->Position.x, camera->Position.y, camera->Position.z);


    for( auto mesh : *obj->meshes){
        // auto mesh = obj->mesh;
        count++;

        // renderObject(*obj);

        // bufferVertices(context->current_scene->objects[0]);
        //  auto vertices = mesh->vertices;
        //   num_vertices = vertices.size();
        //glBindBuffer(GL_ARRAY_BUFFER, mesh->vao->position);

        //            glGenBuffers(1, &vertexbuffer);
        //            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        //            glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        //for each model different gluniform Model
        // bindMVP();

        if(!context->current_scene->pointLights.empty()){
            auto light = context->current_scene->pointLights.at(0);

            if (light)
                renderLight(*light);

        }
        // transformBullet(obj);
        //transformEuler(obj);
        //transformQuaternion(obj);

        // draw_vertices();

        renderMesh(mesh);

    }

    //cerr  <<  "\n\nCounter - " << count << endl;

    if(render_texture){
        renderTexture(tex);
    }

}



void RenderSystem::renderMesh( const Mesh &mesh){

    assert(mesh.vao!=nullptr);

    mesh.vao->bind();

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vao->position);
    glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );


    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vao->normal);
    glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
                );


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao->index);

    //cerr << "crash!!!!" << endl;

    glDrawElements(
                GL_TRIANGLES,      // mode
                mesh.numFaces*3,    // count
                GL_UNSIGNED_INT,   // type
                (void*)0           // element array buffer offset
                );

    // glDrawArrays(GL_TRIANGLES, 0, mesh->numFaces*3 );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    //  glDisableVertexAttribArray(2);
    mesh.vao->unbind();
}


//TODO: maybe Mesh??
void RenderSystem::updateMVP(const Object *object){

    glUseProgram(shaderProgram);

    context->input_system->calculateMVP();

    mat4 modelMatrix = object->transform;

    mat4 projectionMatrix = context->input_system->projectionMatrix;

    mat4 viewMatrix = context->input_system->viewMatrix;

    mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(MVP));
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}



void RenderSystem::bindMVP(){

    mat4 MVP = context->input_system->calculateMVP();

    mat4 modelMatrix = context->input_system->modelMatrix;

    mat4 projectionMatrix = context->input_system->projectionMatrix;

    mat4 viewMatrix = context->input_system->viewMatrix;

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
}






// Optionally render the shadowmap (for debug only)
void RenderSystem::renderShadowMap(){
    // Render only on a corner of the window (or we won't see the real rendering...)
    glViewport(0,0,512,512);

    // Use our shader
    glUseProgram(quad_programID);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    // Set our "renderedTexture" sampler to user Texture Unit 0
    glUniform1i(texID, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // Draw the triangle !
    // You have to disable GL_COMPARE_R_TO_TEXTURE above in order to see anything !
    //glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
    glDisableVertexAttribArray(0);
}




void RenderSystem::transformQuaternion(const Object *obj){

    mat4 projectionMatrix = context->input_system->projectionMatrix;
    mat4 viewMatrix = context->input_system->viewMatrix;

    vec3 gPosition1(-1.5f, 0.0f, 0.0f);
    vec3 gOrientation1;

    vec3 gPosition2( 1.5f, 0.0f, 0.0f);
    quat gOrientation2;

    bool gLookAtOther = true;

    // As an example, rotate arount the vertical axis at 180°/sec
    gOrientation1.y += 3.14159f/2.0f * deltaTime;


    if (gLookAtOther){
        vec3 desiredDir = gPosition1-gPosition2;
        vec3 desiredUp = vec3(0.0f, 1.0f, 0.0f); // +Y

        // Compute the desired orientation
        quat targetOrientation = normalize(mylookAt(desiredDir, desiredUp));

        // And interpolate
        gOrientation2 = rotateTowards(gOrientation2, targetOrientation, 1.0f*deltaTime);
    }

    mat4 RotationMatrix = mat4_cast(gOrientation2);
    mat4 TranslationMatrix = translate(mat4(), gPosition2); // A bit to the right
    mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
    mat4 modelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;

    mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

}



void RenderSystem::transformEuler(const Object *obj){

    // mat4 MVP = context->input_system->calculateMVP();

    mat4 projectionMatrix = context->input_system->projectionMatrix;
    mat4 viewMatrix = context->input_system->viewMatrix;
    // mat4 modelMatrix = context->input_system->modelMatrix;

    vec3 gPosition1(-1.5f, 0.0f, 0.0f);
    vec3 gOrientation1;

    vec3 gPosition2( 1.5f, 0.0f, 0.0f);
    quat gOrientation2;

    bool gLookAtOther = true;

    // As an example, rotate arount the vertical axis at 180°/sec
    gOrientation1.y += 3.14159f/2.0f * deltaTime;

    // Build the model matrix
    mat4 RotationMatrix = glm::eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
    mat4 TranslationMatrix = glm::translate(mat4(), gPosition1); // A bit to the left
    mat4 ScalingMatrix = glm::scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
    mat4 modelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;

    mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
}



void RenderSystem::draw(){

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );


    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
                1,                                // attribute
                2,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
                );



    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
                );


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    glDrawElements(
                GL_TRIANGLES,      // mode
                indices.size(),    // count
                GL_UNSIGNED_INT,   // type
                (void*)0           // element array buffer offset
                );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}


void RenderSystem::renderTexture(const Texture *tex){
    assert(tex!=nullptr);

    glActiveTexture(GL_TEXTURE0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->width, tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->image);
    glBindTexture(GL_TEXTURE_2D, texture);

    glUniform1i(TextureID, 0);
}


void RenderSystem::renderLight(const PointLight & light){
    vec3 lightPos = light.pos;
    glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
}


RenderSystem::~RenderSystem()
{
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &elementbuffer);

    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &TextureID);
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
}
