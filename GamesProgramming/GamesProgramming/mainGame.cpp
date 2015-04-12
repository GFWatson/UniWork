#include "mainGame.h"
#include "Errors.h"
#include "imageLoader.h"

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

mainGame::mainGame() :
_screenWidth(1024),
_screenHeight(768),
_window(nullptr),
_fps(0.0f), _maxFPS(60.0f), _currentTime(0.0f), _prevTime(0.0f), _deltaTime(0.0f),
_accX(0.2f * _maxFPS), _decX(0.3f * _maxFPS), _accY(-0.5f * _maxFPS), _maxSpeedX(5.0f * _maxFPS), _maxSpeedY(-10.0f * _maxFPS), _jumpSpeed(10.0f * _maxFPS),
_round(1),
_state(gameState::PLAY),
_oneRunTime(0.0f), _oneAttackTime(0.0f), _oneSlideTime(0.0f), _oneSpeedX(0.0f), _oneSpeedY(0.0f), _oneMoveRequest(false),
_twoRunTime(0.0f), _twoAttackTime(0.0f), _twoSlideTime(0.0f), _twoSpeedX(0.0f), _twoSpeedY(0.0f), _twoMoveRequest(false),
_pTime(4000.0f), _lTime1(0.0f), _lTime2(0.0f), _lTime3(0.0f), _lTime4(0.0f), _lTime5(0.0f), _lTime6(0.0f), _lTime7(0.0f), _l1(0.0f), _l2(0.0f), _l3(0.0f), _l4(0.0f), _l5(0.0f), _l6(0.0f), _l7(0.0f)
{
	_camera.init(_screenWidth, _screenHeight);
}

mainGame::~mainGame()
{

}

void mainGame::run()
{
	initSystems();

	_defaultTexture = imageLoader::loadPNG("Textures/Default.png");
	_standingTexture = imageLoader::loadPNG("Textures/StickmanStanding.png");
	_runningRightOneTexture = imageLoader::loadPNG("Textures/StickmanRunningRight1.png");
	_runningRightTwoTexture = imageLoader::loadPNG("Textures/StickmanRunningRight2.png");
	_runningRightThreeTexture = imageLoader::loadPNG("Textures/StickmanRunningRight3.png");
	_runningLeftOneTexture = imageLoader::loadPNG("Textures/StickmanRunningLeft1.png");
	_runningLeftTwoTexture = imageLoader::loadPNG("Textures/StickmanRunningLeft2.png");
	_runningLeftThreeTexture = imageLoader::loadPNG("Textures/StickmanRunningLeft3.png");
	_jumpingRightTexture = imageLoader::loadPNG("Textures/StickmanJumpRight.png");
	_jumpingLeftTexture = imageLoader::loadPNG("Textures/StickmanJumpLeft.png");
	_blockingRightTexture = imageLoader::loadPNG("Textures/StickmanBlockRight.png");
	_blockingLeftTexture = imageLoader::loadPNG("Textures/StickmanBlockLeft.png");
	_punchingRightTexture = imageLoader::loadPNG("Textures/StickmanPunchRight.png");
	_punchingLeftTexture = imageLoader::loadPNG("Textures/StickmanPunchLeft.png");
	_crouchingRightTexture = imageLoader::loadPNG("Textures/StickmanCrouchRight.png");
	_crouchingLeftTexture = imageLoader::loadPNG("Textures/StickmanCrouchLeft.png");
	_slidingRightTexture = imageLoader::loadPNG("Textures/StickmanSlideRight.png");
	_slidingLeftTexture = imageLoader::loadPNG("Textures/StickmanSlideLeft.png");
	_kickingRightTexture = imageLoader::loadPNG("Textures/StickmanKickRight.png");
	_kickingLeftTexture = imageLoader::loadPNG("Textures/StickmanKickLeft.png");

	_texture = _defaultTexture;
	
	_playerOne.init(50.0f, 50.0f, 48.0f, 64.0f, _standingTexture, playerState::STANDR);
	_playerTwo.init(400.0f, 50.0f, 48.0f, 64.0f, _standingTexture, playerState::STANDR);

	_floor.init(0.0f, 0.0f, 1024.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	
	_p1.init(-150.0f, 65.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p2.init(-150.0f, 65.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p3.init(-150.0f, 65.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p4.init(-150.0f, 170.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p5.init(-150.0f, 170.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p6.init(-150.0f, 170.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p7.init(-150.0f, 255.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p8.init(-150.0f, 255.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p9.init(-150.0f, 255.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p10.init(-150.0f, 340.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p11.init(-150.0f, 340.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p12.init(-150.0f, 340.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p13.init(-150.0f, 425.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p14.init(-150.0f, 425.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p15.init(-150.0f, 425.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p16.init(-150.0f, 510.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p17.init(-150.0f, 510.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p18.init(-150.0f, 510.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p19.init(-150.0f, 595.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p20.init(-150.0f, 595.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	_p21.init(-150.0f, 595.0f, 150.0f, 20.0f, _defaultTexture, playerState::ENVIR);
	
	_sceneList.push_back(&_playerOne);
	_sceneList.push_back(&_playerTwo);

	_sceneList.push_back(&_floor);
	
	_sceneList.push_back(&_p1);
	_sceneList.push_back(&_p2);
	_sceneList.push_back(&_p3);
	_sceneList.push_back(&_p4);
	_sceneList.push_back(&_p5);
	_sceneList.push_back(&_p6);
	_sceneList.push_back(&_p7);
	_sceneList.push_back(&_p8);
	_sceneList.push_back(&_p9);
	_sceneList.push_back(&_p10);
	_sceneList.push_back(&_p11);
	_sceneList.push_back(&_p12);
	_sceneList.push_back(&_p13);
	_sceneList.push_back(&_p14);
	_sceneList.push_back(&_p15);
	_sceneList.push_back(&_p16);
	_sceneList.push_back(&_p17);
	_sceneList.push_back(&_p18);
	_sceneList.push_back(&_p19);
	_sceneList.push_back(&_p20);
	_sceneList.push_back(&_p21);
		

	gameLoop();
		
}

void mainGame::initSystems()
{
	// initialise SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// create window
	_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		fatalError("SDL Window could not be created!");
	}

	// create context
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		fatalError("SDL_GL context could not be created!");
	}

	//Check for joysticks
	if (SDL_NumJoysticks() < 1)
	{
		printf("Warning: No joysticks connected!\n");
	}
	else
	{
		//Load joystick
		_controller1 = SDL_GameControllerOpen(0);
		if (_controller1 == NULL)
		{
			printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
		}
	}

	// initialise glew
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Could not initialise glew!");
	}

	// create two windows for swapping between
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// set background colour
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	initShaders();
}

void mainGame::initShaders() {
	_colourProgram.compileShaders("Shaders/vertexShading.vert.txt", "Shaders/colourShading.frag.txt");
	_colourProgram.addAttribute("vertexPosition");
	_colourProgram.addAttribute("vertexColour");
	_colourProgram.addAttribute("vertexUV");
	_colourProgram.linkShaders();

	_pLocation = _colourProgram.getUniformLocation("P");
	_mLocation = _colourProgram.getUniformLocation("M");
}

void mainGame::gameLoop()
{
	while (_state != gameState::EXIT){
		_currentTime = SDL_GetTicks();
		_deltaTime = _currentTime - _prevTime;
		processInput();
		setScale(&_playerOne);
		setScale(&_playerTwo);
		_playerOne.set(_deltaTime, _oneSpeedX, _oneSpeedY);
		_playerTwo.set(_deltaTime, _twoSpeedX, _twoSpeedY);
		platformGenerator();
		//moveScene();
		collisionDetection(&_playerOne);
		collisionDetection(&_playerTwo);
		drawGame();
		//moderateFPS();
		_prevTime = _currentTime;

	}
}

void mainGame::processInput()
{
	SDL_Event evnt;

	// constant keyboard presses
	// for moving horizontally
	SDL_PumpEvents();
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_D]) {
		if (_playerOne.getState() == playerState::STANDR || _playerOne.getState() == playerState::STANDL || _playerOne.getState() == playerState::RUNL || _playerOne.getState() == playerState::RUNR) {
			_playerOne.setState(playerState::RUNR);
			_oneSpeedX += _accX;
			_oneMoveRequest = true;
			if (_oneRunTime < 0.4f){
				_playerOne.setTexture(_runningRightOneTexture);
			}
			else if (_oneRunTime < 0.8f){
				_playerOne.setTexture(_runningRightTwoTexture);
			}
			else {
				_playerOne.setTexture(_runningRightThreeTexture);
			}

			if (_oneRunTime > 1.2f){
				_oneRunTime = 0.0f;
			}

			_oneRunTime += 0.1f;
		}
	}
	if (keystate[SDL_SCANCODE_A]) {
		if (_playerOne.getState() == playerState::STANDR || _playerOne.getState() == playerState::STANDL || _playerOne.getState() == playerState::RUNL || _playerOne.getState() == playerState::RUNR) {
			_playerOne.setState(playerState::RUNL);
			_oneSpeedX -= _accX;
			_oneMoveRequest = true;
			if (_oneRunTime < 0.4f){
				_playerOne.setTexture(_runningLeftOneTexture);
			}
			else if (_oneRunTime < 0.8f){
				_playerOne.setTexture(_runningLeftTwoTexture);
			}
			else {
				_playerOne.setTexture(_runningLeftThreeTexture);
			}

			if (_oneRunTime > 1.2f){
				_oneRunTime = 0.0f;
			}

			_oneRunTime += 0.1f;
		}
	}

	// constant movement controller

	const Sint16 axisState = SDL_GameControllerGetAxis(_controller1, SDL_CONTROLLER_AXIS_LEFTX);
	if (axisState > 10000) {
		if (_playerTwo.getState() == playerState::STANDR || _playerTwo.getState() == playerState::STANDL || _playerTwo.getState() == playerState::RUNL || _playerTwo.getState() == playerState::RUNR) {
			_playerTwo.setState(playerState::RUNR);
			_twoSpeedX += _accX;
			_twoMoveRequest = true;
			if (_twoRunTime < 0.4f){
				_playerTwo.setTexture(_runningRightOneTexture);
			}
			else if (_twoRunTime < 0.8f){
				_playerTwo.setTexture(_runningRightTwoTexture);
			}
			else {
				_playerTwo.setTexture(_runningRightThreeTexture);
			}

			if (_twoRunTime > 1.2f){
				_twoRunTime = 0.0f;
			}

			_twoRunTime += 0.1f;
		}
	}
	else if (axisState < -10000) {
		if (_playerTwo.getState() == playerState::STANDR || _playerTwo.getState() == playerState::STANDL || _playerTwo.getState() == playerState::RUNL || _playerTwo.getState() == playerState::RUNR) {
			_playerTwo.setState(playerState::RUNL);
			_twoSpeedX -= _accX;
			_twoMoveRequest = true;
			if (_twoRunTime < 0.4f){
				_playerTwo.setTexture(_runningLeftOneTexture);
			}
			else if (_twoRunTime < 0.8f){
				_playerTwo.setTexture(_runningLeftTwoTexture);
			}
			else {
				_playerTwo.setTexture(_runningLeftThreeTexture);
			}

			if (_twoRunTime > 1.2f){
				_twoRunTime = 0.0f;
			}

			_twoRunTime += 0.1f;
		}
	}
	
	// mouse clicks
	// for attacking and blocking
	while (SDL_PollEvent(&evnt)){
		switch (evnt.type){
		case SDL_QUIT:
			_state = gameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (evnt.button.button == SDL_BUTTON_LEFT) {
				switch (_playerOne.getState()){
				case playerState::STANDR:
					_playerOne.setState(playerState::PUNCHR);
					_playerOne.setTexture(_punchingRightTexture);
					break;
				case playerState::BLOCKR:
					_playerOne.setState(playerState::PUNCHR);
					_playerOne.setTexture(_punchingRightTexture);
					break;
				case playerState::JUMPR:
					_playerOne.setState(playerState::HIGHKR);
					_playerOne.setTexture(_kickingRightTexture);
					break;
				case playerState::CROUCHR:
					_playerOne.setState(playerState::LOWKR);
					_playerOne.setTexture(_slidingRightTexture);
					break;
				case playerState::STANDL:
					_playerOne.setState(playerState::PUNCHL);
					_playerOne.setTexture(_punchingLeftTexture);
					break;
				case playerState::BLOCKL:
					_playerOne.setState(playerState::PUNCHL);
					_playerOne.setTexture(_punchingLeftTexture);
					break;
				case playerState::JUMPL:
					_playerOne.setState(playerState::HIGHKL);
					_playerOne.setTexture(_kickingLeftTexture);
					break;
				case playerState::CROUCHL:
					_playerOne.setState(playerState::LOWKL);
					_playerOne.setTexture(_slidingLeftTexture);
					break;
				default:
					break;
				}
			}
			else if (evnt.button.button == SDL_BUTTON_RIGHT){
				if (_playerOne.getState() == playerState::STANDR){
					_playerOne.setState(playerState::BLOCKR);
					_playerOne.setTexture(_blockingRightTexture);
				}
				else if (_playerOne.getState() == playerState::STANDL){
					_playerOne.setState(playerState::BLOCKL);
					_playerOne.setTexture(_blockingLeftTexture);
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (evnt.button.button == SDL_BUTTON_LEFT) {
				switch (_playerOne.getState()) {
				case playerState::PUNCHR:
					_playerOne.setState(playerState::STANDR);
					_playerOne.setTexture(_standingTexture);
					break;
				case playerState::PUNCHL:
					_playerOne.setState(playerState::STANDL);
					_playerOne.setTexture(_standingTexture);
					break;
				case playerState::HIGHKR:
					_playerOne.setState(playerState::JUMPR);
					_playerOne.setTexture(_jumpingRightTexture);
					break;
				case playerState::HIGHKL:
					_playerOne.setState(playerState::JUMPL);
					_playerOne.setTexture(_jumpingLeftTexture);
					break;
				case playerState::LOWKR:
					_playerOne.setState(playerState::STANDR);
					_playerOne.setTexture(_standingTexture);
					break;
				case playerState::LOWKL:
					_playerOne.setState(playerState::STANDL);
					_playerOne.setTexture(_standingTexture);
					break;

				default:
					break;
				}
			}
			else if (evnt.button.button == SDL_BUTTON_RIGHT){
				if (_playerOne.getState() == playerState::BLOCKR){
					_playerOne.setState(playerState::STANDR);
					_playerOne.setTexture(_standingTexture);
				}
				else if (_playerOne.getState() == playerState::BLOCKL){
					_playerOne.setState(playerState::STANDL);
					_playerOne.setTexture(_standingTexture);
				}
			}
			break;

		// one-time keyboard presses
		// for jumping, crouching and sliding
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym) {
			case SDLK_w:
				if (_playerOne.getState() == playerState::STANDR || _playerOne.getState() == playerState::RUNR){
					_oneSpeedY = _jumpSpeed;
					_playerOne.setState(playerState::JUMPR);
					_playerOne.setTexture(_jumpingRightTexture);
				}
				else if (_playerOne.getState() == playerState::STANDL || _playerOne.getState() == playerState::RUNL) {
					_oneSpeedY = _jumpSpeed;
					_playerOne.setState(playerState::JUMPL);
					_playerOne.setTexture(_jumpingLeftTexture);
				}
				break;
			case SDLK_s:
				switch (_playerOne.getState()){
				case playerState::STANDR:
					_playerOne.setState(playerState::CROUCHR);
					_playerOne.setTexture(_crouchingRightTexture);
					break;
				case playerState::STANDL:
					_playerOne.setState(playerState::CROUCHL);
					_playerOne.setTexture(_crouchingLeftTexture);
					break;
				case playerState::RUNR:
					_oneMoveRequest = false;
					_playerOne.setState(playerState::LOWKR);
					_playerOne.setTexture(_slidingRightTexture);

					_oneSlideTime += 0.1f;
					if (_oneSlideTime >= 5.0f) {
						_playerOne.setState(playerState::STANDR);
						_playerOne.setTexture(_standingTexture);
					}

					break;
				case playerState::RUNL:
					_oneMoveRequest = false;
					_playerOne.setState(playerState::LOWKL);
					_playerOne.setTexture(_slidingLeftTexture);

					_oneSlideTime += 0.1f;
					if (_oneSlideTime >= 5.0f) {
						_playerOne.setState(playerState::STANDR);
						_playerOne.setTexture(_standingTexture);
					}
					break;

				default:
					break;
				}
				break;

			default:
				break;
			}
			break;

		// keyboard up includes horizontal movement
		case SDL_KEYUP:
			switch (evnt.key.keysym.sym) {
			case SDLK_d:
				if (_playerOne.getState() != playerState::JUMPR && _playerOne.getState() != playerState::JUMPL) {
					_playerOne.setState(playerState::STANDR);
					_oneMoveRequest = false;
					_playerOne.setTexture(_standingTexture);
					_oneRunTime = 0.0f;
				}
				break;
			case SDLK_a:
				if (_playerOne.getState() != playerState::JUMPR && _playerOne.getState() != playerState::JUMPL) {
					_playerOne.setState(playerState::STANDL);
					_oneMoveRequest = false;
					_playerOne.setTexture(_standingTexture);
					_oneRunTime = 0.0f;
				}
				break;
			case SDLK_w:
				_oneRunTime = 0.0f;
				_oneMoveRequest = false;
				break;
			case SDLK_s:
				_oneMoveRequest = false;
				if (_playerOne.getState() == playerState::CROUCHR || _playerOne.getState() == playerState::LOWKR) {
					_playerOne.setState(playerState::STANDR);
					_playerOne.setTexture(_standingTexture);
					_oneRunTime = 0.0f;
					_oneSlideTime = 0.0f;
				}
				else {
					_playerOne.setState(playerState::STANDL);
					_playerOne.setTexture(_standingTexture);
					_oneRunTime = 0.0f;
					_oneSlideTime = 0.0f;
				}
				break;


			default:
				break;
			}

		//gamepad
			/*
		case SDL_CONTROLLERDEVICEADDED:
			addController(evnt.cdevice);
			break;

		case SDL_CONTROLLERDEVICEREMOVED:
			removeController(evnt.cdevice);
			break;
			*/

		case SDL_CONTROLLERBUTTONDOWN:
			if (evnt.cbutton.button == SDL_CONTROLLER_BUTTON_A && evnt.cbutton.state == SDL_PRESSED) {
				if (_playerTwo.getState() == playerState::STANDR || _playerTwo.getState() == playerState::RUNR){
					_twoSpeedY = _jumpSpeed;
					_playerTwo.setState(playerState::JUMPR);
					_playerTwo.setTexture(_jumpingRightTexture);
				}
				else if (_playerTwo.getState() == playerState::STANDL || _playerTwo.getState() == playerState::RUNL) {
					_twoSpeedY = _jumpSpeed;
					_playerTwo.setState(playerState::JUMPL);
					_playerTwo.setTexture(_jumpingLeftTexture);
				}
			}
			
			if (evnt.cbutton.button == SDL_CONTROLLER_BUTTON_B && evnt.cbutton.state == SDL_PRESSED) {
				switch (_playerTwo.getState()){
				case playerState::STANDR:
					_playerTwo.setState(playerState::CROUCHR);
					_playerTwo.setTexture(_crouchingRightTexture);
					break;
				case playerState::STANDL:
					_playerTwo.setState(playerState::CROUCHL);
					_playerTwo.setTexture(_crouchingLeftTexture);
					break;
				case playerState::RUNR:
					_twoMoveRequest = false;
					_playerTwo.setState(playerState::LOWKR);
					_playerTwo.setTexture(_slidingRightTexture);

					_twoSlideTime += 0.1f;
					if (_twoSlideTime >= 5.0f) {
						_playerTwo.setState(playerState::STANDR);
						_playerTwo.setTexture(_standingTexture);
					}

					break;
				case playerState::RUNL:
					_twoMoveRequest = false;
					_playerTwo.setState(playerState::LOWKL);
					_playerTwo.setTexture(_slidingLeftTexture);

					_twoSlideTime += 0.1f;
					if (_twoSlideTime >= 5.0f) {
						_playerTwo.setState(playerState::STANDR);
						_playerTwo.setTexture(_standingTexture);
					}
					break;

				default:
					break;
				}
			}
			
			break;

		case SDL_CONTROLLERBUTTONUP:
			if (evnt.cbutton.button == SDL_CONTROLLER_BUTTON_A && evnt.cbutton.state == SDL_RELEASED) {
				_twoRunTime = 0.0f;
				_twoMoveRequest = false;
			}

			if (evnt.cbutton.button == SDL_CONTROLLER_BUTTON_B && evnt.cbutton.state == SDL_RELEASED) {
				_twoMoveRequest = false;
				if (_playerTwo.getState() == playerState::CROUCHR || _playerTwo.getState() == playerState::LOWKR) {
					_playerTwo.setState(playerState::STANDR);
					_playerTwo.setTexture(_standingTexture);
					_twoRunTime = 0.0f;
					_twoSlideTime = 0.0f;
				}
				else {
					_playerTwo.setState(playerState::STANDL);
					_playerTwo.setTexture(_standingTexture);
					_twoRunTime = 0.0f;
					_twoSlideTime = 0.0f;
				}
			}
			break;

		case SDL_CONTROLLERAXISMOTION:
			switch (evnt.caxis.axis) {
			case SDL_CONTROLLER_AXIS_LEFTX:
				if (evnt.caxis.value < 10000 && evnt.caxis.value > -10000) {
					if (_playerTwo.getState() != playerState::JUMPR && _playerTwo.getState() != playerState::JUMPL && _playerTwo.getState() == playerState::RUNR) {
						_playerTwo.setState(playerState::STANDR);
						_twoMoveRequest = false;
						_playerTwo.setTexture(_standingTexture);
						_twoRunTime = 0.0f;
					}
					else if (_playerTwo.getState() != playerState::JUMPR && _playerTwo.getState() != playerState::JUMPL && _playerTwo.getState() == playerState::RUNL) {
						_playerTwo.setState(playerState::STANDL);
						_twoMoveRequest = false;
						_playerTwo.setTexture(_standingTexture);
						_twoRunTime = 0.0f;
					}
				}
				break;

			case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
				if (evnt.caxis.value > 0) {
					switch (_playerTwo.getState()){
					case playerState::STANDR:
						_playerTwo.setState(playerState::PUNCHR);
						_playerTwo.setTexture(_punchingRightTexture);
						break;
					case playerState::BLOCKR:
						_playerTwo.setState(playerState::PUNCHR);
						_playerTwo.setTexture(_punchingRightTexture);
						break;
					case playerState::JUMPR:
						_playerTwo.setState(playerState::HIGHKR);
						_playerTwo.setTexture(_kickingRightTexture);
						break;
					case playerState::CROUCHR:
						_playerTwo.setState(playerState::LOWKR);
						_playerTwo.setTexture(_slidingRightTexture);
						break;
					case playerState::STANDL:
						_playerTwo.setState(playerState::PUNCHL);
						_playerTwo.setTexture(_punchingLeftTexture);
						break;
					case playerState::BLOCKL:
						_playerTwo.setState(playerState::PUNCHL);
						_playerTwo.setTexture(_punchingLeftTexture);
						break;
					case playerState::JUMPL:
						_playerTwo.setState(playerState::HIGHKL);
						_playerTwo.setTexture(_kickingLeftTexture);
						break;
					case playerState::CROUCHL:
						_playerTwo.setState(playerState::LOWKL);
						_playerTwo.setTexture(_slidingLeftTexture);
						break;
					default:
						break;
					}
				}
				else {
					switch (_playerTwo.getState()) {
					case playerState::PUNCHR:
						_playerTwo.setState(playerState::STANDR);
						_playerTwo.setTexture(_standingTexture);
						break;
					case playerState::PUNCHL:
						_playerTwo.setState(playerState::STANDL);
						_playerTwo.setTexture(_standingTexture);
						break;
					case playerState::HIGHKR:
						_playerTwo.setState(playerState::JUMPR);
						_playerTwo.setTexture(_jumpingRightTexture);
						break;
					case playerState::HIGHKL:
						_playerTwo.setState(playerState::JUMPL);
						_playerTwo.setTexture(_jumpingLeftTexture);
						break;
					case playerState::LOWKR:
						_playerTwo.setState(playerState::STANDR);
						_playerTwo.setTexture(_standingTexture);
						break;
					case playerState::LOWKL:
						_playerTwo.setState(playerState::STANDL);
						_playerTwo.setTexture(_standingTexture);
						break;

					default:
						break;
					}
				}
				break;

			case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
				break;

			default:
				break;

			}
			break;

		default:
			break;
		}
	}





	// gravity
	_oneSpeedY += _accY;
	_twoSpeedY += _accY;

	// limit speed
	if (_oneSpeedX > _maxSpeedX) _oneSpeedX = _maxSpeedX;
	if (_oneSpeedX < -_maxSpeedX) _oneSpeedX = -_maxSpeedX;
	if (_oneSpeedY < _maxSpeedY) _oneSpeedY = _maxSpeedY;
	if (_twoSpeedX > _maxSpeedX) _twoSpeedX = _maxSpeedX;
	if (_twoSpeedX < -_maxSpeedX) _twoSpeedX = -_maxSpeedX;
	if (_twoSpeedY < _maxSpeedY) _twoSpeedY = _maxSpeedY;

	// allows players to stand still
	if (_oneSpeedX > 0 && _oneSpeedX < 10.0f) _oneSpeedX = 0;
	if (_oneSpeedX < 0 && _oneSpeedX > -10.0f) _oneSpeedX = 0;
	if (_twoSpeedX > 0 && _twoSpeedX < 10.0f) _twoSpeedX = 0;
	if (_twoSpeedX < 0 && _twoSpeedX > -10.0f) _twoSpeedX = 0;

	// horizontal decceleration
	if (!_oneMoveRequest && _playerOne.getState() != playerState::JUMPR && _playerOne.getState() != playerState::JUMPL && _playerOne.getState() != playerState::LOWKL && _playerOne.getState() != playerState::LOWKR) {
		if (_oneSpeedX > 2.0f) {
			_oneSpeedX -= _decX;
		}
		else if (_oneSpeedX < -2.0f) {
			_oneSpeedX += _decX;
		} // deccelerate slower if sliding
	else if (_playerOne.getState() == playerState::LOWKR || _playerOne.getState() == playerState::LOWKL) {
		if (_oneSpeedX > 2.0f) {
			_oneSpeedX -= (0.05f * _maxFPS);
		}
		else if (_oneSpeedX < -2.0f) {
			_oneSpeedX += (0.05f * _maxFPS);
		}
	}


	if (!_twoMoveRequest && _playerTwo.getState() != playerState::JUMPR && _playerTwo.getState() != playerState::JUMPL && _playerTwo.getState() != playerState::LOWKL && _playerTwo.getState() != playerState::LOWKR) {
		if (_twoSpeedX > 2.0f) {
			_twoSpeedX -= _decX;
		}
		else if (_twoSpeedX < -2.0f) {
			_twoSpeedX += _decX;
		}
	}
	else if (_playerTwo.getState() == playerState::LOWKR || _playerTwo.getState() == playerState::LOWKL) {
		if (_twoSpeedX > 2.0f) {
			_twoSpeedX -= (0.05f * _maxFPS);
		}
		else if (_twoSpeedX < -2.0f) {
			_twoSpeedX += (0.05f * _maxFPS);
		}
	}
	}
}

void mainGame::collisionDetection(Sprite* player) {
	
	// get bounding boxes of both
	float x, y, width, height, x2, y2, width2, height2;
	player->get(&x, &y, &width, &height);

	for (int i = 0; i < _sceneList.size(); i++) {
		
		_sceneList[i]->get(&x2, &y2, &width2, &height2);

		// if it's not the same object
		if (x == x2 && y == y2 && width == width2 && height == height2) {
			continue;
		}
		// check to see if they're NOT overlapping
		else if (y >= height2 && height <= y2 && x >= width2 && width <= x2) {
			continue;
		}
		else {

			// get collision points of player
			float a, b, c, d, e, f;
			player->getColPoints(&a, &b, &c, &d, &e, &f, 0);
			// check for collision
			// bottom
			if (((a >= x2 && a <= width2) && (b >= y2 && b <= height2)) || ((c >= x2 && c <= width2) && (d >= y2 && d <= height2))) {
				// move back
				player->set(1000.0f, 0.0f, (height2 - y));
				// allow jumping again
				if (player->getState() == playerState::JUMPR) {
					player->setState(playerState::STANDR);
					player->setTexture(_standingTexture);
				}
				else if (player->getState() == playerState::JUMPL) {
					player->setState(playerState::STANDL);
					player->setTexture(_standingTexture);
				}
			}
			player->getColPoints(&a, &b, &c, &d, &e, &f, 1);
			// check for collision
			//top
			if (((a >= x2 && a <= width2) && (b >= y2 && b <= height2)) || ((c >= x2 && c <= width2) && (d >= y2 && d <= height2))) {
				// move back
				player->set(1000.0f, 0.0f, (y2 - height));
			}
			player->getColPoints(&a, &b, &c, &d, &e, &f, 2);
			// check for collision
			// left
			if (((a >= x2 && a <= width2) && (b >= y2 && b <= height2)) || ((c >= x2 && c <= width2) && (d >= y2 && d <= height2)) || ((e >= x2 && e <= width2) && (f >= y2 && f <= height2))) {
				// move back
				player->set(1000.0f, (width2 - x), 0.0f);
			}
			player->getColPoints(&a, &b, &c, &d, &e, &f, 3);
			// check for collision
			// right
			if (((a >= x2 && a <= width2) && (b >= y2 && b <= height2)) || ((c >= x2 && c <= width2) && (d >= y2 && d <= height2)) || ((e >= x2 && e <= width2) && (f >= y2 && f <= height2))) {
				// move back
				player->set(1000.0f, (x2 - width + 2.0f), 0.0f);
			}
		}
				

	}

	if (width >= _screenWidth) {
		player->set(1000.0, (_screenWidth - width), 0.0f);
	}

}

void mainGame::moderateFPS() {
	
}

void mainGame::platformGenerator() {
	// 7 levels of platforms
	// keep time since last platform generated for each
	// random number for each
	// 20% chance & 5 sec gap
	// only generate if platform is offScreenLeft
	_pTime += _deltaTime;
	_lTime1 += _deltaTime;	_lTime2 += _deltaTime; _lTime3 += _deltaTime; _lTime4 += _deltaTime; _lTime5 += _deltaTime; _lTime6 += _deltaTime; _lTime7 += _deltaTime;

	std::srand(_deltaTime);
	_l7 = rand() % 10;	_l6 = rand() % 10;	_l5 = rand() % 10;	_l4 = rand() % 10;	_l3 = rand() % 10;	_l2 = rand() % 10;	_l1 = rand() % 10;

	if (_pTime > 5000.0f) {

		if (_l1 > 6 && _lTime1 > 7000.0f) {
			if (_p1.offScreenLeft())
				_p1.setPos(1528.0f, 65.0f);
			else if (_p2.offScreenLeft())
				_p2.setPos(1528.0f, 65.0f);
			else if (_p3.offScreenLeft())
				_p3.setPos(1528.0f, 65.0f);
			_pTime = 0.0f;
			_lTime1 = 0.0f;
		}

		if (_l2 > 7 && _lTime2 > 7000.0f) {
			if (_p4.offScreenLeft())
				_p4.setPos(1528.0f, 165.0f);
			else if (_p5.offScreenLeft())
				_p5.setPos(1528.0f, 165.0f);
			else if (_p6.offScreenLeft())
				_p6.setPos(1528.0f, 165.0f);
			_pTime = 0.0f;
			_lTime2 = 0.0f;
		}

		if (_l3 > 7 && _lTime3 > 7000.0f) {
			if (_p7.offScreenLeft())
				_p7.setPos(1528.0f, 265.0f);
			else if (_p8.offScreenLeft())
				_p8.setPos(1528.0f, 265.0f);
			else if (_p9.offScreenLeft())
				_p9.setPos(1528.0f, 265.0f);
			_pTime = 0.0f;
			_lTime3 = 0.0f;
		}

		if (_l4 > 7 && _lTime4 > 5000.0f) {
			if (_p10.offScreenLeft())
				_p10.setPos(1528.0f, 365.0f);
			else if (_p11.offScreenLeft())
				_p11.setPos(1528.0f, 365.0f);
			else if (_p12.offScreenLeft())
				_p12.setPos(1528.0f, 365.0f);
			_pTime = 0.0f;
			_lTime4 = 0.0f;
		}

		if (_l5 > 7 && _lTime5 > 5000.0f) {
			if (_p13.offScreenLeft())
				_p13.setPos(1528.0f, 465.0f);
			else if (_p14.offScreenLeft())
				_p14.setPos(1528.0f, 465.0f);
			else if (_p15.offScreenLeft())
				_p15.setPos(1528.0f, 465.0f);
			_pTime = 0.0f;
			_lTime5 = 0.0f;
		}

		if (_l6 > 5 && _lTime6 > 5000.0f) {
			if (_p16.offScreenLeft())
				_p16.setPos(1528.0f, 565.0f);
			else if (_p17.offScreenLeft())
				_p17.setPos(1528.0f, 565.0f);
			else if (_p18.offScreenLeft())
				_p18.setPos(1528.0f, 565.0f);
			_pTime = 0.0f;
			_lTime6 = 0.0f;
		}

		if (_l7 > 5 && _lTime7 > 5000.0f) {
			if (_p19.offScreenLeft())
				_p19.setPos(1528.0f, 665.0f);
			else if (_p20.offScreenLeft())
				_p20.setPos(1528.0f, 665.0f);
			else if (_p21.offScreenLeft())
				_p21.setPos(1528.0f, 665.0f);
			_pTime = 0.0f;
			_lTime7 = 0.0f;
		}
	}
}

void mainGame::moveScene() {
	_p1.set(_deltaTime, -50.0f, 0.0f);
	_p2.set(_deltaTime, -50.0f, 0.0f);
	_p3.set(_deltaTime, -50.0f, 0.0f);
	_p4.set(_deltaTime, -50.0f, 0.0f);
	_p5.set(_deltaTime, -50.0f, 0.0f);
	_p6.set(_deltaTime, -50.0f, 0.0f);
	_p7.set(_deltaTime, -50.0f, 0.0f);
	_p8.set(_deltaTime, -50.0f, 0.0f);
	_p9.set(_deltaTime, -50.0f, 0.0f);
	_p10.set(_deltaTime, -50.0f, 0.0f);
	_p11.set(_deltaTime, -50.0f, 0.0f);
	_p12.set(_deltaTime, -50.0f, 0.0f);
	_p13.set(_deltaTime, -50.0f, 0.0f);
	_p14.set(_deltaTime, -50.0f, 0.0f);
	_p15.set(_deltaTime, -50.0f, 0.0f);
	_p16.set(_deltaTime, -50.0f, 0.0f);
	_p17.set(_deltaTime, -50.0f, 0.0f);
	_p18.set(_deltaTime, -50.0f, 0.0f);
	_p19.set(_deltaTime, -50.0f, 0.0f);
	_p20.set(_deltaTime, -50.0f, 0.0f);
	_p21.set(_deltaTime, -50.0f, 0.0f);

	_playerOne.set(_deltaTime, -50.0f, 0.0f);
}

void mainGame::setScale(Sprite* player) {
	if (player->getState() == playerState::CROUCHL || player->getState() == playerState::CROUCHR)
		player->setScale(1.0f, 0.5f);
	else if (player->getState() == playerState::HIGHKL || player->getState() == playerState::HIGHKR || player->getState() == playerState::LOWKL || player->getState() == playerState::LOWKR)
		player->setScale(2.0f, 0.5f);
	else
		player->setScale(1.0f, 1.0f);

}

void mainGame::drawGame() {

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colourProgram.use();
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = _colourProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	//GLint timeLocation = _colourProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, _time);

	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(_pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	for (int i = 0; i < _sceneList.size(); i++) {
		_mMatrix = _sceneList[i]->getModel();
		glUniformMatrix4fv(_mLocation, 1, GL_FALSE, &(_mMatrix[0][0]));
		_texture = _sceneList[i]->getTexture();
		glBindTexture(GL_TEXTURE_2D, _texture.id);
		_sceneList[i]->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	_colourProgram.unuse();

	SDL_GL_SwapWindow(_window);
}