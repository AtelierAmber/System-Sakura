#include "Window.h"
#include "SakuraErrors.h"

namespace Sakura {

    Window::Window()
    {
    }


    Window::~Window()
    {
    }

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags, int xPosOnScreen, int yPosOnScreen) {
		SDL_DisplayMode display;
		SDL_GetDesktopDisplayMode(0, &display);

		Uint32 flags = SDL_WINDOW_OPENGL;
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			m_screenHeight = display.h;
			m_screenWidth = display.w;
		}
		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if (xPosOnScreen < 0){
			xPosOnScreen = (display.w / 2) - (screenWidth / 2);
		}
		if (yPosOnScreen < 0){
			yPosOnScreen = (display.h / 2) - (screenHeight / 2);
		}

		//Open an SDL window
		m_sdlWindow = SDL_CreateWindow(windowName.c_str(), xPosOnScreen, yPosOnScreen, screenWidth, screenHeight, flags);

        //Set up our OpenGL context
        SDL_GLContext glContext = SDL_GL_CreateContext(m_sdlWindow);
        if (glContext == nullptr) {
            fatalError("SDL_GL context could not be created!");
			return 1;
        }

        //Set up glew (optional but recommended)
        GLenum error = glewInit();
        if (error != GLEW_OK) {
            fatalError("Could not initialize glew!");
			return 1;
        }

        //Check the OpenGL version
        std::printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));

        //Set the background color default to black as default
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        //Set VSYNC
        SDL_GL_SetSwapInterval(0);

        // Enable alpha blend
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return 0;
    }

    void Window::swapBuffer() {
		SDL_GL_SwapWindow(m_sdlWindow);
    }

}