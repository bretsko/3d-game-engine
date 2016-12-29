#pragma once

#include "Texture.h"

class TextureFactory
{
public:
    static shared_ptr<Texture> createTexture(TextureType type);

private:
    TextureFactory() = delete;
};


