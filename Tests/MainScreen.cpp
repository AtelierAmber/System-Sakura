#include "MainScreen.h"
#include <Sakura/include/IMainSystem.h>

#define screen_scale_level 2.0f

MainScreen::MainScreen(Sakura::Window* window) : m_window(window){

}

MainScreen::~MainScreen(){
	/* Empty */
}

int MainScreen::getNextScreenIndex() const {
	return 1;
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

	m_player.init(glm::vec2(32.0f, 64.0f), glm::vec2(12.0f, 8.0f), glm::vec2(32.0f), m_resourceManager, Sakura::ColorRGBA8(255, 255, 255, 255));

	// Compile our static resources
	m_music.push_back(m_resourceManager.loadMusic("Assets/Audio/FoxieEpic.ogg"));
	m_music.push_back(m_resourceManager.loadMusic("Assets/Audio/Arabesque.mp3"));
	m_music.push_back(m_resourceManager.loadMusic("Assets/Audio/carnivalrides.ogg"));
	m_music.push_back(m_resourceManager.loadMusic("Assets/Audio/DarkWinds.OGG"));
	m_music.push_back(m_resourceManager.loadMusic("Assets/Audio/Heroic Demise (New).mp3"));
	m_music.push_back(m_resourceManager.loadMusic("Assets/Audio/No More Magic.mp3"));
	m_music.push_back(m_resourceManager.loadMusic("Assets/Audio/radakan - old crypt.ogg"));
	m_music.push_back(m_resourceManager.loadMusic("Assets/Audio/Soliloquy.mp3"));
	m_music.push_back(m_resourceManager.loadMusic("Assets/Audio/The Dark Amulet.mp3"));
	m_music.push_back(m_resourceManager.loadMusic("Assets/Audio/the_kings_crowning_v1.mp3"));
	m_music.push_back(m_resourceManager.loadMusic("Assets/Audio/TheLoomingBattle.OGG"));

	// Compile our color shader
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	m_levelManager.init(&m_spriteBatch, &m_debugRenderer, &m_resourceManager);
	m_levelManager.addLevel("Assets/Textures/Levels/Main_Level.png", 
		glm::vec2(320.0f, 320.0f), MIPMAP | PIXELATED | EDGE_CLAMP, {});
	m_levelManager.proceedToLevel(0);

	// Init camera
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(screen_scale_level);
	glm::vec2 cameraPositionOffset((m_window->getScreenWidth() / 2.0f) * (1.0f / screen_scale_level),
		(m_window->getScreenHeight() / 2.0f) * (1.0f / screen_scale_level));
	m_gameGUI.init(this, &m_textureProgram, &m_resourceManager, glm::ivec2(m_window->getScreenWidth(), m_window->getScreenHeight()), cameraPositionOffset, screen_scale_level);
	m_gameGUI.initSpecialData(&m_player);
}

void MainScreen::onExit(){
	m_textureProgram.dispose();
	m_debugRenderer.dispose();
	m_gameGUI.destroy();
	m_resourceManager.destroyResources();
	m_spriteBatch.dispose();
}

void MainScreen::update(float deltaTime){
	m_deltaTime = deltaTime;
	m_gameGUI.update(m_game->inputManager);
	checkInput();

	m_player.update(m_game->inputManager, deltaTime);

	// TODO : COLLISION DETECTION WITH SPATIAL PARTITIONING
	//! IMPLEMENT INTERVAL STEP COLLISION DETECTION

	for (auto& cr : m_levelManager.getLevelData().BoundRects()){
		m_player.collisionRectangle()->calculateRectangleCollision(cr);
	}

	m_camera.setPosition(m_player.getPosition());
	/* Clamp the camera position to the left and bottom edges of the level */
	glm::vec2 cameraLowerBound = glm::vec2(m_camera.getPosition().x - (m_window->getScreenWidth() / 2) / m_camera.getScale(), m_camera.getPosition().y - (m_window->getScreenHeight() / 2) / m_camera.getScale());
	if (cameraLowerBound.x < 0 && cameraLowerBound.y < 0){
		m_camera.setPosition(m_camera.getPosition() - cameraLowerBound);
	}
	else if (cameraLowerBound.x < 0){
		m_camera.setPosition(glm::vec2(m_camera.getPosition().x - cameraLowerBound.x, m_camera.getPosition().y));
	}
	else if (cameraLowerBound.y < 0){
		m_camera.setPosition(glm::vec2(m_camera.getPosition().x, m_camera.getPosition().y - cameraLowerBound.y));
	}
	m_camera.update();
}

void MainScreen::draw(){
	/* Background Color */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_levelManager.drawCurrentBackLayer();

	/* returns a mat4 with the camera projection matrix */
	// *** camera_matrix(m_camera);

	m_player.draw(m_spriteBatch, m_deltaTime);

	/* DEBUG */
	if (debug_game_events){
		m_player.drawDebug(m_debugRenderer);
		m_levelManager.drawDebug();
	}

	m_levelManager.drawCurrentOverLayer();
}

void MainScreen::specificDraw(){
	/* For Elements that require use of a seperate
	* spritebatch, camera, or otherwise need a specific element */
	m_gameGUI.draw(m_GLSLPUniformName, m_game->getFps());
}

void MainScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
	if (m_game->inputManager.isKeyPressed(KeyID::BACKQUOTE)){
		if (!debug_game_events){
			debug_game_events = true;
		}else debug_game_events = false;
	}
	if (m_game->inputManager.isKeyPressed(KeyID::ESCAPE)){
		m_game->exitGame();
	}

	//RESET LEVEL
	if (m_game->inputManager.isKeyDown(KeyID::KeyMod::LSHIFT) && m_game->inputManager.isKeyDown(KeyID::KeyMod::LCTRL) && m_game->inputManager.isKeyPressed(KeyID::r)){
		onExit();
		onEntry();
	}

	//Mouse
	if (m_game->inputManager.isKeyPressed(MouseId::BUTTON_LEFT)){
		glm::vec2 mouseCoords = m_camera.convertScreenToWorld(glm::vec2(m_game->inputManager.getMouseCoords().x, m_game->inputManager.getMouseCoords().y));
		glm::vec2 mouseSize = glm::vec2(mouseCoords.x - m_previousMouseLocation.x, mouseCoords.y - m_previousMouseLocation.y);
		printf("Mouse Coords (x, y): %.2f, %.2f\n Size from previous (w, h): %.2f, %.2f\n", mouseCoords.x, mouseCoords.y, mouseSize.x, mouseSize.y);
		m_previousMouseLocation = mouseCoords;
	}
}