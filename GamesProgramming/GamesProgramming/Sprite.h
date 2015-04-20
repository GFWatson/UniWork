#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "GLTexture.h"
#include "PlayerState.h"

class Sprite
{
public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height, GLTexture tex, playerState state);
	void setColPoints();
	void getColPoints(float* a, float* b, float* c, float* d, float* e, float* f, int side);
	void get(float* x, float* y, float* width, float* height);
	void set(float delta, float x, float y);
	void setPos(float x, float y);
	void setScale(float x, float y);
	GLTexture getTexture();
	void setTexture(GLTexture tex);
	glm::mat4 getModel();
	void setState(playerState state);
	playerState getState();
	float getHit();
	void increaseHit();
	void resetHit();
	void draw();
	bool offScreenLeft();

private:
	float _x;
	float _y;
	float _width;
	float _height;

	float _currentWidth;
	float _currentHeight;
	float _scaleX;
	float _scaleY;
	float _transformX;
	float _transformY;

	float _hitTime;

	float _colPoints[10][2];

	playerState _state;

	GLTexture _texture;
	GLuint _vboID;
	GLuint _vao;
	glm::mat4 _transMat;
	glm::mat4 _scaleMat;
	glm::mat4 _mMatrix;
};

