#pragma once
#include <glad/glad.h>

#ifndef ASSERT(x)
	#define ASSERT(x) if (!(x)) __debugBreak();
#endif

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void ShowLabMessage(const char* message);