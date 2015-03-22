#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "GLTexture.h"

class Sprite
{
public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height, GLTexture tex);
	void get(float* x, float* y, float* width, float* height);
	void set(float delta, float x, float y);
	GLTexture getTexture();
	void setTexture(GLTexture tex);
	glm::mat4 getModel();
	void draw();

private:
	float _x;
	float _y;
	float _width;
	float _height;
	float _transformX;
	float _transformY;
	GLTexture _texture;
	GLuint _vboID;
	GLuint _vao;
	glm::mat4 _mMatrix;
};

