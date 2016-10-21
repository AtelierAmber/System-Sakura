#include "ResourceManager.h"

namespace Sakura {

	void ResourceManager::init(){
		m_audioEngine.init();
	}

	Sakura::TileSheet ResourceManager::getTileSheet(const char* tileSheetPath, const glm::ivec2& tileDims, TexParam textureFlags /*= DEFAULT */){
		TileSheet tileSheet;
		if (tileSheetPath != "NULL"){
			tileSheet.init(getTexture(tileSheetPath, textureFlags), tileDims);
		}
		return tileSheet;
	}

	GLTexture ResourceManager::getTexture(const char* texturePath, TexParam textureFlags /*= DEFAULT */) {
		GLTexture tempTexture;
		if (texturePath != "NULL"){
			tempTexture = m_textureCache.getTexture(texturePath, textureFlags);
		}
		return tempTexture;
    }

	SoundEffect ResourceManager::loadSoundEffect(const char* effectPath){
		return m_audioEngine.loadSoundEffect(std::string(effectPath));
	}

	Music ResourceManager::loadMusic(const char* musicPath){
		return m_audioEngine.loadMusic(std::string(musicPath));
	}

	void ResourceManager::destroyResources(){
		m_textureCache.destroy();
		m_audioEngine.destroy();
	}

}