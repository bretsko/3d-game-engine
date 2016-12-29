
#include "TextureFactory.h"

shared_ptr<Texture> TextureFactory::createTexture(TextureType type) {
    Texture * texture;
    switch (type) {
    case TextureCube:{
        return make_shared<Texture> ("content/sample.png");
    }
    case TextureHuman:{
        //TODO:
    }
    }

};
