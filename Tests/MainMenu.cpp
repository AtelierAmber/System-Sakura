#include "MainMenu.h"
#include <Sakura/include/IMainSystem.h>

MainMenu::MainMenu(Sakura::Window* window) : m_window(window){ }
MainMenu::~MainMenu(){ }

#define screen_scale_level 2.0f

int MainMenu::getNextScreenIndex() const {
	return m_nextScreen;
}

int MainMenu::getPreviousScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

void MainMenu::build(){

}

void MainMenu::destroy(){
	
}

void MainMenu::onEntry(){
	m_GLSLSamplerName = "mySampler";
	m_GLSLPUniformName = "P";
	m_spriteBatch.init();
	m_debugRenderer.init();
	m_resourceManager.init();

	// Compile our static resources
	m_music = m_resourceManager.loadMusic("Assets/Audio/Soliloquy.mp3");

	// Compile the color shader
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(screen_scale_level);
	glm::vec2 cameraPositionOffset((m_window->getScreenWidth() / 2.0f) * (1.0f / screen_scale_level),
		(m_window->getScreenHeight() / 2.0f) * (1.0f / screen_scale_level));
	m_camera.setPosition(cameraPositionOffset);
	m_interface.init(this, &m_textureProgram, &m_resourceManager, glm::ivec2(m_window->getScreenWidth(), m_window->getScreenHeight()), cameraPositionOffset, screen_scale_level);
	m_interface.initButtons(m_window);
	m_background = m_resourceManager.getTexture("Assets/Textures/Levels/Main_Level.png", PIXELATED | EDGE_CLAMP | MIPMAP);
}

void MainMenu::onExit(){
	m_debugRenderer.dispose();
	m_spriteBatch.dispose();
 	m_resourceManager.destroyResources();
	m_textureProgram.dispose();
	m_interface.destroy();
}

void MainMenu::update(float deltaTime){
	m_camera.update();
	m_interface.update(m_game->inputManager);

	checkInput();
}

void MainMenu::draw(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	/* Scrolling background */
	static float scrollTimer = 0.0f;
#define scroll_time_seconds 10
#define max_scroll_time_frames (scroll_time_seconds * 60)
	scrollTimer += 1.0f * scrollSpeed;
	/* 60 frames per second is predicted - deltaTime to regulate the scroll time*/
	float backgroundFX = 0.0f;
	float backgroundSX = m_background.width / screen_scale_level;
	backgroundFX = -((scrollTimer / max_scroll_time_frames) * (m_background.width / screen_scale_level));
	backgroundSX = backgroundFX + m_background.width / screen_scale_level;

	if (scrollTimer > max_scroll_time_frames){ /* 60 * 4 = 240, 240 frames per scroll */
		scrollTimer = 0.0f;
		float tempVal = backgroundSX;
		backgroundSX = backgroundFX;
		backgroundFX = tempVal;
	}

	m_spriteBatch.draw(glm::vec4(backgroundFX, 0.0f, m_background.width / screen_scale_level, m_background.height / screen_scale_level), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_background.id, 0.0f,
		Sakura::ColorRGBA8(255, 255, 255, 255));
	m_spriteBatch.draw(glm::vec4(backgroundSX, 0.0f, m_background.width / screen_scale_level, m_background.height / screen_scale_level), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_background.id, 0.0f,
		Sakura::ColorRGBA8(255, 255, 255, 255));

}

void MainMenu::specificDraw(){
	m_interface.draw(m_GLSLPUniformName, m_game->getFps());
}

/////////////////////////////////////////////////////////////////////////////////////////

void MainMenuGUI::destroyComponents(){
	for (auto& b : m_mainMenuButtons){
		b.destroy();
	}
	for (auto& b : m_optionsMenuButtons){
		b.destroy();
	}
	for (auto& b : m_achievementsMenuButtons){
		b.destroy();
	}
	m_mainMenuButtons.clear();
	m_optionsMenuButtons.clear();
	m_achievementsMenuButtons.clear();
}

void MainMenuGUI::update(Sakura::InputManager& inputManager){
	m_GUICamera.update();
	for (auto& b : m_mainMenuButtons){
		b.update(inputManager, m_GUICamera);
	}
	for (auto& b : m_optionsMenuButtons){
		b.update(inputManager, m_GUICamera);
	}
	for (auto& b : m_achievementsMenuButtons){
		b.update(inputManager, m_GUICamera);
	}
}

void MainMenuGUI::IDraw(float fps){
	switch (m_menuState){
	case MenuState::MAIN:
		for (auto& b : m_mainMenuButtons){
			b.draw(m_GUISpritebatch, m_GUICamera);
		}
		break;
	case MenuState::OPTIONS:
		for (auto& b : m_optionsMenuButtons){
			b.draw(m_GUISpritebatch, m_GUICamera);
		}
		m_GUISpritefont.draw(m_GUISpritebatch, "     Options are not implemented. \nContact me if you suspect this is a bug.", glm::vec2(m_GUICameraBounds.x / 2.0f, m_GUICameraBounds.y / 2.0f), glm::vec2(1.0f), 1.0f, Sakura::ColorRGBA8(0, 0, 0, 255), Sakura::Justification::MIDDLE);
		break;
	case MenuState::ACHIEVMENTS:
		for (auto& b : m_achievementsMenuButtons){
			b.draw(m_GUISpritebatch, m_GUICamera);
		}
		m_GUISpritefont.draw(m_GUISpritebatch, "No Achievments to display at this time.  \nContact me if you suspect this is a bug.", glm::vec2(m_GUICameraBounds.x / 2.0f, m_GUICameraBounds.y / 2.0f), glm::vec2(1.0f), 1.0f, Sakura::ColorRGBA8(0, 0, 0, 255), Sakura::Justification::MIDDLE);
		break;
	default:
		printf("MENU STATE NOT INITIALIZED OR INITIALIZED TO UNKOWN ID");
		break;
	}
}

void MainMenuGUI::initComponents(){
	m_GUISpritefont.initTTF("Assets/Interface/Fonts/alagard.ttf", 20, PIXELATED | MIPMAP | TRANS_BORDER);
}

#define buttonScale 1.5f
#define fontSize 10
void MainMenuGUI::initButtons(Sakura::Window* window){
	m_mainMenuButtons.push_back(createButton("Assets/Interface/Button.png", "Assets/Interface/Fonts/alagard.ttf", fontSize, glm::vec2(1.0f, 1.0f),
		PIXELATED | MIPMAP | EDGE_CLAMP, "Continue", glm::vec2(m_GUICameraBounds.x - 250.0f, m_GUICameraBounds.y / 2.0f + 75.0f),
		[this](){ printf("CONTINUE BUTTON NOT CURRENTLY IMPLEMENTED"); },
		MIPMAP | PIXELATED | EDGE_CLAMP, buttonScale));

	m_mainMenuButtons.push_back(createButton("Assets/Interface/Button.png", "Assets/Interface/Fonts/alagard.ttf", fontSize, glm::vec2(1.0f, 1.0f),
		PIXELATED | MIPMAP | EDGE_CLAMP, "New Game", glm::vec2(m_GUICameraBounds.x - 250.0f, m_GUICameraBounds.y / 2.0f + 25.0f),
		[this](){ m_parentScreen->SetNextScreen(1);
				  m_parentScreen->setState(Sakura::ScreenState::CHANGE_NEXT); },
		MIPMAP | PIXELATED | EDGE_CLAMP, buttonScale));

	m_mainMenuButtons.push_back(createButton("Assets/Interface/Button.png", "Assets/Interface/Fonts/alagard.ttf", fontSize, glm::vec2(1.0f, 1.0f),
		PIXELATED | MIPMAP | EDGE_CLAMP, "Options", glm::vec2(m_GUICameraBounds.x - 250.0f, m_GUICameraBounds.y / 2.0f - 25.0f),
		[this](){ m_menuState = MenuState::OPTIONS; printf("OPTIONS SCREEN CURRENTLY NOT IMPLEMENTED"); },
		MIPMAP | PIXELATED | EDGE_CLAMP, buttonScale));

	m_mainMenuButtons.push_back(createButton("Assets/Interface/Button.png", "Assets/Interface/Fonts/alagard.ttf", fontSize, glm::vec2(1.0f, 1.0f),
		PIXELATED | MIPMAP | EDGE_CLAMP, "Level Creator", glm::vec2(m_GUICameraBounds.x - 250.0f, m_GUICameraBounds.y / 2.0f - 75.0f),
		[this](){ m_parentScreen->SetNextScreen(2);
				  m_parentScreen->setState(Sakura::ScreenState::CHANGE_NEXT); },
		MIPMAP | PIXELATED | EDGE_CLAMP, buttonScale));

	m_mainMenuButtons.push_back(createButton("Assets/Interface/Button.png", "Assets/Interface/Fonts/alagard.ttf", fontSize, glm::vec2(1.0f, 1.0f),
		PIXELATED | MIPMAP | EDGE_CLAMP, "Exit Game", glm::vec2(m_GUICameraBounds.x - 250.0f, m_GUICameraBounds.y / 2.0f - 125.0f),
		[this](){ m_parentScreen->setState(Sakura::ScreenState::EXIT_APPLICATION); }, 
		MIPMAP | PIXELATED | EDGE_CLAMP, buttonScale));

	m_optionsMenuButtons.push_back(createButton("Assets/Interface/Button.png", "Assets/Interface/Fonts/alagard.ttf", fontSize, glm::vec2(1.0f, 1.0f),
		PIXELATED | MIPMAP | EDGE_CLAMP, "Back", glm::vec2(),
		[this](){ m_menuState = MenuState::MAIN; },
		MIPMAP | PIXELATED | EDGE_CLAMP, buttonScale));

	m_achievementsMenuButtons.push_back(createButton("Assets/Interface/Button.png", "Assets/Interface/Fonts/alagard.ttf", fontSize, glm::vec2(1.0f, 1.0f),
		PIXELATED | MIPMAP | EDGE_CLAMP, "Back", glm::vec2(),
		[this](){ m_menuState = MenuState::MAIN; },
		MIPMAP | PIXELATED | EDGE_CLAMP, buttonScale));
}

void MainMenu::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}

	if (m_game->inputManager.isKeyDown(KeyID::EQUALS)){
		scrollSpeed = 5.0f;
	}
	else {
		scrollSpeed = 1.0f;
	}
}