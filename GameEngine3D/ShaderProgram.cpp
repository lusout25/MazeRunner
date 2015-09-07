#include "ShaderProgram.h"
#include "Error.h"
#include <vector>
#include <fstream>

namespace GameEngine3D
{

	ShaderProgram::ShaderProgram() : _numAttributes(0),
		_programID(0),
		_vertexShaderID(0),
		_fragmentShaderID(0)
	{

	}


	ShaderProgram::~ShaderProgram()
	{
	}

	void ShaderProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilepath)
	{
		_programID = glCreateProgram();

		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0)
		{
			fatalError("Vertex shader failed to be created.");
		}

		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0)
		{
			fatalError("Fragment shader failed to be created.");
		}

		compileShader(vertexShaderFilePath, _vertexShaderID);
		compileShader(fragmentShaderFilepath, _fragmentShaderID);
	}

	void ShaderProgram::compileShader(const std::string& filePath, GLuint id)
	{
		std::fstream shaderFile(filePath);
		if (shaderFile.fail())
		{
			perror(filePath.c_str());
			fatalError("Failed to open " + filePath);
		}

		std::string fileContents = "";
		std::string line;

		while (std::getline(shaderFile, line))
		{
			fileContents += line + "\n";
		}
		shaderFile.close();

		const char* contentsPtr = fileContents.c_str(); 
		glShaderSource(id, 1, &contentsPtr, nullptr);
		glCompileShader(id);

		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(id);

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile.");
		}
	}

	void ShaderProgram::linkShaders()
	{
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		glLinkProgram(_programID);

		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			glDeleteProgram(_programID);

			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shaders failed to link.");

			return;
		}

		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}

	void ShaderProgram::addAttribute(const std::string attributeName)
	{
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
	}

	void ShaderProgram::use()
	{
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	void ShaderProgram::unuse()
	{
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}

	GLint ShaderProgram::getUniformLocation(const std::string& uniformName)
	{
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			fatalError("Uniform " + uniformName + " not found in shader.");
		}
		return location;
	}

}