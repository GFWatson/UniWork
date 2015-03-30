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
	//void collisionDetection(float* x, float* y, float* width, float* height);
	void drawGame();

	SDL_Window* _window;
	int _screenWidth;
	int _screenHeight;
	gameState _state;

	std::vector<Sprite> _sceneList;

	//float* _x;
	//float* _y;
	//float* _width;
	//float* _height;

	Camera _camera;

	Sprite _playerOne;
	playerState _oneState;
	collisionState _oneCollision;
	GLTexture _oneTexture;
	float _oneX;
	float _oneY;
	float _oneRunTime;
	float _oneAttackTime;

	Sprite _floor;

	GLSLProgram _colourProgram;
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

	float _time;
};

