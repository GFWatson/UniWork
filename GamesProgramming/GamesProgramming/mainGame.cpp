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
_accX(0.2f * _maxFPS), _decX(0.3f * _maxFPS), _accY(-0.5f * _maxFPS), _maxSpeedX(5.0f * _maxFPS), _maxSpeedY(-10.0f * _maxFPS), _jumpSpeed(8.0f * _maxFPS),
_state(gameState::PLAY),
_oneState(playerState::STANDR), _oneCollision(collisionState::NONE), _oneRunTime(0.0f), _oneAttackTime(0.0f), _oneSpeedX(0.0f), _oneSpeedY(0.0f), _oneMoveRequest(false)
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

	_playerOne.init(50.0f, 50.0f, 48.0f, 64.0f, _standingTexture);
	_floor.init(0.0f, 0.0f, 1024.0f, 20.0f, _defaultTexture);

	_sceneList.push_back(&_playerOne);
	_sceneList.push_back(&_floor);

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
		//std::cout << _oneSpeedX << std::endl;
		//std::cout << _oneSpeedY << std::endl;
		std::cout << "x = " << _oneSpeedX << std::endl;
		_playerOne.set(_deltaTime, _oneSpeedX, _oneSpeedY);
		collisionDetection(&_playerOne);
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
		if (_oneState == playerState::STANDR || _oneState == playerState::STANDL || _oneState == playerState::RUNL || _oneState == playerState::RUNR) {
			_oneState = playerState::RUNR;
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
		if (_oneState == playerState::STANDR || _oneState == playerState::STANDL || _oneState == playerState::RUNL || _oneState == playerState::RUNR) {
			_oneState = playerState::RUNL;
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
				switch (_oneState){
				case playerState::STANDR:
					_oneState = playerState::PUNCHR;
					_playerOne.setTexture(_punchingRightTexture);
					break;
				case playerState::BLOCKR:
					_oneState = playerState::PUNCHR;
					_playerOne.setTexture(_punchingRightTexture);
					break;
				case playerState::JUMPR:
					_oneState = playerState::HIGHKR;
					_playerOne.setTexture(_kickingRightTexture);
					break;
				case playerState::CROUCHR:
					_oneState = playerState::LOWKR;
					_playerOne.setTexture(_slidingRightTexture);
					break;
				case playerState::STANDL:
					_oneState = playerState::PUNCHL;
					_playerOne.setTexture(_punchingLeftTexture);
					break;
				case playerState::BLOCKL:
					_oneState = playerState::PUNCHL;
					_playerOne.setTexture(_punchingLeftTexture);
					break;
				case playerState::JUMPL:
					_oneState = playerState::HIGHKL;
					_playerOne.setTexture(_kickingLeftTexture);
					break;
				case playerState::CROUCHL:
					_oneState = playerState::LOWKL;
					_playerOne.setTexture(_slidingLeftTexture);
					break;
				default:
					break;
				}
			}
			else if (evnt.button.button == SDL_BUTTON_RIGHT){
				if (_oneState == playerState::STANDR){
					_oneState = playerState::BLOCKR;
					_playerOne.setTexture(_blockingRightTexture);
				}
				else if (_oneState == playerState::STANDL){
					_oneState = playerState::BLOCKL;
					_playerOne.setTexture(_blockingLeftTexture);
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (evnt.button.button == SDL_BUTTON_LEFT) {
				switch (_oneState) {
				case playerState::PUNCHR:
					_oneState = playerState::STANDR;
					_playerOne.setTexture(_standingTexture);
					break;
				case playerState::PUNCHL:
					_oneState = playerState::STANDL;
					_playerOne.setTexture(_standingTexture);
					break;
				case playerState::HIGHKR:
					_oneState = playerState::JUMPR;
					_playerOne.setTexture(_jumpingRightTexture);
					break;
				case playerState::HIGHKL:
					_oneState = playerState::JUMPL;
					_playerOne.setTexture(_jumpingLeftTexture);
					break;
				case playerState::LOWKR:
					_oneState = playerState::STANDR;
					_playerOne.setTexture(_standingTexture);
					break;
				case playerState::LOWKL:
					_oneState = playerState::STANDL;
					_playerOne.setTexture(_standingTexture);
					break;

				default:
					break;
				}
			}
			else if (evnt.button.button == SDL_BUTTON_RIGHT){
				if (_oneState == playerState::BLOCKR){
					_oneState = playerState::STANDR;
					_playerOne.setTexture(_standingTexture);
				}
				else if (_oneState == playerState::BLOCKL){
					_oneState = playerState::STANDL;
					_playerOne.setTexture(_standingTexture);
				}
			}
			break;

		// one-time keyboard presses
		// for jumping, crouching and sliding
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym) {
			case SDLK_w:
				if (_oneState == playerState::STANDR || _oneState == playerState::RUNR){
					_oneSpeedY = _jumpSpeed;
					_oneState = playerState::JUMPR;
					_playerOne.setTexture(_jumpingRightTexture);
				}
				else if (_oneState == playerState::STANDL || _oneState == playerState::RUNL) {
					_oneSpeedY = _jumpSpeed;
					_oneState = playerState::JUMPL;
					_playerOne.setTexture(_jumpingLeftTexture);
				}
				break;
			case SDLK_s:
				switch (_oneState){
				case playerState::STANDR:
					_oneState = playerState::CROUCHR;
					_playerOne.setTexture(_crouchingRightTexture);
					break;
				case playerState::STANDL:
					_oneState = playerState::CROUCHL;
					_playerOne.setTexture(_crouchingLeftTexture);
					break;
				case playerState::RUNR:
					_oneState = playerState::LOWKR;
					_playerOne.setTexture(_slidingRightTexture);
					break;
				case playerState::RUNL:
					_oneState = playerState::LOWKL;
					_playerOne.setTexture(_slidingLeftTexture);
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
				_oneState = playerState::STANDR;
				_oneMoveRequest = false;
				_playerOne.setTexture(_standingTexture);
				_oneRunTime = 0.0f;
				break;
			case SDLK_a:
				_oneState = playerState::STANDL;
				_oneMoveRequest = false;
				_playerOne.setTexture(_standingTexture);
				_oneRunTime = 0.0f;
				break;
			case SDLK_w:
				if (_oneState == playerState::JUMPR) {
					_oneState = playerState::STANDR;
					_playerOne.setTexture(_standingTexture);
					_oneRunTime = 0.0f;
				}
				else {
					_oneState = playerState::STANDL;
					_playerOne.setTexture(_standingTexture);
					_oneRunTime = 0.0f;
				}
				break;
			case SDLK_s:
				if (_oneState == playerState::CROUCHR || _oneState == playerState::LOWKR) {
					_oneState = playerState::STANDR;
					_playerOne.setTexture(_standingTexture);
					_oneRunTime = 0.0f;
				}
				else {
					_oneState = playerState::STANDL;
					_playerOne.setTexture(_standingTexture);
					_oneRunTime = 0.0f;
				}
				break;


			default:
				break;
			}

		default:
			break;
		}
	}

	// gravity
	_oneSpeedY += _accY;

	// limit speed
	if (_oneSpeedX > _maxSpeedX) _oneSpeedX = _maxSpeedX;
	if (_oneSpeedX < -_maxSpeedX) _oneSpeedX = -_maxSpeedX;
	if (_oneSpeedY < _maxSpeedY) _oneSpeedY = _maxSpeedY;

	// allows players to stand still
	if (_oneSpeedX > 0 && _oneSpeedX < _decX) _oneSpeedX = 0;
	if (_oneSpeedX < 0 && _oneSpeedX > -_decX) _oneSpeedX = 0;

	// horizontal decceleration
	if (!_oneMoveRequest) {
		if (_oneSpeedX > 0.0f) {
			_oneSpeedX -= _decX;
		}
		else if (_oneSpeedX < 0.0f) {
			_oneSpeedX += _decX;
		}
	}

}

void mainGame::collisionDetection(Sprite* player) {
	for (int i = 0; i < _sceneList.size(); i++) {
		float x;
		float y;
		float width;
		float height;
		float x2;
		float y2;
		float width2;
		float height2;
		player->get(&x, &y, &width, &height);
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
			// hit at bottom
			if (!(y >= height2)) {
				player->set(1000.0f, 0.0f, (height2 - y));
			}
			// hit at top
			else if (!(height <= y2)) {

			}
			// hit at left
			else if (!(x >= width2)) {

			}
			// hit at right
			else {

			}
				
		}
	}

}

void mainGame::moderateFPS() {
	
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