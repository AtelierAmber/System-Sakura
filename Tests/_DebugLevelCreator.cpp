#include "_DebugLevelCreator.h"
#include <Sakura/include/IMainSystem.h>

_DebugLevelCreator::_DebugLevelCreator(Sakura::Window* window) : m_window(window){ }


_DebugLevelCreator::~_DebugLevelCreator(){ }

#define screen_scale_level 2

int _DebugLevelCreator::getNextScreenIndex() const {
	return m_nextScreen;
}

int _DebugLevelCreator::getPreviousScreenIndex() const {
	return 0;
}

void _DebugLevelCreator::build(){

}

void _DebugLevelCreator::destroy(){

}

void _DebugLevelCreator::onEntry(){
	m_GLSLSamplerName = "mySampler";
	m_GLSLPUniformName = "P";
	m_spriteBatch.init();
	m_debugRenderer.init();
	m_resourceManager.init();

	// Compile the color shader
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	m_textureSheet = m_resourceManager.getTileSheet("Assets/Textures/LevelEditor/roguelikeSheet.png", glm::ivec2(17, 17), PIXELATED | MIPMAP | EDGE_CLAMP);

	m_levelManager.init(&m_spriteBatch, &m_debugRenderer, &m_resourceManager);

	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	glm::vec2 cameraPositionOffset((m_window->getScreenWidth() / 2.0f) * (1.0f / screen_scale_level),
		(m_window->getScreenHeight() / 2.0f) * (1.0f / screen_scale_level));
	m_camera.setPosition(cameraPositionOffset);
	m_GUI.init(this, &m_textureProgram, &m_resourceManager, glm::ivec2(m_window->getScreenWidth(), m_window->getScreenHeight()), cameraPositionOffset, screen_scale_level);
	m_GUI.initComponents();
}

void _DebugLevelCreator::onExit(){
	m_debugRenderer.dispose();
	m_spriteBatch.dispose();
	m_resourceManager.destroyResources();
	m_textureProgram.dispose();
	m_GUI.destroy();
}

void _DebugLevelCreator::draw(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void _DebugLevelCreator::specificDraw(){
	m_GUI.draw(m_GLSLPUniformName, m_game->getFps());
}

void _DebugLevelCreator::checkInput() {
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
}

void _DebugLevelCreator::update(float deltaTime){
	m_camera.update();
	m_GUI.update(m_game->inputManager);

	checkInput();
}

void _DebugLevelCreator::resetLevel(){
	m_currentTextureLocations.clear();
	m_currentTextureOverlayLocations.clear();
	m_currentTextureOverlayUVS.clear();
	m_currentTextureUVS.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////

void gui::destroyComponents(){
	for (auto& it : m_buttons){
		it.destroy();
	}
	m_buttons.clear();
}

void gui::update(Sakura::InputManager& inputManager){
	for (auto& it : m_buttons){
		it.update(inputManager, m_GUICamera);
	}
}

void gui::IDraw(float fps){
	for (auto& it : m_buttons){
		it.draw(m_GUISpritebatch, m_GUICamera);
	}
}

void gui::initComponents(){
	m_GUISpritefont.initTTF("Assets/Interface/Fonts/scifibit.ttf", 5, PIXELATED | MIPMAP | EDGE_CLAMP);
		initButtons();
}

void gui::createNewLevel(){
	
}

void gui::initSpecificComponents(Sakura::DebugRenderer* debugRenderer){
	initButtons();
}

void gui::initButtons(){
// 	m_GUIElements.push_back(m_resourceManager->getTexture("Assets/Interface/LevelEditor/CheckBox_Button.png", LINEAR | MIPMAP | EDGE_CLAMP));
// 	m_GUIElements.push_back(m_resourceManager->getTexture("Assets/Interface/LevelEditor/CheckBox_Button_Selected.png", LINEAR | MIPMAP | EDGE_CLAMP));
// 	m_GUIElements.push_back(m_resourceManager->getTexture("Assets/Interface/LevelEditor/Option_Button.png", LINEAR | MIPMAP | EDGE_CLAMP));
// 	m_GUIElements.push_back(m_resourceManager->getTexture("Assets/Interface/LevelEditor/Option_Button_Selected.png", LINEAR | MIPMAP | EDGE_CLAMP));
// 	m_GUIElements.push_back(m_resourceManager->getTexture("Assets/Interface/LevelEditor/Selected_Texture.png", LINEAR | MIPMAP | EDGE_CLAMP));
// 
	m_buttons.push_back(createButton("Assets/Interface/Button.png", "Assets/Interface/Fonts/scifibit.ttf", 5, glm::vec2(1.0f, 1.0f),
		PIXELATED | MIPMAP | EDGE_CLAMP, "Clear Current Level", glm::vec2(0.0f, 0.0f), 
		[this](){m_guiState = GUIState::CLEAR_LEVEL; },
		LINEAR | MIPMAP | EDGE_CLAMP, 1.0f));
}
