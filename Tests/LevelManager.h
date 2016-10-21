#pragma once
#include "Level.h"

#include <Sakura/include/DebugRenderer.h>
#include <Sakura/include/GLSLProgram.h>
#include <Sakura/include/SpriteBatch.h>
#include <Sakura/include/ResourceManager.h>

class LevelManager
{
public:
	LevelManager();
	~LevelManager();

	void init(Sakura::SpriteBatch* spriteBatch, Sakura::DebugRenderer* debugRenderer, Sakura::ResourceManager* resourceManager);

	void updateLevel();

	void drawCurrentBackLayer();
	void drawCurrentOverLayer();

	void drawDebug();

	/* Returns the index of the added level */
	unsigned int addLevel(const char* filename, glm::vec2 levelSize, TexParam texParams, std::vector<glm::vec4> levelCollisionRects);
	unsigned int createNewLevel(const std::vector<glm::vec4>& textureUVS, const std::vector<glm::vec4>& textureLocations,
								const std::vector<glm::vec4>& textureOverlayUVS, const std::vector<glm::vec4>& textureOverlayLocations);
	Level getLevelData() const { return m_levelList[m_currentLevel]; }

	void proceedToNextLevel(){ m_currentLevel++; }
	void proceedToPreviousLevel(){ m_currentLevel--; }
	void proceedToLevel(unsigned int level){ m_currentLevel = level; }

private:
	int m_currentLevel = 0;
	std::vector<Level> m_levelList;

	Sakura::SpriteBatch* m_spriteBatch;
	Sakura::DebugRenderer* m_debugRenderer;
	Sakura::ResourceManager* m_resourceManager;
};

