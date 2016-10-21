#include "MainScreen.h"
#include <Sakura/include/IMainSystem.h>

#define screen_scale_level 1.0f

/************************************************************************/
/* GUI                                                                  */
/************************************************************************/
MainGUI::MainGUI(){ /* Empty */ }

MainGUI::~MainGUI(){ /* Empty */ }

void MainGUI::destroyComponents(){
	m_resumeButton.destroy();
	m_optionsButton.destroy();
	m_quitButton.destroy();
}

void MainGUI::update(Sakura::InputManager& inputManager){
	m_GUICamera.update();
	switch (state){
	case GAMEPLAY:

		break;
	case MENU:
		m_resumeButton.update(inputManager, m_GUICamera);
		m_optionsButton.update(inputManager, m_GUICamera);
		m_quitButton.update(inputManager, m_GUICamera);
		break;
	case OPTIONSmain:
		m_quitButton.update(inputManager, m_GUICamera);
		break;
	default:
		std::printf("Gui has no state!\n");
		break;
	}
}

void MainGUI::IDraw(float fps){
	switch (state){
	case GAMEPLAY:

		break;
	case MENU:
		m_resumeButton.draw(m_GUISpritebatch, m_GUICamera, true);
		m_optionsButton.draw(m_GUISpritebatch, m_GUICamera, true);
		m_quitButton.draw(m_GUISpritebatch, m_GUICamera, true);
		break;
	case OPTIONSmain:
		m_GUISpritefont.draw(m_GUISpritebatch, "OPTIONS SCREEN", glm::vec2(m_parentWindow->getScreenWidth() / 2, m_parentWindow->getScreenHeight() / 2), glm::vec2(0.5f), ALWAYS_ON_TOP, Sakura::ColorRGBA8(255, 255, 255, 255), Sakura::Justification::MIDDLE);
		break;
	default:
		std::printf("Gui has no state!\n");
		break;
	}
}

void MainGUI::initComponents(){
	m_GUISpritefont.initTTF("Assets/Fonts/destructobeambb_reg.ttf", 48, MIPMAP | LINEAR | TRANS_BORDER);
}

#define BUTTON_SCALE 2.0f
void MainGUI::initButtons(Sakura::Window* window){
	m_parentWindow = window;
	m_resumeButton = createButton("Assets/Sprites/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 48, glm::vec2(0.5f), MIPMAP | LINEAR | TRANS_BORDER, "Resume", glm::vec4(window->getScreenWidth() / 2 - (46 * BUTTON_SCALE), ((float)window->getScreenHeight() / 3) * 2, 92 * BUTTON_SCALE, 25 * BUTTON_SCALE),
		[this](){ state = GAMEPLAY; }, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_optionsButton = createButton("Assets/Sprites/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 48, glm::vec2(0.5f), MIPMAP | LINEAR | TRANS_BORDER, "Options", glm::vec4(window->getScreenWidth() / 2 - (46 * BUTTON_SCALE), window->getScreenHeight() / 2, 92 * BUTTON_SCALE, 25 * BUTTON_SCALE),
		[this](){ state = OPTIONSmain; }, MIPMAP | PIXELATED | EDGE_CLAMP);
	m_quitButton = createButton("Assets/Sprites/menu_button.png", "Assets/Fonts/destructobeambb_reg.ttf", 48, glm::vec2(0.5f), MIPMAP | LINEAR | TRANS_BORDER, "Quit", glm::vec4(window->getScreenWidth() / 2 - (46 * BUTTON_SCALE), window->getScreenHeight() / 3, 92 * BUTTON_SCALE, 25 * BUTTON_SCALE),
		[this](){ m_parentScreen->setState(Sakura::ScreenState::EXIT_APPLICATION); },
		MIPMAP | PIXELATED | EDGE_CLAMP);
}

/************************************************************************/
/* SCREEN                                                               */
/************************************************************************/
MainScreen::MainScreen(Sakura::Window* window) : m_window(window){
	/* Empty */
}

MainScreen::~MainScreen(){
	/* Empty */
}

int MainScreen::getNextScreenIndex() const {
	return 0;
}

int MainScreen::getPreviousScreenIndex() const {
	return 0;
}

void MainScreen::build(){

}

void MainScreen::destroy(){

}

void MainScreen::onEntry(){
	m_GLSLSamplerName = "mySampler";
	m_GLSLPUniformName = "P";
	m_spriteBatch.init();
	m_debugRenderer.init();
	m_resourceManager.init();
	m_glyphSortType = Sakura::GlyphSortType::FRONT_TO_BACK;

	// Compile the color shader
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	// Compile static resources
	m_mainMusic = m_resourceManager.loadMusic("Assets/Audio/Music/n-Dimensions.ogg");
	m_bossMusic = m_resourceManager.loadMusic("Assets/Audio/Music/Orbital Colossus.ogg");
	m_background = m_resourceManager.getTexture("Assets/Sprites/Grid.png", MIPMAP | PIXELATED | EDGE_CLAMP);
	m_userFont.initTTF("Assets/Fonts/destructobeambb_reg.ttf", 72, MIPMAP | LINEAR | TRANS_BORDER);
	m_enemyFont.initTTF("Assets/Fonts/Sprykski.ttf", 72, MIPMAP | LINEAR | TRANS_BORDER);

	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(screen_scale_level);
	glm::vec2 cameraPositionOffset((m_window->getScreenWidth() / 2.0f) * (1.0f / screen_scale_level),
		(m_window->getScreenHeight() / 2.0f) * (1.0f / screen_scale_level));
	m_camera.setPosition(cameraPositionOffset);
	m_interface.init(this, &m_textureProgram, &m_resourceManager, glm::ivec2(m_window->getScreenWidth(), m_window->getScreenHeight()), cameraPositionOffset, screen_scale_level);
	m_interface.initButtons(m_window);
}

void MainScreen::onExit(){
	m_textureProgram.dispose();
	m_debugRenderer.dispose();
	m_resourceManager.destroyResources();
	m_spriteBatch.dispose();
	m_interface.destroy();
	m_userFont.dispose();
	m_enemyFont.dispose();
}

void MainScreen::update(float deltaTime){
	m_deltaTime = deltaTime;
	checkInput();

	/* Update game objects */

	m_camera.update();
	m_interface.update(m_game->inputManager);
	/* Finish Updates*/
}

void MainScreen::draw(){
	/* Background Color */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* returns a mat4 with the camera projection matrix */
	// *** camera_matrix(m_camera);
	/* Draw game elements concurrently */

	m_spriteBatch.draw(glm::vec4(0.0f, 0.0f, m_window->getScreenWidth(), m_window->getScreenHeight()), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_background.id, -500.0f, Sakura::ColorRGBA8(255, 255, 255, 255));

	m_userFont.draw(m_spriteBatch, "HELLOW", glm::vec2(50.0f, 50.0f), glm::vec2(1.0f, 1.0f), 1.0f, Sakura::ColorRGBA8(255, 0, 0, 255), Sakura::Justification::LEFT);
	m_enemyFont.draw(m_spriteBatch, "DIE!!", glm::vec2(50.0f, 150.0f), glm::vec2(1.0f, 1.0f), 1.0f, Sakura::ColorRGBA8(255, 0, 0, 255), Sakura::Justification::LEFT);
	/* DEBUG */
	if (debug_game_events){
		m_debugRenderer.drawBox(glm::vec4(2.0f, 2.0f, 4.0f, 4.0f), Sakura::ColorRGBA8(255, 255, 255, 255), 0);
	}
	/* Finish draw */
}

void MainScreen::specificDraw(){
	/* For Elements that require use of a seperate
	* spritebatch, camera, or otherwise need a specific element */
	m_interface.draw(m_GLSLPUniformName, m_game->getFps());
}

/* Process and check for all defined game inputs */
void MainScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
	/* Enable DEBUG mode */
	if (m_game->inputManager.wasKeyDown(KeyID::BACKQUOTE)){
		if (!debug_game_events){
			debug_game_events = true;
		}
		else debug_game_events = false;
	}

	/* Reset Screen */
	if (m_game->inputManager.isKeyDown(KeyID::KeyMod::LSHIFT) && m_game->inputManager.isKeyDown(KeyID::KeyMod::LCTRL) && m_game->inputManager.isKeyPressed(KeyID::r)){
		onExit();
		onEntry();
	}

	//Mouse
	if (m_game->inputManager.isKeyPressed(MouseId::BUTTON_LEFT)){
		if (debug_game_events){
			glm::vec2 mouseCoords = m_camera.convertScreenToWorld(glm::vec2(m_game->inputManager.getMouseCoords().x, m_game->inputManager.getMouseCoords().y));
			glm::vec2 mouseSize = glm::vec2(mouseCoords.x - m_previousMouseLocation.x, mouseCoords.y - m_previousMouseLocation.y);
			printf("Mouse Coords (x, y): %.2f, %.2f\n Size from previous (w, h): %.2f, %.2f\n", mouseCoords.x, mouseCoords.y, mouseSize.x, mouseSize.y);
			m_previousMouseLocation = mouseCoords;
		}
	}
}