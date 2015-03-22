#include "Sprite.h"
#include "Vertex.h"

#include <cstddef>
#include <iostream>

Sprite::Sprite() : _vboID(0)
{
	
}


Sprite::~Sprite()
{
	if (_vboID != 0){
		glDeleteBuffers(1, &_vboID);
	}
}

void Sprite::init(float x, float y, float width, float height, GLTexture tex) {
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	_transformX = x;
	_transformY = y;

	_texture = tex;

	// create vao
	if (_vao == 0) {
		glGenVertexArrays(1, &_vao);
	}

	glBindVertexArray(_vao);

	// create vboID
	if (_vboID == 0) {
		glGenBuffers(1, &_vboID);
	}

	Vertex vertexData[6];

	// first triangle
	vertexData[0].setPosition(width, height);
	vertexData[0].setUV(1.0f, 1.0f);
	vertexData[1].setPosition(0.0f, height);
	vertexData[1].setUV(0.0f, 1.0f);
	vertexData[2].setPosition(0.0f, 0.0f);
	vertexData[2].setUV(0.0f, 0.0f);

	// second triangle
	vertexData[3].setPosition(0.0f, 0.0f);
	vertexData[3].setUV(0.0f, 0.0f);
	vertexData[4].setPosition(width, 0.0f);
	vertexData[4].setUV(1.0f, 0.0f);
	vertexData[5].setPosition(width, height);
	vertexData[5].setUV(1.0f, 1.0f);

	// set colour
	for (int i = 0; i < 6; i++) {
		vertexData[i].setColour(255, 255, 255, 255);
	}

	// bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// where the data is
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, colour));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	// unbind buffer
	glBindVertexArray(0);

	// set model matrix
	_mMatrix = glm::translate(_mMatrix, glm::vec3(_transformX, _transformY, 0.0));

	// reset width and height to positions
	_width += _x;
	_height += _y;
}

void Sprite::get(float* x, float* y, float* width, float* height) {
	x = &_x;
	y = &_y;
	width = &_width;
	height = &_height;
}

void Sprite::set(float delta, float x, float y) {
	_x += (x * (delta / 1000.0f));
	_y += (y * (delta / 1000.0f));
	_width += _x;
	_height += _y;
	_transformX = (x * (delta / 1000.0f));
	_transformY = (y * (delta / 1000.0f));
	_mMatrix = glm::translate(_mMatrix, glm::vec3(_transformX, _transformY, 0.0));
}

GLTexture Sprite::getTexture() {
	return _texture;
}

void Sprite::setTexture(GLTexture tex) {
	_texture = tex;
}

glm::mat4 Sprite::getModel() {
	return _mMatrix;
}

void Sprite::draw() {
	// bind buffer
	glBindVertexArray(_vao);

	// draw
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//unbind buffer
	glBindVertexArray(0);

}
