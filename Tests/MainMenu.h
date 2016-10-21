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

enum class MenuState{
	MAIN,
	OPTIONS,
	ACHIEVMENTS
};

class MainMenuGUI :
	public Sakura::GUIInterface
{
public:

	void update(Sakura::InputManager& inputManager) override;

	void IDraw(float fps) override;

	void initComponents() override;
	void initButtons(Sakura::Window* window);

	glm::vec2 GUIDimensions(){

	}

	void destroyComponents() override;

private:
	MenuState m_menuState = MenuState::MAIN;

	std::vector<Sakura::GUIButton> m_mainMenuButtons;
	std::vector<Sakura::GUIButton> m_optionsMenuButtons;
	std::vector<Sakura::GUIButton> m_achievementsMenuButtons;
};

class MainMenu :
	public Sakura::IScreen
{
public:
	MainMenu(Sakura::Window* window);
	~MainMenu();

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

private:
	float scrollSpeed = 1.0f;

	MainMenuGUI m_interface;

	Sakura::Window* m_window = nullptr;

	Sakura::GLTexture m_background;
	Sakura::Music m_music;
};