#include "Camera.h"


Camera::Camera() : x(0.0f), y(0.0f), cameraMatrix(1.0f), iniW(500.0), iniH(500.0f), width(500.0f), height(500.0f)
{

}


Camera::~Camera()
{
}

void Camera::init(float screenWidth, float screenHeight) {
	width = screenWidth;
	height = screenHeight;
	iniW = width;
	iniH = height;
	cameraMatrix = glm::ortho(x, width, y, height);
}

void Camera::zoom(float distance) {
	width += distance;
	if (width > iniW) {
		width = iniW;
	}
	else if (width < 50.0f) {
		width = 50.0f;
	}
	height += distance;
	if (height > iniH) {
		height = iniH;
	}
	else if (height < 50.0f) {
		height = 50.0f;
	}
	cameraMatrix = glm::ortho(x, width, y, height);
}