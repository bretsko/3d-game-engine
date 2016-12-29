
#pragma once

#include "Common.h"

struct Shader
{

    //public:
    Shader(string vertLocation,string fragLocation);
    Shader(const GLuint p) : program(p), uniforms_lighting(true) {}


    //------------------ Functions ------------------------
    GLuint uniform(const char* name);

    //Enables or disables the flag for if lighting uniforms should be set
    void setUniformsLighting(bool flag)
    {
        uniforms_lighting = flag;
    }

    // Get the flag for if lighting uniforms should be set
    bool getUniformsLighting()
    {
        return  uniforms_lighting;
    }


    //------------------ Properties ------------------------

    // private:

    GLuint program;

    // Cached list of uniform locations
    map<const char*, GLuint> uniforms;

    // Should the various lighting uniforms be set?
    bool uniforms_lighting;
};

