#pragma once

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader
{
public:

	Shader(const char* vertexPath, const char* fragmentPath);
	void Use();
	// Const ensures that a call to the method doens't change any of the object's member data
	// Only member data marked as "mutable" can change
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, glm::mat4& value) const;
	void SetVec4(const std::string& name, glm::vec4& value) const;
	void SetVec4(const std::string& name, const float* const value) const;
	void SetVec3(const std::string& name, const float* const value) const;
	void SetVec3(const std::string& name, const float x, const float y, const float z) const;
	unsigned int GetId() const;

private:

	// Shader program ID
	mutable unsigned int Id;

	void ReadSourceCode(const char* vertexPath, const char* fragmentPath) const;
	void CompileShaders(const char* vertexShaderCode, const char* fragmentShaderCode) const;
	bool CheckShaderStatus(int shader, const char* shaderName) const;
	bool CheckShaderProgramStatus(int shaderProgram) const;

};