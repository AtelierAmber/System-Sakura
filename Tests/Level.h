#pragma once
#include <Sakura/include/ResourceManager.h>
#include <Sakura/include/Bounds.h>
#include <Sakura/include/SpriteBatch.h>
#include "Enemy.h"

#include <vector>

// TODO !!!!! TRANSFORM INTO INTERFACE : SEPERATE CLASSES PER LEVEL

struct Level{
public:
	void init(const char* filename, glm::vec2 levelSize, Sakura::ResourceManager* resourceManager, TexParam texParams){
		m_levelTexture = resourceManager->getTileSheet(filename, glm::ivec2(2, 1), texParams);
		m_levelSize = levelSize;
		m_backgroundLayerUV = m_levelTexture.getUVs(0);
		m_overlayLayerUV = m_levelTexture.getUVs(1);
	}

	void init(const std::vector<glm::vec4>& textureUVS, const std::vector<glm::vec4>& textureLocations){
		m_textureUVS = textureUVS;
		m_textureLocations = textureLocations;
	}

	void initAllBoundRects(const std::vector<glm::vec4>& destRects){
		for (std::size_t i = 0; i < destRects.size(); ++i){
			m_levelCollisionRects.push_back(Sakura::Rect(destRects[i].x, destRects[i].y, destRects[i].z, destRects[i].w, true));
		}
	}

	void initObjects(){
		//TODO : INITIALIZING OBJECTS
	}

	void update(float deltaTime){
		for (auto& e : m_levelEnemies){
			//TODO : UPDATE ENEMIES
		}
		//TODO : UPDATE OBJECTS
	}

	void drawBackground(Sakura::SpriteBatch* spriteBatch){
		for (std::size_t i = 0; i < m_textureUVS.size(); ++i){
			spriteBatch->draw(m_textureLocations[i], m_textureUVS[i], m_textureSheet.texture.id, 1.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		}
	}

	void drawOverlay(Sakura::SpriteBatch* spriteBatch){
		for (std::size_t i = 0; i < m_textureUVS.size(); ++i){
			spriteBatch->draw(m_textureOverlayLocations[i], m_textureOverlayUVS[i], m_textureSheet.texture.id, 1.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
		}
	}

	unsigned int Index(){ return index; }
	void SetIndex(unsigned int IndexNum){ index = IndexNum; }
	glm::vec4 BackgroundLayerUV() const { return m_backgroundLayerUV; }
	glm::vec4 OverlayLayerUV() const { return m_overlayLayerUV; }
	glm::vec2 LevelSize() const { return m_levelSize; }
	Sakura::GLTexture LevelTexture() const { return m_levelTexture.texture; }
	std::vector<Sakura::Rect> BoundRects() { return m_levelCollisionRects; }

private:
	Sakura::TileSheet m_levelTexture;
	glm::vec2 m_levelSize;
	glm::vec4 m_backgroundLayerUV;
	glm::vec4 m_overlayLayerUV;

	std::vector<glm::vec4> m_textureUVS;
	std::vector<glm::vec4> m_textureLocations;
	std::vector<glm::vec4> m_textureOverlayUVS;
	std::vector<glm::vec4> m_textureOverlayLocations;
	Sakura::TileSheet m_textureSheet;

	glm::vec2 m_playerSpawn;
	std::vector<Enemy> m_levelEnemies;
	//std::vector<Object> m_levelObjects;

	std::vector<Sakura::Rect> m_levelCollisionRects;

	unsigned int index;
};