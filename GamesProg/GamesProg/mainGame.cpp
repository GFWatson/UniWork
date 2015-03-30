#include "mainGame.h"
#include "Errors.h"
#include "imageLoader.h"

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

mainGame::mainGame() :
_screenWidth(1024),
_screenHeight(768),
_time(0.0f), _window(nullptr),
_state(gameState::PLAY), _oneState(playerState::STANDR), _oneCollision(collisionState::NONE), _oneRunTime(0.0f), _oneAttackTime(0.0f)
{
	_camera.init(_screenWidth, _screenHeight);
}


mainGame::~mainGame()
{

}


void mainGame::run()
{
	initSystems();

	_playerOne.init(50.0f, 50.0f, 48.0f, 64.0f);
	_floor.init(0.0f, 0.0f, 1024.0f, 20.0f);

	_sceneList.push_back(_playerOne);
	_sceneList.push_back(_floor);

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

	_oneTexture = _standingTexture;

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
}

void mainGame::gameLoop()
{
	while (_state != gameState::EXIT){
		//processInput();
		//_time += 0.01;
		drawGame();
	}
}


void mainGame::processInput()
{
	SDL_Event evnt;

	// constant keyboard presses
	SDL_PumpEvents();
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_D]) {
		if (_oneState == playerState::STANDR || _oneState == playerState::STANDL || _oneState == playerState::RUNL || _oneState == playerState::RUNR) {
			_oneState = playerState::RUNR;

			if (_oneRunTime < 0.4f){
				_oneTexture = _runningRightOneTexture;
			}
			else if (_oneRunTime < 0.8f){
				_oneTexture = _runningRightTwoTexture;
			}
			else {
				_oneTexture = _runningRightThreeTexture;
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

			if (_oneRunTime < 0.4f){
				_oneTexture = _runningLeftOneTexture;
			}
			else if (_oneRunTime < 0.8f){
				_oneTexture = _runningLeftTwoTexture;
			}
			else {
				_oneTexture = _runningLeftThreeTexture;
			}

			if (_oneRunTime > 1.2f){
				_oneRunTime = 0.0f;
			}

			_oneRunTime += 0.1f;
		}
	}

	// one-time keyboard presses
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
					_oneTexture = _punchingRightTexture;
					break;
				case playerState::BLOCKR:
					_oneState = playerState::PUNCHR;
					_oneTexture = _punchingRightTexture;
					break;
				case playerState::JUMPR:
					_oneState = playerState::HIGHKR;
					_oneTexture = _kickingRightTexture;
					break;
				case playerState::CROUCHR:
					_oneState = playerState::LOWKR;
					_oneTexture = _slidingRightTexture;
					break;
				case playerState::STANDL:
					_oneState = playerState::PUNCHL;
					_oneTexture = _punchingLeftTexture;
					break;
				case playerState::BLOCKL:
					_oneState = playerState::PUNCHL;
					_oneTexture = _punchingLeftTexture;
					break;
				case playerState::JUMPL:
					_oneState = playerState::HIGHKL;
					_oneTexture = _kickingLeftTexture;
					break;
				case playerState::CROUCHL:
					_oneState = playerState::LOWKL;
					_oneTexture = _slidingLeftTexture;
					break;
				default:
					break;
				}
			}
			else if (evnt.button.button == SDL_BUTTON_RIGHT){
				if (_oneState == playerState::STANDR){
					_oneState = playerState::BLOCKR;
					_oneTexture = _blockingRightTexture;
				}
				else if (_oneState == playerState::STANDL){
					_oneState = playerState::BLOCKL;
					_oneTexture = _blockingLeftTexture;
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (evnt.button.button == SDL_BUTTON_LEFT) {
				switch (_oneState) {
				case playerState::PUNCHR:
					_oneState = playerState::STANDR;
					_oneTexture = _standingTexture;
					break;
				case playerState::PUNCHL:
					_oneState = playerState::STANDL;
					_oneTexture = _standingTexture;
					break;
				case playerState::HIGHKR:
					_oneState = playerState::JUMPR;
					_oneTexture = _jumpingRightTexture;
					break;
				case playerState::HIGHKL:
					_oneState = playerState::JUMPL;
					_oneTexture = _jumpingLeftTexture;
					break;
				case playerState::LOWKR:
					_oneState = playerState::STANDR;
					_oneTexture = _standingTexture;
					break;
				case playerState::LOWKL:
					_oneState = playerState::STANDL;
					_oneTexture = _standingTexture;
					break;

				default:
					break;
				}
			}
			else if (evnt.button.button == SDL_BUTTON_RIGHT){
				if (_oneState == playerState::BLOCKR){
					_oneState = playerState::STANDR;
					_oneTexture = _standingTexture;
				}
				else if (_oneState == playerState::BLOCKL){
					_oneState = playerState::STANDL;
					_oneTexture = _standingTexture;
				}
			}
			break;
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym) {
			case SDLK_w:
				if (_oneState == playerState::STANDR || _oneState == playerState::RUNR){
					_oneState = playerState::JUMPR;
					_oneTexture = _jumpingRightTexture;
				}
				else if (_oneState == playerState::STANDL || _oneState == playerState::RUNL) {
					_oneState = playerState::JUMPL;
					_oneTexture = _jumpingLeftTexture;
				}
				break;
			case SDLK_s:
				switch (_oneState){
				case playerState::STANDR:
					_oneState = playerState::CROUCHR;
					_oneTexture = _crouchingRightTexture;
					break;
				case playerState::STANDL:
					_oneState = playerState::CROUCHL;
					_oneTexture = _crouchingLeftTexture;
					break;
				case playerState::RUNR:
					_oneState = playerState::LOWKR;
					_oneTexture = _slidingRightTexture;
					break;
				case playerState::RUNL:
					_oneState = playerState::LOWKL;
					_oneTexture = _slidingLeftTexture;
					break;

				default:
					break;
				}
				break;

			default:
				break;
			}
			break;
		case SDL_KEYUP:
			switch (evnt.key.keysym.sym) {
			case SDLK_d:
				_oneState = playerState::STANDR;
				_oneTexture = _standingTexture;
				_oneRunTime = 0.0f;
				break;
			case SDLK_a:
				_oneState = playerState::STANDL;
				_oneTexture = _standingTexture;
				_oneRunTime = 0.0f;
				break;
			case SDLK_w:
				if (_oneState == playerState::JUMPR) {
					_oneState = playerState::STANDR;
					_oneTexture = _standingTexture;
					_oneRunTime = 0.0f;
				}
				else {
					_oneState = playerState::STANDL;
					_oneTexture = _standingTexture;
					_oneRunTime = 0.0f;
				}
				break;
			case SDLK_s:
				if (_oneState == playerState::CROUCHR || _oneState == playerState::LOWKR) {
					_oneState = playerState::STANDR;
					_oneTexture = _standingTexture;
					_oneRunTime = 0.0f;
				}
				else {
					_oneState = playerState::STANDL;
					_oneTexture = _standingTexture;
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
}
/*
void mainGame::collisionDetection(float* x, float* y, float* width, float* height) {
	for (int i = 0; i < sizeof(_sceneList); i++) {

		if (*x == *_x && *y == *_y && *width == *_width && *height == *_height) {
			continue;
		}
		else if (*y >= (*_y + *_height) && (*y + *height) <= *_y && *x >= (*_x + *_width) && (*x + *width) <= *_x) {
			continue;
		}
		else {
			if (!(*y >= (*_y + *_height))) {

			}
			else if (!((*y + *height) <= *_y)) {

			}
			else if (!(*x >= (*_x + *_width))) {

			}
			else {

			}

		}
	}

}
*/
void mainGame::drawGame() {

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colourProgram.use();
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, _oneTexture.id);
	//GLint textureLocation = _colourProgram.getUniformLocation("mySampler");
	//glUniform1i(textureLocation, 0);

	//GLint timeLocation = _colourProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, _time);
	
	GLint mLocation = _colourProgram.getUniformLocation("M");
	glm::mat4 oneModelMatrix;

	GLint pLocation = _colourProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(mLocation, 1, GL_FALSE, &(oneModelMatrix[0][0]));
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_playerOne.draw();

	glm::mat4 floorModelMatrix;
	glUniformMatrix4fv(mLocation, 1, GL_FALSE, &(floorModelMatrix[0][0]));

	_floor.draw();

	//glBindTexture(GL_TEXTURE_2D, 0);

	_colourProgram.unuse();

	SDL_GL_SwapWindow(_window);
}