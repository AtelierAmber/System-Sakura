#pragma once

#include <Sakura/include/IMainSystem.h>
#include <Sakura/include/IScreen.h>

#include "MainScreen.h"
#include "MainMenu.h"
#include "_DebugLevelCreator.h"

class MainGame :
	public Sakura::IMainSystem
{
public:
	MainGame(std::string title = "DEFAULT TITLE", unsigned int screenWidth = 500, unsigned int screenHeight = 500, unsigned int windowFlags = 0, int winPositionX = -1, int winPositionY = -1);
	~MainGame();

	// Called on initialization
	void onInit();
	// For adding all screens
	void addScreens();
	// Called when exiting
	void onExit();
private:
	std::unique_ptr<MainMenu> m_mainMenu = nullptr;
	std::unique_ptr<MainScreen> m_screen = nullptr;
	std::unique_ptr<_DebugLevelCreator> m_levelCreator = nullptr;
};

