#pragma once
#include <Sakura/include/IScreen.h>
#include <Sakura/include/GUIInterface.h>
#include <Sakura/include/Window.h>
#include <Sakura/include/SpriteBatch.h>
#include <Sakura/include/SpriteFont.h>
#include <Sakura/include/DebugRenderer.h>
#include <Sakura/include/GLSLProgram.h>
#include <Sakura/include/Camera2D.h>
#include <Sakura/include/ResourceManager.h>
#include "LevelManager.h"

enum class GUIState{
	DEFAULT,
	CLEAR_LEVEL,
	SAVE_LEVEL,

};

class gui : 
	public Sakura::GUIInterface{

public:
	virtual void destroyComponents() override;

	virtual void update(Sakura::InputManager& inputManager) override;

	virtual void IDraw(float fps) override;

	void createNewLevel();

	virtual void initComponents() override;
	void initSpecificComponents(Sakura::DebugRenderer* debugRenderer);
	void initButtons();

	void getState();
	void setState(GUIState state);
	
private:
	GUIState m_guiState;

	std::vector<Sakura::GUIButton> m_buttons;
};

class _DebugLevelCreator : 
	public Sakura::IScreen
{
public:
	_DebugLevelCreator(Sakura::Window* window);
	~_DebugLevelCreator();

	virtual int getNextScreenIndex() const override;
	virtual int getPreviousScreenIndex() const override;

	virtual void build() override;
	virtual void destroy() override;

	virtual void onEntry() override;
	virtual void onExit() override;

	virtual void update(float deltaTime) override;
	void checkInput();

	virtual void draw() override;
	virtual void specificDraw() override;

	void resetLevel();

private:
	Sakura::Window* m_window = nullptr;

	std::vector<glm::vec4> m_currentTextureUVS;
	std::vector<glm::vec4> m_currentTextureLocations;
	std::vector<glm::vec4> m_currentTextureOverlayUVS;
	std::vector<glm::vec4> m_currentTextureOverlayLocations;
	Sakura::TileSheet m_textureSheet;

	LevelManager m_levelManager;

	gui m_GUI;
};

