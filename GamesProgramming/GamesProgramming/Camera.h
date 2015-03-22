#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void init(int screenWidth, int screenHeight);
	glm::mat4 getCameraMatrix() { return _cameraMatrix; }

private:
	glm::vec2 _position;
	glm::mat4 _cameraMatrix;
	int _screenWidth;
	int _screenHeight;
};

