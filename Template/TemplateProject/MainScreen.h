#pragma once

#include <Sakura/include/IScreen.h>
#include <Sakura/include/Window.h>
#include <Sakura/include/SpriteBatch.h>
#include <Sakura/include/SpriteFont.h>
#include <Sakura/include/DebugRenderer.h>
#include <Sakura/include/GLSLProgram.h>
#include <Sakura/include/Camera2D.h>
#include <Sakura/include/ResourceManager.h>

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
	bool debug_game_events = false;

	float m_deltaTime = 1.0f;

	Sakura::Window* m_window = nullptr;

	glm::vec2 m_previousMouseLocation;
};