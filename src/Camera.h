#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:

	enum CameraMovement
	{
		FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN
	};

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = -90.0f, float pitch = 0.0f);


	glm::mat4 GetViewMatrix() const
	{ 
		return glm::lookAt(_position, _position + _front, _up);
	}

	void ProcessKeyboardInput(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);

	glm::vec3 Position() const { return _position; }
	glm::vec3 Front() const { return _front; }
	glm::vec3 Up() const { return _up; }

private:

	void UpdateCameraVectors();

private:

	glm::vec3 _position;
	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _worldUp;

	float _yaw;
	float _pitch;

	float _movementSpeed;
	float _mouseSensitivity;
};
