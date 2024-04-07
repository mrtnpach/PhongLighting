#version 460 core
layout (location = 0) in vec3 vertexPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPos, 1.0);
	//gl_Position = vec4(0, 0, -6, 1);
}