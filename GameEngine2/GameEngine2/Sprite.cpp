#include "Sprite.h"
#include "Vertex.h"

#include <cstddef>
#include <iostream>
#include <glm/gtx/transform.hpp>

Sprite::Sprite() : vboID(0), vao(0)
{

}


Sprite::~Sprite()
{
	if (vboID != 0) {
		glDeleteBuffers(1, &vboID);
	}
}

void Sprite::init(float i, float j, float width, float height, GLTexture tex) {
	x = i;
	y = j;
	w = width;
	h = height;
	texture = tex;

	// create vao
	if (vao == 0) {
		glGenVertexArrays(1, &vao);
	}

	glBindVertexArray(vao);

	// create vboID
	if (vboID == 0) {
		glGenBuffers(1, &vboID);
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
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
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

	//make model matrix
	mMatrix = glm::translate(mMatrix, glm::vec3(x, y, 0.0f));

}

GLTexture Sprite::getTexture() {
	return texture;
}

void Sprite::draw() {
	// bind buffer
	glBindVertexArray(vao);

	// draw
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//unbind buffer
	glBindVertexArray(0);

}

glm::mat4 Sprite::getModel() {
	return mMatrix;
}

float Sprite::getX() {
	return x;
}

float Sprite::getY() {
	return y;
}

void Sprite::changeTexture(GLTexture tex) {
	texture = tex;
}
