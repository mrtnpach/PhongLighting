#include "Camera.h"
#include "LabDebug.h"

Camera::Camera(glm::vec3 position,
	glm::vec3 worldUp,
	float yaw, float pitch)
	: _front(glm::vec3(0.0f, 0.0f, -1.0f)),
	_movementSpeed(2.5f),
	_mouseSensitivity(0.1f)
{
	_position = position;
	_worldUp = worldUp;
	_yaw = yaw;
	_pitch = pitch;
	UpdateCameraVectors();
}

void Camera::ProcessKeyboardInput(CameraMovement direction, float deltaTime)
{
	float velocity = _movementSpeed * deltaTime;
	switch (direction)
	{
	case CameraMovement::FORWARD:
		_position += _front * velocity; break;
	case CameraMovement::BACKWARD:
		_position -= _front * velocity; break;
	case CameraMovement::RIGHT:
		_position += _right * velocity; break;
	case CameraMovement::LEFT:
		_position -= _right * velocity; break;
	case CameraMovement::UP:
		_position += _worldUp * velocity; break;
	case CameraMovement::DOWN:
		_position -= _worldUp * velocity; break;
	}
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= _mouseSensitivity;
	yOffset *= _mouseSensitivity;

	_yaw += xOffset;
	_pitch += yOffset;

	if (constrainPitch)
	{
		if (_pitch > 89.0f)
			_pitch = 89.0f;
		if (_pitch < -89.0f)
			_pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	// New front vector
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	// Normalize and assign
	_front = glm::normalize(front);

	// Calculate right and up vector
	// Right is used for horizontal movement
	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}