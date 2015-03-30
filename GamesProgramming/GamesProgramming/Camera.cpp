#include "Camera.h"


Camera::Camera() : _position(0.0f, 0.0f), _cameraMatrix(1.0f), _screenWidth(1028), _screenHeight(768)
{
	
}


Camera::~Camera()
{
}

void Camera::init(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_cameraMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
}