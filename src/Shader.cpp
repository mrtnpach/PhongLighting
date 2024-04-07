#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	ReadSourceCode(vertexPath, fragmentPath);
}

void Shader::Use()
{
	glUseProgram(Id);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(Id, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(Id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(Id, name.c_str()), value);
}

void Shader::SetMat4(const std::string& name, glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetVec4(const std::string& name, glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(Id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, const float* const value) const
{
	glUniform4fv(glGetUniformLocation(Id, name.c_str()), 1, value);
}

void Shader::SetVec3(const std::string& name, const float* const value) const
{
	glUniform3fv(glGetUniformLocation(Id, name.c_str()), 1, value);
}

void Shader::SetVec3(const std::string& name, const float x, const float y, const float z) const
{
	float vector[] = {x, y, z};
	glUniform3fv(glGetUniformLocation(Id, name.c_str()), 1, vector);
}


void Shader::ReadSourceCode(const char* vertexPath, const char* fragmentPath) const
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// Ensure that ifstreams objects throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream;
		std::stringstream fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// Close files
		vShaderFile.close();
		fShaderFile.close();
		// Stream to string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR READING SHADER SOURCE\n";
	}

	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();
	CompileShaders(vertexShaderCode, fragmentShaderCode);
}

void Shader::CompileShaders(const char* vertexShaderCode, const char* fragmentShaderCode) const
{
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	CheckShaderStatus(vertexShader, "vertexShader");

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	CheckShaderStatus(fragmentShader, "fragmentShader");

	this->Id = glCreateProgram();
	glAttachShader(Id, vertexShader);
	glAttachShader(Id, fragmentShader);
	glLinkProgram(Id);
	CheckShaderProgramStatus(Id);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

bool Shader::CheckShaderStatus(int shader, const char* shaderName) const
{
	int compilationSuccess;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationSuccess);
	if (!compilationSuccess)
	{
		int infoLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
		char* log = new char[infoLength];
		glGetShaderInfoLog(shader, infoLength, NULL, log);
		std::cout << "Error compiling " << shaderName << "\n";
		std::cout << log;
		delete[] log;
	}
	return compilationSuccess;
}

bool Shader::CheckShaderProgramStatus(int shaderProgram) const
{
	int linkingSuccess;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkingSuccess);
	if (!linkingSuccess)
	{
		GLenum error = glGetError();
		int infoLength;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLength);
		char* log = new char[infoLength];
		glGetProgramInfoLog(shaderProgram, infoLength, NULL, log);
		std::cout << "Error linking shader program:\n" << log << std::endl;
		delete[] log;
	}
	return linkingSuccess;
}

unsigned int Shader::GetId() const
{
	return Id;
}
