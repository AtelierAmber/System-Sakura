#include "MainScreen.h"
#include <Sakura/include/IMainSystem.h>

#define screen_scale_level 2.0f

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
	m_glyphSortType = Sakura::GlyphSortType::BACK_TO_FRONT;

	// Compile static resources

	// Compile color shader
	m_textureProgram.compileShaders("../SystemSakura/defaultShaders/textureShading.vert", "../SystemSakura/defaultShaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	// Init camera
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(screen_scale_level);
	glm::vec2 cameraPositionOffset((m_window->getScreenWidth() / 2.0f) * (1.0f / screen_scale_level),
		(m_window->getScreenHeight() / 2.0f) * (1.0f / screen_scale_level));
}

void MainScreen::onExit(){
	m_textureProgram.dispose();
	m_debugRenderer.dispose();
	m_resourceManager.destroyResources();
	m_spriteBatch.dispose();
}

void MainScreen::update(float deltaTime){
	m_deltaTime = deltaTime;
	checkInput();

	/* Update game objects */

	m_camera.update();
	/* Finish Updates*/
}

void MainScreen::draw(){
	/* Background Color */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* returns a mat4 with the camera projection matrix */
	// *** camera_matrix(m_camera);
	/* Draw game elements concurrently */
	
	
	/* DEBUG */
	if (debug_game_events){
		
	}
	/* Finish draw */
}

void MainScreen::specificDraw(){
	/* For Elements that require use of a seperate
	* spritebatch, camera, or otherwise need a specific element */

}

/* Process and check for all defined game inputs */
void MainScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
	/* Enable DEBUG mode */
	if (m_game->inputManager.isKeyPressed(KeyID::BACKQUOTE)){
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