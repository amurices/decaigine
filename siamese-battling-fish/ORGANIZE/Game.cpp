#include "Game.h"


void errorBreaker(std::string errorName)
{
	std::cout << "Error caught: " << errorName << std::endl;
	SDL_Quit();
	exit(1);
}

Game::Game() :
	_WindowW(1024),
	_WindowH(768),
	_GameState(GameState::GOING),
	_time(0.0f),
	_gameTimer(Denjine::GameTimer())
{
	// Call the physical game world's constructor
	b2Vec2 gravity(0.0f, -20.0f);
	_gameWorld = new b2World(gravity);
}


Game::~Game()
{
}



// This function starts our game through the initialize() method, then goes in our game loop.
void Game::run()
{
	initialize();	// We set up, and then
	loop();			// we loop
	Denjine::ResourceManager::unloadResources(_soundSystem._system);	// If game ends, unload audio and texture assets
}

// Wrapper around initializing whatever flags and components our middleware needs. Lots of error handling happens here.
void Game::initialize()
{
	Denjine::init();
	_camera.initCamera(_WindowW, _WindowH);
	_soundSystem.init();
	_window.init("Gheime", _WindowW, _WindowH, 0);
	_spriteBatch.init();
	initializeShaders();

	// Texture and audio assets loading
	static GLuint textureGround = Denjine::ResourceManager::getTexture("Textures/coral_platform.png");
	static GLuint textureFish = Denjine::ResourceManager::getTexture("Textures/siamese_fighter_1.png");
	static GLuint textureSwd = Denjine::ResourceManager::getTexture("Textures/6BOT/fire_blade_of_tai.png");
	static GLuint textureMngStr = Denjine::ResourceManager::getTexture("Textures/morningStar.png");
	static GLuint fireBall = Denjine::ResourceManager::getTexture("Textures/fireBall.png");
	static GLuint blueFireBall = Denjine::ResourceManager::getTexture("Textures/blueFireBall.png");

	Denjine::ResourceManager::getAudio("Audio/DMB.mp3", &(_soundSystem._bgmObject1), _soundSystem._system, Denjine::PlaybackType::STREAM);
	Denjine::ResourceManager::getAudio("Audio/GLF.mp3", &(_soundSystem._bgmObject0), _soundSystem._system, Denjine::PlaybackType::STREAM);
	// Play BGM 0, BGM 1 paused, set eternal loops
	_soundSystem.playAudio((_soundSystem._bgmObject0), true , 0); _soundSystem._bgmChannel0->setLoopCount(-1);
	_soundSystem.playAudio((_soundSystem._bgmObject1), false, 0); _soundSystem._bgmChannel1->setLoopCount(-1);


	glm::vec2 posP1(-200, 250); // center of player1's entity
	glm::vec2 posP2(200, 250); // center of player2's entity

	glm::vec4 uv(0.0f, 0.0f, -1.0f, -1.0f);
	glm::vec4 uv1(0.0f, 0.0f, -1.0f, -1.0f);


	Denjine::Color color(255, 255, 255, 255);

	_player1 = Player(_gameWorld, posP1, uv, uv1, textureFish, textureSwd, WeaponType::HUNTER, FishType::HUNTERFISH);
	_player1.init();

	_player2 = Player(_gameWorld, posP2, uv, uv1, textureFish, fireBall, WeaponType::WIZARD, FishType::WARRIORFISH);
	_player2.init();

	_platforms.emplace_back(_gameWorld, glm::vec2(200, -100), glm::vec2(200, 20), glm::vec4(0.0f, 0.0f, 10.0f, -1.0f), textureGround);
	_platforms.emplace_back(_gameWorld, glm::vec2(-200, -100), glm::vec2(200, 20), glm::vec4(0.0f, 0.0f, 10.0f, -1.0f), textureGround);
	_platforms.emplace_back(_gameWorld, glm::vec2(0, 0), glm::vec2(100, 100), glm::vec4(0.0f, 0.0f, 1.0f, -1.0f), textureGround);
	for (int i = 0; i < _platforms.size(); i++)
		_platforms[i].init();
}

void Game::initializeShaders() {

	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}


// Looper getting input -> make timing and FPS into static class
void Game::loop()
{
	int counter = 0;
	
//	_soundSystem.playAudio()
	while (_GameState != GameState::ENDING)
	{
		counter++;
		// Start timing the frame
		_profiler.beginFrame();

		// ---- input processing and updating
		_profiler.beginTimingSubsystem();

		getInput();
		processInput();
		_interfacer.updateInterfacer();
		
		_profiler._inputTime = _profiler.endTimingSubsystem();

		// ---- camera translation
		_profiler.beginTimingSubsystem();

		_camera.updateCamera();
		
		_profiler._cameraTime = _profiler.endTimingSubsystem();

		// ---- timing and frame control
		_profiler.beginTimingSubsystem();

		_gameTimer.calculateFPS();
		_gameTimer.limitFPS();

		_profiler._timerTime = _profiler.endTimingSubsystem();
		
		// ---- physics; updates
		_profiler.beginTimingSubsystem();
		for (int i = 0; i < _platforms.size(); i++)
			_platforms[i].update();
		
		if (_player1._hp > 0)
			_player1.update();
		else
			_player1.die();

		if (_player2._hp > 0)
			_player2.update();
		else
			_player2.die();


		float dt = 1 / _gameTimer.getFPS();
		for (int i = 0; i < 4; i++)
			physicsLoop(dt / 2);

		physicsCollision();

		_profiler._physicsTime = _profiler.endTimingSubsystem();
		// ---- physics; check GameEnding conditions:
		
		if (_player1._bodyEntity._entityBody->GetPosition().y < -400.0f) // or if the sword falls too deep
			_GameState = GameState::ENDING;
		
		
		// ---- graphics
		_profiler.beginTimingSubsystem();

		graphicsLoop();
		
		_profiler._graphicsTime = _profiler.endTimingSubsystem();
		
		
		// ---- end frame
		float totalFrameTime = _profiler.endFrame();

		// ------------------------------------ report
		if (counter > 100) // print this every second:
		{
			std::cout << "Frame ticks: " << totalFrameTime << std::endl;
			std::cout << "Input getting and processing: " << _profiler._inputTime / totalFrameTime << std::endl;
			std::cout << "Camera translating and updating: " << _profiler._cameraTime / totalFrameTime << std::endl;
			std::cout << "Timing and framerate control: " << _profiler._timerTime / totalFrameTime << std::endl;
			std::cout << "Graphics processing: " << _profiler._graphicsTime / totalFrameTime << std::endl;
			std::cout << "Physics processing: " << _profiler._physicsTime / totalFrameTime << std::endl;
			std::cout << "Framerate: " << _gameTimer.getFPS() << std::endl;
			std::cout << std::endl << std::endl;

			counter = 0;
		}

	}
}

void Game::graphicsLoop()
{
	draw();
}

void Game::physicsLoop(float dt)
{
	_gameWorld->Step(dt, 6, 2);
}

void Game::physicsCollision()
{
	if (_player1._dealingDmg) // If player1 is attacking, let's see if he/she hits something
	{
		b2ContactEdge* contacts = _player1._weaponEntity._entityBody->GetContactList();
		if (contacts != NULL && contacts->contact->GetFixtureB()->GetBody() == _player2._bodyEntity._entityBody) // if we hit player 2,
		{
			std::cout << "P1 dealt damage to P2! P2's health = " << _player2._hp << std::endl;

			if (_player2._invulnerabilityDuration <= 0) // do damage
			{
				_player2._hp -= _player1._attack;
				_player2.turnInvulnerable();
			}
		}
	}

	if (_player2._dealingDmg)
	{
		b2ContactEdge* contacts = _player2._weaponEntity._entityBody->GetContactList();
		if (contacts != NULL && contacts->contact->GetFixtureA()->GetBody() == _player1._bodyEntity._entityBody) // if we hit player 2,
		{
			std::cout << "P2 dealt damage to P1! P1's health = " << _player1._hp << std::endl;

			if (_player1._invulnerabilityDuration <= 0) // do damage
			{
				_player1._hp -= _player2._attack;
				_player1.turnInvulnerable();

			}
		}
	}
}

void Game::getInput()
{
	SDL_Event e; // Variable to poll what happens, user-wise

	while (SDL_PollEvent(&e))	// We just want the interfacer to get our input. How we handle it is up to the Game class.
	{
		switch (e.type)
		{
			case SDL_QUIT: 
				_GameState = GameState::ENDING;
				break;
			case SDL_MOUSEMOTION:
				// do things with the new mouse
				break;

			case SDL_KEYDOWN:
				_interfacer.pressKey(e.key.keysym.sym); // Tell the map to consider key pressed
				break;
			case SDL_KEYUP:
				_interfacer.releaseKey(e.key.keysym.sym);
				break;
		}
	}
	// So now, during this frame, our input has been gotten. Everything was stored in our interfacer.
}

void Game::processInput()
{
	// All that lies below are game controls

	// ------------------------------- CAMERA CONTROLS 
	if (_interfacer.isKeyPressed(SDLK_UP))
	{
		_camera.setScale(_camera.getScale() * 1.11f);
	}
	if (_interfacer.isKeyPressed(SDLK_LEFT))
	{
		_camera.setWorldPosition(_camera.getWorldPosition() + glm::vec2(10.0f, 0.0f));
	}
	if (_interfacer.isKeyPressed(SDLK_DOWN))
	{
		_camera.setScale(_camera.getScale() * 0.91f);
	}
	if (_interfacer.isKeyPressed(SDLK_RIGHT))
	{
		_camera.setWorldPosition(_camera.getWorldPosition() + glm::vec2(-10.0f, 0.0f));
	}





	// ------------------------------- PLAYER 1 CONTROLS 
	b2Vec2 p1movement(0.0f, 0.0f);
	bool moveP1 = false;
	if (_interfacer.isKeyPressed(SDLK_a))
	{
		p1movement.x = -1.0f;
		moveP1 = true;
	}

	if (_interfacer.isKeyPressed(SDLK_d))
	{
		p1movement.x = 1.0f;
		moveP1 = true;
	}

	if (_interfacer.isKeyPressed(SDLK_w))
	{
		p1movement.y = 1.0f;
		moveP1 = true;
	}

	if (_interfacer.isKeyPressed(SDLK_s))
	{
		p1movement.y = -1.0f;
		moveP1 = true;
	}


	if (moveP1)
		_player1.move(glm::vec2(p1movement.x, p1movement.y)); 


	else // if not moving, drag velocity and face forward
	{
		b2Vec2 dragVel = _player1._bodyEntity._entityBody->GetLinearVelocity();
		dragVel = DRAG_VELOCITY_FACTOR * dragVel;
		_player1._bodyEntity._entityBody->SetLinearVelocity(dragVel);

		float ang = _player1._bodyEntity._entityBody->GetAngle();
		_player1._bodyEntity._entityBody->SetAngularVelocity(3 * (- ang));
	}

	if (_interfacer.isKeyTriggered(SDLK_c))
	{
		_player1.jump();
	}

	if (_player1._wpnType == WeaponType::WIZARD) // wizard rotates wpn to attack
	{
		static bool last;
		if (_interfacer.isKeyPressed(SDLK_e))
		{
			_player1._dealingDmg = true;
			last = true;
			_player1.rotate(MOTOR_SPEED_FASTEST);
		}
		else if (_interfacer.isKeyPressed(SDLK_r))
		{
			_player1._dealingDmg = true;
			last = false;
			_player1.rotate(- MOTOR_SPEED_FASTEST);
		}
		else
		{
			if (last)
				_player1.rotate(MOTOR_SPEED_SLOWEST);
			else
				_player1.rotate(-MOTOR_SPEED_SLOWEST);

			_player1._dealingDmg = false;
		}
	}

	if (_player1._wpnType == WeaponType::HUNTER)
	{
		// -------------------------------- wpn rotation controls
		if (_interfacer.isKeyPressed(SDLK_e))
		{
			_player1.rotate(MOTOR_SPEED_SLOW);
		}

		else if (_interfacer.isKeyPressed(SDLK_r))
		{
			_player1.rotate(-MOTOR_SPEED_SLOW);
		}
		else
		{
			_player1.rotate(0);
		}

		// ------------------------------- attack control
		if (_interfacer.isKeyTriggered(SDLK_3))
		{
			_player1.pierce();
		}
	}

	if (_player1._wpnType == WeaponType::WARRIOR)
	{
		
		// ------------------------------- attack control
		if (_interfacer.isKeyTriggered(SDLK_3))
		{
			_player1.slash();
		}

	}

	_player1.attack();


	// ------------------------------- PLAYER 2 CONTROLS 
	b2Vec2 p2movement(0.0f, 0.0f);
	bool moveP2 = false;
	if (_interfacer.isKeyPressed(SDLK_j))
	{
		p2movement.x = -1.0f;
		moveP2 = true;
	}

	if (_interfacer.isKeyPressed(SDLK_l))
	{
		p2movement.x = 1.0f;
		moveP2 = true;
	}

	if (_interfacer.isKeyPressed(SDLK_i))
	{
		p2movement.y = 1.0f;
		moveP2 = true;
	}

	if (_interfacer.isKeyPressed(SDLK_k))
	{
		p2movement.y = -1.0f;
		moveP2 = true;
	}


	if (moveP2)
		_player2.move(glm::vec2(p2movement.x, p2movement.y));
	else
	{
		b2Vec2 dragVel = _player2._bodyEntity._entityBody->GetLinearVelocity();
		dragVel = DRAG_VELOCITY_FACTOR * dragVel;
		_player2._bodyEntity._entityBody->SetLinearVelocity(dragVel);

		float ang = _player2._bodyEntity._entityBody->GetAngle();
		_player2._bodyEntity._entityBody->SetAngularVelocity(3 * (-ang));
	}

	if (_interfacer.isKeyTriggered(SDLK_c))
	{
		_player1.jump();
	}

	if (_player2._wpnType == WeaponType::WIZARD) // wizard rotates wpn to attack
	{
		static bool laster;
		if (_interfacer.isKeyPressed(SDLK_o))
		{
			_player2._dealingDmg = true;
			laster = true;
			_player2.rotate(MOTOR_SPEED_FASTEST);
		}
		else if (_interfacer.isKeyPressed(SDLK_p))
		{
			_player2._dealingDmg = true;
			laster = false;
			_player2.rotate(-MOTOR_SPEED_FASTEST);
		}
		else
		{
			if (laster)
				_player2.rotate(MOTOR_SPEED_SLOWEST);
			else
				_player2.rotate(-MOTOR_SPEED_SLOWEST);

			_player2._dealingDmg = false;
		}
	}


	if (_player2._wpnType == WeaponType::HUNTER)
	{
		// -------------------------------- wpn rotation controls
		if (_interfacer.isKeyPressed(SDLK_o))
		{
			_player2.rotate(MOTOR_SPEED_SLOW);
		}

		else if (_interfacer.isKeyPressed(SDLK_p))
		{
			_player2.rotate(-MOTOR_SPEED_SLOW);
		}
		else
		{
			_player2.rotate(0);
		}

		// ------------------------------- attack control
		if (_interfacer.isKeyTriggered(SDLK_0))
		{
			_player2.pierce();
		}
	}

	if (_player2._wpnType == WeaponType::WARRIOR)
	{

		// ------------------------------- attack control
		if (_interfacer.isKeyTriggered(SDLK_0))
		{
			_player2.slash();
		}

	}

	_player2.attack();


	// BGM controls
	if (_interfacer.isKeyTriggered(SDLK_CAPSLOCK))
	{
		bool pause;
		_soundSystem._bgmChannel0->getPaused(&pause);
		pause = !pause;
		_soundSystem._bgmChannel0->setPaused(pause);
	}
	if (_interfacer.isKeyTriggered(SDLK_TAB))
	{
		bool pause;
		_soundSystem._bgmChannel1->getPaused(&pause);
		pause = !pause;
		_soundSystem._bgmChannel1->setPaused(pause);
	}

}

void Game::draw()
{
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);

	// Get locations of elements
	GLint texLoc = _colorProgram.getUniformLocation("textex");
	GLint pLoc = _colorProgram.getUniformLocation("P");
	// Get the camera matrix to use as a parameter for OpenGL
	glm::mat4 cameraMatrix = _camera.getProjMatrix();

	// Set up the shader programs' parameters
	glUniform1i(texLoc, 0);
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, &(cameraMatrix[0][0]) );

	// Tell the spriteBatch that this is the beginning of a frame
	_spriteBatch.begin();

	Denjine::Color color(255, 255, 255, 255);


	for (int i = 0; i < 1; i++)
	{
	//	if (_camera.isSpriteInView(	glm::vec2(_playerEntity._drawPosDim.x, _playerEntity._drawPosDim.y),
	//		glm::vec2(_playerEntity._drawPosDim.x + _playerEntity._drawPosDim.z, _playerEntity._drawPosDim.y + _playerEntity._drawPosDim.w)))
		{
			for (int j = 0; j < _platforms.size(); j++)
				_spriteBatch.draw(*(_platforms[j]._drawPosDims), _platforms[j]._drawUvDims, _platforms[j]._texture, 0.0f, color, 0.0f);

			// Draw P1's body and weapon
			_spriteBatch.draw(*(_player1._drawPosDims), _player1._drawUvDims, _player1._swapTex, 0.0f, color, _player1._drawAngle);
			_spriteBatch.draw(*(_player1._wpDrawPosDims), _player1._wpDrawUvDims, _player1._swapWTex, 0.0f, color, _player1._wpDrawAngle);
			// Draw P2's body and weapon
			_spriteBatch.draw(*(_player2._drawPosDims), _player2._drawUvDims, _player2._swapTex, 0.0f, color, _player2._drawAngle);
			_spriteBatch.draw(*(_player2._wpDrawPosDims), _player2._wpDrawUvDims, _player2._swapWTex, 0.0f, color, _player2._wpDrawAngle);
		}
	}

	// Tell the spriteBatch that it can set up for the next frame
	_spriteBatch.end();
	
	// Render everything on our spriteBatch to the screen
	_spriteBatch.renderBatch();
	// unbind textures
	glBindTexture(GL_TEXTURE_2D, 0);
	// stop using our shader program
	_colorProgram.unuse();			
	// swap the back buffer for the front buffer of our window
	_window.swapBuffers();
}

