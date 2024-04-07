#include "LabDebug.h"
#include <iostream>

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string severityStr;
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:	severityStr = "ERROR:HIGH";		break;
		case GL_DEBUG_SEVERITY_MEDIUM:	severityStr = "ERROR:MEDIUM";	break;
		case GL_DEBUG_SEVERITY_LOW:		severityStr = "ERROR:LOW";		break;
		default:						severityStr = "NOTIFICATION";	break;
	}
	if(!severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		std::cout << "OpenGL:" << severityStr << " -> " << message << std::endl;
}

void ShowLabMessage(const char* message)
{
	std::cout << message << std::endl;
}