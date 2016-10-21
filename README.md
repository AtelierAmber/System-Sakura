========================================================================
    STATIC LIBRARY : SakuraEngine Project Overview
========================================================================

/////////////////////////////////////////////////////////////////////////////
Preparing a new project:

	-  VC++ Directories -> Include Directories -> $(SolutionDir)depends/include/
	-  VC++ Directories -> Include Directories -> $(SolutionDir)
	-  VC++ Directories -> Library Directories -> $(SolutionDir)depends/lib/
	-  VC++ Directories -> Library Directories -> $(SolutionDir)Debug/ or $(SolutionDir)Release/
	-  Linker -> Additional Dependencies -> 
SDL2.lib
SDL2main.lib
SDL2_ttf.lib
SDL2_mixer.lib
openGL32.lib
glew32.lib
SakuraEngine.lib

	- SET SUBSYSTEM : Linker -> System -> Console (/SUBSYSTEM:CONSOLE)
	- Must include shader files

/////////////////////////////////////////////////////////////////////////////
ENGINE:
/// Member Variables
    Sakura::Window m_window; ///< The game window
    
    Sakura::GLSLProgram m_textureProgram; ///< The shader program

    Sakura::InputManager m_inputManager; ///< Handles input

    Sakura::Camera2D m_camera; ///< Main Camera

-----------------------------------------------------------------------------------------------------
//Systems Initializing
    Sakura::init();
	///Initializes all the necessary Engine parameters///

-----------------------------------------------------------------------------------------------------
    m_window.create(char*, int, int, int);
	m_window.create(char*, int, int, int, int, int, int, int)
	///Create's the window centered on screen with (windowName, windowWidth, windowHeight, SDL Flags)
	///Or (windowName, windowWidth, windowHeight, SDL Flags, windowPositionX, windowPositionY)
	///windowPositionX, windowPositionY default to 0
	///-1 windowPositionX or windowPositionY sets the window in the center of the screen
		///SDL Flags (can be OR together 4 (borderless)| 2(Fullscreen))
		Borderless = 2
		Fullscreen = 4
		Invisible = 1
		Windowed = 0

-----------------------------------------------------------------------------------------------------
    glClearColor(float, float, float, float);
	///Used to set background color, 0.0f being no value, 
	///1.0f being full 255 value(red value, green value, blue value, alpha value)

------------------------------------------------------------------------------------------------------
    m_textureProgram.compileShaders(string, string);
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();
	///Shader initialization
	///Compile shaders takes parameters (Fragment Shader FilePath, Vertex Shader FilePath)
	///Add Attribute takes parameters (Data Type as it appears in the shader files; As A String)
	///Link Shaders links the attributes to the sader files

-------------------------------------------------------------------------------------------------------
    m_camera.init(int, int);
	///Initializes the camera with parameters (window width, window height)

///Development

/////////////////////////////////////////////////////////////////////////////
Other notes:

Will work on optimization later on

Thanks to Ben (makingGamesWithBen) for the tutorials that allowed me to 
develop this engine

/////////////////////////////////////////////////////////////////////////////
