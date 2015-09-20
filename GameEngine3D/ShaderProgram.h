#pragma once
#include <string>
#include<GL/glew.h>
#include "Error.h"
#include <vector>
#include <fstream>

using namespace std;

namespace GameEngine3D
{

	class ShaderProgram
	{

	public:
		ShaderProgram();
		~ShaderProgram();
		void compileShaders(const string& vertexShaderFilePath, const string& fragmentShaderFilepath);

		void linkShaders();
		void addAttribute(const string attributeName);

		GLint getUniformLocation(const string& uniformName);

		void use();
		void unuse();

	private:
		int _numAttributes;

		void compileShader(const string& filePath, GLuint id);

		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
	};

}

