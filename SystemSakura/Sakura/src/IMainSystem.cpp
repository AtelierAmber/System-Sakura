#include "IMainSystem.h"
#include "Timing.h"
#include "ScreenList.h"
#include "IScreen.h"
#include "Sakura.h"

#include <algorithm>

namespace Sakura {

	// Some helpful constants.
	const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
	const int MAX_PHYSICS_STEPS = 5; // Max number of physics steps per frame
	const float MS_PER_SECOND = 1000.0f; // Number of milliseconds in a second
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame
	const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime

	IMainSystem::IMainSystem(std::string title /*= "DEFAULT TITLE"*/, unsigned int screenWidth /*= 500*/, unsigned int screenHeight /*= 500*/, unsigned int windowFlags /*= 0*/, int winPositionX /*= -1*/, int winPositionY /*= -1 */){
		m_screenList = std::make_unique<ScreenList>(this);
		m_windowTitle = title;
		m_windowFlags = windowFlags;
		m_windowDims = wDims(screenWidth, screenHeight);
		m_windowPos = glm::ivec2(winPositionX, winPositionY);
	}


	IMainSystem::~IMainSystem(){
	}
	
	int IMainSystem::run(){
		if (!init()){
			return -1;
		}

		FpsLimiter limiter;
		limiter.setMaxFPS(60.0f);

		m_isRunning = true;

		// Start our previousTicks variable
		Uint32 previousTicks = SDL_GetTicks();
		float totalDeltaTime = NULL;

		while (m_isRunning){
			limiter.begin();

			// Calculate the frameTime in milliseconds
			Uint32 newTicks = SDL_GetTicks();
			Uint32 frameTime = newTicks - previousTicks;
			previousTicks = newTicks; // Store newTicks in previousTicks so we can use it next frame
			// Get the total delta time
			totalDeltaTime = (float)frameTime / DESIRED_FRAMETIME;

			int i = 0; // This counter makes sure we don't spiral to death!
			// Loop while we still have steps to process.
			while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
				// The deltaTime should be the the smaller of the totalDeltaTime and MAX_DELTA_TIME
				float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
				// Update all physics here and pass in deltaTime

				inputManager.update();
				update(deltaTime);

				// Since we just took a step that is length deltaTime, subtract from totalDeltaTime
				totalDeltaTime -= deltaTime;
				// Increment our frame counter so we can limit steps to MAX_PHYSICS_STEPS
				++i;
			}
			if (m_isRunning){
				draw();
				m_window.swapBuffer();
			}
			m_fps = limiter.end();
		}
		return 0;
	}

	void IMainSystem::exitGame(){
		m_currentScreen->onExit();
		if (m_screenList){
			m_screenList->destroy();
			m_screenList.reset();
		}
		
		///TODO// : WINDOW DESTROYING

		m_isRunning = false;
	}

	void IMainSystem::update(float deltaTime){
		if (m_currentScreen){
			switch (m_currentScreen->getState()){
			case ScreenState::RUNNING:
				m_currentScreen->update(deltaTime);
				break;
			case ScreenState::CHANGE_NEXT:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->moveNext();
				if (m_currentScreen){
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::CHANGE_PREVIOUS:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->movePrevious();
				if (m_currentScreen){
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::EXIT_APPLICATION:
				exitGame();
				break;
			default:
				break;
			}
		}
		else {
			exitGame();
		}
	}

	void IMainSystem::draw(){
		glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());
		if (m_currentScreen && m_currentScreen->getState() == ScreenState::RUNNING){
			m_currentScreen->EngineDrawMeathod();
		}
	}

	void IMainSystem::onSDLEvent(SDL_Event& evnt){
		switch (evnt.type) {
		case SDL_QUIT:
			exitGame();
			break;
		case SDL_MOUSEMOTION:
			inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager.releaseKey(evnt.button.button);
			break;
		}
	}

	bool IMainSystem::init(){
		Sakura::init();

		if (m_window.create(m_windowTitle, m_windowDims.width, m_windowDims.height, m_windowFlags, m_windowPos.x, m_windowPos.x)) return false;
		onInit();
		addScreens();

		m_currentScreen = m_screenList->getCurrent();
		m_currentScreen->onEntry();
		m_currentScreen->setRunning();

		return true;
	}

}