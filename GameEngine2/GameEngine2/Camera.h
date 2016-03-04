#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void init(float screenWidth, float screenHeight);
	glm::mat4 getCameraMatrix() { return cameraMatrix; }
	void zoom(float distance);
	void pan(float up, float across);

private:
	float x;
	float y;
	glm::mat4 cameraMatrix;
	float iniW;
	float iniH;
	float width;
	float height;
};

