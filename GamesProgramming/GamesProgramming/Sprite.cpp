#include "Sprite.h"
#include "Vertex.h"

#include <cstddef>
#include <iostream>
#include <glm/gtx/transform.hpp>

Sprite::Sprite() : _vboID(0), _vao(0)
{
	
}


Sprite::~Sprite()
{
	if (_vboID != 0){
		glDeleteBuffers(1, &_vboID);
	}
}

void Sprite::init(float x, float y, float width, float height, GLTexture tex, playerState state) {
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	_currentWidth = width;
	_currentHeight = height;

	_scaleX = 1.0f;
	_scaleY = 1.0f;

	_transformX = x;
	_transformY = y;

	_hitTime = 0.0f;

	_texture = tex;

	_state = state;

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

	// set scale and translation
	_transMat = glm::translate(_mMatrix, glm::vec3(_transformX, _transformY, 0.0));
	_scaleMat = glm::scale(_mMatrix, glm::vec3(_scaleX, _scaleY, 1.0f));

	// set model matrix
	_mMatrix = _transMat * _scaleMat;

	// set collision points
	setColPoints();

}

void Sprite::setColPoints() {
	float hor = _currentWidth / 3;
	float ver = _currentHeight / 3;

 	for (int x = 0; x < 10; x++) {
		switch (x){
		// bottom
		case 0:
			_colPoints[x][0] = _x + hor;
			_colPoints[x][1] = _y;
			break;
		case 1:
			_colPoints[x][0] = _x + (hor * 2);
			_colPoints[x][1] = _y;
			break;
		// top
		case 2:
			_colPoints[x][0] = _x + hor;
			_colPoints[x][1] = _currentHeight + _y;
			break;
		case 3:
			_colPoints[x][0] = _x + (hor * 2);
			_colPoints[x][1] = _currentHeight + _y;
			break;
		// left
		case 4:
			_colPoints[x][0] = _x;
			_colPoints[x][1] = _y + ver;
			break;
		case 5:
			_colPoints[x][0] = _x;
			_colPoints[x][1] = _y + (ver * 2 + 3.0f);
			break;
		case 6:
			_colPoints[x][0] = _x;
			_colPoints[x][1] = _y + (_currentHeight / 2);
			break;
		// right
		case 7:
			_colPoints[x][0] = _currentWidth + _x;
			_colPoints[x][1] = _y + ver;
			break;
		case 8:
			_colPoints[x][0] = _currentWidth + _x;
			_colPoints[x][1] = _y + (ver * 2 + 3.0f);
			break;
		case 9:
			_colPoints[x][0] = _currentWidth + _x;
			_colPoints[x][1] = _y + (_currentHeight / 2);
			break;

		default:
			_colPoints[x][0] = 0.0f;
			_colPoints[x][1] = 0.0f;
			break;
		}

	}
}

void Sprite::getColPoints(float* a, float* b, float* c, float* d, float* e, float* f, int side) {
	switch (side){
	// bottom
	case 0:
		*a = _colPoints[0][0];
		*b = _colPoints[0][1];
		*c = _colPoints[1][0];
		*d = _colPoints[1][1];
		break;
	// top
	case 1:
		*a = _colPoints[2][0];
		*b = _colPoints[2][1];
		*c = _colPoints[3][0];
		*d = _colPoints[3][1];
		break;
	// left
	case 2:
		*a = _colPoints[4][0];
		*b = _colPoints[4][1];
		*c = _colPoints[5][0];
		*d = _colPoints[5][1];
		*e = _colPoints[6][0];
		*f = _colPoints[6][1];
		break;
	// right
	case 3:
		*a = _colPoints[7][0];
		*b = _colPoints[7][1];
		*c = _colPoints[8][0];
		*d = _colPoints[8][1];
		*e = _colPoints[9][0];
		*f = _colPoints[9][1];
		break;

	default:
		*a = 0.0f;
		*b = 0.0f;
		*c = 0.0f;
		*d = 0.0f;
		*e = 0.0f;
		*f = 0.0f;
		break;
	}
}

void Sprite::get(float* x, float* y, float* width, float* height) {
	*x = _x;
	*y = _y;
	*width = _currentWidth + _x;
	*height = _currentHeight + _y;
}

void Sprite::set(float delta, float x, float y) {
	_transformX = (x * (delta / 1000.0f));
	_transformY = (y * (delta / 1000.0f));
	_x += _transformX;
	_y += _transformY;
	glm::vec3 transVec = glm::vec3(_transformX, _transformY, 0.0f);
	glm::vec3 scaleVec = glm::vec3(_scaleX, _scaleY, 1.0f);
	//_transMat = glm::translate(transVec);
	_transMat = glm::translate(_transMat, glm::vec3(_transformX, _transformY, 0.0f));
	_scaleMat = glm::scale(scaleVec);
	_mMatrix = _transMat *_scaleMat;
	_currentWidth = _width * _scaleX;
	_currentHeight = _height * _scaleY;
	setColPoints();
}

void Sprite::setPos(float x, float y) {
	int difX = x - _x;
	int difY = y - _y;
	set(1000.0f, difX, difY);
	setColPoints();
}

void Sprite::setScale(float x, float y) {
	_scaleX = x;
	_scaleY = y;
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

void Sprite::setState(playerState state) {
	_state = state;
}

playerState Sprite::getState() {
	return _state;
}

float Sprite::getHit(){
	return _hitTime;
}

void Sprite::increaseHit(){
	_hitTime++;
}

void Sprite::resetHit() {
	_hitTime = 0.0f;
}

void Sprite::draw() {
	// bind buffer
	glBindVertexArray(_vao);

	// draw
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//unbind buffer
	glBindVertexArray(0);

}

bool Sprite::offScreenLeft() {
	if ((_currentWidth + _x) <= 0.0f)
		return true;
	else
		return false;
}