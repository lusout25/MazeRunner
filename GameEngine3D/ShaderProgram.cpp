#include "ShaderProgram.h"

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

	void ShaderProgram::compileShaders(const string& vertexShaderFilePath, const string& fragmentShaderFilepath)
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

	void ShaderProgram::compileShader(const string& filePath, GLuint id)
	{
		string fileContents = "", line;
		GLint success, maxLength;
		fstream shaderFile(filePath);

		if (shaderFile.fail())
		{
			perror(filePath.c_str());
			fatalError("Failed to open " + filePath);
		}

		while (getline(shaderFile, line))
		{
			fileContents += line + "\n";
		}
		shaderFile.close();

		const char* contentsPtr = fileContents.c_str(); 
		glShaderSource(id, 1, &contentsPtr, nullptr);
		glCompileShader(id);

		success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(id);

			printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile.");
		}
	}

	void ShaderProgram::linkShaders()
	{
		GLint isLinked, maxLength;

		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		glLinkProgram(_programID);

		isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			glDeleteProgram(_programID);

			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			printf("%s\n", &(errorLog[0]));
			fatalError("Shaders failed to link.");

			return;
		}

		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}

	void ShaderProgram::addAttribute(const string attributeName)
	{
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
	}

	void ShaderProgram::use()
	{
		int i;

		glUseProgram(_programID);
		for (i = 0; i < _numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	void ShaderProgram::unuse()
	{
		int i;

		glUseProgram(0);
		for (i = 0; i < _numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}

	GLint ShaderProgram::getUniformLocation(const string& uniformName)
	{
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			fatalError("Uniform " + uniformName + " not found in shader.");
		}
		return location;
	}

}