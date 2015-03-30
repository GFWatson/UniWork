#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>

#include "Sprite.h"
#include "GLSLProgram.h"
#include "GLTexture.h"
#include "Camera.h"

enum class gameState{ PLAY, EXIT };
enum class playerState{ STANDR, STANDL, RUNR, CROUCHR, LOWKR, BLOCKR, PUNCHR, JUMPR, HIGHKR, RUNL, CROUCHL, LOWKL, BLOCKL, PUNCHL, JUMPL, HIGHKL };
enum class collisionState{ NONE, FLOOR };

class mainGame
{
public:
	mainGame();
	~mainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void collisionDetection(Sprite* player);
	void moderateFPS();
	void drawGame();

	SDL_Window* _window;
	int _screenWidth;
	int _screenHeight;
	gameState _state;

	std::vector<Sprite*> _sceneList;

	float _fps, _maxFPS;
	float _currentTime, _prevTime, _deltaTime;

	GLint _pLocation, _mLocation;
	glm::mat4 _pMatrix, _mMatrix;

	float _accX, _decX, _accY, _maxSpeedX, _maxSpeedY, _jumpSpeed;

	Camera _camera;

	Sprite _playerOne;
	playerState _oneState;
	collisionState _oneCollision;
	float _oneRunTime;
	float _oneAttackTime;
	float _oneSpeedX, _oneSpeedY;
	bool _oneMoveRequest;

	Sprite _floor;

	GLSLProgram _colourProgram;
	GLTexture _defaultTexture;
	GLTexture _standingTexture;
	GLTexture _runningRightOneTexture;
	GLTexture _runningRightTwoTexture;
	GLTexture _runningRightThreeTexture;
	GLTexture _runningLeftOneTexture;
	GLTexture _runningLeftTwoTexture;
	GLTexture _runningLeftThreeTexture;
	GLTexture _jumpingRightTexture;
	GLTexture _jumpingLeftTexture;
	GLTexture _blockingRightTexture;
	GLTexture _blockingLeftTexture;
	GLTexture _punchingRightTexture;
	GLTexture _punchingLeftTexture;
	GLTexture _crouchingRightTexture;
	GLTexture _crouchingLeftTexture;
	GLTexture _slidingRightTexture;
	GLTexture _slidingLeftTexture;
	GLTexture _kickingRightTexture;
	GLTexture _kickingLeftTexture;

	GLTexture _texture;
};

