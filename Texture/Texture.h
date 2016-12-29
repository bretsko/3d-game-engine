#pragma once

#include "Common.h"

#include <assimp/texture.h>

enum TextureType{
    TextureCube, TextureHuman
};

class Texture{
public:

    Texture(const char * str);
    ~Texture();

    GLuint loadBMP_custom(const char * imagepath);

    // Load a .DDS file using GLFW's own loader
    GLuint loadDDS(const char * imagepath);

    //------------------ Properties ------------------------

    int width;
    int height;
    int comp;
    unsigned char* image;

    GLuint id;
    string type;
    aiString path;
};

