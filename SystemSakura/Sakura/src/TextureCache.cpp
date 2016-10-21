#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace Sakura {

    TextureCache::TextureCache()
    {
    }


    TextureCache::~TextureCache(){ /* Empty */ }

    GLTexture TextureCache::getTexture(const char* texturePath, TexParam textureFlags) {

        //lookup the texture and see if its in the map
        auto mit = m_textureMap.find(std::string(texturePath));

        //check if its not in the map
        if (mit == m_textureMap.end()) {
            //Load the texture
            GLTexture newTexture = ImageLoader::loadPNG(texturePath, textureFlags);

            //Insert it into the map
            m_textureMap.insert(make_pair(std::string(texturePath), newTexture));

			//! Loaded New Texture!
            return newTexture;
        }
		//! Used Cached Texture!
        return mit->second;
    }

}