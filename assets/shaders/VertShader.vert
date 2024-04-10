#version 460 core

layout (location = 0) in vec3 iVertPos;
layout (location = 1) in vec2 iTexCoord;
layout (location = 2) in vec3 iNormalVec;

out vec4 fColor;
out vec2 fTexCoord;
out vec3 fNormalVec;
out vec3 FragPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(iVertPos, 1.0f);
	fTexCoord = iTexCoord;
	
	FragPos =  vec3(modelMatrix *  vec4(iVertPos, 1.0)); // Convert vertex coords from MS to WS
	// Inverting a matrix is taxing, try to calculate using CPU and sending to GPU
	// Use normal matrix in presence of non uniform scaling
	fNormalVec = mat3(transpose(inverse(modelMatrix))) * iNormalVec;
}