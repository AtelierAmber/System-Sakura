#include "GameGUI.h"

#include "Player.h"

GameGUI::GameGUI(){}

GameGUI::~GameGUI(){}

void GameGUI::initSpecialData(Player* player){
	m_player = player;
}

void GameGUI::update(Sakura::InputManager& inputManager){
	m_GUICamera.update();
	m_backButton.update(inputManager,m_GUICamera);
}

void GameGUI::IDraw(float fps){
	std::string format = "FPS : %.1f";
	char fpsBuffer[64];
	sprintf_s(fpsBuffer, format.c_str(), fps + 0.1f);
	m_GUISpritefont.draw(m_GUISpritebatch, fpsBuffer, glm::vec2(m_GUICameraBounds.x - 100.0f, m_GUICameraBounds.y - m_GUISpritefont.getFontHeight() / m_GUICamera.getScale()), glm::vec2(0.35f), 1.0f, Sakura::ColorRGBA8(255, 255, 255, 255));
	m_backButton.draw(m_GUISpritebatch, m_GUICamera);
}

void GameGUI::initComponents(){
	//m_GUIElements.push_back(m_resourceManager->getTexture("Assets/Textures/Hearts/heart_sharp_empty.png", PIXELATED | MIPMAP | EDGE_CLAMP));
	m_GUISpritefont.initTTF("Assets/Fonts/pixelmix/pixelmix_bold.ttf", 25, PIXELATED | MIPMAP | EDGE_CLAMP);
	m_backButton = createButton("Assets/Interface/Button.png", "Assets/Interface/Fonts/TEACP.ttf", 20, glm::vec2(1.0f, 1.0f), DEFAULT, "BACK", glm::vec2(0.0f, 0.0f), [this](){ m_parentScreen->setState(Sakura::ScreenState::CHANGE_PREVIOUS); }, DEFAULT);
}

void GameGUI::destroyComponents(){
	m_backButton.destroy();
}
