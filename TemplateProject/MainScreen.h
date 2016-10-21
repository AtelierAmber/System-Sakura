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

enum GUIState{
	GAMEPLAY = 0,
	MENU = 1,
	OPTIONSmain = 2,
	FLEET = 3
};

class MainGUI :
	public Sakura::GUIInterface
{
public:

	MainGUI();
	~MainGUI();

	void destroyComponents() override;

	void update(Sakura::InputManager& inputManager) override;

	void IDraw(float fps) override;

	void initComponents() override;

	void initButtons(Sakura::Window* window);

	void setState(GUIState newState){ state = newState; }
	GUIState getState(){ return state; }

private:
	/* Resume, Options, Quit */
	Sakura::GUIButton m_resumeButton;
	Sakura::GUIButton m_optionsButton;
	Sakura::GUIButton m_quitButton;

	/* Fleet Managment */
	Sakura::GUIButton m_warshipsButton;

	Sakura::Window* m_parentWindow;
	GUIState state = GAMEPLAY;
};

class MainScreen :
	public Sakura::IScreen
{
public:
	MainScreen(Sakura::Window* window);
	~MainScreen();

	//Returns the index of the next or previous screen when changing screens
	virtual int getNextScreenIndex() const override;
	virtual int getPreviousScreenIndex() const override;

	//Called at beginning and end of application
	virtual void build() override;
	virtual void destroy() override;

	//Called when a screen enters and exits focus
	virtual void onEntry() override;
	virtual void onExit() override;

	//Called in the main game loop
	virtual void update(float deltaTime) override;
	void checkInput();
	virtual void draw() override;
	virtual void specificDraw() override;

private:
	bool debug_game_events = true;

	float m_deltaTime = 1.0f;

	Sakura::Window* m_window = nullptr;

	Sakura::GLTexture m_background;

	MainGUI m_interface;

	Sakura::Music m_mainMusic;
	Sakura::Music m_bossMusic;
	glm::vec2 m_previousMouseLocation;
	Sakura::SpriteFont m_userFont;
	Sakura::SpriteFont m_enemyFont;
};