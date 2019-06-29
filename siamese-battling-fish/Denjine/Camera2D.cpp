#include "Camera2D.h"

namespace Denjine{

Camera2D::Camera2D() :
	_worldPosition(0.0f, 0.0f),
	_projMatrix(1.0f),
	_scale(1.0f),
	_update(true),
	_screenWidth(800),
	_screenHeight(800)
{

}


Camera2D::~Camera2D()
{
}


void Camera2D::initCamera(int w, int h)
{
	_screenWidth = w;
	_screenHeight = h;
	_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
}

void Camera2D::updateCamera()
{
	if (_update)
	{
		// Translate camera
		glm::vec3 translate(-_worldPosition.x + _screenWidth / 2, -_worldPosition.y + _screenHeight / 2, 0.0f);
		_projMatrix = glm::translate(_orthoMatrix, translate);

		// Scale camera
		glm::vec3 scale(_scale, _scale, 0.0f);
		_projMatrix = glm::scale(glm::mat4(1.0f), scale) * _projMatrix;
		_update = false;
	}
}

bool Camera2D::isSpriteInView(const glm::vec2& pos, const glm::vec2& dim)
{
	glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / _scale;

	// The minimum distance before a collision occurs
	const float MIN_DISTANCE_X = dim.x / 2.0f + scaledScreenDimensions.x / 2.0f;
	const float MIN_DISTANCE_Y = dim.y / 2.0f + scaledScreenDimensions.y / 2.0f;

	// Center position of the parameters
	glm::vec2 centerPos = pos + dim / 2.0f;
	// Center position of the camera
	glm::vec2 centerCameraPos = _worldPosition;
	// Vector from the input to the camera
	glm::vec2 distVec = centerPos - centerCameraPos;

	// Get the depth of the collision
	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

	if (xDepth > 0 && yDepth > 0) {
		return true;
	}
	return false;
}

}