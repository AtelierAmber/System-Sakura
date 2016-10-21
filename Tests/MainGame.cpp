#include "MainGame.h"
#include <Sakura/include/ScreenList.h>

MainGame::MainGame(std::string title /*= "DEFAULT TITLE"*/, unsigned int screenWidth /*= 500*/, unsigned int screenHeight /*= 500*/, unsigned int windowFlags /*= 0*/, int winPositionX /*= -1*/, int winPositionY /*= -1*/) : IMainSystem(title.c_str(), screenWidth, screenHeight, windowFlags, winPositionX, winPositionY){

}

MainGame::~MainGame(){

}

void MainGame::onInit(){
	
}

void MainGame::addScreens(){
	m_screen = std::make_unique<MainScreen>(&m_window);
	m_mainMenu = std::make_unique<MainMenu>(&m_window);
	m_levelCreator = std::make_unique<_DebugLevelCreator>(&m_window);

	m_screenList->addScreen(m_mainMenu.get());
	m_screenList->addScreen(m_screen.get());
	m_screenList->addScreen(m_levelCreator.get());
	m_screenList->setScreen(m_mainMenu->getScreenIndex());
}

void MainGame::onExit(){

}
