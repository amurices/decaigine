#pragma once
#include <Denjine/Headerz.h>	// OpenGL, SDL, etc
#include <Denjine/Denjine.h>

// Subsystems
#include <Denjine/Window.h>				// window wrapper
#include <Denjine/Camera2D.h>			// camera unit
#include <Denjine/Interfacer.h>			// user input getter and storage
#include <Denjine/GLSLprogram.h>		// shader wizardry
#include <Denjine/Profiler.h>
#include <Denjine/Entity.h>
#include <Denjine/ResourceManager.h>	// load/unload resources (audio and textures)
#include <Denjine/GameTimer.h>			// timing, FPS calculation and limiting
#include <Denjine/IOManager.h>			// file input/output, parsing
#include <Denjine/SpriteBatch.h>		// meat of the renderer
#include <Denjine/AudioSystem.h>		// soundsystem

// Game classes
#include "Player.h"
#include "Platform.h"



enum class GameState
{
	GOING,	// Game goes
	ENDING	// Game stops
};

class Game
{
public:
	Game();
	Game(glm::vec2 gravity);
	~Game();
	// run the game
	void run();


	// ------------- Public attributes
	
	
private:
	// ------------- Methods of the game

	void initialize();			// Starts everything from SDL, OpenGL, etc
	void initializeShaders();	// sets up our shaders. shaders will be very minimal for this rudimentary engine
	void loop();				// Looper, starring makeup JGL
	void graphicsLoop();		// Different processings
	void physicsLoop(float dt);	// per frame
	void physicsCollision();	// check for relevant events between characters
	void getInput();			// wrapper that handles input through SDL
	void processInput();		// figure out what to do with input; generic function
	void draw();				// Drawer; will interact with out container of renderobjects
	
	// ------------- Attributes of the game
	int							_WindowW, _WindowH;
	GameState					_GameState;			// State of the game, of course (FSM)
	Player						_player1;			// two player game
	Player						_player2;
	std::vector<Platform>		_platforms;			// as many platforms as level designer wants

	// Denjine subsystems
	Denjine::AudioSystem		_soundSystem;
	Denjine::GLSLProgram		_colorProgram;
	Denjine::SpriteBatch		_spriteBatch;
	Denjine::GameTimer			_gameTimer;
	Denjine::Camera2D			_camera;
	Denjine::Interfacer			_interfacer;
	Denjine::Window				_window;
	Denjine::Profiler			_profiler;


	// Physical game world
	b2World* _gameWorld;


	float			_time;

};

