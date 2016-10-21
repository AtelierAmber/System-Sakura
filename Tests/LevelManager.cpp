#include "LevelManager.h"

LevelManager::LevelManager()
{
}


LevelManager::~LevelManager()
{
}

void LevelManager::init(Sakura::SpriteBatch* spriteBatch, Sakura::DebugRenderer* debugRenderer, Sakura::ResourceManager* resourceManager){
	m_spriteBatch = spriteBatch;
	m_debugRenderer = debugRenderer;
	m_resourceManager = resourceManager;
}

void LevelManager::updateLevel(){

}

void LevelManager::drawCurrentBackLayer(){
	m_spriteBatch->draw(glm::vec4(0.0f, 0.0f, m_levelList[m_currentLevel].LevelSize()), m_levelList[m_currentLevel].BackgroundLayerUV(), m_levelList[m_currentLevel].LevelTexture().id, 0.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
}

void LevelManager::drawCurrentOverLayer(){
	m_spriteBatch->draw(glm::vec4(0.0f, 0.0f, m_levelList[m_currentLevel].LevelSize()), m_levelList[m_currentLevel].OverlayLayerUV(), m_levelList[m_currentLevel].LevelTexture().id, 1.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
}

void LevelManager::drawDebug(){
	for (auto& re : m_levelList[m_currentLevel].BoundRects()){
		m_debugRenderer->drawBox(glm::vec4(re.x1, re.y2, re.width, re.height), Sakura::ColorRGBA8(255, 255, 255, 255), 0.0f);
	}
}

unsigned int LevelManager::createNewLevel(const std::vector<glm::vec4>& textureUVS, 
										  const std::vector<glm::vec4>& textureLocations,
										  const std::vector<glm::vec4>& textureOverlayUVS, 
										  const std::vector<glm::vec4>& textureOverlayLocations){
	return (int)m_levelList.size();
}

unsigned int LevelManager::addLevel(const char* filename, glm::vec2 levelSize, TexParam texParams, std::vector<glm::vec4> levelCollisionRects){
	Level newLevel;
	newLevel.init(filename, levelSize, m_resourceManager, texParams);
	newLevel.initAllBoundRects(levelCollisionRects);
	newLevel.SetIndex(m_levelList.size());
	m_levelList.push_back(newLevel);
	return m_levelList.size() - 1;
}
