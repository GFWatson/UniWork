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

	void init(float i, float j, float width, float height, GLTexture tex);
	GLTexture getTexture();
	void draw();
	glm::mat4 getModel();
	float getX();
	float getY();
	void changeTexture(GLTexture tex);

private:
	float x;
	float y;
	float w;
	float h;

	GLTexture texture;
	GLuint vboID;
	GLuint vao;
	glm::mat4 mMatrix;
};

