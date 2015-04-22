#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>

#include "Sprite.h"
#include "GLSLProgram.h"
#include "GLTexture.h"
#include "Camera.h"
#include "PlayerState.h"

enum class gameState{ PLAY, EXIT };

class mainGame
{
public:
	mainGame();
	~mainGame();

	void run();

private:
	void initGame();
	void initSystems();
	void initShaders();
	void menuLoop();
	void gameLoop();
	void processInput();
	void collisionDetection(Sprite* player);
	void playerCollision(Sprite* player1, Sprite* player2, int dir);
	void platformGenerator();
	void moveScene();
	void setScale(Sprite* player);
	void checkPoint();
	void scoreTex();
	void checkWin();
	void resetGame();
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

	int _round;

	Camera _camera;

	SDL_GameController* _controller1;

	Sprite _mainMenu;
	gameState _menuState;

	Sprite _win;

	Sprite _playerOne;
	float _oneRunTime, _oneSlideTime;
	float _oneSpeedX, _oneSpeedY;
	bool _oneMoveRequest;
	Sprite _oneScore;
	int _one;

	Sprite _playerTwo;
	float _twoRunTime, _twoSlideTime;
	float _twoSpeedX, _twoSpeedY;
	bool _twoMoveRequest;
	Sprite _twoScore;
	int _two;

	Sprite _floor;

	Sprite _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8, _p9, _p10, _p11, _p12, _p13, _p14, _p15, _p16, _p17, _p18, _p19, _p20, _p21;

	float _pTime;
	float _lTime1, _lTime2, _lTime3, _lTime4, _lTime5, _lTime6, _lTime7;
	float _l1, _l2, _l3, _l4, _l5, _l6, _l7;

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
	GLTexture _mainMenuTexture;
	GLTexture _score10Texture;
	GLTexture _score11Texture;
	GLTexture _score12Texture;
	GLTexture _score13Texture;
	GLTexture _score20Texture;
	GLTexture _score21Texture;
	GLTexture _score22Texture;
	GLTexture _score23Texture;
	GLTexture _oneWinTexture;
	GLTexture _twoWinTexture;

	GLTexture _texture;
};

